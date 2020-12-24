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
 * @brief     C++ API: textbox controls
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/core_fwd.h>
#include <gui/core/guidefs.h>
#include <gui/draw/draw_fwd.h>
#include <gui++-look-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace paint {

    GUIPP_LOOK_EXPORT void text_box (const draw::graphics& graph,
                                     const core::rectangle& area,
                                     const std::vector<std::string>& lines,
                                     const draw::font& fnt,
                                     os::color foreground,
                                     os::color background,
                                     const text_origin_t& origin_t,
                                     const core::range<core::basic_point<int> >& selection,
                                     const core::basic_point<int>& cursor_pos,
                                     const core::point& offset,
                                     const bool is_focused);

  } // paint

} // gui
