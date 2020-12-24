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
 * @brief     C++ API: basic structures
 *
 * @file
 */

#pragma once


// --------------------------------------------------------------------------
//
// Common includes
//
#include <cstdint>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<typename T>
    struct basic_size;

    typedef basic_size<float> size;
    typedef basic_size<uint32_t> native_size;

    // --------------------------------------------------------------------------
    template<typename T>
    struct basic_point;

    typedef basic_point<float> point;
    typedef basic_point<int32_t> native_point;

    // --------------------------------------------------------------------------
    template<typename T, typename S>
    struct basic_rectangle;

    typedef basic_rectangle<float, float> rectangle;
    typedef basic_rectangle<int32_t, uint32_t> native_rect;

    // --------------------------------------------------------------------------
    template<typename T>
    struct range;

    // --------------------------------------------------------------------------
  } // namespace core

} // namespace gui
