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

 #ifdef GUIPP_X11

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/window_event_handler.h"
#include "gui/win/overlapped_window.h"
#include "gui/win/native.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    os::key_state get_key_state (const core::event& e) {
      return static_cast<os::key_state>(get_event_state<XKeyEvent>(e));
    }

    // --------------------------------------------------------------------------
    os::key_symbol get_key_symbol (const core::event& e) {
      return XLookupKeysym(const_cast<XKeyEvent*>(&e.xkey), 0);
    }

    // --------------------------------------------------------------------------
    std::string get_key_chars (const core::event& e) {
      XIC ic = native::x11::get_window_ic(e.xany.window);
      if (ic) {
        Status status;
        char text[8] = {0};
        KeySym keySym = 0;
        int len = Xutf8LookupString(ic, const_cast<XKeyEvent*>(&e.xkey), text, 8, &keySym, &status);
        if ((status == XLookupChars) || (status == XLookupBoth)) {
          return std::string(text, len);
        }
      } else {
        char text[8] = {0};
        KeySym keySym = 0;
        int len = XLookupString(const_cast<XKeyEvent*>(&e.xkey), text, 8, &keySym, 0);
        return std::string(text, len);
      }
      return std::string();
    }
    // --------------------------------------------------------------------------
    core::context* get_context (const core::event& e) {
      return get_client_data<0, core::context*>(e);
    }
    // --------------------------------------------------------------------------
    const core::native_rect* get_paint_rect (const core::event& e) {
      return get_client_data<1, const core::native_rect*>(e);
    }
    // --------------------------------------------------------------------------
    core::rectangle get_client_data_rect (const core::event& e) {
      auto& l = e.xclient.data.l;
      return core::global::scale_from_native(core::native_rect{static_cast<short>(l[0]), static_cast<short>(l[1]),
                                                               static_cast<unsigned short>(l[2]), static_cast<unsigned short>(l[3])});
    }
    // --------------------------------------------------------------------------
    core::size get_client_data_size (const core::event& e) {
      auto& l = e.xclient.data.l;
      return core::size(os::size{static_cast<unsigned short>(l[0]), static_cast<unsigned short>(l[1])});
    }
    // --------------------------------------------------------------------------
    template<>
    window* get_client_data<0, window*>(const core::event& e) {
      os::window id = e.xclient.data.l[0];
      return native::get_window(id);
    }
    // --------------------------------------------------------------------------
    template<>
    core::point::type get_client_data<0, core::point::type>(const core::event& e) {
      return *get_client_data<0, core::point::type*>(e);
    }
    // --------------------------------------------------------------------------
    window* get_current_focus_window (const core::event&) {
      return global::get_current_focus_window();
    }
    // --------------------------------------------------------------------------
    core::native_point get_root_mouse_pos (const core::event& e) {
      auto me = (event_type_cast<XMotionEvent>(e));
      return core::native_point{me.x_root, me.y_root};
    }
    // --------------------------------------------------------------------------
    std::map<Window, core::rectangle> s_last_place;
    // --------------------------------------------------------------------------
    template<>
    const core::size& get_last_geometry<core::size>(os::window w) {
      return s_last_place[w].size();
    }
    // --------------------------------------------------------------------------
    template<>
    const core::point& get_last_geometry<core::point>(os::window w) {
      return s_last_place[w].position();
    }
    // --------------------------------------------------------------------------
    template<>
    const core::rectangle& get_last_geometry<core::rectangle>(os::window w) {
      return s_last_place[w];
    }
    // --------------------------------------------------------------------------
    void update_last_geometry (os::window w, const core::rectangle& r) {
      s_last_place[w] = r;
    }
    // --------------------------------------------------------------------------
    void clear_last_geometry (os::window w) {
      s_last_place.erase(w);
    }
    // --------------------------------------------------------------------------
    bool is_mouse_event (const core::event& e) {
      switch (e.type) {
        case MotionNotify:
        case ButtonPress:
        case ButtonRelease:
        case EnterNotify:
        case LeaveNotify:
          return true;
        default:
          return false;
      }
    }

    core::native_point get_mouse_event_point (const core::event& e) {
      return mouse_move_event::Caller::get_param<1>(e);
    }

    bool is_key_event (const core::event& e) {
      switch (e.type) {
        case KeyPress:
        case KeyRelease:
          return true;
        default:
          return false;
      }
    }

    bool is_hotkey_event (const core::event& e) {
      return (e.type == KeyPress);
    }

  } // win

} // gui

#endif // GUIPP_X11
