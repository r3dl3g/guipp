
#include "min_max_group.h"

view::min_max_group::min_max_group (byte min, byte max) {
  on_create([&, min, max] (gui::win::window*, const gui::core::rectangle&) {
    main_label.create(*this);
    min_label.create(*this, "Min:");
    min_edit.create(*this, ostreamfmt((int)min));
    max_label.create(*this, "Max:");
    max_edit.create(*this, ostreamfmt((int)max));
  });
}
