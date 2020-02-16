
#pragma once

#include <gui/ctrl/control.h>
#include <gui/win/container.h>
#include "opencv2/core/types.hpp"

namespace view {

  class color_view : public gui::ctrl::control {
  public:
    typedef gui::ctrl::control super;
    typedef gui::ctrl::no_erase_window_class<color_view> clazz;
    typedef gui::byte byte;

    color_view ();

    void create (const gui::win::container& parent,
                 const gui::core::rectangle& place = gui::core::rectangle::def);

    void paint (const gui::draw::graphics& graph);
    void set_rgb_color (const cv::Vec3b& rgb);
    void set_bgr_color (const cv::Vec3b& bgr);
    void set_hsv_color (const cv::Vec3b& hsv);

    gui::os::color value;
  };

} // namespace view
