
#include "side_bar.h"
#include <memory>

namespace view {

  side_bar::side_bar () {
    for (int i = 0; i< 2; ++i) {
      color_keys.emplace_back(std::unique_ptr<color_key_group>(new color_key_group()));
    }
    on_create([&] (gui::win::window*, const gui::core::rectangle&) {
      for (auto& i : color_keys) {
        i->create(*this);
        get_layout().add(gui::layout::lay(*i));
      }
    });
  }
  // --------------------------------------------------------------------------
  void side_bar::set (const data::color_sets& s) {
    get_layout().remove_all();
    color_keys.clear();
    while (s.count() > color_keys.size()) {
      color_keys.emplace_back(std::unique_ptr<color_key_group>(new color_key_group()));
    }
    int idx = 0;
    for (auto& i : color_keys) {
      i->create(*this);
      get_layout().add(gui::layout::lay(*i));
      i->set_name(s.ranges()[idx].name());
      i->set(s.ranges()[idx++]);
    }
    layout();
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

  void side_bar::add (const std::string& name) {
    color_key_group* grp = new color_key_group(name);
    color_keys.emplace_back(std::unique_ptr<color_key_group>(grp));
    grp->create(*this);
    get_layout().add(gui::layout::lay(grp));
    layout();
  }

} // namespace view
