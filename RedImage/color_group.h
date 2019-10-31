
#pragma once

#include "color_view.h"

#include <gui/layout/layout_container.h>
#include <gui/layout/lineup_layout.h>

namespace view {

  class color_group : public gui::win::group_window<gui::layout::horizontal_lineup<128, 1, 1>, gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::horizontal_lineup<128, 1, 1>, gui::color::very_very_light_gray> super;

    color_group () {
      on_create([&] (gui::win::window*, const gui::core::rectangle&) {
        min_color.create(*this);
        max_color.create(*this);
      });
    }

    color_view min_color;
    color_view max_color;
  };

} // namespace view
