
#pragma once

#include "color_range_view.h"

#include <gui/ctrl/label.h>
#include <gui/ctrl/std_dialogs.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/grid_layout.h>

namespace view {

  class color_group : public gui::win::group_window<gui::layout::grid_adaption<1, 2, 1, 0>, gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::grid_adaption<1, 2, 1, 0>, gui::color::very_very_light_gray> super;

    color_group () {
      on_create([&] (gui::win::window*, const gui::core::rectangle&) {
        label.create(*this);
        color.create(*this);
        get_layout().add({&label, &color});
        label.on_left_btn_dblclk([&] (gui::os::key_state, gui::core::point) {
          gui::ctrl::input_dialog::ask(*this, "Change Filter name", "Please enter new name of the filter",
                                       get_label(), "Ok", "Cancel", [&] (const std::string& name) {
            set_label(name);
          });
        });
      });
    }

    void set_label (const std::string& l) {
      label.set_text(l);
    }

    std::string get_label () const {
      return label.get_text();
    }

    void set_colors (const cv::Vec3b& min, const cv::Vec3b& max) {
      color.set_hsv_color_range(min, max);
    }

  private:
    gui::ctrl::label label;
    color_range_view color;
  };

} // namespace view
