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
 * @brief     textbox look
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/core_fwd.h>
#include <gui/core/guidefs.h>
#include <gui/draw/draw_fwd.h>
#include <gui/ctrl/look/gui++-look-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace look {

    GUIPP_LOOK_EXPORT void text_box (draw::graphics& graph,
                                     const core::rectangle& area,
                                     const std::vector<std::string>& lines,
                                     const draw::font& fnt,
                                     os::color foreground,
                                     os::color background,
                                     const text_origin_t& origin_t,
                                     const core::range<core::basic_point<int> >& selection,
                                     const core::basic_point<int>& cursor_pos,
                                     const core::point& offset,
                                     bool is_focused,
                                     bool is_enabled);

  } // look

} // gui
