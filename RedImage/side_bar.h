
#pragma once

#include "color_key_group.h"
#include "color_sets.h"

namespace view {

  class side_bar : public gui::win::group_window<gui::layout::vertical_lineup<164, 0, 2>, gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::vertical_lineup<164, 0, 10>, gui::color::very_very_light_gray> super;

    enum {
      COLOR_COUNT = 2
    };

    side_bar ();

    void set (const data::color_sets&);
    data::color_sets get () const;

    std::vector<color_key_group> colors;

  };

} // namespace view
