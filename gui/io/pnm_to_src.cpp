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
#include <base/string_util.h>
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
      void write_pixel (std::ostream& out, pixel::bw_pixel pixel) {
        static const char values[] = {'0', '1'};
#ifdef WIN32
        out << (values[static_cast<byte>(pixel)]);
#else
        out << (values[static_cast<bool>(pixel)]);
#endif
      }

      void write_pixel (std::ostream& out, pixel::gray_pixel pixel) {
        out << std::setw(3) << static_cast<int>(pixel.value);
      }

      void write_pixel (std::ostream& out, pixel::rgb_pixel pixel) {
        out << std::setw(3) << static_cast<int>(pixel.red) << ", "
            << std::setw(3) << static_cast<int>(pixel.green) << ", "
            << std::setw(3) << static_cast<int>(pixel.blue);
      }

    } // src

  } // io

}
