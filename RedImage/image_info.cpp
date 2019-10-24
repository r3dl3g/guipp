
#include "image_info.h"

#include <gui/draw/drawers.h>
#include <gui/draw/frames.h>
#include <iomanip>

using namespace gui;

image_info::image_info ()
  : image_info(sys_fs::path())
{}
// --------------------------------------------------------------------------
image_info::image_info (const sys_fs::path& filename)
  : image_info(filename, gui::draw::pixmap())
{}
// --------------------------------------------------------------------------
image_info::image_info (const sys_fs::path& filename, draw::pixmap&& image)
  : good(0)
  , bad(0)
  , off(0)
  , quality(0)
  , filename(filename)
  , image(std::move(image))
{}
// --------------------------------------------------------------------------
void image_info::set_info (float good, float bad, float off, float quality) {
  this->good = good;
  this->bad = bad;
  this->off = off;
  this->quality = quality;
}
// --------------------------------------------------------------------------
void image_info::drawer (const image_info& info,
                         const draw::graphics& g,
                         const core::rectangle& place,
                         const draw::brush& background,
                         bool selected,
                         bool hilited) {
  using namespace draw;
  if (selected) {
    g.fill(draw::rectangle(place), color::very_light_blue);
  } else if (hilited) {
    g.fill(draw::rectangle(place), color::very_light_cyan);
  } else {
    g.fill(draw::rectangle(place), background);
  }
  g.copy_from(info.image, place.top_left() + core::point(2, 2));
  core::size sz(150, 20);
  const auto p = place.top_left() + core::point(160, 5);
  const auto o = text_origin::vcenter_left;
  g.text(draw::text_box(info.filename.filename().string(), core::rectangle(p, sz), o), font::system(), color::black);
  g.text(draw::text_box(ostreamfmt("Good: " << std::fixed << std::setprecision(2) << info.good), core::rectangle(p + core::point(0, 20), sz), o), font::system(), color::black);
  g.text(draw::text_box(ostreamfmt("Bad: " << std::fixed << std::setprecision(2) << info.bad), core::rectangle(p + core::point(0, 40), sz), o), font::system(), color::black);
  g.text(draw::text_box(ostreamfmt("Off: " << std::fixed << std::setprecision(2) << info.off), core::rectangle(p + core::point(0, 60), sz), o), font::system(), color::black);
  g.text(draw::text_box(ostreamfmt("Quality: " << std::fixed << std::setprecision(2) << info.quality), core::rectangle(p + core::point(0, 80), sz), o), font::system(), color::black);
  frame::raised_deep_relief(g, place);
}