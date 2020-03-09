
#include "side_bar.h"
#include <memory>

namespace view {

  // --------------------------------------------------------------------------
  side_bar::side_bar () {
    for (int i = 0; i< 2; ++i) {
      color_keys.emplace_back(std::unique_ptr<color_key_group>(new color_key_group()));
    }
    for (auto& i : color_keys) {
      i->set(data::hsv_range());
    }
    on_create([&] (gui::win::window*, const gui::core::rectangle&) {
      init_sets ();
    });
  }
  // --------------------------------------------------------------------------
  void side_bar::init_sets () {
    get_layout().remove_all();
    for (auto& i : color_keys) {
      if (!i->is_valid()) {
        i->create(*this);
      }
      get_layout().add(gui::layout::lay(*i));
      i->colors.remove_button.on_clicked([&] () {
        gui::ctrl::yes_no_dialog::ask(*this, "Remove Filter",
                                      ostreamfmt("Do you really want to remove filter '" << i->get_name() << "'?"),
                                      "Yes", "No", [&] (bool yes_answered) {
          if (yes_answered) {
            gui::win::run_on_main([&] () {
              get_layout().remove_all();
              auto it = std::find(color_keys.begin(), color_keys.end(), i);
              if (it != color_keys.end()) {
                color_keys.erase(it);
              }
              for (auto& c : color_keys) {
                get_layout().add(gui::layout::lay(c.get()));
              }
              resize(gui::core::size(size().width(), (layout_type::dimension + layout_type::gap + layout_type::separatior_width) * color_keys.size()));
//              layout();
            });
          }
        });
      });
    }
  }
  // --------------------------------------------------------------------------
  void side_bar::set (const data::color_sets& s) {
    get_layout().remove_all();
    color_keys.clear();
    while (s.count() > color_keys.size()) {
      color_keys.emplace_back(std::unique_ptr<color_key_group>(new color_key_group()));
    }
    init_sets();
    layout();
    int idx = 0;
    for (auto& i : color_keys) {
      i->set_name(s.ranges()[idx].name());
      i->set(s.ranges()[idx++]);
    }
  }
  // --------------------------------------------------------------------------
  data::color_sets side_bar::get () const {
    data::color_sets sets;
    sets.count(color_keys.size());
    sets.ranges().resize(color_keys.size());
    int idx = 0;
    for (auto& i : color_keys) {
      sets.ranges()[idx++] = i->get();
    }
    return sets;
  }
  // --------------------------------------------------------------------------
  void side_bar::add (const std::string& name) {
    color_key_group* grp = new color_key_group(name);
    color_keys.emplace_back(std::unique_ptr<color_key_group>(grp));
    init_sets ();
    layout();
  }
  // --------------------------------------------------------------------------

} // namespace view
