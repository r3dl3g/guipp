/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://wwrothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    standard lib
 *
 * Customer   -
 *
 * @brief     C++ API: button
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/core_fwd.h>
#include <gui/core/color.h>
#include <gui/core/button_state.h>
#include <gui/draw/draw_fwd.h>
#include <gui/ctrl/look/look_and_feel.h>
#include <gui++-look-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace look {

    template<look_and_feel_t L = system_look_and_feel>
    void button_frame (const draw::graphics& graph,
                                             const core::rectangle& r,
                                             bool enabled, bool pushed,
                                             bool hilited, bool focused);

    template<>
    GUIPP_LOOK_EXPORT void button_frame<look_and_feel_t::metal> (const draw::graphics& graph,
                                                                 const core::rectangle& r,
                                                                 bool enabled, bool pushed,
                                                                 bool hilited, bool focused);

    template<>
    GUIPP_LOOK_EXPORT void button_frame<look_and_feel_t::w95> (const draw::graphics& graph,
                                                               const core::rectangle& r,
                                                               bool enabled, bool pushed,
                                                               bool hilited, bool focused);

    template<>
    GUIPP_LOOK_EXPORT void button_frame<look_and_feel_t::osx> (const draw::graphics& graph,
                                                               const core::rectangle& r,
                                                               bool enabled, bool pushed,
                                                               bool hilited, bool focused);

    template<look_and_feel_t L = system_look_and_feel>
    inline void button_frame (const draw::graphics& graph,
                              const core::rectangle& r,
                              const core::button_state::is& state) {
      button_frame<L>(graph, r,
                      state.enabled(), state.pushed(),
                      state.hilited(), state.focused());
    }

    GUIPP_LOOK_EXPORT void simple_frame (const draw::graphics& graph,
                                         const core::rectangle& r,
                                         bool hilite = false,
                                         uint32_t horizontal = 3,
                                         uint32_t vertical = 3);

    GUIPP_LOOK_EXPORT void flat_button (const draw::graphics& g,
                                        const core::rectangle& r,
                                        const std::string& text,
                                        const core::button_state::is& state,
                                        os::color foreground = color::white,
                                        os::color background = color::dark_gray);

    template<os::color foreground, os::color background>
    void color_flat_button (const draw::graphics& g,
                            const core::rectangle& r,
                            const std::string& text,
                            const core::button_state::is& state) {
      flat_button(g, r, text, state, foreground, background);
    }

    GUIPP_LOOK_EXPORT void push_button (const draw::graphics& graph,
                                        const core::rectangle& r,
                                        const std::string& text,
                                        const core::button_state::is& state);

    template<look_and_feel_t L = system_look_and_feel>
    void radio_button_t (const draw::graphics& graph,
                         const core::rectangle& area,
                         const std::string& text,
                         const core::button_state::is& state);

    template<>
    GUIPP_LOOK_EXPORT void radio_button_t<look_and_feel_t::metal> (const draw::graphics& graph,
                                                                   const core::rectangle& area,
                                                                   const std::string& text,
                                                                   const core::button_state::is& state);

    template<>
    GUIPP_LOOK_EXPORT void radio_button_t<look_and_feel_t::w95> (const draw::graphics& graph,
                                                                 const core::rectangle& area,
                                                                 const std::string& text,
                                                                 const core::button_state::is& state);

    template<>
    GUIPP_LOOK_EXPORT void radio_button_t<look_and_feel_t::osx> (const draw::graphics& graph,
                                                                 const core::rectangle& area,
                                                                 const std::string& text,
                                                                 const core::button_state::is& state);

    GUIPP_LOOK_EXPORT void radio_button (const draw::graphics& graph,
                                         const core::rectangle& area,
                                         const std::string& text,
                                         const core::button_state::is& state);

    template<look_and_feel_t L = system_look_and_feel>
    void check_box_t (const draw::graphics& graph,
                      const core::rectangle& area,
                      const std::string& text,
                      const core::button_state::is& state);

    template<>
    GUIPP_LOOK_EXPORT void check_box_t<look_and_feel_t::metal> (const draw::graphics& graph,
                                                                const core::rectangle& area,
                                                                const std::string& text,
                                                                const core::button_state::is& state);

    template<>
    GUIPP_LOOK_EXPORT void check_box_t<look_and_feel_t::w95> (const draw::graphics& graph,
                                                              const core::rectangle& area,
                                                              const std::string& text,
                                                              const core::button_state::is& state);

    template<>
    GUIPP_LOOK_EXPORT void check_box_t<look_and_feel_t::osx> (const draw::graphics& graph,
                                                              const core::rectangle& area,
                                                              const std::string& text,
                                                              const core::button_state::is& state);

    GUIPP_LOOK_EXPORT void check_box (const draw::graphics& graph,
                                      const core::rectangle& area,
                                      const std::string& text,
                                      const core::button_state::is& state);

    GUIPP_LOOK_EXPORT void switch_button (const draw::graphics& graph,
                                          const core::rectangle& rect,
                                          const std::string& text,
                                          const core::button_state::is& state);

    GUIPP_LOOK_EXPORT void animated_switch_button (const draw::graphics& graph,
                                                   const core::rectangle& rect,
                                                   const std::string& text,
                                                   const core::button_state::is& state,
                                                   float animation_step = 1.0F);

    GUIPP_LOOK_EXPORT void tab_button (const draw::graphics& g,
                                       const core::rectangle& r,
                                       const std::string& text,
                                       const core::button_state::is& state,
                                       os::color foreground,
                                       alignment_t a);

    template<os::color foreground, alignment_t align>
    void aligned_tab_button (const draw::graphics& g,
                             const core::rectangle& r,
                             const std::string& text,
                             const core::button_state::is& state) {
      tab_button(g, r, text, state, foreground, align);
    }

  } // namespace look

  // --------------------------------------------------------------------------

} // gui
