#include "color_key.h"

color_key::color_key (byte min, byte max)
  : min_max(min, max)
{
  on_create([&, min, max] (gui::win::window*, const gui::core::rectangle&) {
    min_max.create(*this);
    min_scroll.create(*this);
    min_scroll.set_min_max(min, max);
    max_scroll.create(*this);
    max_scroll.set_min_max(min, max);
  });
  min_scroll.on_scroll([&] (const gui::core::point::type) {
    min_max.min_edit.set_text(ostreamfmt((int)min_scroll.get_value()));
  });
  max_scroll.on_scroll([&] (const gui::core::point::type) {
    min_max.max_edit.set_text(ostreamfmt((int)max_scroll.get_value()));
  });
  min_max.min_edit.on_content_changed([&] () {
    gui::ctrl::scroll_bar::type value = 0;
    std::istringstream(min_max.min_edit.get_text()) >> value;
    min_scroll.set_value(value, false);
  });
  min_max.max_edit.on_content_changed([&] () {
    gui::ctrl::scroll_bar::type value = 0;
    std::istringstream(min_max.max_edit.get_text()) >> value;
    max_scroll.set_value(value, false);
  });
}
// --------------------------------------------------------------------------
void color_key::set (const std::string& name, const data::range& value) {
  min_max.main_label.set_text(name);
  set(value);
}
// --------------------------------------------------------------------------
void color_key::set (const data::range& value) {
  set_min(value.min());
  set_max(value.max());
}
// --------------------------------------------------------------------------
data::range color_key::get () const {
  return data::range(get_min(), get_max());
}
// --------------------------------------------------------------------------
void color_key::set_min (byte value) {
  min_max.min_edit.set_text(ostreamfmt((int)value));
  min_scroll.set_value(value, false);
}
// --------------------------------------------------------------------------
void color_key::set_max (byte value) {
  min_max.max_edit.set_text(ostreamfmt((int)value));
  max_scroll.set_value(value, false);
}
// --------------------------------------------------------------------------
void color_key::set_range (byte min, byte max) {
  set_min(min);
  set_max(max);
}
// --------------------------------------------------------------------------
gui::byte color_key::get_min () const {
  return (byte)min_scroll.get_value();
}
// --------------------------------------------------------------------------
gui::byte color_key::get_max () const {
  return (byte)max_scroll.get_value();
}
