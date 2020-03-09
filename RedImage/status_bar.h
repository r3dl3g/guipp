
#pragma once

#include "color_view.h"

#include <gui/layout/layout_container.h>
#include <gui/layout/adaption_layout.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/button.h>

namespace view {

  class status_bar : public gui::win::group_window<gui::layout::horizontal_adaption<2, 10>, gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::horizontal_adaption<2, 10>, gui::color::very_very_light_gray> super;

    using status_label = gui::ctrl::basic_label<gui::text_origin::vcenter_left,
    gui::draw::frame::sunken_deep_relief,
    gui::color::black,
    gui::color::very_very_light_gray>;

    status_bar ();

    enum {
      LABEL_COUNT = 4
    };

    gui::ctrl::animated_switch_button<> side_bar_toggle;
    color_view color;
    super left_view;
    status_label labels[LABEL_COUNT];
  };

} // namespace view
