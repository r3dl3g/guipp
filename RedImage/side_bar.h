
#pragma once

#include "color_key_group.h"
#include "color_sets.h"

namespace view {

  class side_bar : public gui::win::group_window<gui::layout::vertical_lineup<84, 0, 2, 2>, gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::vertical_lineup<84, 0, 2, 2>, gui::color::very_very_light_gray> super;
    typedef std::vector<std::unique_ptr<color_key_group>> color_key_group_list;

    side_bar ();

    void init_sets ();

    void set (const data::color_sets&);
    data::color_sets get () const;
    void add (const std::string&);

    color_key_group_list color_keys;

  };

} // namespace view
