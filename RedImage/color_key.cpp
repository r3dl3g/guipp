#include "color_key.h"

namespace view {

  scroll_bar_group::scroll_bar_group (byte min, byte max) {
    on_create([&, min, max] (gui::win::window*, const gui::core::rectangle&) {
      min_scroll.create(*this);
      min_scroll.set_min_max(min, max);
      max_scroll.create(*this);
      max_scroll.set_min_max(min, max);
      get_layout().add({&min_scroll, &max_scroll});
    });
  }

  color_key::color_key (byte min, byte max)
    : min_max(min, max)
    , scrolls(min, max)
  {
    on_create([&] (gui::win::window*, const gui::core::rectangle&) {
      min_max.create(*this);
      scrolls.create(*this);
      get_layout().add({&min_max, &scrolls});
    });
    scrolls.min_scroll.on_scroll([&] (const gui::core::point::type) {
      min_max.min_edit.set_text(ostreamfmt((int)scrolls.min_scroll.get_value()));
    });
    scrolls.max_scroll.on_scroll([&] (const gui::core::point::type) {
      min_max.max_edit.set_text(ostreamfmt((int)scrolls.max_scroll.get_value()));
    });
    min_max.min_edit.on_content_changed([&] () {
      gui::ctrl::scroll_bar::type value = 0;
      std::istringstream(min_max.min_edit.get_text()) >> value;
      scrolls.min_scroll.set_value(value, false);
    });
    min_max.max_edit.on_content_changed([&] () {
      gui::ctrl::scroll_bar::type value = 0;
      std::istringstream(min_max.max_edit.get_text()) >> value;
      scrolls.max_scroll.set_value(value, false);
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
    scrolls.min_scroll.set_value(value, false);
  }
  // --------------------------------------------------------------------------
  void color_key::set_max (byte value) {
    min_max.max_edit.set_text(ostreamfmt((int)value));
    scrolls.max_scroll.set_value(value, false);
  }
  // --------------------------------------------------------------------------
  void color_key::set_range (byte min, byte max) {
    set_min(min);
    set_max(max);
  }
  // --------------------------------------------------------------------------
  gui::byte color_key::get_min () const {
    return (byte)scrolls.min_scroll.get_value();
  }
  // --------------------------------------------------------------------------
  gui::byte color_key::get_max () const {
    return (byte)scrolls.max_scroll.get_value();
  }

} // namespace view
