
#include "thumb_view.h"

thumb_view::thumb_view () {
  set_item_size({ 304, 104 });
  set_background(gui::color::very_light_gray);
  set_border({ 10, 10 });
  set_spacing({ 5, 5 });
}
// --------------------------------------------------------------------------
void thumb_view::update_list () {
  set_drawer(list);
  set_count(list.size());
}
