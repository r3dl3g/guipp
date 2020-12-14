/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: window events
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <cstddef>

#ifdef GUIPP_QT
#include <QtGui/qevent.h>
#endif // GUIPP_QT

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <gui/core/event_handler.h>
#include <gui/core/gui_types.h>
#include <gui/win/window_event_proc.h>


namespace gui {

  namespace win {

    class window;

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT os::key_state get_key_state (const core::event& e);
    GUIPP_WIN_EXPORT os::key_symbol get_key_symbol (const core::event& e);
    GUIPP_WIN_EXPORT std::string get_key_chars (const core::event& e);
    GUIPP_WIN_EXPORT core::point get_root_mouse_pos (const core::event& e);

    namespace event {

      // --------------------------------------------------------------------------
      template<core::event_matcher fnct>
      struct functor {
        inline bool operator() (const core::event& e) {
          return fnct(e);
        }
      };

    }

    GUIPP_WIN_EXPORT void send_client_message (window* win, os::message_type message, long l1 = 0, long l2 = 0);
    GUIPP_WIN_EXPORT void post_client_message (window* win, os::message_type message, long l1 = 0, long l2 = 0);

    GUIPP_WIN_EXPORT void send_client_message (window* win, os::message_type message, const core::size& sz);
    GUIPP_WIN_EXPORT void send_client_message (window* win, os::message_type message, const core::rectangle& wr);

#ifdef GUIPP_WIN
    // --------------------------------------------------------------------------
    template<int I, typename T>
    T get_param (const core::event& e);
    // --------------------------------------------------------------------------
    template<> GUIPP_WIN_EXPORT os::graphics get_param<0>(const core::event& e);
    template<> GUIPP_WIN_EXPORT window* get_param<0>(const core::event& e);
    template<> GUIPP_WIN_EXPORT bool get_param<0>(const core::event& e);
    template<> GUIPP_WIN_EXPORT unsigned int get_param<0>(const core::event& e);
    template<> GUIPP_WIN_EXPORT int get_param<0>(const core::event& e);
    template<> GUIPP_WIN_EXPORT os::window get_param<0, os::window>(const core::event& e);
    // --------------------------------------------------------------------------
    template<> GUIPP_WIN_EXPORT window* get_param<1>(const core::event& e);
    template<> GUIPP_WIN_EXPORT core::point get_param<1>(const core::event& e);
    template<> GUIPP_WIN_EXPORT core::size get_param<1>(const core::event& e);
    template<> GUIPP_WIN_EXPORT core::rectangle get_param<1>(const core::event& e);
    // --------------------------------------------------------------------------
    template<typename T>
    core::rectangle get_rect (const core::event& e) {
      T& p = *reinterpret_cast<T*>(e.lParam);
      return core::global::scale_from_native(core::native_rect(p.x, p.y, p.cx, p.cy));
    }

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT window* get_window_from_cs (const core::event& e);
    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT window* get_window_from_id (const core::event& e);
    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT unsigned int get_flags_from_wp (const core::event& e);
    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT core::point::type get_wheel_delta (const core::event& e);
    // --------------------------------------------------------------------------
    template<bool down>
    bool any_button_matcher (const core::event& e);

    template<>
    GUIPP_WIN_EXPORT bool any_button_matcher<true> (const core::event& e);

    template<>
    GUIPP_WIN_EXPORT bool any_button_matcher<false> (const core::event& e);

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT bool match_key (const core::event& e, os::event_id id, os::key_symbol sym);

    // --------------------------------------------------------------------------
    template<os::event_id id, os::key_symbol sym, os::key_state state>
    bool key_symbol_matcher (const core::event& e) {
      return match_key(e, id, sym) && core::bit_mask<gui::os::key_state, state>::is_set(get_key_state(e));
    }

    template<os::key_symbol sym, os::key_state state>
    bool key_down_matcher (const core::event& e) {
      return key_symbol_matcher<WM_KEYDOWN, sym, state>(e);
    }

    template<os::key_symbol sym, os::key_state state>
    bool key_up_matcher (const core::event& e) {
      return key_symbol_matcher<WM_KEYUP, sym, state>(e);
    }

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT bool mouse_enter_matcher (const core::event& e);

    // --------------------------------------------------------------------------
    template<os::event_id id, bool show>
    inline bool visibility_event_type_matcher (const core::event& e) {
      return (e.type == id) && (get_param<0, bool>(e) == show);
    }

    // --------------------------------------------------------------------------
    struct GUIPP_WIN_EXPORT pos_changing_getter : core::params<core::rectangle>::getter<get_param<1, core::rectangle>> {
      typedef core::params<core::rectangle>::getter<get_param<1, core::rectangle>> super;

      pos_changing_getter (const function cb)
        : super(cb)
      {}

      template<class T>
      pos_changing_getter (T* win, void(T::*changingfn)(core::rectangle &))
        : super(win, changingfn)
      {}

