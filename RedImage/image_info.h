
#pragma once

#include <gui/draw/graphics.h>
#include <gui/draw/bitmap.h>
#include <gui/ctrl/file_tree.h>

struct image_info {

  image_info ();
  image_info (const sys_fs::path& filename);
  image_info (const sys_fs::path& filename, gui::draw::pixmap&& image);

  void set_info (float good, float bad, float off, float quality);

  gui::draw::pixmap image;

  float good;
  float bad;
  float off;
  float quality;

  sys_fs::path filename;

  static void drawer (const image_info& info,
                      const gui::draw::graphics& g,
                      const gui::core::rectangle& place,
                      const gui::draw::brush& background,
                      bool selected,
                      bool hilited);

};
