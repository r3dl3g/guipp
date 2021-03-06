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
 * @brief     progress bar look
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/core_fwd.h>
#include <gui/core/size.h>
#include <gui/draw/draw_fwd.h>
#include <gui/ctrl/look/gui++-look-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace look {

    GUIPP_LOOK_EXPORT void vertical_progress_bar (draw::graphics& graph,
                                                  const core::rectangle& area,
                                                  const std::string& text,
                                                  os::color foreground,
                                                  os::color background,
                                                  const text_origin_t& origin,
                                                  os::color bar_color,
                                                  core::size::type bar_pos);

    GUIPP_LOOK_EXPORT void horizontal_progress_bar (draw::graphics& graph,
                                                    const core::rectangle& area,
                                                    const std::string& text,
                                                    os::color foreground,
                                                    os::color background,
                                                    const text_origin_t& origin,
                                                    os::color bar_color,
                                                    core::size::type bar_pos);

  } // namespace look

} // namespace gui
