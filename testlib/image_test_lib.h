// testlib.h
//
#include <vector>
#include <gui/draw/bitmap.h>

namespace testing {

  typedef std::vector<uint32_t> colorline;

  typedef std::vector<colorline> colormap;

  colormap data2colormap (const char* data, const int bytes_per_pixel, const int bytes_per_line, const int height);

  colormap pixmap2colormap (const gui::draw::pixmap&);

  template<gui::PixelFormat T>
  colormap datamap2colormap (const gui::draw::datamap<T>&);

  template<> colormap datamap2colormap<gui::PixelFormat::GRAY> (const gui::draw::datamap<gui::PixelFormat::GRAY>&);
  template<> colormap datamap2colormap<gui::PixelFormat::RGB> (const gui::draw::datamap<gui::PixelFormat::RGB>&);
  template<> colormap datamap2colormap<gui::PixelFormat::RGBA> (const gui::draw::datamap<gui::PixelFormat::RGBA>&);
  template<> colormap datamap2colormap<gui::PixelFormat::BGR> (const gui::draw::datamap<gui::PixelFormat::BGR>&);
  template<> colormap datamap2colormap<gui::PixelFormat::BGRA> (const gui::draw::datamap<gui::PixelFormat::BGRA>&);
  template<> colormap datamap2colormap<gui::PixelFormat::ARGB> (const gui::draw::datamap<gui::PixelFormat::ARGB>&);
  template<> colormap datamap2colormap<gui::PixelFormat::ABGR> (const gui::draw::datamap<gui::PixelFormat::ABGR>&);

  std::ostream& operator<< (std::ostream&, const colormap&);

  inline colormap MM(std::initializer_list<colorline> i) {
    return colormap(i);
  }

  typedef std::vector<std::string> pixmap_str;

  pixmap_str pixmap2string (const gui::draw::pixmap&);

  pixmap_str mk_str(std::initializer_list<std::string>);

  std::ostream& operator<< (std::ostream&, const pixmap_str&);

  // --------------------------------------------------------------------------
  constexpr gui::os::color _ = IF_WIN32_ELSE(gui::color::black, (gui::color::rgba_gray<0, 255>::value));
  constexpr gui::os::color R = IF_WIN32_ELSE(gui::color::red,   (gui::color::rgba<0, 0, 255, 255>::value));
  constexpr gui::os::color G = IF_WIN32_ELSE(gui::color::green, (gui::color::rgba<0, 255, 0, 255>::value));
  constexpr gui::os::color B = IF_WIN32_ELSE(gui::color::blue,  (gui::color::rgba<255, 0, 0, 255>::value));
  constexpr gui::os::color W = IF_WIN32_ELSE(gui::color::white, (gui::color::rgba<255, 255, 255, 255>::value));
  constexpr gui::os::color Y = IF_WIN32_ELSE(gui::color::gray,  (gui::color::rgba<0x80, 0x80, 0x80, 255>::value));

  // --------------------------------------------------------------------------
}
