
#pragma once

#include "color_key.h"
#include "color_group.h"
#include "hsv_range.h"

class color_key_group : public gui::win::group_window<gui::layout::vertical_lineup<60, 0, 1>, gui::color::very_very_light_gray> {
public:
  typedef gui::win::group_window<gui::layout::vertical_lineup<80, 0, 2>, gui::color::very_very_light_gray> super;

  color_key_group ();

  void set (const data::hsv_range&);
  data::hsv_range get () const;

  void reset ();
  void update_colors ();

  color_key hue;
  color_key saturation;
  color_key value;
  color_group colors;
};
