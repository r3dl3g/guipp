
#pragma once

#include "gui/draw/bitmap.h"
#include <opencv2/imgproc/imgproc.hpp>

gui::draw::pixmap cvMat2pixmap (const cv::Mat& source);
