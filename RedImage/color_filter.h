
#pragma once

#include "hsv_range.h"

#include "opencv2/core/core.hpp"

namespace proc {

  struct color_filter {
    data::hsv_range range;

    color_filter ();
    color_filter (const data::hsv_range& range);

    cv::Mat calc (const cv::Mat& hsv_image, bool erode = false) const;
    cv::Mat calc (const cv::Mat& hsv_image, cv::Mat mask[8], bool erode = false) const;

    static float calc_portion (const cv::Mat& mask);
  };

}
