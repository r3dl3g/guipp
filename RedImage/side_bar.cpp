
#include "side_bar.h"

namespace view {

  side_bar::side_bar () {
    colors.resize(COLOR_COUNT);
    on_create([&] (gui::win::window*, const gui::core::rectangle&) {
      for (auto i = colors.begin(), e = colors.end(); i != e; ++i) {
        i->create(*this);
        get_layout().add(gui::layout::win(*i));
      }
    });
  }
  // --------------------------------------------------------------------------
  void side_bar::set (const data::color_sets& s) {
    for (auto i = colors.begin(), e = colors.end(); i != e; ++i) {
      i->set(s.ranges()[std::distance(colors.begin(), i)]);
    }
  }
  // --------------------------------------------------------------------------
  data::color_sets side_bar::get () const {
    data::color_sets sets;
    for (auto i = colors.begin(), e = colors.end(); i != e; ++i) {
      sets.ranges()[std::distance(colors.begin(), i)] = i->get();
    }
    return sets;
  }

} // namespace view
