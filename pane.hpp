/*
 * pane.hpp
 *
 *  Created on: 23 Feb 2015
 *      Author: paul
 */

#ifndef PANE_HPP_
#define PANE_HPP_

namespace plots
{
	struct Pane
	{
		boost::optional<std::string> title;

		static double constexpr clientInsetIfTitle = 24;
		static double constexpr titleTopMargin = 4;
		static double constexpr titleFontSize = 18;

		void display(cairo_t * const context, Rectangle bounds) const
		{
			if (title) {
				cairo_save(context);
				Colour::black().setAsSourceFor(context);
				cairo_set_font_size(context, titleFontSize);
				cairo_move_to(context, bounds.centre().x, bounds.topLeft.y + titleTopMargin);
				cairo_identity_matrix(context);
				cairo_text_extents_t extents;
				cairo_text_extents(context, title->c_str(), &extents);
				cairo_rel_move_to(context, -extents.width / 2 - extents.x_bearing, -extents.y_bearing);
				cairo_show_text(context, title->c_str());
				cairo_restore(context);
				bounds.topLeft.y += clientInsetIfTitle;
			}
			displayClient(context, bounds);
		}

	protected:

		explicit Pane(boost::optional<std::string> const &title_)
			: title(title_)
		{
		}

	private:

		virtual void displayClient(cairo_t * const, Rectangle const &) const = 0;
	};
}

#endif /* PANE_HPP_ */
