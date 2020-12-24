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
 * @brief     C++ API: table
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/frames.h>
#include <gui/draw/draw_fwd.h>
#include <gui/ctrl/item_state.h>
#include <util/string_util.h>
#include <gui++-look-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace paint {

    template<typename T,
             draw::frame::drawer F = draw::frame::sunken_relief>
    void text_cell (const T& t,
                    const draw::graphics& graph,
                    const core::rectangle& place,
                    const text_origin_t& align,
                    const os::color& foreground,
                    const os::color& background,
                    const ctrl::item_state& state);

    template<>
    GUIPP_LOOK_EXPORT void text_cell<std::string, draw::frame::no_frame>(const std::string& t,
                                                                         const draw::graphics& graph,
                                                                         const core::rectangle& place,
                                                                         const text_origin_t& align,
                                                                         const os::color& foreground,
                                                                         const os::color& background,
                                                                         const ctrl::item_state& state);

  } // namespace paint

} // gui

#include <gui/look/table.inl>
