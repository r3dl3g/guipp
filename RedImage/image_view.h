
#pragma once

#include <opencv2/core/core.hpp>

#include <gui/ctrl/control.h>
#include <gui/win/container.h>
#include "gui/draw/bitmap.h"

class image_view : public gui::ctrl::control {
public:
  typedef gui::ctrl::control super;
  typedef gui::ctrl::no_erase_window_class<image_view> clazz;

  image_view ();

  void create (const gui::win::container& parent,
               const gui::core::rectangle& place = gui::core::rectangle::def);

  void paint (const gui::draw::graphics& graph);
  void set_image (const gui::draw::pixmap& source);
  void set_image_and_scale (const cv::Mat& source);

private:
  gui::draw::pixmap image;
};
