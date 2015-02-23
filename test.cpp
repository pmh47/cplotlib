/*
 * test.cpp
 *
 *  Created on: 6 Feb 2015
 *      Author: paul
 */

#include "cplotlib.hpp"

int main(int argc, char *argv[])
{
	plots::Figure {
		plots::Plot2D::makeWithImage("/home/paul/holyrood-penguins-2.bmp", {
			plots::Patch::makeRectangle(plots::Rectangle{{450, 0}, {600, 636}}, plots::Patch::Attributes())
		})
	}.display();

	plots::Figure fred{ plots::Grid::make(
		{
			{plots::Plot2D::make({
				std::make_shared<plots::Image>("/home/paul/pmh.png", plots::Rectangle{plots::Point2D{10, 10}, plots::Point2D{70, 70}}),
				plots::Patch::makeRectangle(plots::Rectangle{plots::Point2D{50, 40}, plots::Point2D{70, 70}}, plots::Patch::Attributes())
			})},
			{plots::Plot2D::makeWithImage("/home/paul/holyrood-penguins-2.bmp"), plots::Plot2D::make({std::make_shared<plots::Image>("/home/paul/pmh.png")})}
		},
		4
	) };
	fred.display();

	return 0;
}

