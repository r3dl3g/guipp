/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     save/load images to/from pnm file or stream.
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include <util/string_util.h>
#include <gui/io/pnm.h>


namespace gui {

  namespace io {

    pixel_format_t pnm2bpp (PNM pnm) {
      switch (pnm) {
        case PNM::P1:
        case PNM::P4:
          return pixel_format_t::BW;
        case PNM::P2:
        case PNM::P5:
          return pixel_format_t::GRAY;
        case PNM::P3:
        case PNM::P6:
          return pixel_format_t::RGB;
      }
      return pixel_format_t::Undefined;
    }

    PNM bpp2pnm (pixel_format_t bpp, bool binary) {
      switch (bpp) {
        case pixel_format_t::BW: return binary ? PNM::P4 : PNM::P1;
        case pixel_format_t::GRAY: return binary ? PNM::P5 : PNM::P2;
        case pixel_format_t::Undefined: return PNM(0);
        default: return binary ? PNM::P6 : PNM::P3;
      }
    }

    int bpp2max (pixel_format_t bpp) {
      switch (bpp) {
        case pixel_format_t::BW: return 0;
        case pixel_format_t::Undefined: return -1;
        default: return 255;
      }
    }

    std::ostream& operator<< (std::ostream& out, const PNM& pnm) {
      out << 'P' << static_cast<int>(pnm);
      return out;
    }

    std::istream& operator>> (std::istream& in, PNM& pnm) {
      char magic_char;
      in >> magic_char;
      if (magic_char != 'P') {
        throw std::runtime_error("stream contains no pnm");
      }
      std::skipws(in);
      int magic_num;
      in >> magic_num;
      if ((magic_num < 1) || (magic_num > 6)) {
        throw std::runtime_error("stream contains no valid pnm magic number");
      }
      pnm = PNM(magic_num);
      in.get(); // ending whitespace
      return in;
    }

    // --------------------------------------------------------------------------
    void write_pnm_header (std::ostream& out, PNM pnm, const draw::bitmap_info& bmi, int max) {
      out << pnm << '\n';
      out << "# pnm created by gui++\n";
      out << bmi.width << ' ' << bmi.height;
      if (max) {
        out << ' ' << max;
      }
      // In Win32 \n will create 0d 0a when the ofstream is create in text mode.
      // even when we write("\n", 1)!
      std::streampos p1 = out.tellp();
      out << '\n';
      std::streampos p2 = out.tellp();
      if (p2 - p1 > 1) {
        out.seekp(p1 + std::streamoff(1));
      }
    }

