/*
 * grid.hpp
 *
 *  Created on: 23 Feb 2015
 *      Author: paul
 */

#ifndef GRID_HPP_
#define GRID_HPP_

namespace plots
{
	struct Grid : public Pane
	{
		std::size_t const rows, columns;
		std::vector<std::vector<std::shared_ptr<Pane>>> const panes;
		double const spacing;

		Grid() = delete;

		explicit Grid(std::vector<std::vector<std::shared_ptr<Pane>>> const &panes_, double const margin_ = 0., boost::optional<std::string> const &title_ = boost::none_t())
			: Pane(title_), rows(panes_.size()), columns(getColumnCount(panes_)), panes(panes_), spacing(margin_)
		{
			if (rows == 0 || columns == 0)
				throw std::runtime_error("expected non-zero number of rows and columns");
		}

		static std::shared_ptr<Grid> make(std::vector<std::vector<std::shared_ptr<Pane>>> const &panes_, double const margin_ = 0., boost::optional<std::string> const &title_ = boost::none_t())
		{
			return std::make_shared<Grid>(panes_, margin_, title_);
		}

		static std::shared_ptr<Grid> makeColwise(std::vector<std::vector<std::shared_ptr<Pane>>> const &transposedPanes, double const margin_ = 0., boost::optional<std::string> const &title_ = boost::none_t())
		{
			std::vector<std::vector<std::shared_ptr<Pane>>> panes_;
			for (std::size_t columnIndex = 0; columnIndex < transposedPanes.size(); ++columnIndex) {
				for (std::size_t rowIndex = 0; rowIndex < transposedPanes[columnIndex].size(); ++rowIndex) {
					if (panes_.size() <= rowIndex)
						panes_.resize(rowIndex + 1);
					if (panes_[rowIndex].size() <= columnIndex)
						panes_[rowIndex].resize(columnIndex + 1);
					panes_[rowIndex][columnIndex] = transposedPanes[columnIndex][rowIndex];
				}
			}
			return std::make_shared<Grid>(panes_, margin_, title_);
		}

		static std::shared_ptr<Grid> makeBalanced(std::vector<std::shared_ptr<Pane>> const &panes_, double const margin_ = 0., boost::optional<std::string> const &title_ = boost::none_t())
		{
			auto const rowLength = (std::size_t)( std::sqrt(panes_.size()) + 0.9 );
			std::vector<std::vector<std::shared_ptr<Pane>>> rearrangedPanes(1);
			for (std::size_t paneIndex = 0; paneIndex < panes_.size(); ++paneIndex) {
				if (rearrangedPanes.back().size() == rowLength)
					rearrangedPanes.push_back( { } );
				rearrangedPanes.back().push_back(panes_[paneIndex]);
			}
			return std::make_shared<Grid>(rearrangedPanes, margin_, title_);
		}

	protected:

		void displayClient(cairo_t * const context, Rectangle const &bounds) const override
		{
			auto const deltaX = (bounds.width() - spacing) / columns;
			auto const deltaY = (bounds.height() - spacing) / rows;
			for (std::size_t row = 0; row < rows; ++row) {
				for (std::size_t column = 0; column < panes[row].size(); ++column) {
					Rectangle const innerBounds{
						{bounds.topLeft.x + spacing + deltaX * column, bounds.topLeft.y + spacing + deltaY * row},
						{bounds.topLeft.x + deltaX * (column + 1), bounds.topLeft.y + deltaY * (row + 1)}
					};
					panes[row][column]->display(context, innerBounds);
				}
			}
		}

	private:

		static std::size_t getColumnCount(std::vector<std::vector<std::shared_ptr<Pane>>> const &panes_)
		{
			std::size_t maximum = 0;
			for (auto const &row : panes_) {
				if (row.size() > maximum)
					maximum = row.size();
			}
			return maximum;
		}
	};
}

#endif /* GRID_HPP_ */
