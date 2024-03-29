/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://wwrothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     various button looks
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
#include <gui/core/button_state.h>
#include <gui/draw/draw_fwd.h>
#include <gui/ctrl/look/look_and_feel.h>
#include <gui/ctrl/look/gui++-look-export.h>


namespace gui {

  namespace draw {

    template<pixel_format_t T>
    class datamap;

    typedef datamap<pixel_format_t::RGB> rgbmap;

  }
  // --------------------------------------------------------------------------
  namespace look {

    // --------------------------------------------------------------------------
    namespace win10 {

      GUIPP_LOOK_EXPORT os::color get_button_color (const core::button_state::is& state);
      GUIPP_LOOK_EXPORT os::color get_button_frame_color (const core::button_state::is& state);

    } // namespace win10

    // --------------------------------------------------------------------------
    namespace osx {

      GUIPP_LOOK_EXPORT const draw::rgbmap& get_frame ();
      GUIPP_LOOK_EXPORT const draw::rgbmap& get_disabled_frame ();
      GUIPP_LOOK_EXPORT const draw::rgbmap& get_focused_frame ();
      GUIPP_LOOK_EXPORT const draw::rgbmap& get_pressed_frame ();

    } // namespace osx

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void simple_frame (draw::graphics& graph,
                                         const core::rectangle& r,
                                         bool hilite = false,
                                         uint32_t horizontal = 3,
                                         uint32_t vertical = 3);

    GUIPP_LOOK_EXPORT void simple_frame (draw::graphics& g,
                                         const core::rectangle& r,
                                         const core::button_state::is& state,
                                         os::color foreground,
                                         os::color background);

    template<uint32_t horizontal = 3, uint32_t vertical = 3>
    inline void simple_frame (draw::graphics& g,
                       const core::rectangle& r,
                       const core::button_state::is& state) {
      simple_frame(g, r, state.hilited(), horizontal, vertical);
    }

    // --------------------------------------------------------------------------
    template<look_and_feel_t L = system_look_and_feel>
    void button_frame_t (draw::graphics& graph,
                         const core::rectangle& r,
                         const core::button_state::is& state,
                         os::color fg = color::black,
                         os::color bg = color::gray);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_LOOK_EXPORT void button_frame_t<look_and_feel_t::metal> (draw::graphics& graph,
                                                                   const core::rectangle& r,
                                                                   const core::button_state::is& state,
                                                                   os::color fg,
                                                                   os::color bg);

    template<>
    GUIPP_LOOK_EXPORT void button_frame_t<look_and_feel_t::w95> (draw::graphics& graph,
                                                                 const core::rectangle& r,
                                                                 const core::button_state::is& state,
                                                                 os::color fg,
                                                                 os::color bg);

    template<>
    GUIPP_LOOK_EXPORT void button_frame_t<look_and_feel_t::w10> (draw::graphics& graph,
                                                                 const core::rectangle& r,
                                                                 const core::button_state::is& state,
                                                                 os::color fg,
                                                                 os::color bg);

    template<>
    GUIPP_LOOK_EXPORT void button_frame_t<look_and_feel_t::osx> (draw::graphics& graph,
                                                                 const core::rectangle& r,
                                                                 const core::button_state::is& state,
                                                                 os::color fg,
                                                                 os::color bg);

    GUIPP_LOOK_EXPORT void button_frame (draw::graphics& graph,
                                         const core::rectangle& r,
                                         const core::button_state::is& state,
                                         os::color foreground,
                                         os::color background);

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void button_text (draw::graphics& g,
                                        const core::rectangle& r,
                                        const std::string& text,
                                        const core::button_state::is& state,
                                        os::color foreground = color::windowTextColor(),
                                        os::color background = color::transparent);

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT os::color get_flat_button_background (const core::button_state::is& state,
                                                            os::color background);

