
#pragma once

#include "image_view.h"

#include <gui/win/layout_container.h>
#include <gui/win/grid_layout.h>

template<int N>
class over_view : public gui::win::group_window<gui::layout::grid_adaption<N/2, 2, 0, 1>, gui::color::very_very_light_gray> {
public:
  typedef gui::win::group_window<gui::layout::grid_adaption<N/2, 2, 0, 1>, gui::color::very_very_light_gray> super;

  over_view ();

  image_view image_views[N];

};

template<int N>
over_view<N>::over_view () {
  super::on_create([&] (gui::win::window*, const gui::core::rectangle&) {
    for(int i = 0; i < N; ++i) {
      image_views[i].create(*this);
    }
  });
}
