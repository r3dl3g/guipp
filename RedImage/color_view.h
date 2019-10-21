
#pragma once

#include <gui/ctrl/control.h>
#include <gui/win/container.h>

class color_view : public gui::ctrl::control {
public:
  typedef gui::ctrl::control super;
  typedef gui::ctrl::no_erase_window_class<color_view> clazz;
  typedef gui::byte byte;

  color_view ();

  void create (const gui::win::container& parent,
               const gui::core::rectangle& place = gui::core::rectangle::def);

  void paint (const gui::draw::graphics& graph);
  void set_rgb_color (byte r, byte g, byte b);
  void set_hsv_color (byte h, byte s, byte v);

  gui::os::color value;
};
