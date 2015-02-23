/*
 * patch.hpp
 *
 *  Created on: 23 Feb 2015
 *      Author: paul
 */

#ifndef PATCH_HPP_
#define PATCH_HPP_

namespace plots
{
	struct Patch : public Object
	{
		struct Attributes
		{
			FillStyle fillStyle;
			Colour fillColour;
			LineStyle lineStyle;
			Colour lineColour;
			float alpha;
		} attributes;
		
		std::vector<Point2D> points;
		Rectangle bounds;
		
		Patch(std::vector<Point2D> const &points_, Attributes const &attributes_)
			: points(points_), attributes(attributes_)
		{
			// ** ...
//			bounds = ...;
		}
		
		static std::shared_ptr<Patch> makeRectangle(Rectangle const &bounds, Attributes const &attributes)
		{
			std::vector<Point2D> points;
			points.push_back(bounds.topLeft);
			points.push_back(Point2D{bounds.bottomRight.x, bounds.topLeft.y});
			points.push_back(bounds.bottomRight);
			points.push_back(Point2D{bounds.topLeft.x, bounds.bottomRight.y});
			return std::make_shared<Patch>(points, attributes);
		}

		virtual void display(cairo_t *const context) const override
		{
			if (points.empty())
				return;
			cairo_save(context);
			cairo_move_to(context, points.front().x, points.front().y);
			for (auto pointIt = points.begin() + 1; pointIt != points.end(); ++pointIt)
				cairo_line_to(context, pointIt->x, pointIt->y);
			cairo_close_path(context);
			if (attributes.fillStyle != FillStyle::none) {
				attributes.fillColour.setAsSourceFor(context, attributes.alpha);
				cairo_fill_preserve(context);
			}
			if (attributes.lineStyle != LineStyle::none) {
				attributes.lineColour.setAsSourceFor(context, attributes.alpha);
				setLineWidth(context, 1);
				cairo_stroke(context);
			}
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

#endif /* PATCH_HPP_ */
