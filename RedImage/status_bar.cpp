
#include "status_bar.h"

view::status_bar::status_bar () {
  on_create([&] (gui::win::window*, const gui::core::rectangle&) {
    for (status_label& l : labels) {
      l.create(*this);
      get_layout().add(gui::layout::win(l));
    }
    color.create(*this);
    get_layout().add(gui::layout::win(color));
    set_children_visible();
  });
}
