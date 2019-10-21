
#pragma once

#include "image_info.h"

#include <gui/ctrl/tile_view.h>

class thumb_view : public gui::ctrl::vertical_tile_view {
public:
  typedef gui::ctrl::vertical_tile_view super;
  typedef gui::ctrl::simple_list_data<image_info, image_info::drawer> image_info_list;

  thumb_view ();

  void update_list ();

  image_info_list list;
};
