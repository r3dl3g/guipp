// SettingsFile Test: Test for reading and writing the camera settings file
//

#include "image_test_lib.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <iomanip>


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
#ifdef GUIPP_X11
    gui::core::native_size sz = img.native_size();
    XImage* xim = XGetImage(gui::core::global::get_instance(), img.get_id(), 0, 0, sz.width(), sz.height(), AllPlanes, ZPixmap);
    auto str = data2string(xim->data, xim->bits_per_pixel / 8, xim->bytes_per_line, xim->height);
    XDestroyImage(xim);
#elif GUIPP_WIN
    BITMAP bmi;
    GetObject(img.get_id(), sizeof (BITMAP), &bmi);
    gui::blob data;
    data.resize(bmi.bmHeight * bmi.bmWidthBytes);
    GetBitmapBits(img.get_id(), (LONG)data.size(), data.data());
    auto str = data2string((const char*)data.data(), bmi.bmBitsPixel / 8, bmi.bmWidthBytes, bmi.bmHeight);
#elif GUIPP_QT
    auto pic = img.get_id()->toImage();
    auto str = data2string((const char*)pic.constBits(), pic.depth() / 8, pic.bytesPerLine(), pic.height());
#endif
    return str;
  }

  pixmap_str mk_str(std::initializer_list<std::string> i) {
    return pixmap_str(i);
  }

  // --------------------------------------------------------------------------
  colormap data2colormap (char const* raw_data, const int bits_per_pixel, const int bytes_per_line, const int width, const int height) {
    colormap result;
    result.reserve(height);
    for (int y = 0; y < height; ++y) {
      colorline line;
      line.reserve(width);
      int y_offset = y * bytes_per_line;
      switch (bits_per_pixel) {
        case 32:
          for (int x = 0; x < width * 4; x += 4) {
            line.push_back(*reinterpret_cast<const uint32_t*>(raw_data + y_offset + x));
          }
          break;
        case 24:
          for (int x = 0; x < width * 3; x += 3) {
            line.push_back(*reinterpret_cast<const uint32_t*>(raw_data + y_offset + x) & 0x00ffffff);
          }
          break;
        case 8:
          for (int x = 0; x < width; ++x) {
            uint32_t gray = *reinterpret_cast<const unsigned char*>(raw_data + y_offset + x);
            line.push_back(gray | gray << 8 | gray << 16);
          }
          break;
        case 1: {
          const char* scanline = raw_data + y_offset;
          for (int x = 0; x < width; ++x) {
            const char bits = scanline[x >> 3];
//#ifdef WIN32
//            const char test_bit = (bits >> (~x & 7)) & 1;
//#else
//            const char test_bit = (bits >> (x & 7)) & 1;
//#endif // WIN32
            const int bit = gui::core::get_bit(bits, x & 7);
            line.push_back(gui::color::system_bw_colors::value[bit]);
          }
          break;
        }
      }
      result.push_back(line);
    }
    return result;
  }

  colormap pixmap2colormap (const gui::draw::basic_map& map) {
#ifdef GUIPP_X11
    gui::core::native_size sz = map.native_size();
    XImage* xim = XGetImage(gui::core::global::get_instance(), map.get_id(), 0, 0, sz.width(), sz.height(), AllPlanes, ZPixmap);
    auto result = data2colormap(xim->data, xim->bits_per_pixel, xim->bytes_per_line, xim->width, xim->height);
    XDestroyImage(xim);
    return result;
#elif GUIPP_WIN
    BITMAP bmp;
    GetObject(map.get_id(), sizeof (BITMAP), &bmp);
    gui::blob data;
    data.resize(bmp.bmWidthBytes * bmp.bmHeight);
    int res = GetBitmapBits(map.get_id(), (LONG)data.size(), data.data());
    if (res != data.size()) {
      std::cerr << "GetBitmapBits returned " << res << " expected:" << data.size() << std::endl;
    }
    //if (1 == bmp.bmBitsPixel) { // Windows bitmaps are inverted
    //  for (auto& c : data) {
    //    c = ~c;
    //  }
    //}
    return data2colormap((const char*)data.data(), bmp.bmBitsPixel, bmp.bmWidthBytes, bmp.bmWidth, bmp.bmHeight);
#elif GUIPP_QT
    QImage img = map.get_id()->toImage();
//    if (img.depth() == 1) {
//      img.invertPixels();
//    }
//      QImage gray = img.convertToFormat(QImage::Format_Grayscale8);
//      return data2colormap((const char*)gray.constBits(), gray.depth(), gray.bytesPerLine(), gray.width(), gray.height());
//    } else {
      return data2colormap((const char*)img.constBits(), img.depth(), img.bytesPerLine(), img.width(), img.height());
//    }
#endif
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
} // namespace testing

// --------------------------------------------------------------------------
namespace std {

  // --------------------------------------------------------------------------
  ostream& operator<< (ostream& out, const testing::colormap& m) {
    out << endl << '{' << endl;
    //bool first_line = true;
    for(const auto& line: m) {
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
          case testing::_ & 0xffffff: out << "_"; break;
          case testing::R & 0xffffff: out << "R"; break;
          case testing::G & 0xffffff: out << "G"; break;
          case testing::B & 0xffffff: out << "B"; break;
          case testing::W & 0xffffff: out << "W"; break;
          case testing::Y & 0xffffff: out << "Y"; break;
          case testing::V & 0xffffff: out << "V"; break;
          case testing::M & 0xffffff: out << "M"; break;
          case testing::D & 0xffffff: out << "D"; break;
          case testing::L & 0xffffff: out << "L"; break;
          default:
            out << "0x" << setw(6) << setfill('0') << hex << (v & 0xffffff);
            break;
        }
      }
      out << "}," << endl;
    }
    out << '}';
    return out;
  }

  // --------------------------------------------------------------------------
  ostream& operator<< (ostream& out, const testing::graysmap& m) {
    out << '[';
    bool first_line = true;
    for(const auto& line: m) {
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
      out << ']' << endl;
    }
    out << ']';
    return out;
  }

  ostream& operator<< (ostream& o, const testing::pixmap_str& v) {
    for (const auto& i : v) {
      o << i << endl;
    }
    return o;
  }

  // --------------------------------------------------------------------------
} // namespace std
