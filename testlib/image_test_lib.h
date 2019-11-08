// testlib.h
//
#include <vector>
#include <gui/draw/bitmap.h>

namespace testing {

  typedef std::vector<gui::os::color> colorline;
  typedef std::vector<colorline> colormap;

  typedef std::vector<uint8_t> grayline;
  typedef std::vector<grayline> graysmap;

  bool operator== (const colorline&, const colorline&);
  bool operator== (const colormap&, const colormap&);

  colormap data2colormap (const char* data, const int bytes_per_pixel, const int bytes_per_line, const int height);

  colormap pixmap2colormap (const gui::draw::pixmap&);

  template<gui::PixelFormat T>
  colormap datamap2colormap (const gui::draw::datamap<T>& img) {
    auto data = img.get_data();
    colormap result;
    for (uint32_t y = 0; y < data.height(); ++y) {
      const auto row = data.row(y);
      colorline line;
      line.reserve(data.width());
      for (uint32_t x = 0; x < data.width(); ++x) {
        auto v = row[x];
        using namespace gui::pixel;
        line.emplace_back(gui::pixel::get_color(v));
      }
      result.emplace_back(line);
    }
    return result;
  }

  template<gui::PixelFormat T>
  gui::draw::datamap<T> colormap2datamap (const colormap& colors) {
    const auto width = colors[0].size();
    const auto height = colors.size();
    gui::draw::datamap<T> img(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    auto data = img.get_data();
    for (int y = 0; y < height; ++y) {
      auto row = data.row(y);
      const colorline& line = colors[y];
      for (int x = 0; x < width; ++x) {
        row[x] = gui::draw::to_pixel<T, gui::os::color>::to(line[x]);
      }
    }
    return img;
  }

  graysmap datamap2graysmap (const gui::draw::graymap&);
  gui::draw::graymap graysmap2datamap (const graysmap&);

  std::ostream& operator<< (std::ostream&, const colormap&);
  std::ostream& operator<< (std::ostream&, const graysmap&);

  inline colormap CM(std::initializer_list<colorline> i) {
    return colormap(i);
  }

  inline graysmap GM(std::initializer_list<grayline> i) {
    return graysmap(i);
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
  constexpr gui::os::color V = IF_WIN32_ELSE(gui::color::very_light_gray,  (gui::color::rgba<0xE0, 0xE0, 0xE0, 255>::value));
  constexpr gui::os::color M = IF_WIN32_ELSE(gui::color::medium_gray,  (gui::color::rgba<0xA0, 0xA0, 0xA0, 255>::value));
  constexpr gui::os::color D = IF_WIN32_ELSE(gui::color::dark_gray,  (gui::color::rgba<0x40, 0x40, 0x40, 255>::value));

  // --------------------------------------------------------------------------
}
