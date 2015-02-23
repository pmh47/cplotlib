/*
 * cplotlib.hpp
 *
 *  Created on: 6 Feb 2015
 *      Author: paul
 */

#ifndef CPLOTLIB_HPP_
#define CPLOTLIB_HPP_

#include <limits>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <boost/optional.hpp>
#include <boost/range/join.hpp>

#include <Eigen/Dense>

#include <opencv2/opencv.hpp>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include "utilities.hpp"
#include "object.hpp"
#include "series.hpp"
#include "rasterised-object.hpp"
#include "image.hpp"
#include "patch.hpp"
#include "label.hpp"
#include "pane.hpp"
#include "plot2d.hpp"
#include "grid.hpp"
#include "figure.hpp"

#endif /* CPLOTLIB_HPP_ */
