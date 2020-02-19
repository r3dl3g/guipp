
#pragma once

#include "image_view.h"
#include "opencv2/core/types.hpp"

namespace view {

  class color_range_view : public image_view {
  public:
    typedef image_view super;

    color_range_view ();

    void set_hsv_color_range (const cv::Vec3b& min, const cv::Vec3b& max);

  private:
    void update ();

    cv::Vec3b min;
    cv::Vec3b max;

  };

} // namespace view
