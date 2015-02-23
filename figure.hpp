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
		
		explicit Figure(std::shared_ptr<Pane> const &content_)
			: content(content_)
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

			XStoreName(dsp, da, content->title ? content->title->c_str() : "Untitled Figure");

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
			auto const surfacePtr = createX11Surface(800, 600);
			auto const contextPtr = cairo_create(surfacePtr);
			runEventLoop(contextPtr, surfacePtr);
			cairo_destroy(contextPtr);
			destroyX11Surface(surfacePtr);
		}

		void save(std::string const &filename) const
		{
		}
	};
}

#endif /* FIGURE_HPP_ */
