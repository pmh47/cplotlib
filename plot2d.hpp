/*
 * plot2d.hpp
 *
 *  Created on: 23 Feb 2015
 *      Author: paul
 */

#ifndef PLOT2D_HPP_
#define PLOT2D_HPP_

namespace plots
{
	struct Plot2D : public Pane
	{
		struct AxisProperties
		{
			bool visible;  // a margin is left for the axis (and elements drawn) iff this is set
			boost::optional<Range> range;  // minima of axis ranges are at top left of device space
			boost::optional<double> tickInterval;
	//		bool logarithmic;
		};

		AxisProperties xAxisProperties, yAxisProperties;
		std::vector<std::shared_ptr<Object>> primaryObjects;

//		boost::optional<AxisProperties> yAxis2Properties;
//		std::vector<std::shared_ptr<Object>> secondaryObjects;

		enum class LegendStyle
		{
			none,
			topRight,
			bottomRight
		} legendStyle;

		Plot2D() : Pane(boost::none_t()) { }
		
		Plot2D(std::vector<std::shared_ptr<Object>> const &primaryObjects_, AxisProperties const &xAxisProperties_, AxisProperties const &yAxisProperties_, boost::optional<std::string> const &title_)
			: Pane(title_)
			, xAxisProperties(xAxisProperties_)
			, yAxisProperties(yAxisProperties_)
			, primaryObjects(primaryObjects_)
		{
			if (primaryObjects.empty() /*&& secondaryObjects.empty()*/)
				contentBounds = { {0, 0}, {1, 1} };
			else {
				contentBounds = {
					{ std::numeric_limits<double>::max(), std::numeric_limits<double>::max() },
					{ std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest() }
				};
				for (auto const &object : /*boost::join(*/primaryObjects/*, secondaryObjects)*/) {
					auto const objectBounds = object->getBounds();
					contentBounds.topLeft.x = std::min(contentBounds.topLeft.x, objectBounds.topLeft.x);
					contentBounds.topLeft.y = std::min(contentBounds.topLeft.y, objectBounds.topLeft.y);
					contentBounds.bottomRight.x = std::max(contentBounds.bottomRight.x, objectBounds.bottomRight.x);
					contentBounds.bottomRight.y = std::max(contentBounds.bottomRight.y, objectBounds.bottomRight.y);
				}
			}
		}
		
		static std::shared_ptr<Plot2D> make(std::vector<std::shared_ptr<Object>> const &primaryObjects_)
		{
			return std::make_shared<Plot2D>(primaryObjects_, AxisProperties{true, boost::none_t()}, AxisProperties{true, boost::none_t()}, boost::none_t());
		}

		static std::shared_ptr<Plot2D> makeWithImage(std::string const &imageFilename, std::vector<std::shared_ptr<Object>> const &overlaidObjects = std::vector<std::shared_ptr<Object>>(), boost::optional<std::string> const &title = boost::none_t())
		{
			std::vector<std::shared_ptr<Object>> objects;
			objects.push_back( std::make_shared<Image>(imageFilename) );
			objects.insert(objects.end(), overlaidObjects.begin(), overlaidObjects.end());
			return std::make_shared<Plot2D>(objects, AxisProperties{false, boost::none_t()}, AxisProperties{false, boost::none_t()}, title);
		}

	protected:

