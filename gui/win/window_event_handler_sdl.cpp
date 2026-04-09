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
 * @brief     generic window events
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_SDL

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/window_event_handler.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    os::key_state get_key_state (const core::event& e) {
      return static_cast<os::key_state>(e.key.keysym.mod);
    }

    os::key_symbol get_key_symbol (const core::event& e) {
      return e.key.keysym.sym;
    }

    std::string get_key_chars (const core::event& e) {
      return e.text.text;
    }

    // --------------------------------------------------------------------------
    core::rectangle get_client_data_rect (const core::event& e) {
      return *static_cast<const core::rectangle*>(e.user.data1);
    }

    core::context* get_context (const core::event& e) {
      return static_cast<core::context*>(e.user.data1);
    }

    const core::native_rect* get_paint_rect (const core::event& e) {
      return static_cast<const core::native_rect*>(e.user.data2);
    }

    bool is_mouse_event (const core::event& e) {
      switch (e.type) {
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
          return true;
        default:
          return false;
      }
    }

    core::native_point get_mouse_event_point (const core::event& e) {
      switch (e.type) {
        case SDL_MOUSEMOTION:
          return mouse_move_event::Caller::get_param<1>(e);
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          return btn_down_event::Caller::get_param<1>(e);
        case SDL_MOUSEWHEEL:
          return wheel_x_event::Caller::get_param<1>(e);
        default:
        return core::native_point::undefined;
      }
    }

    bool is_key_event (const core::event& e) {
      switch (e.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        case SDL_TEXTINPUT:
          return true;
        default:
          return false;
      }
    }

    bool is_hotkey_event (const core::event& e) {
      return (e.type == SDL_KEYDOWN);
    }

  } // win

} // gui

#endif // GUIPP_SDL
