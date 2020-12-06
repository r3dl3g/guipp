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
    gui::blob data;
    data.resize(bmi.bmHeight * bmi.bmWidthBytes);
    GetBitmapBits(img.get_id(), (LONG)data.size(), data.data());
    auto str = data2string((const char*)data.data(), bmi.bmBitsPixel / 8, bmi.bmWidthBytes, bmi.bmHeight);
#endif // WIN32
#ifdef QT_WIDGETS_LIB
    auto pic = img.get_id().toImage();
    auto str = data2string((const char*)pic.constBits(), pic.depth() / 8, pic.bytesPerLine(), pic.height());
#endif // QT_WIDGETS_LIB
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
    gui::blob data;
    data.resize(bmp.bmWidthBytes * bmp.bmHeight);
    int res = GetBitmapBits(img.get_id(), (LONG)data.size(), data.data());
    if (res != data.size()) {
      std::cerr << "GetBitmapBits returned " << res << " expected:" << data.size() << std::endl;
    }
    auto result = data2colormap((const char*)data.data(), bmp.bmBitsPixel / 8, bmp.bmWidthBytes, bmp.bmHeight);
#endif // WIN32
#ifdef QT_WIDGETS_LIB
    auto pic = img.get_id().toImage();
    auto result = data2colormap((const char*)pic.constBits(), pic.depth() / 8, pic.bytesPerLine(), pic.height());
#endif // QT_WIDGETS_LIB
    return result;
  }

  // --------------------------------------------------------------------------
  graysmap datamap2graysmap (const gui::draw::graymap& img) {
    auto data = img.get_data();
    graysmap result;
    for (uint32_t y = 0; y < data.height(); ++y) {
      const auto row = data.row(y);
      grayline line;
      line.reserve(data.width());
      for (uint32_t x = 0; x < data.width(); ++x) {
        line.emplace_back(row[x].value);
      }
      result.emplace_back(line);
    }
    return result;

  }

  gui::draw::graymap graysmap2datamap (const graysmap& colors) {
    const auto width = colors[0].size();
    const auto height = colors.size();
    gui::draw::graymap img(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    auto data = img.get_data();
    for (int y = 0; y < height; ++y) {
      auto row = data.row(y);
      const grayline& line = colors[y];
      for (int x = 0; x < width; ++x) {
        row[x] = line[x];
      }
    }
    return img;
  }

  // --------------------------------------------------------------------------
  std::ostream& operator<< (std::ostream& out, const colormap& m) {
    out << std::endl << '{' << std::endl;
    //bool first_line = true;
    for(const colorline& line: m) {
      //if (first_line) {
      //  first_line = false;
      //} else {
      //  out << ",";
      //}
      out << '{';
      bool first = true;
      for(const uint32_t v: line) {
        if (first) {
          first = false;
        } else {
          out << ",";
        }
        switch (v & 0xffffff) {
          case _ & 0xffffff: out << "_"; break;
          case R & 0xffffff: out << "R"; break;
          case G & 0xffffff: out << "G"; break;
          case B & 0xffffff: out << "B"; break;
          case W & 0xffffff: out << "W"; break;
          case Y & 0xffffff: out << "Y"; break;
          case V & 0xffffff: out << "V"; break;
          case M & 0xffffff: out << "M"; break;
          case D & 0xffffff: out << "D"; break;
          default:
            out << std::setw(8) << std::setfill('0') << std::hex << v;
            break;
        }
      }
      out << "}," << std::endl;
    }
    out << '}';
    return out;
  }

  // --------------------------------------------------------------------------
  std::ostream& operator<< (std::ostream& out, const graysmap& m) {
    out << '[';
    bool first_line = true;
    for(const grayline& line: m) {
      if (first_line) {
        first_line = false;
      } else {
        out << ",";
      }
      out << '[';
      bool first = true;
      for(const uint8_t v: line) {
        if (first) {
          first = false;
        } else {
          out << ",";
        }
        out << (int)v;
      }
      out << ']' << std::endl;
    }
    out << ']';
    return out;
  }

  // --------------------------------------------------------------------------
  bool operator== (const colorline& lhs, const colorline& rhs) {
    if (lhs.size() != rhs.size()) {
      return false;
    }
    for (int i = 0; i < lhs.size(); ++i) {
      if (gui::color::remove_transparency(lhs[i]) != gui::color::remove_transparency(rhs[i])) {
        return false;
      }
    }
    return true;
  }
  // --------------------------------------------------------------------------
  bool operator== (const colormap& lhs, const colormap& rhs) {
    if (lhs.size() != rhs.size()) {
      return false;
    }
    for (int i = 0; i < lhs.size(); ++i) {
      if (!(lhs[i] == rhs[i])) {
        return false;
      }
    }
    return true;

  }
  // --------------------------------------------------------------------------

}
// --------------------------------------------------------------------------