      void operator() (const core::event& e);
    };
    // --------------------------------------------------------------------------
    struct GUIPP_WIN_EXPORT minmax_getter {
      typedef std::function<void (const core::size&, const core::point&, core::size&, core::size&)> function;

      minmax_getter (const function cb)
        : callback(cb)
      {}

      template<class T>
      minmax_getter (T* win, void(T::*fn)(const core::size &, const core::point &, core::size &, core::size &))
        : callback(win, fn)
      {}

      operator bool () const {
        return callback.operator bool ();
      }

      void operator() (const core::event& e);

    private:
      function callback;
    };
    // --------------------------------------------------------------------------
    struct GUIPP_WIN_EXPORT os_paint_getter :
      core::params<os::window, os::graphics>::getter<get_param<0, os::window>, get_param<0, os::graphics>> {

      typedef core::params<os::window, os::graphics>::getter<get_param<0, os::window>, get_param<0, os::graphics>> super;

      os_paint_getter (const function cb)
        : super(cb)
      {}

      template<class T>
      os_paint_getter (T* t, void(T::*callback_)(os::window, os::graphics &))
        : super(util::bind_method(t, callback_))
      {}

      void operator() (const core::event& e);
    };

    // --------------------------------------------------------------------------
    using create_event = core::event_handler<WM_CREATE, 0, core::params<>::getter<>>;

    using close_event = core::event_handler<WM_CLOSE, 0, core::params<>::getter<>, 1>;

    using destroy_event = core::event_handler<WM_DESTROY>;

    using any_key_down_event = core::event_handler<WM_KEYDOWN, 0,
                                                   core::params<os::key_state, os::key_symbol, std::string>::
                                                   getter<get_key_state, get_key_symbol, get_key_chars>>;

    using any_key_up_event = core::event_handler<WM_KEYUP, 0,
                                                 core::params<os::key_state, os::key_symbol>::
                                                 getter<get_key_state, get_key_symbol>>;

    template<os::key_symbol symbol, os::key_state state>
    using key_down_event = core::event_handler<WM_KEYDOWN, 0, core::params<>::getter<>, 0,
                                               event::functor<key_down_matcher<symbol, state>>>;

    template<os::key_symbol symbol, os::key_state state>
    using key_up_event = core::event_handler<WM_KEYUP, 0, core::params<>::getter<>, 0,
                                             event::functor<key_up_matcher<symbol, state>>>;

    using character_event = core::event_handler<WM_CHAR, 0,
                                                core::params<std::string>::
                                                getter<get_key_chars>>;

    using erase_event = core::event_handler<WM_ERASEBKGND, 0,
                                            core::params<os::graphics>::
                                            getter<get_param<0, os::graphics>>>;
    using print_event = core::event_handler<WM_PRINT, 0,
                                            core::params<os::graphics>::
                                            getter<get_param<0, os::graphics>>>;
    using print_client_event = core::event_handler<WM_PRINTCLIENT, 0,
                                                   core::params<os::graphics>::
                                                   getter<get_param<0, os::graphics>>>;

    using redraw_changed_event = core::event_handler<WM_SETREDRAW, 0,
                                                     core::params<bool>::getter<get_param<0, bool>>>;

    using enable_event = core::event_handler<WM_ENABLE, 0,
                                             core::params<bool>::getter<get_param<0, bool>>>;
    using activate_event = core::event_handler<WM_ACTIVATE, 0,
                                               core::params<bool, window*>::
                                               getter<get_param<0, bool>, get_param<1, window*>>>;
    using activate_app_event = core::event_handler<WM_ACTIVATEAPP, 0,
                                                   core::params<bool>::getter<get_param<0, bool>>>;
    using set_focus_event = core::event_handler<WM_SETFOCUS>;
    using lost_focus_event = core::event_handler<WM_KILLFOCUS>;

    using begin_size_or_move_event = core::event_handler<WM_ENTERSIZEMOVE>;
    using end_size_or_move_event = core::event_handler<WM_EXITSIZEMOVE>;

    using move_event = core::event_handler<WM_MOVE, 0,
                                           core::params<core::point>::
                                           getter<get_param<1, core::point>>>;

    using size_event = core::event_handler<WM_SIZE, 0,
                                           core::params<core::size>::
                                           getter<get_param<1, core::size>>>;

    using left_btn_down_event = core::event_handler<WM_LBUTTONDOWN, 0,
                                                    core::params<os::key_state, core::point>::
                                                    getter<get_param<0, os::key_state>,
                                                           get_param<1, core::point>>>;
    using left_btn_up_event = core::event_handler<WM_LBUTTONUP, 0,
                                                  core::params<os::key_state, core::point>::
                                                  getter<get_param<0, os::key_state>,
                                                         get_param<1, core::point>>>;
    using left_btn_dblclk_event = core::event_handler<WM_LBUTTONDBLCLK, 0,
                                                      core::params<os::key_state, core::point>::
                                                      getter<get_param<0, os::key_state>,
                                                             get_param<1, core::point>>>;

