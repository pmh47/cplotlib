/*
 * figure.hpp
 *
 *  Created on: 23 Feb 2015
 *      Author: paul
 */

#ifndef FIGURE_HPP_
#define FIGURE_HPP_

namespace plots
{
	struct Figure
	{
		std::shared_ptr<Pane> const content;
		std::string const title;
		int const defaultWidth;
		int const defaultHeight;
		
		explicit Figure(std::shared_ptr<Pane> const &content_, std::string const &title_ = "Untitled Figure")
			: content(content_), title(title_), defaultWidth(800), defaultHeight(600)
		{
		}

		cairo_surface_t *createX11Surface(int const width, int const height) const
		{
			Display *dsp = XOpenDisplay(NULL);
			if (dsp == NULL)
			  throw std::runtime_error("failed to open X display");
			int const screen = DefaultScreen(dsp);
			Drawable da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, width, height, 0, 0, 0);
			XSelectInput(dsp, da, ButtonPressMask | KeyPressMask | ExposureMask);
			XMapWindow(dsp, da);

			XStoreName(dsp, da, title.c_str());

			cairo_surface_t *surface = cairo_xlib_surface_create(dsp, da, DefaultVisual(dsp, screen), width, height);
			cairo_xlib_surface_set_size(surface, width, height);

			return surface;
		}

		void destroyX11Surface(cairo_surface_t *sfc) const
		{
			Display *dsp = cairo_xlib_surface_get_display(sfc);
			cairo_surface_destroy(sfc);
			XCloseDisplay(dsp);
		}

		void runEventLoop(cairo_t *contextPtr, cairo_surface_t *sfc) const
		{
			auto const displayPtr = cairo_xlib_surface_get_display(sfc);
			auto const drawable = cairo_xlib_surface_get_drawable(sfc);
			for ( ; ; ) {
				XEvent e;
				XNextEvent(cairo_xlib_surface_get_display(sfc), &e);
				switch (e.type) {
					case Expose:
						if (e.xexpose.count == 0) {
							Window rootWindow;
							int x, y;
							unsigned int width, height, border, depth;
							XGetGeometry(displayPtr, drawable, &rootWindow, &x, &y, &width, &height, &border, &depth);
							cairo_xlib_surface_set_size(sfc, width, height);
							Colour::windowBackground().setAsSourceFor(contextPtr);
							cairo_paint(contextPtr);
							content->display(contextPtr, Rectangle{ {0, 0}, {(double)width, (double)height} });
						}
						break;
					case KeyPress:
						return;
				}
			}
		}

		void display() const
		{
			auto const surfacePtr = createX11Surface(defaultWidth, defaultHeight);
			auto const contextPtr = cairo_create(surfacePtr);
			runEventLoop(contextPtr, surfacePtr);
			cairo_destroy(contextPtr);
			destroyX11Surface(surfacePtr);
		}

		void save(std::string const &filename) const
		{
			cairo_surface_t *surfacePtr;
			int width, height;
			bool writePng = false;
			if (filename.substr(filename.length() - 4) == ".pdf") {
				width = defaultWidth / 100 * 72;
				height = defaultHeight / 100 * 72;
				surfacePtr = cairo_pdf_surface_create(filename.c_str(), width, height);
			} else if (filename.substr(filename.length() - 4) == ".eps") {
				width = defaultWidth / 100 * 72;
				height = defaultHeight / 100 * 72;
				surfacePtr = cairo_ps_surface_create(filename.c_str(), width, height);
				cairo_ps_surface_set_eps(surfacePtr, true);
			} else if (filename.substr(filename.length() - 4) == ".png") {
				width = defaultWidth;
				height = defaultHeight;
				surfacePtr = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
				writePng = true;
			} else
				throw std::runtime_error(std::string(__FUNCTION__) + ": unexpected file extension for figure export");
			auto const contextPtr = cairo_create(surfacePtr);
			content->display(contextPtr, Rectangle{ {0, 0}, {width, height} });
			if (writePng)
				cairo_surface_write_to_png(surfacePtr, filename.c_str());
			cairo_destroy(contextPtr);
			cairo_surface_destroy(surfacePtr);
		}
	};
}

#endif /* FIGURE_HPP_ */
