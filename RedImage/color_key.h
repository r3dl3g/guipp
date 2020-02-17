
#pragma once

#include "range.h"

#include <gui/layout/layout_container.h>
#include <gui/layout/lineup_layout.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/edit.h>
#include <gui/ctrl/scroll_bar.h>

namespace view {

  class color_key : public gui::win::group_window<gui::layout::horizontal_lineup<37>,
                                                  gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::horizontal_lineup<37>,
                                   gui::color::very_very_light_gray> super;
    typedef gui::byte byte;

    color_key (byte min = 0, byte max = 255);

    void create (const gui::win::container& parent, const std::string& name);

    void set (const std::string& name, const data::range& value);
    void set (const data::range& value);
    void add (const data::range& value);
    void set_min (byte value);
    void set_max (byte value);
    void set_range (byte min, byte max);
    bool in_range (byte value) const;

    data::range get () const;
    byte get_min () const;
    byte get_max () const;

    gui::ctrl::label main_label;
    gui::ctrl::label_right min_label;
    gui::ctrl::edit_left min_edit;
    gui::ctrl::horizontal_scroll_bar min_scroll;
    gui::ctrl::label_right max_label;
    gui::ctrl::edit_left max_edit;
    gui::ctrl::horizontal_scroll_bar max_scroll;
  };

} // namespace view
