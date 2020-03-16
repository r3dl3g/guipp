
#include "status_bar.h"

view::status_bar::status_bar () {
  on_create([&] (gui::win::window*, const gui::core::rectangle&) {
    color.create(*this);
    side_bar_toggle.create(*this);
    get_layout().add({
      {gui::layout::lay(side_bar_toggle), 40, 0.0F},
      {gui::layout::lay(color), 60, 0.0F}
    });
    for (status_label& l : labels) {
      l.create(*this);
      get_layout().add({gui::layout::lay(l), 100, 1});
    }
    set_children_visible();
  });
}
