/**
 * @copyright (c) 2018-2018 Ing. Buero Rothfuss
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
 * @brief     C++ API: calculate wavelength to rgb.
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
#include <util/string_util.h>
#include <gui/io/wavelength_to_rgb.h>

// --------------------------------------------------------------------------
//
// Source:
//
// http://www.efg2.com/Lab/ScienceAndEngineering/Spectra.htm

namespace gui {

  namespace io {

    namespace optics {

      byte adjust (double color, double factor) {
        if (0.0 == color) {
          return 0;
        } else {
          return static_cast<byte>(round(255.0 * pow(color * factor, 0.80)));
        }
      }

      // --------------------------------------------------------------------------
      os::color wave_length_to_rgb (const double wave_length) {
        double red = 0.0;
        double green = 0.0;
        double blue = 0.0;

        if (wave_length < 440.0) {
          red = (440.0 - wave_length) / (440.0 - 380.0);
          blue = 1.0;
        } else if (wave_length < 490.0) {
          green = (wave_length - 440.0) / (490.0 - 440.0);
          blue = 1.0;
        } else if (wave_length < 510.0) {
          green = 1.0;
          blue = (510.0 - wave_length) / (510.0 - 490.0);
        } else if (wave_length < 580.0) {
          red = (wave_length - 510.0) / (580.0 - 510.0);
          green = 1.0;
        } else if (wave_length < 645.0) {
          red = 1.0;
          green = (645 - wave_length) / (645.0 - 580.0);
        } else {
          red = 1.0;
        }

        double factor = 0.0;
        if (wave_length < 420.0) {
          factor = 0.3 + 0.7 * (wave_length - 380.0) / (420.0 - 380.0);
        } else if (wave_length < 700.0) {
          factor = 1;
        } else {
          factor = 0.3 + 0.7 * (780 - wave_length) / (780.0 - 700.0);
        }

        return gui::color::calc_rgb(adjust(red, factor),
                                    adjust(green, factor),
                                    adjust(blue, factor));
      }

    } // optics

  } // io

} // gui
