// SettingsFile Test: Test for reading and writing the camera settings file
//

#include "image_test_lib.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <iomanip>

using namespace std;

namespace testing {

  pixmap_str data2string (const char* data, const int bytes_per_pixel, const int bytes_per_line, const int height) {
    pixmap_str result;
    for (int y = 0; y < height; ++y) {
      std::ostringstream buffer;
      //    buffer << y << ':';
      int y_offset = y * bytes_per_line;
      for (int x = 0; x < bytes_per_line; x += bytes_per_pixel) {
        if (x > 0) {
          buffer << ' ';
        }
        for (int j = 0; j < bytes_per_pixel; ++j) {
          buffer << std::setw(2) << std::setfill('0') << std::hex << (unsigned short)(unsigned char)data[y_offset + x + j];
        }
      }
      result.emplace_back(buffer.str());
    }
    return result;
  }

  pixmap_str pixmap2string (const gui::draw::pixmap& img) {
  #ifdef X11
    gui::core::native_size sz = img.native_size();
    XImage* xim = XGetImage(gui::core::global::get_instance(), img.get_id(), 0, 0, sz.width(), sz.height(), AllPlanes, ZPixmap);
    auto str = data2string(xim->data, xim->bits_per_pixel / 8, xim->bytes_per_line, xim->height);
    XDestroyImage(xim);
  #endif // X11
  #ifdef WIN32
    BITMAP bmi;
    GetObject(img.get_id(), sizeof (BITMAP), &bmi);
    blob data;
    data.resize(bmi.bmHeight * bmi.bmWidthBytes);
    GetBitmapBits(img.get_id(), (LONG)data.size(), data.data());
    auto str = data2string((const char*)data.data(), bmi.bmBitsPixel / 8, bmi.bmWidthBytes, bmi.bmHeight);
  #endif // WIN32
    return str;
  }

  pixmap_str mk_str(std::initializer_list<string> i) {
    return pixmap_str(i);
  }

  std::ostream& operator<< (std::ostream& o, const pixmap_str& v) {
    for (const auto& i : v) {
      o << i << std::endl;
    }
    return o;
  }
  // --------------------------------------------------------------------------
  colormap data2colormap (const char* data, const int bytes_per_pixel, const int bytes_per_line, const int height) {
    colormap result;
    for (int y = 0; y < height; ++y) {
      colorline line;
      line.reserve(bytes_per_line / bytes_per_pixel);
      int y_offset = y * bytes_per_line;
      for (int x = 0; x < bytes_per_line; x += bytes_per_pixel) {
        line.emplace_back(*reinterpret_cast<const uint32_t*>(data + y_offset + x));
      }
      result.emplace_back(line);
    }
    return result;
  }

  colormap pixmap2colormap (const gui::draw::pixmap& img) {
  #ifdef X11
    gui::core::native_size sz = img.native_size();
    XImage* xim = XGetImage(gui::core::global::get_instance(), img.get_id(), 0, 0, sz.width(), sz.height(), AllPlanes, ZPixmap);
    auto result = data2colormap(xim->data, xim->bits_per_pixel / 8, xim->bytes_per_line, xim->height);
    XDestroyImage(xim);
  #endif // X11
  #ifdef WIN32
    BITMAP bmp;
    GetObject(img.get_id(), sizeof (BITMAP), &bmp);
    blob data;
    data.resize(bmp.bmWidthBytes * bmp.bmHeight);
    int res = GetBitmapBits(img.get_id(), (LONG)data.size(), data.data());
    if (res != data.size()) {
      LogError << "GetBitmapBits returned " << res << " expected:" << data.size();
    }
    auto result = data2colormap((const char*)data.data(), bmp.bmBitsPixel / 8, bmp.bmWidthBytes, bmp.bmHeight);
  #endif // WIN32
    return result;
  }

  namespace detail {

    template<gui::PixelFormat T>
    colormap datamap2colormap (const gui::draw::datamap<T>& img) {
      auto data = img.get_data();
      colormap result;
      for (int y = 0; y < data.height(); ++y) {
        const auto row = data.row(y);
        colorline line;
        line.reserve(data.width());
        for (int x = 0; x < data.width(); ++x) {
          auto v = row[x];
          using namespace gui::pixel;
          line.emplace_back(gui::color::calc_rgb(get_red(v), get_green(v), get_blue(v)));
        }
        result.emplace_back(line);
      }
      return result;
    }

  }

  template<> colormap datamap2colormap<gui::PixelFormat::GRAY> (const gui::draw::datamap<gui::PixelFormat::GRAY>& img) {
    return detail::datamap2colormap<gui::PixelFormat::GRAY>(img);
  }

  template<> colormap datamap2colormap<gui::PixelFormat::RGB> (const gui::draw::datamap<gui::PixelFormat::RGB>& img) {
    return detail::datamap2colormap<gui::PixelFormat::GRAY>(img);
  }

  template<> colormap datamap2colormap<gui::PixelFormat::RGBA> (const gui::draw::datamap<gui::PixelFormat::RGBA>& img) {
    return detail::datamap2colormap<gui::PixelFormat::GRAY>(img);
  }

  template<> colormap datamap2colormap<gui::PixelFormat::BGR> (const gui::draw::datamap<gui::PixelFormat::BGR>& img) {
    return detail::datamap2colormap<gui::PixelFormat::GRAY>(img);
  }

  template<> colormap datamap2colormap<gui::PixelFormat::BGRA> (const gui::draw::datamap<gui::PixelFormat::BGRA>& img) {
    return detail::datamap2colormap<gui::PixelFormat::GRAY>(img);
  }

  template<> colormap datamap2colormap<gui::PixelFormat::ARGB> (const gui::draw::datamap<gui::PixelFormat::ARGB>& img) {
    return detail::datamap2colormap<gui::PixelFormat::GRAY>(img);
  }

  template<> colormap datamap2colormap<gui::PixelFormat::ABGR> (const gui::draw::datamap<gui::PixelFormat::ABGR>& img) {
    return detail::datamap2colormap<gui::PixelFormat::GRAY>(img);
  }

  std::ostream& operator<< (std::ostream& out, const colormap& m) {
    out << '[';
    bool first_line = true;
    for(const colorline& line: m) {
      if (first_line) {
        first_line = false;
      } else {
        out << ",";
      }
      out << '[';
      bool first = true;
      for(const uint32_t v: line) {
        if (first) {
          first = false;
        } else {
          out << ",";
        }
        switch (v) {
          case _: out << "_"; break;
          case R: out << "R"; break;
          case G: out << "G"; break;
          case B: out << "O"; break;
          case W: out << "W"; break;
          case Y: out << "Y"; break;
          default:
            out << std::setw(8) << std::setfill('0') << std::hex << v;
            break;
        }
      }
      out << ']';
    }
    out << ']';
    return out;
  }

}
