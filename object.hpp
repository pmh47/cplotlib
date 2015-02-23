/*
 * object.hpp
 *
 *  Created on: 23 Feb 2015
 *      Author: paul
 */

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

namespace plots
{
	struct Object
	{
		virtual Rectangle getBounds() const = 0;
		virtual boost::optional<LegendEntry> getLegendEntry() const = 0;

		virtual void display(cairo_t *const context) const = 0;
		
	protected:
		
		static void setLineWidth(cairo_t *context, double const width)
		{
			double userX = width, userY = width;
			cairo_device_to_user_distance(context, &userX, &userY);
			cairo_set_line_width(context, std::min(userX, userY));
		}
	};
}

#endif /* OBJECT_HPP_ */
