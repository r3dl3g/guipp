#include "color_key.h"

namespace view {

  color_key::color_key (byte min, byte max)
  {
    on_create([&, min, max] (gui::win::window*, const gui::core::rectangle&) {
      main_label.create(*this);
      min_label.create(*this, "Min:");
      min_edit.create(*this, ostreamfmt((int)min));
      min_edit.set_text_filter(gui::ctrl::filter::unsigned_filter);
      min_scroll.set_min_max_step_value(min, max, 1, min);
      min_scroll.create(*this);

      max_label.create(*this, "Max:");
      max_edit.create(*this, ostreamfmt((int)max));
      max_edit.set_text_filter(gui::ctrl::filter::unsigned_filter);
      max_scroll.set_min_max_step_value(min, max, 1, max);
      max_scroll.create(*this);

      get_layout().add({&main_label, &min_label, &min_edit, &min_scroll, &max_label, &max_edit, &max_scroll});
    });
    min_scroll.on_scroll([&] (const gui::core::point::type) {
      min_edit.set_text(ostreamfmt((int)min_scroll.get_value()));
    });
    max_scroll.on_scroll([&] (const gui::core::point::type) {
      max_edit.set_text(ostreamfmt((int)max_scroll.get_value()));
    });
    min_edit.on_content_changed([&] () {
      gui::ctrl::scroll_bar::type value = 0;
      std::istringstream(min_edit.get_text()) >> value;
      min_scroll.set_value(value, false);
    });
    max_edit.on_content_changed([&] () {
      gui::ctrl::scroll_bar::type value = 0;
      std::istringstream(max_edit.get_text()) >> value;
      max_scroll.set_value(value, false);
    });
  }
  // --------------------------------------------------------------------------
  void color_key::create (const gui::win::container& parent, const std::string& name) {
    super::create(parent);
    main_label.set_text(name);
  }
  // --------------------------------------------------------------------------
  void color_key::set (const std::string& name, const data::range& value) {
    main_label.set_text(name);
    set(value);
  }
  // --------------------------------------------------------------------------
  void color_key::set (const data::range& value) {
    set_min(value.min());
    set_max(value.max());
  }
  // --------------------------------------------------------------------------
  void color_key::add (const data::range& value) {
    set_min(std::min(value.min(), get_min()));
    set_max(std::max(value.max(), get_max()));
  }
  // --------------------------------------------------------------------------
  data::range color_key::get () const {
    return data::range(get_min(), get_max());
  }
  // --------------------------------------------------------------------------
  void color_key::set_min (byte value) {
    min_edit.set_text(ostreamfmt((int)value));
    min_scroll.set_value(value, false);
  }
  // --------------------------------------------------------------------------
  void color_key::set_max (byte value) {
    max_edit.set_text(ostreamfmt((int)value));
    max_scroll.set_value(value, false);
  }
  // --------------------------------------------------------------------------
  bool color_key::in_range (byte value) const {
    if (get_max() < get_min()) {
      return ((get_min() <= value) || (value <= get_max()));
    } else {
      return ((get_min() <= value) && (value <= get_max()));
    }
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

} // namespace view