    using right_btn_down_event = core::event_handler<WM_RBUTTONDOWN, 0,
                                                     core::params<os::key_state, core::point>::
                                                     getter<get_param<0, os::key_state>,
                                                            get_param<1, core::point>>>;
    using right_btn_up_event = core::event_handler<WM_RBUTTONUP, 0,
                                                   core::params<os::key_state, core::point>::
                                                   getter<get_param<0, os::key_state>,
                                                          get_param<1, core::point>>>;
    using right_btn_dblclk_event = core::event_handler<WM_RBUTTONDBLCLK, 0,
                                                       core::params<os::key_state, core::point>::
                                                       getter<get_param<0, os::key_state>,
                                                              get_param<1, core::point>>>;

    using middle_btn_down_event = core::event_handler<WM_MBUTTONDOWN, 0,
                                                      core::params<os::key_state, core::point>::
                                                      getter<get_param<0, os::key_state>,
                                                             get_param<1, core::point>>>;
    using middle_btn_up_event = core::event_handler<WM_MBUTTONUP, 0,
                                                    core::params<os::key_state, core::point>::
                                                    getter<get_param<0, os::key_state>,
                                                           get_param<1, core::point>>>;
    using middle_btn_dblclk_event = core::event_handler<WM_MBUTTONDBLCLK, 0,
                                                        core::params<os::key_state, core::point>::
                                                        getter<get_param<0, os::key_state>,
                                                               get_param<1, core::point>>>;

    using btn_down_event = core::event_handler<WM_LBUTTONDOWN, 0,
                                               core::params<os::key_state, core::point>::
                                               getter<get_param<0, os::key_state>,
                                                      get_param<1, core::point>>,
                                               0,
                                               event::functor<any_button_matcher<true>>>;
    using btn_up_event = core::event_handler<WM_LBUTTONUP, 0,
                                             core::params<os::key_state, core::point>::
                                             getter<get_param<0, os::key_state>,
                                                    get_param<1, core::point>>,
                                             0,
                                             event::functor<any_button_matcher<false>>>;

    using mouse_move_event = core::event_handler<WM_MOUSEMOVE, 0,
                                                 core::params<os::key_state, core::point>::
                                                 getter<get_param<0, os::key_state>,
                                                        get_param<1, core::point>>>;

    using mouse_move_abs_event = core::event_handler<WM_MOUSEMOVE, 0,
                                                     core::params<os::key_state, core::point>::
                                                     getter<get_param<0, os::key_state>,
                                                            get_root_mouse_pos>>;

    using mouse_enter_event = core::event_handler<WM_MOUSEMOVE, 0,
                                                  core::params<>::getter<>,
                                                  0,
                                                  event::functor<mouse_enter_matcher>>;
    using mouse_leave_event = core::event_handler<WM_MOUSELEAVE>;

    using wheel_x_event = core::event_handler<WM_MOUSEHWHEEL, 0,
                                              core::params<core::point::type, core::point>::
                                              getter<get_wheel_delta,
                                                     get_param<1, core::point>>>;
    using wheel_y_event = core::event_handler<WM_MOUSEWHEEL, 0,
                                              core::params<core::point::type, core::point>::
                                              getter<get_wheel_delta,
                                                     get_param<1, core::point>>>;

    using show_event = core::event_handler<WM_SHOWWINDOW, 0,
                                           core::params<>::getter<>,
                                           0,
                                           event::functor<visibility_event_type_matcher<WM_SHOWWINDOW, true>>>;
    using hide_event = core::event_handler<WM_SHOWWINDOW, 0,
                                           core::params<>::getter<>,
                                           0,
                                           event::functor<visibility_event_type_matcher<WM_SHOWWINDOW, false>>>;

    using get_minmax_event = core::event_handler<WM_GETMINMAXINFO, 0, minmax_getter>;

    using layout_event = core::event_handler<core::WM_LAYOUT_WINDOW, 0,
                                             core::params<core::rectangle>::
                                             getter<get_param<1, core::rectangle>>>;

    using paint_event = core::event_handler<WM_PAINT, 0, os_paint_getter>;

    // --------------------------------------------------------------------------

#endif //GUIPP_WIN

#ifdef GUIPP_X11
    // --------------------------------------------------------------------------
    namespace x11 {

      GUIPP_WIN_EXPORT void send_client_message (window* win, Atom message,
                                long l1 = 0, long l2 = 0, long l3 = 0, long l4 = 0, long l5 = 0);

      GUIPP_WIN_EXPORT void send_client_message (window* win, Atom message, const window* w, const core::rectangle& r);

      GUIPP_WIN_EXPORT void prepare_win_for_event (const window* win, os::event_id mask);
      GUIPP_WIN_EXPORT void unprepare_win (const window* win);

    } // namespace x11