		void displayClient(cairo_t * const context, Rectangle const &bounds) const override
		{
			cairo_save(context);
				cairo_rectangle(context, bounds.topLeft.x, bounds.topLeft.y, bounds.width(), bounds.height());
				cairo_clip(context);  // first clip ensures axis decoration, etc., never leaves outer box
				cairo_translate(context, bounds.topLeft.x, bounds.topLeft.y);
				// points henceforth are in pixel space with canonical axes and origin at the top left of the plot
				auto const plotRectangle = getPlotArea(bounds.width(), bounds.height());  // bounds in said CS of the plot boundaries (less than client area due to axis labels, etc.)
				auto const primaryAxisRectangle = getEffectiveAxisRanges();
				cairo_save(context);
					cairo_rectangle(context, plotRectangle.topLeft.x, plotRectangle.topLeft.y, plotRectangle.width(), plotRectangle.height());
					cairo_clip(context);  // second clip ensures plot content never leaves plot area
					Colour::plotBackground().setAsSourceFor(context);
					cairo_paint(context);
					cairo_save(context);
						// Apply primary axis transform and render objects
						auto const scaleX = plotRectangle.width() / primaryAxisRectangle.width();
						auto const scaleY = plotRectangle.height() / primaryAxisRectangle.height();
						cairo_translate(context, plotRectangle.centre().x - primaryAxisRectangle.centre().x * scaleX, plotRectangle.centre().y - primaryAxisRectangle.centre().y * scaleY);
						cairo_scale(context, scaleX, scaleY);  // points henceforth are in primary axis space
						for (auto const &object : primaryObjects)
							object->display(context);
					cairo_restore(context);
					// ** handle objects in secondary y-axis space!
				cairo_restore(context);  // remove axis transform and second clip (to plot area)
				if (xAxisProperties.visible) {
					cairo_move_to(context, plotRectangle.topLeft.x, plotRectangle.bottomRight.y);
					cairo_line_to(context, plotRectangle.bottomRight.x, plotRectangle.bottomRight.y);
					Colour::black().setAsSourceFor(context);
					cairo_stroke(context);
				}
				if (yAxisProperties.visible) {
					cairo_move_to(context, plotRectangle.topLeft.x, plotRectangle.topLeft.y);
					cairo_line_to(context, plotRectangle.topLeft.x, plotRectangle.bottomRight.y);
					Colour::black().setAsSourceFor(context);
					cairo_stroke(context);
				}
				// ** render axis ticks and labels!
			cairo_restore(context);
		}
		
	private:
		
		static double constexpr axisEndPadding = 8;
		static double constexpr axisInset = 24;

		Rectangle contentBounds;
		
		Rectangle getPlotArea(double const deviceWidth, double const deviceHeight) const
		{
			// Result is in origin-corrected pixel (device) space
			Rectangle result{ {0, 0}, {deviceWidth, deviceHeight} };
			if (xAxisProperties.visible) {
				result.topLeft.x += axisEndPadding;
				result.bottomRight.x -= axisEndPadding;
				result.bottomRight.y -= axisInset;
			}
			if (yAxisProperties.visible) {
				result.topLeft.y += axisEndPadding;
				result.bottomRight.y -= axisEndPadding;
				result.topLeft.x += axisInset;
			}
			return result;
		}
		
		Rectangle getEffectiveAxisRanges() const
		{
			// topLeft attribute is wrt pixel (device) space, so for a typical plot, topLeft.y > bottomRight.y
			Rectangle primaryResult, secondaryResult;
			if (xAxisProperties.range) {
				primaryResult.topLeft.x = secondaryResult.topLeft.x = xAxisProperties.range->minimum;
				primaryResult.bottomRight.x = secondaryResult.bottomRight.x = xAxisProperties.range->maximum;
			} else {
				primaryResult.topLeft.x = secondaryResult.topLeft.x = contentBounds.topLeft.x;
				primaryResult.bottomRight.x = secondaryResult.bottomRight.x = contentBounds.bottomRight.x;
			}
			if (yAxisProperties.range) {
				primaryResult.topLeft.y = yAxisProperties.range->minimum;
				primaryResult.bottomRight.y = yAxisProperties.range->maximum;
			} else {
				primaryResult.topLeft.y = contentBounds.topLeft.y;
				primaryResult.bottomRight.y = contentBounds.bottomRight.y;
			}
			return primaryResult;
		}
	};
}

#endif /* PLOT2D_HPP_ */
