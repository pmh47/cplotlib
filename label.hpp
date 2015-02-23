/*
 * label.hpp
 *
 *  Created on: 23 Feb 2015
 *      Author: paul
 */

#ifndef LABEL_HPP_
#define LABEL_HPP_

namespace plots
{
	struct Label : public Object
	{
		enum class HorizontalAlignment
		{
			left, centre, right
		};
		
		enum class VerticalAlignment
		{
			top, middle, baseline, bottom
		};
		
		struct Attributes
		{
			Colour colour;
			double size;
			HorizontalAlignment horizontalAlignment;
			VerticalAlignment verticalAlignment;
		} attributes;
		
		std::string text;
		Point2D location;
		
		Rectangle bounds;
		
		Label(std::string const &text_, Point2D const &location_, Attributes const &attributes_)
			: attributes(attributes_), text(text_), location(location_)
		{
			// ** ...
//			bounds = ...;
		}
		
		virtual void display(cairo_t *const context) const override
		{
			cairo_save(context);
			attributes.colour.setAsSourceFor(context);
			cairo_set_font_size(context, attributes.size);
			cairo_move_to(context, location.x, location.y);
			cairo_identity_matrix(context);
			cairo_text_extents_t extents;
			cairo_text_extents(context, text.c_str(), &extents);
			double const xOffset =
				attributes.horizontalAlignment == HorizontalAlignment::left ?
					0
				: attributes.horizontalAlignment == HorizontalAlignment::centre ?
					extents.width / 2
				: // right
					extents.width;
			double const yOffset =
				attributes.verticalAlignment == VerticalAlignment::top ?
					0
				: attributes.verticalAlignment == VerticalAlignment::middle ?
					extents.height / 2
				: attributes.verticalAlignment == VerticalAlignment::baseline ?
					-extents.y_bearing
				: // bottom
					extents.height;
			cairo_rel_move_to(context, -xOffset - extents.x_bearing, - yOffset - extents.y_bearing);
			cairo_show_text(context, text.c_str());
			cairo_restore(context);
		}
		
		virtual Rectangle getBounds() const override
		{
			return bounds;
		}
		
		virtual boost::optional<LegendEntry> getLegendEntry() const override
		{
			return boost::none_t();
		}
	};
}

#endif /* LABEL_HPP_ */
