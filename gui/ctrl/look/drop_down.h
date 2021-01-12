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
    enum class item_state;

  } // ctrl

  // --------------------------------------------------------------------------
  namespace look {

    template<look_and_feel_t L = system_look_and_feel>
    void drop_down_item_t (const draw::graphics&,
                           const core::rectangle&,
                           const draw::brush&,
                           const std::string&,
                           const ctrl::item_state&);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_item_t<look_and_feel_t::metal> (const draw::graphics&,
                                                                     const core::rectangle&,
                                                                     const draw::brush&,
                                                                     const std::string&,
                                                                     const ctrl::item_state&);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_item_t<look_and_feel_t::w95> (const draw::graphics&,
                                                                   const core::rectangle&,
                                                                   const draw::brush&,
                                                                   const std::string&,
                                                                   const ctrl::item_state&);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_item_t<look_and_feel_t::osx> (const draw::graphics&,
                                                                   const core::rectangle&,
                                                                   const draw::brush&,
                                                                   const std::string&,
                                                                   const ctrl::item_state&);

    GUIPP_LOOK_EXPORT void drop_down_item (const draw::graphics& g,
                                           const core::rectangle& r,
                                           const draw::brush& background,
                                           const std::string& label,
                                           const ctrl::item_state& state);

    template<look_and_feel_t L = system_look_and_feel>
    void drop_down_button_t (const draw::graphics&,
                             const core::rectangle&,
                             const core::button_state::is&,
                             bool);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_button_t<look_and_feel_t::metal> (const draw::graphics&,
                                                                       const core::rectangle&,
                                                                       const core::button_state::is&,
                                                                       bool);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_button_t<look_and_feel_t::w95> (const draw::graphics&,
                                                                     const core::rectangle&,
                                                                     const core::button_state::is&,
                                                                     bool);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_button_t<look_and_feel_t::osx> (const draw::graphics&,
                                                                     const core::rectangle&,
                                                                     const core::button_state::is&,
                                                                     bool);

    GUIPP_LOOK_EXPORT void drop_down_button (const draw::graphics& graph,
                                             const core::rectangle& r,
                                             const core::button_state::is& state,
                                             bool is_open);


    template<look_and_feel_t L = system_look_and_feel>
    void drop_down_t (const draw::graphics&, const core::rectangle&, bool);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_t<look_and_feel_t::metal> (const draw::graphics&, const core::rectangle&, bool);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_t<look_and_feel_t::w95> (const draw::graphics&, const core::rectangle&, bool);

    template<>
    GUIPP_LOOK_EXPORT void drop_down_t<look_and_feel_t::osx> (const draw::graphics&, const core::rectangle&, bool);

    GUIPP_LOOK_EXPORT void drop_down (const draw::graphics& graph,
                                      const core::rectangle& area,
                                      bool focused);

  } // look

} // gui
