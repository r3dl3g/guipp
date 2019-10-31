
#pragma once

#include "image_view.h"

#include <gui/layout/layout_container.h>
#include <gui/layout/grid_layout.h>

namespace view {

  template<int N, int M>
  class over_view : public gui::win::group_window<gui::layout::grid_adaption<N, M, 0, 1>, gui::color::very_very_light_gray> {
  public:
    typedef gui::win::group_window<gui::layout::grid_adaption<N, M, 0, 1>, gui::color::very_very_light_gray> super;

    static const int count = N * M;

    over_view ();

    void set_images (const cv::Mat source[count]);

    image_view image_views[count];

  };

  template<int N, int M>
  over_view<N, M>::over_view () {
    super::on_create([&] (gui::win::window*, const gui::core::rectangle&) {
      for(int i = 0; i < count; ++i) {
        image_views[i].create(*this);
        super::get_layout().add(gui::layout::win(image_views[i]));
      }
    });
  }

  template<int N, int M>
  void over_view<N, M>::set_images (const cv::Mat img[count]) {
    for(int i = 0; i < count; ++i) {
      image_views[i].set_image_and_scale(img[i]);
    }
  }

} // namespace view
