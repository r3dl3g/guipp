
#include "status_bar.h"

view::status_bar::status_bar () {
  on_create([&] (gui::win::window*, const gui::core::rectangle&) {
    for (status_label& l : labels) {
      l.create(*this);
    }
    color.create(*this);
    set_children_visible();
  });
}
