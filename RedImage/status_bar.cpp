
#include "status_bar.h"

view::status_bar::status_bar () {
  on_create([&] (gui::win::window*, const gui::core::rectangle&) {
    left_view.create(*this);
    color.create(left_view);
    side_bar_toggle.create(left_view);
    left_view.get_layout().add({gui::layout::lay(side_bar_toggle), gui::layout::lay(color)});
    get_layout().add(gui::layout::lay(left_view));
    for (status_label& l : labels) {
      l.create(*this);
      get_layout().add(gui::layout::lay(l));
    }
    set_children_visible();
  });
}
