/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
#include <gui/core/string_util.h>
#include <gui/io/pnm.h>


namespace gui {

  namespace io {

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
    void save_pnm_header (std::ostream& out, PNM pnm, const draw::bitmap_info& bmi, int max) {
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
    void load_pnm_header (std::istream& in, PNM& pnm, draw::bitmap_info& bmi, int& max) {
      in >> pnm;
      while (in.peek() == '#') {
        std::string tmp;
        std::getline(in, tmp);
      }
      in >> bmi.width >> bmi.height;
      if ((pnm != PNM::P1) && (pnm != PNM::P4)) {
        in >> max;
      } else {
        max = 1;
      }
      in.get(); // ending whitespace
      std::noskipws(in);

      if (bmi.width < 1) {
        throw std::runtime_error("invalid pnm image width");
      }
      if (bmi.height < 1) {
        throw std::runtime_error("invalid pnm image height");
      }
      if ((max < 1) || (max > 255)) {
        throw std::runtime_error("invalid pnm max value");
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<PNM::P6>(std::ostream& out, const blob& data, const draw::bitmap_info& bmi) {
      const std::size_t n = bmi.mem_size();
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<6> data size missmatch");
      }
      int step = bmi.depth() / 8;
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        const char* d = reinterpret_cast<const char*>(data.data() + (y * bmi.bytes_per_line));
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          out.write(d + 2, 1);
          out.write(d + 1, 1);
          out.write(d, 1);
          d += step;
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<PNM::P6>(std::istream& in, blob& data, draw::bitmap_info& bmi) {
      if (bmi.bits_per_pixel != BPP::RGB) {
        bmi.bits_per_pixel = BPP::RGBA;
      }
      bmi.bytes_per_line = draw::bitmap::calc_bytes_per_line(bmi.width, bmi.bits_per_pixel);
      const std::size_t n = bmi.bytes_per_line * bmi.height;
      data.resize(n);
      std::noskipws(in);
      const int step = bmi.depth() / 8;
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        char* d = reinterpret_cast<char*>(data.data() + (y * bmi.bytes_per_line));
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          in.read(d + 2, 1);
          in.read(d + 1, 1);
          in.read(d, 1);
          if (bmi.bits_per_pixel == BPP::RGBA) {
            d[3] = 0;
          }
          d += step;
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<PNM::P5>(std::ostream& out, const blob& data, const draw::bitmap_info& bmi) {
      const std::size_t n = bmi.mem_size();
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<5> data size missmatch");
      }
      out.write(reinterpret_cast<const char*>(data.data()), n);
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<PNM::P5>(std::istream& in, blob& data, draw::bitmap_info& bmi) {
      bmi.bits_per_pixel = BPP::GRAY;
      bmi.bytes_per_line = bmi.width;
      const std::size_t n = bmi.mem_size();
      data.resize(n);
      std::noskipws(in);
      auto p1 = in.tellg();
      in.read(reinterpret_cast<char*>(data.data()), n);
      auto p2 = in.tellg();
      if (p2 - p1 != n) {
        throw std::runtime_error("not enought input data in load_pnm<5>");
      }
    }

    // --------------------------------------------------------------------------
    template<bit_order O>
    void write_pnm4_line (std::ostream& out, const byte* data, int bytes);

    template<>
    inline void write_pnm4_line<bit_order::msb>(std::ostream& out, const byte* data, int bytes) {
      out.write(reinterpret_cast<const char*>(data), bytes);
    }

    template<>
    inline void write_pnm4_line<bit_order::lsb>(std::ostream& out, const byte* data, int bytes) {
      std::vector<byte> line(bytes);
      for (int x = 0; x < bytes; ++x) {
        line[x] = reverse_bit_order(data[x]) ^ 0xff;
      }
      out.write(reinterpret_cast<char*>(line.data()), bytes);
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<PNM::P4>(std::ostream& out, const blob& data, const draw::bitmap_info& bmi) {
      const std::size_t n = bmi.mem_size();
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<4> data size missmatch");
      }
      if (bmi.width == bmi.bytes_per_line * 8) {
        out.write(reinterpret_cast<const char*>(data.data()), n);
      } else {
        int bytes = (bmi.width + 7) / 8;
        for (uint_fast32_t y = 0; y < bmi.height; ++y) {
          write_pnm4_line<os::bitmap_bit_order>(out, data.data() + (y * bmi.bytes_per_line), bytes);
        }
      }
    }

    // --------------------------------------------------------------------------
    template<bit_order O>
    void load_pnm4_line (std::istream& in, byte* data, int bytes);

    template<>
    inline void load_pnm4_line<bit_order::msb>(std::istream& in, byte* data, int bytes) {
      in.read(reinterpret_cast<char*>(data), bytes);
    }

    template<>
    inline void load_pnm4_line<bit_order::lsb>(std::istream& in, byte* data, int bytes) {
      in.read(reinterpret_cast<char*>(data), bytes);
      for (int x = 0; x < bytes; ++x) {
        data[x] = reverse_bit_order(data[x]) ^ 0xff;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<PNM::P4>(std::istream& in, blob& data, draw::bitmap_info& bmi) {
      bmi.bits_per_pixel = BPP::BW;
      bmi.bytes_per_line = draw::bitmap::calc_bytes_per_line(bmi.width, bmi.bits_per_pixel);
      const std::size_t n = bmi.mem_size();
      data.resize(n);
      std::noskipws(in);
      if (bmi.width == bmi.bytes_per_line * 8) {
        in.read(reinterpret_cast<char*>(data.data()), n);
      } else {
        int bytes = (bmi.width + 7) / 8;
        for (uint_fast32_t y = 0; y < bmi.height; ++y) {
          load_pnm4_line<os::bitmap_bit_order>(in, data.data() + (y * bmi.bytes_per_line), bytes);
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<PNM::P3>(std::ostream& out, const blob& data, const draw::bitmap_info& bmi) {
      const std::size_t n = bmi.mem_size();
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<3> data size missmatch");
      }
      cbyteptr bdata = data.data();
      int step = bmi.depth() / 8;
      for (uint_fast32_t h = 0; h < bmi.height; ++h) {
        cbyteptr i = bdata + (h * bmi.bytes_per_line);
        for (uint_fast32_t w = 0; w < bmi.width; ++w) {
          cbyteptr d = i + w * step;
          out << static_cast<int>(d[2]) << ' ' << static_cast<int>(d[1]) << ' ' << static_cast<int>(d[0]) << ' ';
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<PNM::P3>(std::istream& in, blob& data, draw::bitmap_info& bmi) {
      if (bmi.bits_per_pixel != BPP::RGB) {
        bmi.bits_per_pixel = BPP::RGBA;
      }
      bmi.bytes_per_line = draw::bitmap::calc_bytes_per_line(bmi.width, bmi.bits_per_pixel);
      const std::size_t n = bmi.mem_size();
      data.resize(n);

      std::skipws(in);
      byteptr bdata = data.data();
      int step = bmi.depth() / 8;
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        byteptr i = bdata + (y * bmi.bytes_per_line);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          byteptr d = i + x * step;
          int d0, d1, d2;
          in >> d2 >> d1 >> d0;
          d[0] = static_cast<byte>(d0);
          d[1] = static_cast<byte>(d1);
          d[2] = static_cast<byte>(d2);
          if (BPP::RGBA == bmi.bits_per_pixel) {
            d[3] = 0;
          }
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<PNM::P2>(std::ostream& out, const blob& data, const draw::bitmap_info& bmi) {
      const std::size_t n = bmi.mem_size();
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<2> data size missmatch");
      }
      for (uint_fast32_t h = 0; h < bmi.height; ++h) {
        cbyteptr i = data.data() + (h * bmi.bytes_per_line);
        for (uint_fast32_t w = 0; w < bmi.width; ++w) {
          out << static_cast<int>(i[w]) << ' ';
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<PNM::P2>(std::istream& in, blob& data, draw::bitmap_info& bmi) {
      bmi.bits_per_pixel = BPP::GRAY;
      bmi.bytes_per_line = draw::bitmap::calc_bytes_per_line(bmi.width, bmi.bits_per_pixel);
      const std::size_t n = bmi.mem_size();
      data.resize(n);

      std::skipws(in);
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        byteptr i = data.data() + (y * bmi.bytes_per_line);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          int v;
          in >> v;
          i[x] = static_cast<byte>(v);
        }
      }
    }

    // --------------------------------------------------------------------------
    template<bit_order O>
    void write_pnm1_line (std::ostream& out, byte value, byte bit);

    template<>
    void write_pnm1_line<bit_order::msb>(std::ostream& out, byte value, byte bit) {
      out << (value & bit ? '1' : '0') << ' ';
    }

    template<>
    void write_pnm1_line<bit_order::lsb>(std::ostream& out, byte value, byte bit) {
      out << (value & bit ? '0' : '1') << ' ';
    }

    // --------------------------------------------------------------------------
    template<>
    void save_pnm<PNM::P1>(std::ostream& out, const blob& data, const draw::bitmap_info& bmi) {
      const std::size_t n = bmi.mem_size();
      if (data.size() != n) {
        throw std::invalid_argument("save_pnm<1> data size missmatch");
      }
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        cbyteptr i = data.data() + (y * bmi.bytes_per_line);
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          write_pnm1_line<os::bitmap_bit_order>(out, i[x / 8], system_bw_bits::mask[x % 8]);
        }
        out << std::endl;
      }
    }

    // --------------------------------------------------------------------------
    template<bit_order O>
    void set_pnm1_value (byteptr i, int x, byte value);

    template<>
    void set_pnm1_value<bit_order::msb>(byteptr i, int x, byte value) {
      i[x / 8] = static_cast<byte>(value);
    }

    template<>
    void set_pnm1_value<bit_order::lsb>(byteptr i, int x, byte value) {
      i[x / 8] = static_cast<byte>(value) ^ 0xff;
    }

    // --------------------------------------------------------------------------
    template<>
    void load_pnm<PNM::P1>(std::istream& in, blob& data, draw::bitmap_info& bmi) {
      bmi.bits_per_pixel = BPP::BW;
      bmi.bytes_per_line = draw::bitmap::calc_bytes_per_line(bmi.width, bmi.bits_per_pixel);
      const std::size_t n = bmi.mem_size();
      data.resize(n);

      std::skipws(in);
      for (uint_fast32_t y = 0; y < bmi.height; ++y) {
        byteptr i = data.data() + (y * bmi.bytes_per_line);
        int value = 0;
        for (uint_fast32_t x = 0; x < bmi.width; ++x) {
          char v;
          in >> v;
          const int s = x % 8;
          const byte bit = v == '1' ? system_bw_bits::mask[s] : 0;
          value = value | bit;
          if (s == 7) {
            set_pnm1_value<os::bitmap_bit_order>(i, x, value);
            value = 0;
          }
        }
        set_pnm1_value<os::bitmap_bit_order>(i, (bmi.width - 1), value);
      }
    }

    // --------------------------------------------------------------------------
    void save_pnm (std::ostream& out, const draw::bitmap& bmp, bool binary) {
      if (binary) {
        out << opnm<true>(bmp);
      } else {
        out << opnm<false>(bmp);
      }
    }

    void load_pnm (std::istream& in, draw::bitmap& bmp) {
      in >> ipnm(bmp);
    }

    void save_pnm (const std::string& name, const draw::bitmap& bmp, bool binary) {
      std::ofstream out(name);
      save_pnm(out, bmp, binary);
    }

    void load_pnm (const std::string& name, draw::bitmap& bmp) {
      std::ifstream in(name);
      load_pnm(in, bmp);
    }

  } // io

}
