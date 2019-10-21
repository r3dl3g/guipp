
#pragma once

#include "hsv_range.h"

#include "opencv2/core/core.hpp"

struct color_filter {
  data::hsv_range range;

  color_filter ();
  color_filter (const data::hsv_range& range);

  float calc (const cv::Mat& hsv_image, bool erode = false);
  float calc (const cv::Mat& hsv_image, cv::Mat mask[8], bool erode = false);
};