    GUIPP_LOOK_EXPORT os::color get_flat_button_foreground (const core::button_state::is& state,
                                                            os::color foreground, os::color background);

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void flat_button_frame (draw::graphics& g,
                                              const core::rectangle& r,
                                              const core::button_state::is& state,
                                              os::color foreground = color::white,
                                              os::color background = color::dark_gray);

    template<os::color foreground, os::color background>
    inline void flat_button_frame (draw::graphics& g,
                                   const core::rectangle& r,
                                   const core::button_state::is& state) {
      flat_button_frame(g, r, state, foreground, background);
    }

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void flat_button_text (draw::graphics& g,
                                             const core::rectangle& r,
                                             const std::string& text,
                                             const core::button_state::is& state,
                                             os::color foreground = color::white,
                                             os::color background = color::dark_gray);

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void flat_button (draw::graphics& g,
                                        const core::rectangle& r,
                                        const std::string& text,
                                        const core::button_state::is& state,
                                        os::color foreground = color::white,
                                        os::color background = color::dark_gray);

    GUIPP_LOOK_EXPORT void push_button (draw::graphics& graph,
                                        const core::rectangle& r,
                                        const std::string& text,
                                        const core::button_state::is& state,
                                        os::color foreground = color::windowTextColor(),
                                        os::color background = color::transparent);

    // --------------------------------------------------------------------------
    template<look_and_feel_t L = system_look_and_feel>
    os::color get_button_foreground (const core::button_state::is& s, os::color fg, os::color bg);

    template<>
    GUIPP_LOOK_EXPORT os::color get_button_foreground<look_and_feel_t::metal> (const core::button_state::is& s, os::color fg, os::color bg);

    template<>
    GUIPP_LOOK_EXPORT os::color get_button_foreground<look_and_feel_t::w95> (const core::button_state::is& s, os::color fg, os::color bg);

    template<>
    GUIPP_LOOK_EXPORT os::color get_button_foreground<look_and_feel_t::w10> (const core::button_state::is& s, os::color fg, os::color bg);

    template<>
    GUIPP_LOOK_EXPORT os::color get_button_foreground<look_and_feel_t::osx> (const core::button_state::is& s, os::color fg, os::color bg);

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void check_button_text (draw::graphics& g,
                                              const core::rectangle& r,
                                              const std::string& text,
                                              const core::button_state::is& state,
                                              os::color foreground = color::white,
                                              os::color background = color::dark_gray);

    // --------------------------------------------------------------------------
    template<look_and_feel_t L = system_look_and_feel>
    void radio_button_frame (draw::graphics& graph,
                             const core::rectangle& area,
                             const core::button_state::is& state,
                             os::color foreground,
                             os::color background);

    template<>
    GUIPP_LOOK_EXPORT void radio_button_frame<look_and_feel_t::metal> (draw::graphics& graph,
                                                                       const core::rectangle& area,
                                                                       const core::button_state::is& state,
                                                                       os::color foreground,
                                                                       os::color background);

    template<>
    GUIPP_LOOK_EXPORT void radio_button_frame<look_and_feel_t::w95> (draw::graphics& graph,
                                                                     const core::rectangle& area,
                                                                     const core::button_state::is& state,
                                                                     os::color foreground,
                                                                     os::color background);

    template<>
    GUIPP_LOOK_EXPORT void radio_button_frame<look_and_feel_t::w10> (draw::graphics& graph,
                                                                     const core::rectangle& area,
                                                                     const core::button_state::is& state,
                                                                     os::color foreground,
                                                                     os::color background);

    template<>
    GUIPP_LOOK_EXPORT void radio_button_frame<look_and_feel_t::osx> (draw::graphics& graph,
                                                                     const core::rectangle& area,
                                                                     const core::button_state::is& state,
                                                                     os::color foreground,
                                                                     os::color background);

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void radio_button (draw::graphics& graph,
                                         const core::rectangle& area,
                                         const std::string& text,
                                         const core::button_state::is& state,
                                         os::color foreground = color::windowTextColor(),
                                         os::color background = color::transparent);

