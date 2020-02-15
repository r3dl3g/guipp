
#pragma once

#include <gui/layout/layout_container.h>
#include <gui/layout/lineup_layout.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/edit.h>

namespace view {

  class min_max_group : public gui::win::group_window<gui::layout::horizontal_lineup<52>,
                                                      gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::horizontal_lineup<52>,
                                   gui::color::very_very_light_gray> super;
    typedef gui::byte byte;

    min_max_group (byte min = 0, byte max = 255);

    gui::ctrl::label main_label;
    gui::ctrl::label_right min_label;
    gui::ctrl::edit_left min_edit;
    gui::ctrl::label_right max_label;
    gui::ctrl::edit_left max_edit;
  };

} // namespace view
