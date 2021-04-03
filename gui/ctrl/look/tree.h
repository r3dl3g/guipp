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
 * @brief     C++ API: tree control
 *
 * @file
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
#include <gui/draw/draw_fwd.h>
#include <gui/ctrl/item_state.h>
#include <gui/ctrl/look/gui++-look-export.h>


namespace gui {

  namespace tree {

    typedef void (icon_drawer) (draw::graphics&, const draw::pen&, const core::point&, core::size::type);

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT const icon_drawer* standard_icon_drawer (bool has_children, bool is_open, bool selected);

  } // namespace tree

  namespace look {

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void tree_button (draw::graphics& graph,
                                        const core::rectangle& area,
                                        bool is_open,
                                        const draw::pen&);

    GUIPP_LOOK_EXPORT void tree_node (draw::graphics& graph,
                                      const core::rectangle& area,
                                      const draw::brush& background,
                                      std::size_t depth,
                                      const std::string& label,
                                      const tree::icon_drawer* icon,
                                      bool has_children,
                                      bool is_open,
                                      ctrl::item_state state);

  } // look

} // gui
