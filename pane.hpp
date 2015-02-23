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

		void display(cairo_t * const context, Rectangle const &bounds) const
		{
			// ** render title and adjust bounds
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
