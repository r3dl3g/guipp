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
    XImage* xim = XGetImage(gui::core::global::get_instance(), img.get_os_bitmap(), 0, 0, sz.width(), sz.height(), AllPlanes, ZPixmap);
    auto str = data2string(xim->data, xim->bits_per_pixel / 8, xim->bytes_per_line, xim->height);
    XDestroyImage(xim);
#elif GUIPP_WIN
    BITMAP bmi;
    GetObject(img.get_os_bitmap(), sizeof (BITMAP), &bmi);
    gui::blob data;
    data.resize(bmi.bmHeight * bmi.bmWidthBytes);
    GetBitmapBits(img.get_os_bitmap(), (LONG)data.size(), data.data());
    auto str = data2string((const char*)data.data(), bmi.bmBitsPixel / 8, bmi.bmWidthBytes, bmi.bmHeight);
#elif GUIPP_QT
    auto pic = img.get_os_bitmap()->toImage();
    auto str = data2string((const char*)pic.constBits(), pic.depth() / 8, pic.bytesPerLine(), pic.height());
#endif
    return str;
  }

  pixmap_str mk_str(std::initializer_list<std::string> i) {
    return pixmap_str(i);
  }

#ifdef WIN32
  inline uint32_t flip_rgb (uint32_t n) {
    uint32_t flipped = ((n & 0xff) << 16) | (n & 0xff00) | ((n & 0xff0000) >> 16) | ((n & 0xff000000));
    return flipped;
  }
  inline uint32_t flip_rgba (uint32_t n) {
    uint32_t flipped = ((n & 0xff) << 16) | (n & 0xff00) | ((n & 0xff0000) >> 16) | ((n & 0xff000000));
    return flipped;
  }
#else
  inline uint32_t flip_rgb (uint32_t c) {
    return c;
  }
  inline uint32_t flip_rgba (uint32_t c) {
    return c;
  }
#endif // WIN32

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
            line.push_back(gui::pixel::rgb::build(flip_rgba(*reinterpret_cast<const uint32_t*>(raw_data + y_offset + x) & 0x00ffffff)));
          }
          break;
        case 24:
          for (int x = 0; x < width * 3; x += 3) {
            line.push_back(gui::pixel::rgb::build(flip_rgb(*reinterpret_cast<const uint32_t*>(raw_data + y_offset + x) & 0x00ffffff)));
          }
          break;
        case 8:
          for (int x = 0; x < width; ++x) {
            uint32_t gray = *reinterpret_cast<const unsigned char*>(raw_data + y_offset + x);
            line.push_back(gui::pixel::rgb::build(gray));
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
            line.push_back(gui::pixel::rgb::build(gui::color::system_bw_colors::value[bit]));
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
    XImage* xim = XGetImage(gui::core::global::get_instance(), map.get_os_bitmap(), 0, 0, sz.width(), sz.height(), AllPlanes, ZPixmap);
    auto result = data2colormap(xim->data, xim->bits_per_pixel, xim->bytes_per_line, xim->width, xim->height);
    XDestroyImage(xim);
    return result;
#elif GUIPP_WIN
    BITMAP bmp;
    GetObject(map.get_os_bitmap(), sizeof (BITMAP), &bmp);
    gui::blob data;
    data.resize(bmp.bmWidthBytes * bmp.bmHeight);
    int res = GetBitmapBits(map.get_os_bitmap(), (LONG)data.size(), data.data());
    if (res != data.size()) {
      std::cerr << "GetBitmapBits returned " << res << " expected:" << data.size() << std::endl;
    }
    return data2colormap((const char*)data.data(), bmp.bmBitsPixel, bmp.bmWidthBytes, bmp.bmWidth, bmp.bmHeight);
#elif GUIPP_QT
    QImage img = map.get_os_bitmap()->toImage();
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
        line.emplace_back(row[x]);
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

  colormap CM(const std::vector<std::vector<uint32_t>>& i) {
    colormap cm;
    cm.reserve(i.size());
    for (const auto& l : i) {
      colorline cl;
      cl.reserve(l.size());
      for (const auto& c : l) {
        cl.push_back(gui::pixel::rgb::build(c));
      }
      cm.push_back(cl);
    }
    return cm;
  }

  graysmap GM(const std::vector<std::vector<uint32_t>>& i) {
    graysmap cm;
    cm.reserve(i.size());
    for (const auto& l : i) {
      grayline cl;
      cl.reserve(l.size());
      for (const auto& c : l) {
        cl.push_back(gui::pixel::gray::build((gui::byte)(c & 0xff)));
      }
      cm.push_back(cl);
    }
    return cm;
  }


  // --------------------------------------------------------------------------
  bool operator== (const colorline& lhs, const colorline& rhs) {
    if (lhs.size() != rhs.size()) {
      return false;
    }
    for (int i = 0; i < lhs.size(); ++i) {
      if (!(gui::pixel::rgb::build(lhs[i]) == gui::pixel::rgb::build(rhs[i]))) {
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

  bool operator<= (const colormap& lhs, const colormap& rhs) {
    if (lhs.size() != rhs.size()) {
      return false;
    }
    for (int i = 0; i < lhs.size(); ++i) {
      if (!(lhs[i] > rhs[i])) {
        return false;
      }
    }
    return true;
  }

  bool operator>= (const colormap& lhs, const colormap& rhs) {
    if (lhs.size() != rhs.size()) {
      return false;
    }
    for (int i = 0; i < lhs.size(); ++i) {
      if (!(lhs[i] < rhs[i])) {
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
      for(const auto v: line) {
        if (first) {
          first = false;
        } else {
          out << ",";
        }
        switch (v) {
          case testing::_: out << "_"; break;
          case testing::R: out << "R"; break;
          case testing::G: out << "G"; break;
          case testing::B: out << "B"; break;
          case testing::W: out << "W"; break;
          case testing::Y: out << "Y"; break;
          case testing::V: out << "V"; break;
          case testing::M: out << "M"; break;
          case testing::D: out << "D"; break;
          case testing::L: out << "L"; break;
          default:
            out << "0x" << setw(8) << setfill('0') << hex << (v & 0xffffff);
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
    out << endl << '[';
    bool first_line = true;
    for(const auto& line: m) {
      if (first_line) {
        first_line = false;
      } else {
        out << ",";
      }
      out << '[';
      bool first = true;
      for(const auto v: line) {
        if (first) {
          first = false;
        } else {
          out << ",";
        }
        out << setw(2) << setfill('0') << hex << v;
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
