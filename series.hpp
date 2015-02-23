/*
 * series.hpp
 *
 *  Created on: 23 Feb 2015
 *      Author: paul
 */

#ifndef SERIES_HPP_
#define SERIES_HPP_

namespace plots
{
	struct Series : public Object
	{
		enum class MarkerStyle
		{
			none,
			circle,
			square,
			cross
		};

		Eigen::MatrixX2d points;
		Colour colour;
		LineStyle lineStyle;
		MarkerStyle markerStyle;
		boost::optional<std::string> name;

		boost::optional<LegendEntry> getLegendEntry() const override
		{
			if (name)
				return LegendEntry{*name, colour};
		}
	};
}

#endif /* SERIES_HPP_ */
