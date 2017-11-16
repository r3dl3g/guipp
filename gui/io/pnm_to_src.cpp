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
#include <gui/io/pnm_to_src.h>
#include <gui/core/string_util.h>


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
      std::ostream& operator<< (std::ostream& out, draw::bw_pixel pixel) {
        out << (static_cast<bool>(static_cast<draw::bw_pixel>(pixel)) ? '1' : '0');
        return out;
      }

      std::ostream& operator<< (std::ostream& out, draw::gray_pixel pixel) {
        out << std::setw(3) << static_cast<int>(pixel.value);
        return out;
      }

      std::ostream& operator<< (std::ostream& out, draw::rgb_pixel pixel) {
        out << std::setw(3) << static_cast<int>(pixel.red.value) << ", "
            << std::setw(3) << static_cast<int>(pixel.green.value) << ", "
            << std::setw(3) << static_cast<int>(pixel.blue.value);
        return out;
      }

    } // src

  } // io

}
