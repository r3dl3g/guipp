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

 #ifdef GUIPP_JS

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

    core::context* get_context (const core::event& e) {
      return std::get<core::context*>(e.param_0);
    }

    bool is_mouse_event (const core::event& e) {
      return (static_cast<int>(e.type) >= os::js::MouseMin) && (static_cast<int>(e.type) <= os::js::MouseMax);
    }

    core::native_point get_mouse_event_point (const core::event& e) {
      return mouse_move_event::Caller::get_param<1>(e);
    }

    bool is_key_event (const core::event& e) {
      return (e.type == os::js::event_type::KeyDown) || (e.type == os::js::event_type::KeyUp);
    }

    bool is_hotkey_event (const core::event& e) {
      return (e.type == os::js::event_type::KeyDown);
    }

    os::key_state get_key_state (const core::event& e) {
      return std::get<os::key_state>(e.param_0);
    }

    os::key_symbol get_key_symbol (const core::event& e) {
      return std::get<os::key_symbol>(e.param_1);
    }

    std::string get_key_chars (const core::event& e) {
      return e.chars;
    }

  } // win

} // gui

#endif //GUIPP_JS
