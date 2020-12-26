/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
#include <gui/draw/draw_fwd.h>
#include <gui/look/button_state.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace look {
    GUIPP_LOOK_EXPORT void button_frame (const draw::graphics& graph,
                                         const core::rectangle& r,
                                         const ctrl::button_state::is& state);

    GUIPP_LOOK_EXPORT void button_frame_w95 (const draw::graphics& graph,
                                             const core::rectangle& r,
                                             const ctrl::button_state::is& state);

    GUIPP_LOOK_EXPORT void button_frame_w95 (const draw::graphics& graph,
                                             const core::rectangle& r,
                                             bool enabled, bool pushed,
                                             bool hilited, bool focused);

    GUIPP_LOOK_EXPORT void simple_frame (const draw::graphics& graph,
                                         const core::rectangle& r,
                                         bool hilite = false,
                                         uint32_t horizontal = 3,
                                         uint32_t vertical = 3);

    GUIPP_LOOK_EXPORT void flat_button (const draw::graphics& g,
                                        const core::rectangle& r,
                                        const std::string& text,
                                        const ctrl::button_state::is& state,
                                        os::color foreground = color::white,
                                        os::color background = color::dark_gray);

    template<os::color foreground, os::color background>
    void color_flat_button (const draw::graphics& g,
                            const core::rectangle& r,
                            const std::string& text,
                            const ctrl::button_state::is& state) {
      flat_button(g, r, text, state, foreground, background);
    }

    GUIPP_LOOK_EXPORT void push_button (const draw::graphics& graph,
                                        const core::rectangle& r,
                                        const std::string& text,
                                        const ctrl::button_state::is& state);

    GUIPP_LOOK_EXPORT void radio_button (const draw::graphics& graph,
                                         const core::rectangle& area,
                                         const std::string& text,
                                         const ctrl::button_state::is& state);

    GUIPP_LOOK_EXPORT void check_box (const draw::graphics& graph,
                                      const core::rectangle& area,
                                      const std::string& text,
                                      const ctrl::button_state::is& state);

    GUIPP_LOOK_EXPORT void switch_button (const draw::graphics& graph,
                                          const core::rectangle& rect,
                                          const std::string& text,
                                          const ctrl::button_state::is& state);

    GUIPP_LOOK_EXPORT void animated_switch_button (const draw::graphics& graph,
                                                   const core::rectangle& rect,
                                                   const std::string& text,
                                                   const ctrl::button_state::is& state,
                                                   float animation_step = 1.0F);

    GUIPP_LOOK_EXPORT void tab_button (const draw::graphics& g,
                                       const core::rectangle& r,
                                       const std::string& text,
                                       const ctrl::button_state::is& state,
                                       os::color foreground,
                                       alignment_t a);

    template<os::color foreground, alignment_t align>
    void aligned_tab_button (const draw::graphics& g,
                             const core::rectangle& r,
                             const std::string& text,
                             const ctrl::button_state::is& state) {
      tab_button(g, r, text, state, foreground, align);
    }

  } // namespace look

  // --------------------------------------------------------------------------

} // gui
