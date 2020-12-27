/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: menu
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/core_fwd.h>
#include <gui/core/gui_types.h>
#include <gui/draw/draw_fwd.h>
#include <gui/core/keys.h>
#include <gui/ctrl/item_state.h>
#include <gui++-look-export.h>


namespace gui {

  namespace look {

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void draw_menu_label (const draw::graphics& g,
                                            const core::rectangle& r,
                                            const std::string& label,
                                            char menu_key,
                                            os::color color);

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void menu_item (const draw::graphics& g,
                                      const core::rectangle& r,
                                      const draw::brush& background,
                                      core::point::type text_pos,
                                      core::point::type hotkey_pos,
                                      const std::string& label,
                                      char menu_key,
                                      const draw::masked_bitmap& icon,
                                      const core::hot_key& hotkey,
                                      bool is_sub_menu,
                                      bool separator,
                                      ctrl::item_state state);

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void main_menu_item (const draw::graphics& g,
                                           const core::rectangle& r,
                                           const draw::brush& background,
                                           const std::string& label,
                                           char menu_key,
                                           ctrl::item_state state);

    // --------------------------------------------------------------------------
  } // namespace look

} // gui
