
#pragma once

#include "range.h"
#include "min_max_group.h"

#include <gui/layout/layout_container.h>
#include <gui/layout/lineup_layout.h>
#include <gui/ctrl/scroll_bar.h>

namespace view {

  class scroll_bar_group : public gui::win::group_window<gui::layout::vertical_lineup<10>,
                                                         gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::vertical_lineup<10>,
                                   gui::color::very_very_light_gray> super;
    typedef gui::byte byte;

    scroll_bar_group (byte min = 0, byte max = 255);

    gui::ctrl::horizontal_scroll_bar min_scroll;
    gui::ctrl::horizontal_scroll_bar max_scroll;
  };

  class color_key : public gui::win::group_window<gui::layout::vertical_lineup<20>,
                                                  gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::vertical_lineup<20>,
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

    data::range get () const;
    byte get_min () const;
    byte get_max () const;

    min_max_group min_max;
    scroll_bar_group scrolls;
  };

} // namespace view
