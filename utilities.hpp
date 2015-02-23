/*
 * utilities.hpp
 *
 *  Created on: 23 Feb 2015
 *      Author: paul
 */

#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

namespace plots
{
	struct Range
	{
		double minimum, maximum;
	};

	struct Point2D
	{
		double x, y;
	};

	struct Rectangle
	{
		Point2D topLeft, bottomRight;

		double width() const
		{
			return bottomRight.x - topLeft.x;
		}

		double height() const
		{
			return bottomRight.y - topLeft.y;
		}
		
		Point2D centre() const
		{
			return Point2D{ (topLeft.x + bottomRight.x) / 2, (topLeft.y + bottomRight.y) / 2 };
		}
	};

	struct Colour
	{
		double r, g, b;
		
		void setAsSourceFor(cairo_t *context) const
		{
			cairo_set_source_rgb(context, r, g, b);
		}

		void setAsSourceFor(cairo_t *context, double const alpha) const
		{
			cairo_set_source_rgba(context, r, g, b, alpha);
		}

		static Colour windowBackground()
		{
			return Colour{0.9, 0.9, 0.9};
		}
		
		static Colour plotBackground()
		{
			return white();
		}
		
		static Colour defaultForeground()
		{
			return Colour{0.4, 0.4, 0.8};
		}
		
		static Colour black()
		{
			return Colour{0, 0, 0};
		}
		
		static Colour white()
		{
			return Colour{1, 1, 1};
		}
		
		static Colour fromHsv(double const h, double const s, double const v)
		{
			if (s == 0.0) {
		    	return Colour{v, v, v};
		    } else {
				auto const hh = h * 6;
				auto const i = (long)hh;
				auto const ff = hh - (double)i;
				auto const p = v * (1.0 - s);
				auto const q = v * (1.0 - (s * ff));
				auto const t = v * (1.0 - (s * (1.0 - ff)));
				switch (i) {
					case 0:
						return Colour{v, t, p};
					case 1:
						return Colour{q, v, p};
					case 2:
						return Colour{p, v, t};
					case 3:
						return Colour{p, q, v};
					case 4:
						return Colour{t, p, v};
					case 5:
					default:
						return Colour{v, p, q};
				}
		    }
		}
	};

	enum class FillStyle
	{
		none,
		solid
	};

	enum class LineStyle
	{
		none,
		solid,
		dashed,
		dotted
	};

	struct LegendEntry
	{
		std::string text;
		Colour colour;
	};
}

#endif /* UTILITIES_HPP_ */
