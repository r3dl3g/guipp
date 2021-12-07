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
 * @brief     save image as pnm to cpp file or stream.
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>
#include <iomanip>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/io/pnm_to_src.h>


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
      void save_pnm_header_src (std::ostream& out, const std::string& name, const draw::bitmap_info& bmi) {
        save_pnm_header_src(out, name, bpp2pnm(bmi.pixel_format), bmi.width, bmi.height, bpp2max(bmi.pixel_format));
      }

      // --------------------------------------------------------------------------
      void save_pnm_src (std::ostream& out, const draw::basic_datamap& bmp, const std::string& name) {
        save_pnm_header_src(out, name, bmp.get_info());

        using pf = pixel_format_t;
        switch (bmp.pixel_format()) {
          case pf::BW:    save_pnm_src<pf::BW>(out, bmp.const_reinterpret<pf::BW>()); break;
          case pf::GRAY:  save_pnm_src<pf::GRAY>(out, bmp.const_reinterpret<pf::GRAY>()); break;
          case pf::RGB:   save_pnm_src<pf::RGB>(out, bmp.const_reinterpret<pf::RGB>()); break;
          case pf::RGBA:  save_pnm_src<pf::RGBA>(out, bmp.const_reinterpret<pf::RGBA>()); break;
          case pf::BGR:   save_pnm_src<pf::BGR>(out, bmp.const_reinterpret<pf::BGR>()); break;
          case pf::BGRA:  save_pnm_src<pf::BGRA>(out, bmp.const_reinterpret<pf::BGRA>()); break;
          default:
            throw std::out_of_range(ostreamfmt("Can not save pixel format " << bmp.pixel_format() << "to source"));
        }
      }

      // --------------------------------------------------------------------------
      void save_pnm_src (const std::string& fname, const draw::basic_datamap& bmp, const std::string& name) {
        std::ofstream ofs(fname);
        save_pnm_src(ofs, bmp, name);
      }

      // --------------------------------------------------------------------------
      void write_pixel (std::ostream& out, const core::bit_wrapper<const pixel::mono>& pixel) {
        write_pixel(out, static_cast<const pixel::mono>(pixel));
      }

      void write_pixel (std::ostream& out, const pixel::mono pixel) {
        static const char values[] = {'0', '1'};
#ifdef GUIPP_WIN
        out << (values[static_cast<byte>(pixel)]);
#else
        out << (values[static_cast<bool>(pixel)]);
#endif
      }

      void write_pixel (std::ostream& out, const pixel::gray pixel) {
        out << std::setw(3) << static_cast<int>(pixel.value);
      }

    } // src

  } // io

}
