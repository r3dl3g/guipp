/**
 * @copyright (c) 2018-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     calculate wavelength to rgb.
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>
#include <fstream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/color.h>
#include <gui/io/gui++-io-export.h>


namespace gui {

  namespace io {

    namespace optics {

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      GUIPP_IO_EXPORT os::color wave_length_to_rgb (double wave);

    } // namespace optics

  } // namespace io

} // namespace gui

//#include <gui/io/pnm_to_src.inl>
