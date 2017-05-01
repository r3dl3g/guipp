/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
*                          Riedlinger Str. 8
*                          70327 Stuttgart
*                          Germany
*                          http://www.rothfuss-web.de
*
* @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
*
* Project    standard lib
*
* Customer   -
*
* @brief     C++ API: bitmap
*
* @file
*/

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "pnm.h"
#include "string_util.h"


namespace gui {

  namespace io {

    typedef uint8_t byte;
    typedef const byte* cbyteptr;
    typedef byte* byteptr;
    const byte bit_mask[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

    byte reverse_lookup[16] = {
        0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
        0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf
    };

    byte reverse_bit_order (byte n) {
       // Reverse the top and bottom nibble then swap them.
       return (reverse_lookup[n & 0b1111] << 4) | reverse_lookup[n >> 4];
    }

    // --------------------------------------------------------------------------
    void save_pnm_header (std::ostream& out, int magic_num, int width, int height, int max) {
      out << 'P' << magic_num << '\n';
      out << "# pnm created by gui++\n";
//      out.write("\n", 1);
      out << width << ' ' << height;
      if (max) {
        out << ' ' << max;
      }
      // In Win32 \n will create 0d 0a when the ofstream is create in text mode.
      // even when use write("\n", 1)!
      std::streampos p1 = out.tellp();
      out << '\n';
      std::streampos p2 = out.tellp();
      if (p2 - p1 > 1) {
        out.seekp(p1 + std::streamoff(1));
      }
    }

    // --------------------------------------------------------------------------
    void load_pnm_header (std::istream& in, int& magic_num, int& w, int& h, int& max) {
      char magic_char;
      in >> magic_char;
      if (magic_char != 'P') {
        throw std::runtime_error("stream contains no pnm");
      }
      std::skipws(in);
      in >> magic_num;
      if ((magic_num < 1) || (magic_num > 6)) {
        throw std::runtime_error("stream contains no valid pnm magic number");
      }
      in.get(); // ending whitespace
      while (in.peek() == '#') {
        std::string tmp;
        std::getline(in, tmp);
      }
      in >> w >> h;
      if ((magic_num != 1) && (magic_num != 4)) {
        in >> max;
      } else {
        max = 1;
      }
      in.get(); // ending whitespace
      std::noskipws(in);

      if (w < 1) {
        throw std::runtime_error("invalid pnm image width");
      }
      if (h < 1) {
        throw std::runtime_error("invalid pnm image height");
      }
      if ((max < 1) || (max > 255)) {
        throw std::runtime_error("invalid pnm max value");
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<6> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int bpp) {
      const std::size_t n = bpl * height;
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<6> data size missmatch");
      }
      if (32 == bpp) {
        for (int h = 0; h < height; ++h) {
          const char* d = (data.data() + (h * bpl));
          for (int w = 0; w < width; ++w) {
            out.write(d + 2, 1);
            out.write(d + 1, 1);
            out.write(d, 1);
            d += 4;
          }
        }
      }
      else if (24 == bpp) {
        for (int h = 0; h < height; ++h) {
          const char* d = (data.data() + (h * bpl));
#ifdef WIN32
          out.write(d, width * 3);

#endif // WIN32
#ifdef X11
          for (int w = 0; w < width; ++w) {
            out.write(d + 2, 1);
            out.write(d + 1, 1);
            out.write(d, 1);
            d += 3;
          }
#endif // X11
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<6> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp) {
      if (bpp != 24) {
        bpp = 32;
      }
      bpl = draw::bitmap::calc_bytes_per_line(width, bpp);
      std::size_t n = bpl * height;
      data.resize(n);
      std::noskipws(in);
#ifdef WIN32
      if (bpp == 24) {
        for (int y = 0; y < height; ++y) {
          char* d = (data.data() + (y * bpl));
          in.read(d, width * 3);
        }
      } else {
        for (int y = 0; y < height; ++y) {
          char* d = (data.data() + (y * bpl));
          for (int x = 0; x < width; ++x) {
            in.read(d, 3);
            d[3] = 0;
            d += 4;
          }
        }
      }
#endif // WIN32
#ifdef X11
      for (int y = 0; y < height; ++y) {
        char* d = (data.data() + (y * bpl));
        if (bpp == 24) {
          for (int x = 0; x < width; ++x) {
            in.read(d + 2, 1);
            in.read(d + 1, 1);
            in.read(d, 1);
            d += 3;
          }
        } else {
          for (int x = 0; x < width; ++x) {
            in.read(d + 2, 1);
            in.read(d + 1, 1);
            in.read(d, 1);
            d[3] = 0;
            d += 4;
          }
        }
      }
#endif // X11
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<5> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int) {
      const std::size_t n = bpl * height;
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<5> data size missmatch");
      }
      out.write(data.data(), n);
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<5> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp) {
      bpp = 8;
      bpl = width;
      const std::size_t n = bpl * height;
      data.resize(n);
      std::noskipws(in);
      auto p1 = in.tellg();
      in.read(data.data(), n);
      auto p2 = in.tellg();
      if (p2 - p1 != n) {
        throw std::runtime_error("not enought input data in load_pnm<5>");
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<4> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int) {
      const std::size_t n = bpl * height;
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<4> data size missmatch");
      }
      if (width == bpl * 8) {
         out.write(data.data(), n);
      } else {
        int bytes = (width + 7) / 8;
        for (int y = 0; y < height; ++y) {
          cbyteptr i = reinterpret_cast<cbyteptr>(data.data() + (y * bpl));
          std::vector<byte> line(bytes);
          for (int x = 0; x < bytes; ++x) {
#ifdef WIN32
            line[x] = i[x] ^ 0xff;
#endif // WIN32
#ifdef X11
            line[x] = reverse_bit_order(i[w]) ^ 0xff;
#endif // X11
          }
          out.write(reinterpret_cast<char*>(line.data()), bytes);
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<4> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp) {
      bpp = 1;
      bpl = draw::bitmap::calc_bytes_per_line(width, bpp);
      const std::size_t n = bpl * height;
      data.resize(n);
      std::noskipws(in);
      if (width == bpl * 8) {
         in.read(data.data(), n);
      } else {
        int bytes = (width + 7) / 8;
        std::vector<byte> line(bytes);
        for (int y = 0; y < height; ++y) {
          byteptr i = reinterpret_cast<byteptr>(data.data() + (y * bpl));
          in.read(reinterpret_cast<char*>(line.data()), bytes);
          for (int x = 0; x < bytes; ++x) {
#ifdef WIN32
            i[x] = line[x] ^ 0xff;
#endif // WIN32
#ifdef X11
            i[x] = reverse_bit_order(line[x]) ^ 0xff;
#endif // X11
          }
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<3> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int bpp) {
      const std::size_t n = bpl * height;
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<3> data size missmatch");
      }
      cbyteptr bdata = reinterpret_cast<cbyteptr>(data.data());
      for (int h = 0; h < height; ++h) {
        if (32 == bpp) {
          cbyteptr i = bdata + (h * bpl);
          for (int w = 0; w < width; ++w) {
            cbyteptr d = i + w * 4;
            out << static_cast<int>(d[2]) << ' ' <<  static_cast<int>(d[1]) << ' ' <<  static_cast<int>(d[0]) << ' ';
          }
        } else {
          for (cbyteptr i = bdata + (h * bpl), e = i + bpl; i < e; ++i) {
            out << static_cast<int>(*i) << ' ';
          }
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<3> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp) {
      if (bpp != 24) {
        bpp = 32;
      }
      bpl = draw::bitmap::calc_bytes_per_line(width, bpp);
      const std::size_t n = bpl * height;
      data.resize(n);

      std::skipws(in);
      byteptr bdata = reinterpret_cast<byteptr>(data.data());
      for (int y = 0; y < height; ++y) {
        byteptr i = bdata + (y * bpl);
        if (32 == bpp) {
          for (int x = 0; x < width; ++x) {
            byteptr d = i + x * 4;
            int d0, d1, d2;
#ifdef WIN32
            in >> d0 >> d1 >> d2;
#endif // WIN32
#ifdef X11
            in >> d2 >> d1 >> d0;
#endif // X11
            d[0] = static_cast<byte>(d0);
            d[1] = static_cast<byte>(d1);
            d[2] = static_cast<byte>(d2);
            d[3] = 0;
          }
        } else {
          for (int x = 0; x < width; ++x) {
            byteptr d = i + x * 3;
            int d0, d1, d2;
#ifdef WIN32
            in >> d0 >> d1 >> d2;
#endif // WIN32
#ifdef X11
            in >> d2 >> d1 >> d0;
#endif // X11
            d[0] = static_cast<byte>(d0);
            d[1] = static_cast<byte>(d1);
            d[2] = static_cast<byte>(d2);
          }
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<2> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int) {
      const std::size_t n = bpl * height;
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<2> data size missmatch");
      }
      for (int h = 0; h < height; ++h) {
        cbyteptr i = reinterpret_cast<cbyteptr>(data.data() + (h * bpl));
        for (int w = 0; w < width; ++w) {
          out << static_cast<int>(i[w]) << ' ';
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<2> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp) {
      bpp = 8;
      bpl = draw::bitmap::calc_bytes_per_line(width, bpp);
      const std::size_t n = bpl * height;
      data.resize(n);

      std::skipws(in);
      for (int y = 0; y < height; ++y) {
        byteptr i = reinterpret_cast<byteptr>(data.data() + (y * bpl));
        for (int x = 0; x < width; ++x) {
          int v;
          in >> v;
          i[x] = static_cast<byte>(v);
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<1> (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int) {
      const std::size_t n = bpl * height ;
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<1> data size missmatch");
      }
      for (int h = 0; h < height; ++h) {
        cbyteptr i = reinterpret_cast<cbyteptr>(data.data() + (h * bpl));
        for (int w = 0; w < width; ++w) {
          byte v = i[w / 8];
#ifdef WIN32
          byte bit = bit_mask[7 - w % 8];
#endif // WIN32
#ifdef X11
          byte bit = bit_mask[w % 8];
#endif // X11
          out << (v & bit ? '0' : '1') << ' ';
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<1> (std::istream& in, std::vector<char>& data, int& width, int& height, int& bpl, int& bpp) {
      bpp = 1;
      bpl = draw::bitmap::calc_bytes_per_line(width, bpp);
      const std::size_t n = bpl * height;
      data.resize(n);

      std::skipws(in);
      for (int y = 0; y < height; ++y) {
        byteptr i = reinterpret_cast<byteptr>(data.data() + (y * bpl));
        int value = 0;
        for (int x = 0; x < width; ++x) {
          int v;
          in >> v;
          int s = x % 8;
          value = value | (v << (IF_WIN32(7 - ) s));
          if (s == 7) {
            i[x / 8] = static_cast<byte>(value) ^ 0xff;
            value = 0;
          }
        }
        i[(width - 1)/ 8] = static_cast<byte>(value) ^ 0xff;
      }
    }

    // --------------------------------------------------------------------------
    void save_pnm (std::ostream& out, const draw::bitmap& bmp, bool binary) {
      int w, h, bpl, bpp;
      std::vector<char> data;
      bmp.get_data(data, w, h, bpl, bpp);
      switch (bpp) {
      case 1:
        if (binary) {
          save_pnm_header(out, 4, w, h, 0);
          save_pnm<4>(out, data, w, h, bpl, bpp);
        } else {
          save_pnm_header(out, 1, w, h, 0);
          save_pnm<1>(out, data, w, h, bpl, bpp);
        }
        break;
      case 8:
        if (binary) {
          save_pnm_header(out, 5, w, h, 0);
          save_pnm<5>(out, data, w, h, bpl, bpp);
        } else {
          save_pnm_header(out, 2, w, h, 0);
          save_pnm<2>(out, data, w, h, bpl, bpp);
        }
        break;
      case 24:
      case 32:
        if (binary) {
          save_pnm_header(out, 6, w, h, 0);
          save_pnm<6>(out, data, w, h, bpl, bpp);
        } else {
          save_pnm_header(out, 3, w, h, 0);
          save_pnm<3>(out, data, w, h, bpl, bpp);
        }
        break;
      default:
        throw std::invalid_argument("unsupportet bit per pixel value");
      }
    }

    void load_pnm (std::istream& in, draw::bitmap& bmp) {
      int magic_num, w, h, max;
      load_pnm_header(in, magic_num, w, h, max);

      int bpl, bpp = 24;
      std::vector<char> data;
      switch (magic_num) {
      case 1:
        load_pnm<1>(in, data, w, h, bpl, bpp);
        break;
      case 2:
        load_pnm<2>(in, data, w, h, bpl, bpp);
        break;
      case 3:
        load_pnm<3>(in, data, w, h, bpl, bpp);
        break;
      case 4:
        load_pnm<4>(in, data, w, h, bpl, bpp);
        break;
      case 5:
        load_pnm<5>(in, data, w, h, bpl, bpp);
        break;
      case 6:
        load_pnm<6>(in, data, w, h, bpl, bpp);
        break;
      }
      bmp.create(data, w, h, bpl, bpp);
    }

  } // io

}
