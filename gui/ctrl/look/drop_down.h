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
 * @brief     C++ API: drop down list
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>
#include <vector>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/core_fwd.h>
#include <gui/draw/draw_fwd.h>
#include <gui/ctrl/item_state.h>
#include <gui/core/button_state.h>
#include <gui/ctrl/look/look_and_feel.h>
#include <gui++-look-export.h>


namespace gui {

  namespace ctrl {

    struct button_state;
    struct item_state;

  } // ctrl

  // --------------------------------------------------------------------------
  namespace look {

    GUIPP_LOOK_EXPORT void drop_down_item (draw::graphics& g,
                                           const core::rectangle& r,
                                           const draw::brush& background,
                                           const std::string& label,
                                           const ctrl::item_state& state);

    GUIPP_LOOK_EXPORT std::vector<core::point> get_up_down_polygon (const core::rectangle& area, bool up);

    template<look_and_feel_t L = system_look_and_feel>
    void drop_down_button_t (draw::graphics&,
                             const core::rectangle&,
                             const core::button_state::is&,
                             bool);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_button_t<look_and_feel_t::metal> (draw::graphics&,
                                                                       const core::rectangle&,
                                                                       const core::button_state::is&,
                                                                       bool);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_button_t<look_and_feel_t::w95> (draw::graphics&,
                                                                     const core::rectangle&,
                                                                     const core::button_state::is&,
                                                                     bool);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_button_t<look_and_feel_t::w10> (draw::graphics&,
                                                                     const core::rectangle&,
                                                                     const core::button_state::is&,
                                                                     bool);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_button_t<look_and_feel_t::osx> (draw::graphics&,
                                                                     const core::rectangle&,
                                                                     const core::button_state::is&,
                                                                     bool);

    GUIPP_LOOK_EXPORT void drop_down_button (draw::graphics& graph,
                                             const core::rectangle& r,
                                             const core::button_state::is& state,
                                             bool is_open);


    template<look_and_feel_t L = system_look_and_feel>
    void drop_down_t (draw::graphics&, const core::rectangle&, const core::button_state::is&);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_t<look_and_feel_t::metal> (draw::graphics&,
                                                                const core::rectangle&,
                                                                const core::button_state::is&);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_t<look_and_feel_t::w95> (draw::graphics&,
                                                              const core::rectangle&,
                                                              const core::button_state::is&);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_t<look_and_feel_t::w10> (draw::graphics&,
                                                              const core::rectangle&,
                                                              const core::button_state::is&);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_t<look_and_feel_t::osx> (draw::graphics&,
                                                              const core::rectangle&,
                                                              const core::button_state::is&);

    GUIPP_LOOK_EXPORT void drop_down (draw::graphics& graph,
                                      const core::rectangle& area,
                                      const core::button_state::is& state);

  } // look

} // gui
