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
 * @brief     C++ API: basic window types
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef GUIPP_X11
# include <X11/XKBlib.h>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>


namespace gui {

  namespace core {

    constexpr byte bw_bits<bit_order_t::lsb_first>::value[];
    constexpr byte bw_bits<bit_order_t::msb_first>::value[];

    constexpr byte system_bw_bits::mask[];
    constexpr byte system_bw_bits::shift[];

  } // namespace core

} // namespace gui