    // --------------------------------------------------------------------------
    template<os::event_id id, os::event_id btn>
    bool event_button_matcher (const core::event& e) {
      return (e.type == id) && (e.xbutton.button == btn);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    const T& event_type_cast (const core::event& e);
    // --------------------------------------------------------------------------
    template<>
    inline const XButtonEvent& event_type_cast<XButtonEvent>(const core::event& e) {
      return e.xbutton;
    }

    template<>
    inline const XCreateWindowEvent& event_type_cast<XCreateWindowEvent>(const core::event& e) {
      return e.xcreatewindow;
    }

    template<>
    inline const XFocusChangeEvent& event_type_cast<XFocusChangeEvent>(const core::event& e) {
      return e.xfocus;
    }

    template<>
    inline const XKeyEvent& event_type_cast<XKeyEvent>(const core::event& e) {
      return e.xkey;
    }

    template<>
    inline const XMotionEvent& event_type_cast<XMotionEvent>(const core::event& e) {
      return e.xmotion;
    }

    template<>
    inline const XConfigureEvent& event_type_cast<XConfigureEvent>(const core::event& e) {
      return e.xconfigure;
    }

    template<>
    inline const XConfigureRequestEvent& event_type_cast<XConfigureRequestEvent>(const core::event& e) {
      return e.xconfigurerequest;
    }

    template<>
    inline const XCrossingEvent& event_type_cast<XCrossingEvent>(const core::event& e) {
      return e.xcrossing;
    }

    template<>
    inline const XDestroyWindowEvent& event_type_cast<XDestroyWindowEvent>(const core::event& e) {
      return e.xdestroywindow;
    }

    // --------------------------------------------------------------------------
    template<typename T, typename C>
    struct get {
      static T param (const core::event& e);
    };
    // --------------------------------------------------------------------------
    template<typename C>
    struct get<core::size, C> {
      static core::size param (const core::event& e) {
        return core::size(core::global::scale_from_native<core::size::type>(event_type_cast<C>(e).width),
                          core::global::scale_from_native<core::size::type>(event_type_cast<C>(e).height));
      }

    };
    // --------------------------------------------------------------------------
    template<typename C>
    struct get<core::point, C> {
      static core::point param (const core::event& e) {
        return core::point(core::global::scale_from_native<core::point::type>(event_type_cast<C>(e).x),
                           core::global::scale_from_native<core::point::type>(event_type_cast<C>(e).y));
      }

    };
    // --------------------------------------------------------------------------
    template<typename C>
    struct get<core::rectangle, C> {
      static core::rectangle param (const core::event& e) {
        return core::rectangle(get<core::point, C>::param(e), get<core::size, C>::param(e));
      }

    };
    // --------------------------------------------------------------------------
    template<typename T>
    const T& get_last_place (os::window);

    template<>
    GUIPP_WIN_EXPORT const core::size& get_last_place<core::size>(os::window);

    template<>
    GUIPP_WIN_EXPORT const core::point& get_last_place<core::point>(os::window);

    template<>
    GUIPP_WIN_EXPORT const core::rectangle& get_last_place<core::rectangle>(os::window);

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT void update_last_place (os::window, const core::rectangle&);
    GUIPP_WIN_EXPORT void clear_last_place (os::window);

    // --------------------------------------------------------------------------
    template<typename T, os::event_id E, typename C>
    bool move_size_matcher (const core::event& e) {
      os::event_id t = e.type;
      if (t == E) {
        const C& c = event_type_cast<C>(e);
        return (get_last_place<T>(c.window) != get<T, C>::param(e));
      }
      return false;
    }

    // --------------------------------------------------------------------------
    template<int M, os::event_id E, typename C>
    bool mode_matcher (const core::event& e) {
      return (e.type == E) && (event_type_cast<C>(e).mode == M);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline unsigned int get_state (const core::event& e) {
      return event_type_cast<T>(e).state & ~0x10;
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline unsigned int get_button (const core::event& e) {
      return event_type_cast<T>(e).button;
    }

    // --------------------------------------------------------------------------
    template<int I, typename T>
    inline T get_client_data (const core::event& e) {
      return (T)e.xclient.data.l[I];
    }

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT core::rectangle get_client_data_rect (const core::event& e);
    GUIPP_WIN_EXPORT core::size get_client_data_size (const core::event& e);
    // --------------------------------------------------------------------------
    template<>
    GUIPP_WIN_EXPORT window* get_client_data<0, window*>(const core::event& e);
    // --------------------------------------------------------------------------
    template<typename T>
    inline window* get_window (const core::event& e) {
      return detail::get_window(event_type_cast<T>(e).window);
    }

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT window* get_current_focus_window (const core::event&);
    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT os::graphics get_graphics (const core::event&);
    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT os::window get_draw_window (const core::event&);
    // --------------------------------------------------------------------------
    template<os::message_type& M>
    inline bool client_message_matcher (const core::event& e) {
      return (e.type == ClientMessage) && (e.xclient.message_type == M);
    }

    // --------------------------------------------------------------------------
    template<os::message_type& M>
    inline bool protocol_message_matcher (const core::event& e) {
      return (e.type == ClientMessage) && (e.xclient.message_type == core::x11::WM_PROTOCOLS) && (e.xclient.data.l[0] == M);
    }

    // --------------------------------------------------------------------------
    template<os::event_id E, os::key_symbol symbol, os::key_state state>
    inline bool key_symbol_matcher (const core::event& e) {
      return (e.type == E) &&
             (get_key_symbol(e) == symbol) &&
             core::bit_mask<os::key_state, state>::is_set(get_key_state(e));
    }

    // --------------------------------------------------------------------------

    template<os::event_id B>
    struct double_click_matcher {
      inline bool operator() (const core::event& e) {
        if ((e.type == ButtonRelease) && (e.xbutton.button == B)) {
          Time diff = e.xbutton.time - last_up;
          last_up = e.xbutton.time;
          return (diff < 300);
        }
        return false;
      }

      Time last_up;
    };

    // --------------------------------------------------------------------------
    template<int D, int U>
    inline core::point::type get_wheel_delta (const core::event& e) {
      switch (e.xbutton.button) {
        case D: return 1.0F;
        case U: return -1.0F;
      }
      return 0;
    }

    // --------------------------------------------------------------------------
    template<int D, int U>
    bool wheel_button_matcher (const core::event& e) {
      return (e.type == ButtonPress) && ((e.xbutton.button == D) || (e.xbutton.button == U));
    }

    // --------------------------------------------------------------------------
    using create_event = core::event_handler<ClientMessage, 0, core::params<>::getter<>, 0,
                                             event::functor<client_message_matcher<core::x11::WM_CREATE_WINDOW>>>;

    using close_event = core::event_handler<ClientMessage, 0,
                                            core::params<>::getter<>, 1,
                                            event::functor<protocol_message_matcher<core::x11::WM_DELETE_WINDOW>>>;

    using destroy_event = core::event_handler<DestroyNotify, StructureNotifyMask>;

    using any_key_down_event = core::event_handler<KeyPress, KeyPressMask,
                                                   core::params<os::key_state, os::key_symbol, std::string>::
                                                   getter<get_key_state, get_key_symbol, get_key_chars>>;

    using any_key_up_event = core::event_handler<KeyRelease, KeyReleaseMask,
                                                 core::params<os::key_state, os::key_symbol>::
                                                 getter<get_key_state, get_key_symbol>>;

    template<os::key_symbol symbol, os::key_state state>
    using key_down_event = core::event_handler<KeyPress, KeyPressMask, core::params<>::getter<>, 0,
                                               event::functor<key_symbol_matcher<KeyPress, symbol, state>>>;


    template<os::key_symbol symbol, os::key_state state>
    using key_up_event = core::event_handler<KeyRelease, KeyPressMask, core::params<>::getter<>, 0,
                                             event::functor<key_symbol_matcher<KeyRelease, symbol, state>>>;

    using mouse_move_event = core::event_handler<MotionNotify, PointerMotionMask,
                                                 core::params<os::key_state, core::point>::
                                                 getter<get_state<XMotionEvent>,
                                                        get<core::point, XMotionEvent>::param>>;

    using mouse_move_abs_event = core::event_handler<MotionNotify, PointerMotionMask,
                                                     core::params<os::key_state, core::point>::
                                                     getter<get_state<XMotionEvent>,
                                                            get_root_mouse_pos>>;

    using left_btn_down_event = core::event_handler<ButtonPress, ButtonPressMask,
                                                    core::params<os::key_state, core::point>::
                                                    getter<get_state<XButtonEvent>,
                                                           get<core::point, XButtonEvent>::param>,
                                                    0,
                                                    event::functor<event_button_matcher<ButtonPress, Button1>>>;

    using left_btn_up_event = core::event_handler<ButtonRelease, ButtonReleaseMask | ButtonPressMask,
                                                  core::params<os::key_state, core::point>::
                                                  getter<get_state<XButtonEvent>,
                                                         get<core::point, XButtonEvent>::param>,
                                                  0,
                                                  event::functor<event_button_matcher<ButtonRelease, Button1>>>;

    using right_btn_down_event = core::event_handler<ButtonPress, ButtonPressMask,
                                                     core::params<os::key_state, core::point>::
                                                     getter<get_state<XButtonEvent>,
                                                            get<core::point, XButtonEvent>::param>,
                                                     0,
                                                     event::functor<event_button_matcher<ButtonPress, Button3>>>;

    using right_btn_up_event = core::event_handler<ButtonRelease, ButtonReleaseMask | ButtonPressMask,
                                                   core::params<os::key_state, core::point>::
                                                   getter<get_state<XButtonEvent>,
                                                          get<core::point, XButtonEvent>::param>,
                                                   0,
                                                   event::functor<event_button_matcher<ButtonRelease, Button3>>>;

    using middle_btn_down_event = core::event_handler<ButtonPress, ButtonPressMask,
                                                      core::params<os::key_state, core::point>::
                                                      getter<get_state<XButtonEvent>,
                                                             get<core::point, XButtonEvent>::param>,
                                                      0,
                                                      event::functor<event_button_matcher<ButtonPress, Button2>>>;

    using middle_btn_up_event = core::event_handler<ButtonRelease, ButtonReleaseMask | ButtonPressMask,
                                                    core::params<os::key_state, core::point>::
                                                    getter<get_state<XButtonEvent>,
                                                           get<core::point, XButtonEvent>::param>,
                                                    0,
                                                    event::functor<event_button_matcher<ButtonRelease, Button2>>>;

    using btn_down_event = core::event_handler<ButtonPress, ButtonPressMask,
                                               core::params<os::key_state, core::point>::
                                               getter<get_state<XButtonEvent>,
                                                      get<core::point, XButtonEvent>::param>>;

    using btn_up_event = core::event_handler<ButtonRelease, ButtonReleaseMask | ButtonPressMask,
                                             core::params<os::key_state, core::point>::
                                             getter<get_state<XButtonEvent>,
                                                    get<core::point, XButtonEvent>::param>>;

    using left_btn_dblclk_event = core::event_handler<ButtonRelease, ButtonPressMask | ButtonReleaseMask,
                                                      core::params<os::key_state, core::point>::
                                                      getter<get_state<XButtonEvent>,
                                                             get<core::point, XButtonEvent>::param>,
                                                      0,
                                                      double_click_matcher<Button1>>;

    using right_btn_dblclk_event = core::event_handler<ButtonRelease, ButtonPressMask | ButtonReleaseMask,
                                                       core::params<os::key_state, core::point>::
                                                       getter<get_state<XButtonEvent>,
                                                              get<core::point, XButtonEvent>::param>,
                                                       0,
                                                       double_click_matcher<Button3>>;
    using middle_btn_dblclk_event = core::event_handler<ButtonRelease, ButtonPressMask | ButtonReleaseMask,
                                                        core::params<os::key_state, core::point>::
                                                        getter<get_state<XButtonEvent>,
                                                               get<core::point, XButtonEvent>::param>,
                                                        0,
                                                        double_click_matcher<Button2>>;

    using wheel_x_event = core::event_handler<ButtonPress, ButtonPressMask,
                                              core::params<core::point::type, core::point>::
                                              getter<get_wheel_delta<6, 7>,
                                              get<core::point, XButtonEvent>::param>,
                                              0,
                                              event::functor<wheel_button_matcher<6, 7>>>;

    using wheel_y_event = core::event_handler<ButtonPress, ButtonPressMask,
                                              core::params<core::point::type, core::point>::
                                              getter<get_wheel_delta<Button4, Button5>,
                                                     get<core::point, XButtonEvent>::param>,
                                              0,
                                              event::functor<wheel_button_matcher<Button4, Button5>>>;

    template<orientation_t O>
    struct wheel_event {};

    template<>
    struct wheel_event<orientation_t::horizontal> : public wheel_x_event {};

    template<>
    struct wheel_event<orientation_t::vertical> : public wheel_y_event {};

    using show_event = core::event_handler<MapNotify, StructureNotifyMask>;
    using hide_event = core::event_handler<UnmapNotify, StructureNotifyMask>;

    using set_focus_event = core::event_handler<FocusIn, FocusChangeMask>;
    using lost_focus_event = core::event_handler<FocusOut, FocusChangeMask>;

    using mouse_enter_event = core::event_handler<EnterNotify, EnterWindowMask,
                                            core::params<>::getter<>,
                                            0,
                                            event::functor<mode_matcher<NotifyNormal, EnterNotify, XCrossingEvent>>>;
    using mouse_leave_event = core::event_handler<LeaveNotify, LeaveWindowMask,
                                            core::params<>::getter<>,
                                            0,
                                            event::functor<mode_matcher<NotifyNormal, LeaveNotify, XCrossingEvent>>>;

    using move_event = core::event_handler<ConfigureNotify, StructureNotifyMask,
                                     core::params<core::point>::
                                     getter<get<core::point, XConfigureEvent>::param>,
                                     0,
                                     event::functor<move_size_matcher<core::point,
                                                                      ConfigureNotify,
                                                                      XConfigureEvent>>>;
    using size_event = core::event_handler<ConfigureNotify, StructureNotifyMask,
                                     core::params<core::size>::
                                     getter<get<core::size, XConfigureEvent>::param>,
                                     0,
                                     event::functor<move_size_matcher<core::size,
                                                                      ConfigureNotify,
                                                                      XConfigureEvent>>>;

    using layout_event = core::event_handler<ClientMessage, 0,
                                       core::params<core::rectangle>::
                                       getter<get_client_data_rect>,
                                       0,
                                       event::functor<client_message_matcher<core::WM_LAYOUT_WINDOW>>>;

    using paint_event = core::event_handler<Expose, ExposureMask,
                                         core::params<os::window, os::graphics>::
                                         getter<get_draw_window, get_graphics>>;

    // --------------------------------------------------------------------------
#endif // GUIPP_X11

#ifdef GUIPP_QT
    // --------------------------------------------------------------------------
    class QClientEvent : public QEvent {
    public:
      explicit QClientEvent (Type type, const core::rectangle& rect);
      explicit QClientEvent (Type type, long l1, long l2);

      ~QClientEvent ();

      const core::rectangle &rect() const;
      long l1 () const;
      long l2 () const;

    protected:
      core::rectangle m_rect;
      long m_l1;
      long m_l2;
    };

    template<int I>
    long get_param (const core::event&);

    template<>
    long get_param<0> (const core::event&);

    template<>
    long get_param<1> (const core::event&);

    // --------------------------------------------------------------------------
    template<os::event_id E, os::key_symbol symbol, os::key_state state>
    inline bool key_symbol_matcher (const core::event& e) {
      return (e.type() == E) &&
             (get_key_symbol(e) == symbol) &&
             core::bit_mask<os::key_state, state>::is_set(get_key_state(e));
    }

    inline int get_mouse_button (const core::event& e) {
      return e.cast<QMouseEvent>().buttons();
    }

    inline core::point get_mouse_point (const core::event& e) {
      const QMouseEvent& m = e.cast<QMouseEvent>();
      return core::point(gui::os::point(m.x(), m.y()));
    }

    inline core::point get_root_mouse_pos (const core::event& e) {
      const QMouseEvent& m = e.cast<QMouseEvent>();
      return core::point(gui::os::point(m.globalX(), m.globalY()));
    }

    template<os::event_id id, os::key_state btn>
    inline bool event_button_matcher (const core::event& e) {
      return (e.type() == id) && (e.cast<QMouseEvent>().button() == btn);
    }

    core::point::type get_wheel_delta_x (const core::event& e);
    core::point::type get_wheel_delta_y (const core::event& e);

    template<typename T>
    inline core::point get_point (const core::event& e) {
      return core::point(e.cast<T>().pos());
    }

    template<typename T>
    inline core::size get_size (const core::event& e) {
      return core::size(e.cast<T>().size());
    }

    inline bool wheel_button_matcher_x (const core::event& e) {
      return (e.type() == QEvent::Wheel) && (get_wheel_delta_x(e) != 0);
    }

    inline bool wheel_button_matcher_y (const core::event& e) {
      return (e.type() == QEvent::Wheel) && (get_wheel_delta_y(e) != 0);
    }

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT core::rectangle get_client_data_rect (const core::event& e);
    GUIPP_WIN_EXPORT os::graphics get_graphics (const core::event&);
    GUIPP_WIN_EXPORT os::window get_draw_window (const core::event&);

    // --------------------------------------------------------------------------
    using create_event = core::event_handler<core::qt::WM_CREATE_WINDOW>;

    using close_event = core::event_handler<QEvent::Close, 0,
                                            core::params<>::getter<>, 1>;

    using destroy_event = core::event_handler<core::qt::WM_DESTROY_WINDOW>;

    using any_key_down_event = core::event_handler<QEvent::KeyPress, 0,
                                                   core::params<os::key_state, os::key_symbol, std::string>::
                                                   getter<get_key_state, get_key_symbol, get_key_chars>>;

    using any_key_up_event = core::event_handler<QEvent::KeyRelease, 0,
                                                 core::params<os::key_state, os::key_symbol>::
                                                 getter<get_key_state, get_key_symbol>>;

    template<os::key_symbol symbol, os::key_state state>
    using key_down_event = core::event_handler<QEvent::KeyPress, 0, core::params<>::getter<>, 0,
                                               event::functor<key_symbol_matcher<QEvent::KeyPress, symbol, state>>>;


    template<os::key_symbol symbol, os::key_state state>
    using key_up_event = core::event_handler<QEvent::KeyRelease, 0, core::params<>::getter<>, 0,
                                             event::functor<key_symbol_matcher<QEvent::KeyRelease, symbol, state>>>;

    using mouse_move_event = core::event_handler<QEvent::MouseMove, 0,
                                                 core::params<os::key_state, core::point>::
                                                 getter<get_mouse_button, get_mouse_point>>;

    using mouse_move_abs_event = core::event_handler<QEvent::MouseMove, 0,
                                                     core::params<os::key_state, core::point>::
                                                     getter<get_mouse_button,
                                                            get_root_mouse_pos>>;

    using left_btn_down_event = core::event_handler<QEvent::MouseButtonPress, 0,
                                                    core::params<os::key_state, core::point>::
                                                    getter<get_mouse_button, get_mouse_point>,
                                                    0,
                                                    event::functor<event_button_matcher<QEvent::MouseButtonPress, Qt::LeftButton>>>;

    using left_btn_up_event = core::event_handler<QEvent::MouseButtonRelease, 0,
                                                  core::params<os::key_state, core::point>::
                                                  getter<get_mouse_button, get_mouse_point>,
                                                  0,
                                                  event::functor<event_button_matcher<QEvent::MouseButtonRelease, Qt::LeftButton>>>;

    using right_btn_down_event = core::event_handler<QEvent::MouseButtonPress, 0,
                                                     core::params<os::key_state, core::point>::
                                                     getter<get_mouse_button, get_mouse_point>,
                                                     0,
                                                     event::functor<event_button_matcher<QEvent::MouseButtonPress, Qt::RightButton>>>;

    using right_btn_up_event = core::event_handler<QEvent::MouseButtonRelease, 0,
                                                   core::params<os::key_state, core::point>::
                                                   getter<get_mouse_button, get_mouse_point>,
                                                   0,
                                                   event::functor<event_button_matcher<QEvent::MouseButtonRelease, Qt::RightButton>>>;

    using middle_btn_down_event = core::event_handler<QEvent::MouseButtonPress, 0,
                                                      core::params<os::key_state, core::point>::
                                                      getter<get_mouse_button, get_mouse_point>,
                                                      0,
                                                      event::functor<event_button_matcher<QEvent::MouseButtonPress, Qt::MiddleButton>>>;

    using middle_btn_up_event = core::event_handler<QEvent::MouseButtonRelease, 0,
                                                    core::params<os::key_state, core::point>::
                                                    getter<get_mouse_button, get_mouse_point>,
                                                    0,
                                                    event::functor<event_button_matcher<QEvent::MouseButtonRelease, Qt::MiddleButton>>>;

    using btn_down_event = core::event_handler<QEvent::MouseButtonPress, 0,
                                               core::params<os::key_state, core::point>::
                                               getter<get_mouse_button, get_mouse_point>>;

    using btn_up_event = core::event_handler<QEvent::MouseButtonRelease, 0,
                                             core::params<os::key_state, core::point>::
                                             getter<get_mouse_button, get_mouse_point>>;

    using left_btn_dblclk_event = core::event_handler<QEvent::MouseButtonDblClick, 0,
                                                      core::params<os::key_state, core::point>::
                                                      getter<get_mouse_button, get_mouse_point>,
                                                      0,
                                                      event::functor<event_button_matcher<QEvent::MouseButtonDblClick, Qt::LeftButton>>>;

    using right_btn_dblclk_event = core::event_handler<QEvent::MouseButtonDblClick, 0,
                                                       core::params<os::key_state, core::point>::
                                                       getter<get_mouse_button, get_mouse_point>,
                                                       0,
                                                       event::functor<event_button_matcher<QEvent::MouseButtonDblClick, Qt::RightButton>>>;
    using middle_btn_dblclk_event = core::event_handler<QEvent::MouseButtonDblClick, 0,
                                                        core::params<os::key_state, core::point>::
                                                        getter<get_mouse_button, get_mouse_point>,
                                                        0,
                                                        event::functor<event_button_matcher<QEvent::MouseButtonDblClick, Qt::MiddleButton>>>;

    using wheel_x_event = core::event_handler<QEvent::Wheel, 0,
                                              core::params<core::point::type, core::point>::
                                              getter<get_wheel_delta_x, get_point<QWheelEvent>>,
                                              0,
                                              event::functor<wheel_button_matcher_x>>;

    using wheel_y_event = core::event_handler<QEvent::Wheel, 0,
                                              core::params<core::point::type, core::point>::
                                              getter<get_wheel_delta_y, get_point<QWheelEvent>>,
                                              0,
                                              event::functor<wheel_button_matcher_y>>;

    template<orientation_t O>
    struct wheel_event {};

    template<>
    struct wheel_event<orientation_t::horizontal> : public wheel_x_event {};

    template<>
    struct wheel_event<orientation_t::vertical> : public wheel_y_event {};

    using show_event = core::event_handler<QEvent::Show>;
    using hide_event = core::event_handler<QEvent::Hide>;

    using set_focus_event = core::event_handler<QEvent::FocusIn>;
    using lost_focus_event = core::event_handler<QEvent::FocusOut>;

    using mouse_enter_event = core::event_handler<QEvent::Enter>;
    using mouse_leave_event = core::event_handler<QEvent::Leave>;

    using move_event = core::event_handler<QEvent::Move, 0,
                                           core::params<core::point>::
                                           getter<get_point<QMoveEvent>>>;
    using size_event = core::event_handler<QEvent::Resize, 0,
                                           core::params<core::size>::
                                           getter<get_size<QResizeEvent>>>;

    using layout_event = core::event_handler<core::WM_LAYOUT_WINDOW, 0,
                                             core::params<core::rectangle>::
                                             getter<get_client_data_rect>>;

    using paint_event = core::event_handler<QEvent::Paint, 0,
                                            core::params<os::window, os::graphics>::
                                            getter<get_draw_window, get_graphics>>;

    // --------------------------------------------------------------------------
#endif // GUIPP_QT

  } // namespace win

} // namespace gui