    // --------------------------------------------------------------------------
    template<look_and_feel_t L = system_look_and_feel>
    void check_box_frame (draw::graphics& graph,
                          const core::rectangle& area,
                          const core::button_state::is& state,
                          os::color foreground,
                          os::color background);

    template<>
    GUIPP_LOOK_EXPORT void check_box_frame<look_and_feel_t::metal> (draw::graphics& graph,
                                                                    const core::rectangle& area,
                                                                    const core::button_state::is& state,
                                                                    os::color foreground,
                                                                    os::color background);

    template<>
    GUIPP_LOOK_EXPORT void check_box_frame<look_and_feel_t::w95> (draw::graphics& graph,
                                                                  const core::rectangle& area,
                                                                  const core::button_state::is& state,
                                                                  os::color foreground,
                                                                  os::color background);

    template<>
    GUIPP_LOOK_EXPORT void check_box_frame<look_and_feel_t::w10> (draw::graphics& graph,
                                                                  const core::rectangle& area,
                                                                  const core::button_state::is& state,
                                                                  os::color foreground,
                                                                  os::color background);

    template<>
    GUIPP_LOOK_EXPORT void check_box_frame<look_and_feel_t::osx> (draw::graphics& graph,
                                                                  const core::rectangle& area,
                                                                  const core::button_state::is& state,
                                                                  os::color foreground,
                                                                  os::color background);

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void check_box (draw::graphics& graph,
                                      const core::rectangle& area,
                                      const std::string& text,
                                      const core::button_state::is& state,
                                      os::color foreground = color::windowTextColor(),
                                      os::color background = color::transparent);

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void switch_button (draw::graphics& graph,
                                          const core::rectangle& rect,
                                          const core::button_state::is& state,
                                          os::color foreground = color::buttonTextColor(),
                                          os::color background = color::buttonColor());

    GUIPP_LOOK_EXPORT void switch_button_text (draw::graphics& graph,
                                               const core::rectangle& rect,
                                               const std::string& text,
                                               const core::button_state::is& state,
                                               os::color foreground = color::buttonTextColor(),
                                               os::color background = color::buttonColor());

    // --------------------------------------------------------------------------
    GUIPP_LOOK_EXPORT void animated_switch_button (draw::graphics& graph,
                                                   const core::rectangle& rect,
                                                   const core::button_state::is& state,
                                                   os::color foreground,
                                                   os::color background,
                                                   float animation_step = 1.0F);

    // --------------------------------------------------------------------------
    template<look_and_feel_t L = system_look_and_feel>
    void tab_button (draw::graphics& g,
                     const core::rectangle& r,
                     const core::button_state::is& state,
                     os::color foreground,
                     alignment_t a) {
      button_frame(g, r, state, foreground, color::transparent);
    }

    template<>
    GUIPP_LOOK_EXPORT void tab_button<look_and_feel_t::metal> (draw::graphics& g,
                                                               const core::rectangle& r,
                                                               const core::button_state::is& state,
                                                               os::color foreground,
                                                               alignment_t a);

    GUIPP_LOOK_EXPORT void tab_button_text (draw::graphics& graph,
                                            const core::rectangle& rect,
                                            const std::string& text,
                                            const core::button_state::is& state,
                                            os::color foreground = color::buttonTextColor(),
                                            os::color background = color::buttonColor());

    // --------------------------------------------------------------------------
    template<alignment_t align>
    void aligned_tab_button (draw::graphics& g,
                             const core::rectangle& r,
                             const core::button_state::is& state,
                             os::color fg,
                             os::color bg) {
      tab_button<>(g, r, state, fg, align);
    }

    // --------------------------------------------------------------------------
  } // namespace look

  // --------------------------------------------------------------------------

} // gui
