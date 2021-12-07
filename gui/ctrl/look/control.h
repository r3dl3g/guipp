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
 * @brief     basic control looks
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/core_fwd.h>
#include <gui/core/color.h>
#include <gui/draw/draw_fwd.h>
#include <gui/draw/text_origin.h>
#include <gui/ctrl/look/gui++-look-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace ctrl {

    struct item_state;

  } // ctrl

  namespace look {

    GUIPP_LOOK_EXPORT os::color get_text_color (ctrl::item_state state,
                                                os::color def_color = color::windowTextColor());

    GUIPP_LOOK_EXPORT os::color get_background_color (ctrl::item_state state,
                                                      os::color def_color);

    GUIPP_LOOK_EXPORT void text_item (draw::graphics&,
                                      const core::rectangle& place,
                                      const draw::brush& background,
                                      const std::string& text,
                                      const ctrl::item_state& state,
                                      const text_origin_t& origin_t = text_origin_t::vcenter_left);

  } // namespace look

} // gui
