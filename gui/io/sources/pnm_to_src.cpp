/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui io lib
 *
 * Customer   -
 *
 * @brief     C++ API: save bitmap as pnm to cpp file or stream.
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>
#include <sstream>
#include <iomanip>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "pnm_to_src.h"
#include "string_util.h"


namespace gui {

  namespace io {

    namespace src {
      // --------------------------------------------------------------------------
      void save_pnm_header_src (std::ostream& out, const std::string& name, PNM pnm, int width, int height, int max) {
        out << "static const unsigned char " << name << "[] = {" << std::endl
            << "'P', '" << static_cast<int>(pnm) << "', '\\n', ";
        std::ostringstream strm;
        strm << width << ' ' << height;
        if (max) {
          strm << ' ' << max;
        }
        std::string line = strm.str();
        for (char c : line) {
          out << "\'" << c << "\', ";
        }
        out << "'\\t', " << std::endl;
      }

      // --------------------------------------------------------------------------
      void save_pnm_src_rgb (std::ostream& out, const std::vector<char>& data, int width, int height, int bpl, int step) {
        const std::size_t n = bpl * height;
        if (data.size() != n) {
          throw std::invalid_argument("save_pnm_src_rgb data size missmatch");
        }
        out.fill(' ');
        for (int h = 0; h < height; ++h) {
          cbyteptr d = reinterpret_cast<cbyteptr>(data.data() + (h * bpl));
          for (int w = 0; w < width; ++w) {
            out << std::setw(3) << (int)(d[2]) << ", "
                << std::setw(3) << (int)(d[1]) << ", "
                << std::setw(3) << (int)(d[0]);
            if ((h == height - 1) && (w == width - 1)) {
              out << "};";
            } else {
              out << ", ";
            }
            d += step;
          }
          out << std::endl;
        }
      }

      // --------------------------------------------------------------------------
      template<>
      void save_pnm_src<BPP::RGBA>(std::ostream& out, const std::vector<char>& data, int width, int height, int bpl) {
        save_pnm_src_rgb(out, data, width, height, bpl, static_cast<int>(BPP::RGBA) / 8);
      }

      // --------------------------------------------------------------------------
      template<>
      void save_pnm_src<BPP::RGB>(std::ostream& out, const std::vector<char>& data, int width, int height, int bpl) {
        save_pnm_src_rgb(out, data, width, height, bpl, static_cast<int>(BPP::RGB) / 8);
      }

      // --------------------------------------------------------------------------
      template<>
      void save_pnm_src<BPP::GRAY>(std::ostream& out, const std::vector<char>& data, int width, int height, int bpl) {
        const std::size_t n = bpl * height;
        if (data.size() != n) {
          throw std::invalid_argument("save_pnm_src<GRAY> data size missmatch");
        }
        out.fill(' ');
        for (int h = 0; h < height; ++h) {
          cbyteptr d = reinterpret_cast<cbyteptr>(data.data() + (h * bpl));
          for (int w = 0; w < width; ++w) {
            out << std::setw(3) << (int)(d[w]);
            if ((h == height - 1) && (w == width - 1)) {
              out << "};";
            } else {
              out << ", ";
            }
          }
          out << std::endl;
        }
      }

      // --------------------------------------------------------------------------
      template<bit_order O>
      void write_bit (std::ostream& out, byte value, byte bit);

      template<>
      inline void write_bit<bit_order::msb>(std::ostream& out, byte value, byte bit) {
        out << (value & bit ? '1' : '0');
      }

      template<>
      inline void write_bit<bit_order::lsb>(std::ostream& out, byte value, byte bit) {
        out << (value & bit ? '0' : '1');
      }

      // --------------------------------------------------------------------------
      inline void write_byte (std::ostream& out, byte b) {
        out << "0b";
        for (int i = 0; i < 8; ++i) {
          write_bit<os::bitmap_bit_order>(out, b, system_bw_bits::mask[i]);
        }
      }

      // --------------------------------------------------------------------------
      template<>
      void save_pnm_src<BPP::BW>(std::ostream& out, const std::vector<char>& data, int width, int height, int bpl) {
        const std::size_t n = bpl * height;
        if (data.size() != n) {
          throw std::invalid_argument("save_pnm_srcBW> data size missmatch");
        }
        const int bytes = (width + 7) / 8;
        for (int y = 0; y < height; ++y) {
          cbyteptr i = reinterpret_cast<cbyteptr>(data.data() + (y * bpl));
          for (int x = 0; x < bytes; ++x) {
            write_byte(out, i[x]);
            if ((y == height - 1) && (x == bytes - 1)) {
              out << "};";
            } else {
              out << ", ";
            }
          }
          out << std::endl;
        }
      }

      // --------------------------------------------------------------------------
      void save_pnm_src (std::ostream& out, const draw::bitmap& bmp, const std::string& name) {
        out << opnm(bmp, name);
      }

      void save_pnm_src (const std::string& fname, const draw::bitmap& bmp, const std::string& name) {
        std::ofstream(fname) << opnm(bmp, name);
      }

      // --------------------------------------------------------------------------
      void opnm::write (std::ostream& out) const {
        int w, h, bpl;
        BPP bpp;
        std::vector<char> data;
        bmp.get_data(data, w, h, bpl, bpp);
        switch (bpp) {
        case BPP::BW:
          save_pnm_header_src(out, name, BPP2PNM<BPP::BW, true>::pnm, w, h, 0);
          save_pnm_src<BPP::BW>(out, data, w, h, bpl);
          break;
        case BPP::GRAY:
          save_pnm_header_src(out, name, BPP2PNM<BPP::GRAY, true>::pnm, w, h, 255);
          save_pnm_src<BPP::GRAY>(out, data, w, h, bpl);
          break;
        case BPP::RGB:
          save_pnm_header_src(out, name, BPP2PNM<BPP::RGB, true>::pnm, w, h, 255);
          save_pnm_src<BPP::RGB>(out, data, w, h, bpl);
          break;
        case BPP::RGBA:
          save_pnm_header_src(out, name, BPP2PNM<BPP::RGBA, true>::pnm, w, h, 255);
          save_pnm_src<BPP::RGBA>(out, data, w, h, bpl);
          break;
        default:
          throw std::invalid_argument("unsupportet bit per pixel value");
        }
      }

    } // src

  } // io

}
