/*
 * image.hpp
 *
 *  Created on: 23 Feb 2015
 *      Author: paul
 */

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

namespace plots
{
	struct Image : public RasterisedObject
	{
		explicit Image(std::string const &filename, boost::optional<Rectangle> const &maybeBounds = boost::none_t())
		{
			auto const mat = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
			assert( mat.channels() == 3 );
			width = mat.cols;
			height = mat.rows;
			if (maybeBounds)
				location = *maybeBounds;
			else
				location = { Point2D{0, 0}, Point2D{(double)width, (double)height} };
			int const stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, mat.cols);
			buffer.resize(mat.rows * stride);
			for (int row = 0; row < mat.rows; ++row) {
				for (int col = 0; col < mat.cols; ++col) {
					buffer[row * stride + col * 4 + 0] = mat.data[row * mat.cols * 3 + col * 3 + 0];
					buffer[row * stride + col * 4 + 1] = mat.data[row * mat.cols * 3 + col * 3 + 1];
					buffer[row * stride + col * 4 + 2] = mat.data[row * mat.cols * 3 + col * 3 + 2];
				}
			}
			surfacePtr = cairo_image_surface_create_for_data(buffer.data(), CAIRO_FORMAT_RGB24, mat.cols, mat.rows, stride);
			patternPtr = cairo_pattern_create_for_surface(surfacePtr);
		}
	};
}

#endif /* IMAGE_HPP_ */
