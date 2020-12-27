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
 * @brief     C++ API: basic controls
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/core_fwd.h>
#include <gui/draw/draw_fwd.h>
#include <gui/draw/text_origin.h>
#include <gui++-look-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace ctrl {

    enum class item_state;

  } // ctrl

  namespace look {

    GUIPP_LOOK_EXPORT void text_item (const draw::graphics&,
                                      const core::rectangle& place,
                                      const draw::brush& background,
                                      const std::string& text,
                                      const ctrl::item_state& state,
                                      const text_origin_t& origin_t = text_origin_t::vcenter_left);

  } // namespace look

} // gui