    // --------------------------------------------------------------------------
    draw::bitmap_info read_pnm_header (std::istream& in, PNM& pnm, int& max) {
      in >> pnm;
      while (in.peek() == '#') {
        std::string tmp;
        std::getline(in, tmp);
      }
      uint32_t width, height;
      in >> width >> height;
      if ((pnm != PNM::P1) && (pnm != PNM::P4)) {
        in >> max;
      } else {
        max = 1;
      }
      in.get(); // ending whitespace
      std::noskipws(in);

      if (width < 1) {
        throw std::runtime_error("invalid pnm image width");
      }
      if (height < 1) {
        throw std::runtime_error("invalid pnm image height");
      }
      if ((max < 1) || (max > 255)) {
        throw std::runtime_error("invalid pnm max value");
      }

      return { width, height, pnm2bpp(pnm) };
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P6, pixel_format_t::RGB> (std::ostream& out,
                                               const draw::const_image_data<pixel_format_t::RGB>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = img.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          auto pixel = row[x];
          out.write(reinterpret_cast<char*>(&pixel.red), 1);
          out.write(reinterpret_cast<char*>(&pixel.green), 1);
          out.write(reinterpret_cast<char*>(&pixel.blue), 1);
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P6, pixel_format_t::BGR> (std::ostream& out,
                                               const draw::const_image_data<pixel_format_t::BGR>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = img.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          auto pixel = row[x];
          out.write(reinterpret_cast<char*>(&pixel.red), 1);
          out.write(reinterpret_cast<char*>(&pixel.green), 1);
          out.write(reinterpret_cast<char*>(&pixel.blue), 1);
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P6, pixel_format_t::RGBA> (std::ostream& out,
                                                const draw::const_image_data<pixel_format_t::RGBA>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = img.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          auto pixel = row[x];
          out.write(reinterpret_cast<char*>(&pixel.red), 1);
          out.write(reinterpret_cast<char*>(&pixel.green), 1);
          out.write(reinterpret_cast<char*>(&pixel.blue), 1);
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P6, pixel_format_t::BGRA> (std::ostream& out,
                                                const draw::const_image_data<pixel_format_t::BGRA>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = img.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          auto pixel = row[x];
          out.write(reinterpret_cast<char*>(&pixel.red), 1);
          out.write(reinterpret_cast<char*>(&pixel.green), 1);
          out.write(reinterpret_cast<char*>(&pixel.blue), 1);
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    draw::rgbmap read_pnm<PNM::P6> (std::istream& in, const draw::bitmap_info& bmi) {
      draw::rgbmap img(bmi.size());
      std::noskipws(in);
      auto data = img.get_data();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = data.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          auto& pixel = row[x];
          in.read(reinterpret_cast<char*>(&pixel.red), 1);
          in.read(reinterpret_cast<char*>(&pixel.green), 1);
          in.read(reinterpret_cast<char*>(&pixel.blue), 1);
        }
      }
      return img;
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P5, pixel_format_t::GRAY> (std::ostream& out,
                                                const draw::const_image_data<pixel_format_t::GRAY>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      const std::size_t n = bmi.mem_size();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        out.write(reinterpret_cast<const char*>(img.row(y).data(0, bmi.width)), bmi.width);
      }
    }

    // --------------------------------------------------------------------------
    template<>
    draw::graymap read_pnm<PNM::P5> (std::istream& in, const draw::bitmap_info& bmi) {
      draw::graymap img(bmi.size());
      auto n = bmi.width * bmi.height;
      std::noskipws(in);
      auto p1 = in.tellg();
      auto raw = img.get_data();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        in.read(reinterpret_cast<char*>(raw.row(y).data(0, bmi.width)), bmi.width);
      }
      auto p2 = in.tellg();
      auto m = p2 - p1;
      if (m != n) {
        throw std::runtime_error("not enought input data in read_pnm<5>");
      }
      return img;
    }

    // --------------------------------------------------------------------------
    template<core::bit_order_t O>
    void write_pnm4_line (std::ostream&, draw::const_image_data<pixel_format_t::BW>::raw_type, int);

    template<>
    inline void write_pnm4_line<core::bit_order_t::msb_first> (std::ostream& out,
                                                         const draw::const_image_data<pixel_format_t::BW>::raw_type data,
                                                         int bytes) {
      out.write(reinterpret_cast<const char*>(data.data(0, bytes)), bytes);
    }

    template<>
    inline void write_pnm4_line<core::bit_order_t::lsb_first> (std::ostream& out,
                                                         const draw::const_image_data<pixel_format_t::BW>::raw_type data,
                                                         int bytes) {
      std::vector<byte> line(bytes);
      for (int x = 0; x < bytes; ++x) {
          line[x] = core::reverse_bit_order(data[x]) ^ 0xff;
        }
      out.write(reinterpret_cast<char*>(line.data()), bytes);
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P4, pixel_format_t::BW> (std::ostream& out, const draw::const_image_data<pixel_format_t::BW>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      int bytes = (bmi.width + 7) / 8;
      const auto& raw = img.raw_data();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        write_pnm4_line<core::os::bitmap_bit_order>(out, raw.sub(y * bmi.bytes_per_line, bmi.bytes_per_line), bytes);
      }
    }

    // --------------------------------------------------------------------------
    template<core::bit_order_t O>
    void read_pnm4_line (std::istream& in, draw::image_data<pixel_format_t::BW>::raw_type, int bytes);

    template<>
    inline void read_pnm4_line<core::bit_order_t::msb_first>(std::istream& in, draw::image_data<pixel_format_t::BW>::raw_type data, int bytes) {
      in.read(reinterpret_cast<char*>(data.data(0, bytes)), bytes);
    }

    template<>
    inline void read_pnm4_line<core::bit_order_t::lsb_first>(std::istream& in, draw::image_data<pixel_format_t::BW>::raw_type data, int bytes) {
      std::vector<byte> line(bytes);
      in.read(reinterpret_cast<char*>(line.data()), bytes);
      for (int x = 0; x < bytes; ++x) {
        data[x] = core::reverse_bit_order(line[x])/* ^ 0xff*/;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    draw::bwmap read_pnm<PNM::P4> (std::istream& in, const draw::bitmap_info& bmi) {
      draw::bwmap img(bmi.size());
      int bytes = (bmi.width + 7) / 8;
      auto& raw = img.get_data().raw_data();
      std::noskipws(in);
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        read_pnm4_line<core::os::bitmap_bit_order>(in, raw.sub(y * bmi.bytes_per_line, bmi.bytes_per_line), bytes);
      }
      return img;
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P3, pixel_format_t::RGB> (std::ostream& out, const draw::const_image_data<pixel_format_t::RGB>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = img.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          auto pixel = row[x];
          out << static_cast<int>(pixel.red) << ' '
              << static_cast<int>(pixel.green) << ' '
              << static_cast<int>(pixel.blue) << ' ';
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P3, pixel_format_t::BGR> (std::ostream& out, const draw::const_image_data<pixel_format_t::BGR>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = img.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          auto pixel = row[x];
          out << static_cast<int>(pixel.red) << ' '
              << static_cast<int>(pixel.green) << ' '
              << static_cast<int>(pixel.blue) << ' ';
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P3, pixel_format_t::BGRA> (std::ostream& out, const draw::const_image_data<pixel_format_t::BGRA>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = img.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          auto pixel = row[x];
          out << static_cast<int>(pixel.red) << ' '
              << static_cast<int>(pixel.green) << ' '
              << static_cast<int>(pixel.blue) << ' ';
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P3, pixel_format_t::RGBA> (std::ostream& out, const draw::const_image_data<pixel_format_t::RGBA>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = img.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          auto pixel = row[x];
          out << static_cast<int>(pixel.red) << ' '
              << static_cast<int>(pixel.green) << ' '
              << static_cast<int>(pixel.blue) << ' ';
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    draw::rgbmap read_pnm<PNM::P3> (std::istream& in, const draw::bitmap_info& bmi) {
      draw::rgbmap img(bmi.size());
      std::skipws(in);
      auto raw = img.get_data();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = raw.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          int r, g, b;
          in >> r;
          in >> g;
          in >> b;
          row[x] = pixel::rgb{
                   static_cast<byte>(r),
                   static_cast<byte>(g),
                   static_cast<byte>(b)
        };
        }
      }
      return img;
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P2, pixel_format_t::GRAY>(std::ostream& out, const draw::const_image_data<pixel_format_t::GRAY>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = img.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          auto pixel = row[x];
          out << static_cast<int>(pixel.value) << ' ';
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    draw::graymap read_pnm<PNM::P2> (std::istream& in, const draw::bitmap_info& bmi) {
      draw::graymap img(bmi.size());
      std::skipws(in);
      auto raw = img.get_data();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = raw.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          int v;
          in >> v;
          row[x].value = static_cast<byte>(v);
        }
      }
      return img;
    }

    // --------------------------------------------------------------------------
    template<>
    void write_pnm<PNM::P1, pixel_format_t::BW>(std::ostream& out, const draw::const_image_data<pixel_format_t::BW>& img) {
      const draw::bitmap_info& bmi = img.get_info();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = img.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          auto pixel = row[x];
          out << (static_cast<bool>(pixel == pixel::mono::black) ? '1' : '0') << ' ';
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    draw::bwmap read_pnm<PNM::P1> (std::istream& in, const draw::bitmap_info& bmi) {
      draw::bwmap img(bmi.size());
      std::skipws(in);
      auto raw = img.get_data();
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        auto row = raw.row(y);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          char v;
          in >> v;
          row[x] = (v == '1') ? pixel::mono::black : pixel::mono::white;
        }
      }
      return img;
    }

    // --------------------------------------------------------------------------
    void load_pnm (std::istream& in, draw::basic_datamap& bmp) {
      int max;
      PNM pnm;
      draw::bitmap_info bmi = read_pnm_header(in, pnm, max);
      switch (pnm) {
        case PNM::P1: bmp = read_pnm<PNM::P1>(in, bmi); break;
        case PNM::P2: bmp = read_pnm<PNM::P2>(in, bmi); break;
        case PNM::P3: bmp = read_pnm<PNM::P3>(in, bmi); break;
        case PNM::P4: bmp = read_pnm<PNM::P4>(in, bmi); break;
        case PNM::P5: bmp = read_pnm<PNM::P5>(in, bmi); break;
        case PNM::P6: bmp = read_pnm<PNM::P6>(in, bmi); break;
      }
    }

  } // io

}
