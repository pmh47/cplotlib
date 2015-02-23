/*
 * rasterised-object.hpp
 *
 *  Created on: 23 Feb 2015
 *      Author: paul
 */

#ifndef RASTERISED_OBJECT_HPP_
#define RASTERISED_OBJECT_HPP_

namespace plots
{
	struct RasterisedObject : public Object
	{
		virtual void display(cairo_t *const context) const override
		{
			cairo_save(context);
			cairo_set_source_rgb(context, 1, 0, 0);
			cairo_translate(context, location.topLeft.x, location.topLeft.y);
			cairo_rectangle(context, 0, 0, location.width(), location.height());
			cairo_set_source(context, patternPtr);
			cairo_matrix_t patternMatrix;
			cairo_matrix_init_scale(&patternMatrix, width / location.width(), height / location.height());
			cairo_pattern_set_matrix(patternPtr, &patternMatrix);
			cairo_fill(context);
			cairo_restore(context);
		}
		
		virtual Rectangle getBounds() const override
		{
			return location;
		}
		
		virtual boost::optional<LegendEntry> getLegendEntry() const override
		{
			return boost::none_t();
		}
		
	protected:
		
		virtual ~RasterisedObject()
		{
			cairo_pattern_destroy(patternPtr);
			cairo_surface_destroy(surfacePtr);
		}
		
		std::vector<unsigned char> buffer;
		cairo_surface_t *surfacePtr;
		cairo_pattern_t *patternPtr;
		int width, height;
		
		Rectangle location;  // in axis space
	};
}

#endif /* RASTERISED_OBJECT_HPP_ */
