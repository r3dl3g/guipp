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

  colormap data2colormap (const char* data, const int bits_per_pixel, const int bytes_per_line, const int width, const int height);
  colormap pixmap2colormap (const gui::draw::basic_map&);

  template<gui::pixel_format_t T>
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

  template<gui::pixel_format_t T>
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

  inline colormap CM(const std::vector<colorline>& i) {
    return colormap(i);
  }

  inline graysmap GM(const std::vector<grayline>& i) {
    return graysmap(i);
  }

  typedef std::vector<std::string> pixmap_str;

  pixmap_str pixmap2string (const gui::draw::pixmap&);

  pixmap_str mk_str(const std::vector<std::string>&);

  // --------------------------------------------------------------------------
  constexpr gui::os::color _ = gui::color::black;
  constexpr gui::os::color R = gui::color::red;
  constexpr gui::os::color G = gui::color::green;
  constexpr gui::os::color B = gui::color::blue;
  constexpr gui::os::color W = gui::color::white;
  constexpr gui::os::color Y = gui::color::medium_gray;
  constexpr gui::os::color V = gui::color::very_light_gray;
  constexpr gui::os::color M = gui::color::very_very_light_gray;
  constexpr gui::os::color D = gui::color::light_gray;
  constexpr gui::os::color L = gui::color::gray;

  // --------------------------------------------------------------------------
} // namespace testing

namespace std {

  ostream& operator<< (ostream&, const testing::colormap&);
  ostream& operator<< (ostream&, const testing::graysmap&);
  ostream& operator<< (ostream&, const testing::pixmap_str&);


  // --------------------------------------------------------------------------
} // namespace std
