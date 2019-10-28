
#include "color_key_group.h"

namespace view {

  color_key_group::color_key_group ()
    : hue(0, 180)
    , saturation()
    , value()
  {
    on_create([&] (gui::win::window*, const gui::core::rectangle&) {
      hue.create(*this);
      saturation.create(*this);
      value.create(*this);
      colors.create(*this);
    });
  }
  // --------------------------------------------------------------------------
  void color_key_group::set (const data::hsv_range& hsv) {
    hue.set("H", hsv.hue());
    saturation.set("S", hsv.saturation());
    value.set("V", hsv.value());
    update_colors();
  }
  // --------------------------------------------------------------------------
  void color_key_group::reset () {
    hue.set_range(0, 180);
    saturation.set_range(0, 255);
    value.set_range(0, 255);
    update_colors();
  }
  // --------------------------------------------------------------------------
  void color_key_group::update_colors () {
    colors.min_color.set_hsv_color(hue.get_min(), saturation.get_min(), value.get_min());
    colors.max_color.set_hsv_color(hue.get_max(), saturation.get_max(), value.get_max());
  }
  // --------------------------------------------------------------------------
  data::hsv_range color_key_group::get () const {
    return data::hsv_range(hue.get(), saturation.get(), value.get());
  }

} // namespace view
