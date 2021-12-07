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
 * @brief     forward declarations for core types
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


// --------------------------------------------------------------------------
//
// Common includes
//
#include <cstdint>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "coordinate_system.h"


namespace gui {

  namespace core {

    enum class coordinate_system;

    // --------------------------------------------------------------------------
    template<typename T, coordinate_system C>
    struct basic_size;

    typedef basic_size<float, coordinate_system::local> size;
    typedef basic_size<uint32_t, coordinate_system::surface> native_size;

    // --------------------------------------------------------------------------
    template<typename T, coordinate_system C>
    struct basic_point;

    typedef basic_point<float, coordinate_system::local> point;
    typedef basic_point<int32_t, coordinate_system::surface> native_point;

    // --------------------------------------------------------------------------
    template<typename T, typename S, coordinate_system C>
    struct basic_rectangle;

    typedef basic_rectangle<float, float, coordinate_system::local> rectangle;
    typedef basic_rectangle<int32_t, uint32_t, coordinate_system::surface> native_rect;

    // --------------------------------------------------------------------------
    template<typename T>
    struct range;

    // --------------------------------------------------------------------------
  } // namespace core

} // namespace gui
