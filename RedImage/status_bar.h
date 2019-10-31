
#pragma once

#include "color_view.h"

#include <gui/layout/layout_container.h>
#include <gui/layout/adaption_layout.h>
#include <gui/ctrl/label.h>

namespace view {

  class status_bar : public gui::win::group_window<gui::layout::horizontal_adaption<2, 10>, gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::horizontal_adaption<2, 10>, gui::color::very_very_light_gray> super;

    using status_label = gui::ctrl::basic_label<gui::text_origin::vcenter_left,
    gui::draw::frame::sunken_deep_relief,
    gui::color::black,
    gui::color::very_very_light_gray>;

    status_bar ();

    status_label labels[4];
    color_view color;
  };

} // namespace view
