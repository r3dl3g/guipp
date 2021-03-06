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
 * @brief     edit look
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>

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

    GUIPP_LOOK_EXPORT void edit_line (draw::graphics& graph,
                                      const core::rectangle& area,
                                      const std::string& text,
                                      const draw::font& fnt,
                                      os::color foreground,
                                      os::color background,
                                      const text_origin_t& origin_t,
                                      const core::range<size_t>& selection,
                                      size_t cursor_pos,
                                      size_t scroll_pos,
                                      bool is_focused,
                                      bool is_enabled);

  } // look

} // gui
