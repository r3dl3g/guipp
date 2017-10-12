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
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "event.h"
#include "bind_method.h"
#include "gui_types.h"
#include "window_event_proc.h"
#include "logger.h"

namespace gui {

  namespace win {

    class window;

    // --------------------------------------------------------------------------
    template<int I, typename T> T get_param (const core::event& e);
    template<> os::graphics get_param<0> (const core::event& e);
    template<> window* get_param<0> (const core::event& e);

    // --------------------------------------------------------------------------
    os::key_state get_key_state (const core::event& e);
    os::key_symbol get_key_symbol (const core::event& e);
    std::string get_key_chars (const core::event& e);
    core::point get_root_mouse_pos (const core::event& e);

    // --------------------------------------------------------------------------
    template <os::event_id id>
    struct event_type_match {
      bool operator() (const core::event& e) {
        return (e.type == id);
      }
    };

    // --------------------------------------------------------------------------
    template<typename T>
    using param_getter = T(*)(const core::event&);

    // --------------------------------------------------------------------------
    template<typename... Ts>
    struct params {

      template<param_getter<Ts>... Fs>
      struct getter {

        using callback = void(Ts...);
        typedef std::function<callback> function;

        getter (const function& f)
          :f(f)
        {}

        getter (function&& f)
          :f(std::move(f))
        {}

        template<class C>
        getter (C* t, void(C::*method)(Ts...))
          : f(core::bind_method(t, method))
        {}

        operator bool () const {
          return (bool)f;
        }

        void operator() (const core::event& e) {
          f(Fs(e)...);
        }

      protected:
        function f;
      };
    };

    // --------------------------------------------------------------------------
    template<os::event_id E,
             os::event_id Mask = 0,
             typename C = params<>::getter<>,
             os::event_result R = 0,
             typename M = event_type_match<E>>
    struct event_handler {

      typedef M Matcher;
      typedef C Caller;
      typedef typename Caller::function function;

      static constexpr os::event_id mask = Mask;

      event_handler (const function cb)
        : caller(cb)
      {}

      template<typename T, typename F>
      event_handler (T* t, F f)
        : caller(core::bind_method(t, f))
      {}

      event_handler (const event_handler& rhs)
        : caller(rhs.caller)
        , matcher(rhs.matcher)
      {}

      event_handler (event_handler&& rhs)
        : caller(std::move(rhs.caller))
        , matcher(std::move(rhs.matcher))
      {}

      bool operator() (const core::event& e, os::event_result& result) {
        if (matcher(e) && caller) {
          caller(e);
          result = R;
          return true;
        }
        return false;
      }

      os::event_id get_event_id () const {
        return E;
      }

    protected:
      Caller caller;
      Matcher matcher;
    };

#ifdef WIN32
    // --------------------------------------------------------------------------
    template<> bool get_param<0> (const core::event& e);
    template<> unsigned int get_param<0> (const core::event& e);
    template<> int get_param<0> (const core::event& e);
    template<> window* get_param<1> (const core::event& e);
    template<> core::point get_param<1> (const core::event& e);
    template<> core::size get_param<1> (const core::event& e);
    // --------------------------------------------------------------------------
    template<typename T>
    core::rectangle get_rect (const core::event& e) {
      T& p = *reinterpret_cast<T*>(e.lParam);
      return core::rectangle(static_cast<core::point::type>(p.x),
                             static_cast<core::point::type>(p.y),
                             static_cast<core::size::type>(p.cx),
                             static_cast<core::size::type>(p.cy));
    }
    // --------------------------------------------------------------------------
    window* get_window_from_cs (const core::event& e);
    // --------------------------------------------------------------------------
    unsigned int get_flags_from_wp (const core::event& e);
    // --------------------------------------------------------------------------
    core::point_type get_wheel_delta (const core::event& e);
    // --------------------------------------------------------------------------
    template<bool down>
    struct any_button_match {
      bool operator() (const core::event& e);
    };

    template<>
    struct any_button_match<true> {
      bool operator() (const core::event& e) {
        return (e.type == WM_LBUTTONDOWN) || (e.type == WM_RBUTTONDOWN) || (e.type == WM_MBUTTONDOWN);
      }
    };

    template<>
    struct any_button_match<false> {
      bool operator() (const core::event& e) {
        return (e.type == WM_LBUTTONUP) || (e.type == WM_RBUTTONUP) || (e.type == WM_MBUTTONUP);
      }
    };
    // --------------------------------------------------------------------------
    template <os::event_id E, os::key_symbol symbol, os::key_state state>
    struct key_matcher {
      bool operator() (const core::event& e) {
        return (e.type == E) &&
               (get_key_symbol(e) == symbol) &&
               bit_mask<os::key_state, state>::is_set(get_key_state(e));
      }
    };
    // --------------------------------------------------------------------------
    struct mouse_enter_matcher {
      bool operator() (const core::event& e);
    };
    // --------------------------------------------------------------------------
    template <os::event_id id, bool show>
    struct visibility_event_type_match {
      bool operator() (const core::event& e) {
        return (e.type == id) && (get_param<0, bool>(e) == show);
      }
    };
    // --------------------------------------------------------------------------
    struct pos_changing_getter : params<core::rectangle>::getter<get_param<0, core::rectangle>> {
      typedef params<core::rectangle>::getter<get_param<0, core::rectangle>> super;

      pos_changing_getter (const function cb)
        : super(cb)
      {}

      template<class T>
      pos_changing_getter (T* win, void(T::*changingfn)(core::rectangle&))
        : super(win, changingfn)
      {}

      void operator() (const core::event& e);
    };
    // --------------------------------------------------------------------------
    struct minmax_getter {
      typedef std::function<void(const core::size&, const core::point&, core::size&, core::size&)> function;

      minmax_getter (const function cb)
        : callback(cb)
      {}

      template<class T>
      minmax_getter (T* win, void(T::*fn)(const core::size&, const core::point&, core::size&, core::size&))
        : callback(win, fn)
      {}

      operator bool () const {
        return callback.operator bool();
      }

      void operator() (const core::event& e);

    private:
      function callback;
    };
    // --------------------------------------------------------------------------
    struct os_paint_getter : params<os::graphics>::getter<get_param<0, os::graphics>> {
      typedef params<os::graphics>::getter<get_param<0, os::graphics>> super;

      os_paint_getter (const function cb)
        :super(cb)
      {}

      template<class T>
      os_paint_getter (T* t, void(T::*callback_)(os::graphics&))
        : super(gui::core::bind_method(t, callback_))
      {}

      void operator() (const core::event& e);
    };

    // --------------------------------------------------------------------------
    using create_event = event_handler<WM_CREATE, 0,
                                       params<window*, core::rectangle>::
                                       getter<get_window_from_cs,
                                              get_rect<CREATESTRUCT>>>;

    using close_event = event_handler<WM_CLOSE>;

    using destroy_event = event_handler<WM_DESTROY>;

    using any_key_down_event = event_handler<WM_KEYDOWN, 0,
                                             params<os::key_state, os::key_symbol, std::string>::
                                             getter<get_key_state, get_key_symbol, get_key_chars>>;

    using any_key_up_event = event_handler<WM_KEYUP, 0,
                                           params<os::key_state, os::key_symbol>::
                                           getter<get_key_state, get_key_symbol>>;

    template <os::key_symbol symbol, os::key_state state>
    using key_down_event = event_handler<WM_KEYDOWN, 0, params<>::getter<>, 0,
                                         key_matcher<WM_KEYDOWN, symbol, state>>;


    template <os::key_symbol symbol, os::key_state state>
    using key_up_event = event_handler<WM_KEYUP, 0, params<>::getter<>, 0,
                                       key_matcher<WM_KEYUP, symbol, state>>;

    using character_event = event_handler<WM_CHAR, 0,
                                          params<std::string>::
                                          getter<get_key_chars>>;

    using erase_event = event_handler<WM_ERASEBKGND, 0,
                                      params<os::graphics>::
                                      getter<get_param<0, os::graphics>>>;
    using print_event = event_handler<WM_PRINT, 0,
                                      params<os::graphics>::
                                      getter<get_param<0, os::graphics>>>;
    using print_client_event = event_handler<WM_PRINTCLIENT, 0,
                                             params<os::graphics>::
                                             getter<get_param<0, os::graphics>>>;

    using redraw_changed_event = event_handler<WM_SETREDRAW, 0,
                                               params<bool>::getter<get_param<0, bool>>>;

    using enable_event = event_handler<WM_ENABLE, 0,
                                       params<bool>::getter<get_param<0, bool>>>;
    using activate_event = event_handler<WM_ACTIVATE, 0,
                                         params<bool, window*>::
                                         getter<get_param<0, bool>, get_param<1, window*>>>;
    using activate_app_event = event_handler<WM_ACTIVATEAPP, 0,
                                             params<bool>::getter<get_param<0, bool>>>;
    using set_focus_event = event_handler<WM_SETFOCUS, 0,
                                          params<window*>::getter<get_param<0, window*>>>;
    using lost_focus_event = event_handler<WM_KILLFOCUS, 0,
                                           params<window*>::getter<get_param<0, window*>>>;

    using begin_size_or_move_event = event_handler<WM_ENTERSIZEMOVE>;
    using end_size_or_move_event = event_handler<WM_EXITSIZEMOVE>;

    using move_event = event_handler<WM_MOVE, 0,
                                     params<core::point>::
                                     getter<get_param<1, core::point>>>;
    using moving_event = event_handler<WM_MOVING, 0,
                                       params<core::point>::
                                       getter<get_param<1, core::point>>,
                                       TRUE>;

    using size_event = event_handler<WM_SIZE, 0,
                                     params<core::size>::
                                     getter<get_param<1, core::size>>>;
    using sizing_event = event_handler<WM_SIZING, 0,
                                       params<core::size>::
                                       getter<get_param<1, core::size>>,
                                       TRUE>;

    using left_btn_down_event = event_handler<WM_LBUTTONDOWN, 0,
                                              params<os::key_state, core::point>::
                                              getter<get_param<0, os::key_state>,
                                                     get_param<1, core::point>>>;
    using left_btn_up_event = event_handler<WM_LBUTTONUP, 0,
                                            params<os::key_state, core::point>::
                                            getter<get_param<0, os::key_state>,
                                                   get_param<1, core::point>>>;
    using left_btn_dblclk_event = event_handler<WM_LBUTTONDBLCLK, 0,
                                                params<os::key_state, core::point>::
                                                getter<get_param<0, os::key_state>,
                                                       get_param<1, core::point>>>;

    using right_btn_down_event = event_handler<WM_RBUTTONDOWN, 0,
                                               params<os::key_state, core::point>::
                                               getter<get_param<0, os::key_state>,
                                                      get_param<1, core::point>>>;
    using right_btn_up_event = event_handler<WM_RBUTTONUP, 0,
                                             params<os::key_state, core::point>::
                                             getter<get_param<0, os::key_state>,
                                                    get_param<1, core::point>>>;
    using right_btn_dblclk_event = event_handler<WM_RBUTTONDBLCLK, 0,
                                                 params<os::key_state, core::point>::
                                                 getter<get_param<0, os::key_state>,
                                                        get_param<1, core::point>>>;

    using middle_btn_down_event = event_handler<WM_MBUTTONDOWN, 0,
                                                params<os::key_state, core::point>::
                                                getter<get_param<0, os::key_state>,
                                                       get_param<1, core::point>>>;
    using middle_btn_up_event = event_handler<WM_MBUTTONUP, 0,
                                              params<os::key_state, core::point>::
                                              getter<get_param<0, os::key_state>,
                                                     get_param<1, core::point>>>;
    using middle_btn_dblclk_event = event_handler<WM_MBUTTONDBLCLK, 0,
                                                  params<os::key_state, core::point>::
                                                  getter<get_param<0, os::key_state>,
                                                         get_param<1, core::point>>>;

    using btn_down_event = event_handler<WM_LBUTTONDOWN, 0,
                                         params<os::key_state, core::point>::
                                         getter<get_param<0, os::key_state>,
                                                get_param<1, core::point>>,
                                         0, any_button_match<true>>;
    using btn_up_event = event_handler<WM_LBUTTONUP, 0,
                                       params<os::key_state, core::point>::
                                       getter<get_param<0, os::key_state>,
                                              get_param<1, core::point>>,
                                       0, any_button_match<false>>;

    using mouse_move_event = event_handler<WM_MOUSEMOVE, 0,
                                           params<os::key_state, core::point>::
                                           getter<get_param<0, os::key_state>,
                                                  get_param<1, core::point>>>;

    using mouse_move_abs_event = event_handler<WM_MOUSEMOVE, 0,
                                               params<os::key_state, core::point>::
                                               getter<get_param<0, os::key_state>,
                                                      get_root_mouse_pos>>;

    using mouse_enter_event = event_handler<WM_MOUSEMOVE, 0,
                                            params<>::getter<>,
                                            0,
                                            mouse_enter_matcher>;
    using mouse_leave_event = event_handler<WM_MOUSELEAVE>;

    using wheel_x_event = event_handler<WM_MOUSEHWHEEL, 0,
                                        params<core::point_type, core::point>::
                                        getter<get_wheel_delta,
                                               get_param<1, core::point>>>;
    using wheel_y_event = event_handler<WM_MOUSEWHEEL, 0,
                                        params<core::point_type, core::point>::
                                        getter<get_wheel_delta,
                                               get_param<1, core::point>>>;

    using show_event = event_handler<WM_SHOWWINDOW, 0,
                                     params<>::getter<>,
                                     0,
                                     visibility_event_type_match<WM_SHOWWINDOW, true>>;
    using hide_event = event_handler<WM_SHOWWINDOW, 0,
                                     params<>::getter<>,
                                     0,
                                     visibility_event_type_match<WM_SHOWWINDOW, false>>;

    using place_event = event_handler<WM_WINDOWPOSCHANGED, 0,
                                      params<core::rectangle>::getter<
                                                       get_rect<WINDOWPOS>>>;

    using placing_event = event_handler<WM_WINDOWPOSCHANGING, 0, pos_changing_getter>;

    using get_minmax_event = event_handler<WM_GETMINMAXINFO, 0, minmax_getter>;

    using os_paint_event = event_handler<WM_PAINT, 0, os_paint_getter>;

    // --------------------------------------------------------------------------
    void send_client_message (const window* win, os::event_id message, long l1 = 0, long l2 = 0);

#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    namespace x11 {

      extern Atom WM_CREATE_WINDOW;
      extern Atom WM_DELETE_WINDOW;
      extern Atom WM_PROTOCOLS;
      extern Atom WM_TAKE_FOCUS;

      int init_messages ();

      void init_atom (Atom& message, const char* name);

      void send_client_message (const window* win, Atom message,
                                long l1 = 0, long l2 = 0, long l3 = 0, long l4 = 0, long l5 = 0);

      void send_client_message (const window* win, Atom message, const window* w, const core::rectangle& r);

    } // namespace x11

    // --------------------------------------------------------------------------
    template <os::event_id id, os::event_id btn>
    struct event_button_match {
      bool operator() (const core::event& e) {
        return (e.type == id) && (e.xbutton.button == btn);
      }
    };
    // --------------------------------------------------------------------------
    template <typename T>
    const T& cast_event_type (const core::event& e);
    // --------------------------------------------------------------------------
    template <>
    inline const XButtonEvent& cast_event_type<XButtonEvent>(const core::event& e) {
      return e.xbutton;
    }
    template <>
    inline const XCreateWindowEvent& cast_event_type<XCreateWindowEvent>(const core::event& e) {
      return e.xcreatewindow;
    }
    template <>
    inline const XFocusChangeEvent& cast_event_type<XFocusChangeEvent>(const core::event& e) {
      return e.xfocus;
    }
    template <>
    inline const XKeyEvent& cast_event_type<XKeyEvent>(const core::event& e) {
      return e.xkey;
    }
    template <>
    inline const XMotionEvent& cast_event_type<XMotionEvent>(const core::event& e) {
      return e.xmotion;
    }
    template <>
    inline const XConfigureEvent& cast_event_type<XConfigureEvent>(const core::event& e) {
      return e.xconfigure;
    }
    template <>
    inline const XConfigureRequestEvent& cast_event_type<XConfigureRequestEvent>(const core::event& e) {
      return e.xconfigurerequest;
    }
    template <>
    inline const XCrossingEvent& cast_event_type<XCrossingEvent>(const core::event& e) {
      return e.xcrossing;
    }
    // --------------------------------------------------------------------------
    template<typename T>
    T& get_last_place(os::window);

    template<>
    core::size& get_last_place<core::size>(os::window);

    template<>
    core::point& get_last_place<core::point>(os::window);

    template<>
    core::rectangle& get_last_place<core::rectangle>(os::window);

    void update_last_place (os::window, const core::rectangle&);
    void clear_last_place (os::window);

    // --------------------------------------------------------------------------
    template<typename T, os::event_id E, typename C>
    struct move_size_matcher {
      bool operator() (const core::event& e) {
        os::event_id t = e.type;
        if (t == E) {
          const C& c = cast_event_type<C>(e);
          return (get_last_place<T>(c.window) != T(c));
        }
        return false;
      }
    };

    template<int M, os::event_id E, typename C>
    struct mode_matcher {
      bool operator() (const core::event& e) {
        return (e.type == E) && (cast_event_type<C>(e).mode == M);
      }
    };

    // --------------------------------------------------------------------------
    template<typename T>
    inline unsigned int get_state (const core::event& e) {
      return cast_event_type<T>(e).state;
    }
    // --------------------------------------------------------------------------
    template<typename T>
    inline unsigned int get_button (const core::event& e) {
      return cast_event_type<T>(e).button;
    }
    // --------------------------------------------------------------------------
    template<typename T>
    inline T* get_event_ptr (const core::event& e) {
      return const_cast<T*>(&(cast_event_type<T>(e)));
    }
    // --------------------------------------------------------------------------
    template<int I, typename T>
    inline T get_client_data (const core::event& e) {
      return (T)e.xclient.data.l[I];
    }
    // --------------------------------------------------------------------------
    core::rectangle get_client_data_rect (const core::event& e);
    // --------------------------------------------------------------------------
    template<>
    window* get_client_data<0, window*> (const core::event& e);
    // --------------------------------------------------------------------------
    template<typename T, typename C>
    inline T get_param (const core::event& e) {
      return T(cast_event_type<C>(e));
    }
    // --------------------------------------------------------------------------
    template<typename T>
    inline window* get_window (const core::event& e) {
      return detail::get_window(cast_event_type<T>(e).window);
    }
    // --------------------------------------------------------------------------
    window* get_current_focus_window (const core::event&);
    // --------------------------------------------------------------------------
    template <Atom& M>
    struct client_message_matcher {
      bool operator() (const core::event& e) {
        return (e.type == ClientMessage) && (e.xclient.message_type == M);
      }
    };
    // --------------------------------------------------------------------------
    template <Atom& M>
    struct protocol_message_matcher {
      bool operator() (const core::event& e) {
        return (e.type == ClientMessage) && (e.xclient.message_type == x11::WM_PROTOCOLS) && (e.xclient.data.l[0] == M);
      }
    };
    // --------------------------------------------------------------------------
    template <os::event_id E, os::key_symbol symbol, os::key_state state>
    struct key_matcher {
      bool operator() (const core::event& e) {
        return (e.type == E) &&
               (get_key_symbol(e) == symbol) &&
               bit_mask<os::key_state, state>::is_set(get_key_state(e));
      }
    };
    // --------------------------------------------------------------------------
    template<os::event_id B>
    struct double_click_matcher {
      bool operator() (const core::event& e) {
        if ((e.type == ButtonRelease) && (e.xbutton.button == B)) {
          Time last_up = s_last_up[e.xbutton.window];
          Time diff = e.xbutton.time - last_up;
//          LogDebug << "double_click_matcher(" << e.xbutton.button
//                   << ") at:" << e.xbutton.time << " last:" << last_up << " diff:" << diff;
          s_last_up[e.xbutton.window] = e.xbutton.time;
          return (diff < 300);
        }
        return false;
      }

    protected:
      static std::map<Window, Time> s_last_up;
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
    template <int D, int U>
    struct wheel_button_match {
      bool operator() (const core::event& e) {
        return (e.type == ButtonPress) && ((e.xbutton.button == D) || (e.xbutton.button == U));
      }
    };
    // --------------------------------------------------------------------------
    template<os::event_id B> std::map<Window, Time> double_click_matcher<B>::s_last_up;

    // --------------------------------------------------------------------------
    using create_event = event_handler<ClientMessage, 0,
                                       params<window*, core::rectangle>::
                                       getter<get_client_data<0, window*>, get_client_data_rect>,
                                       0,
                                       client_message_matcher<x11::WM_CREATE_WINDOW>>;

    using close_event = event_handler<ClientMessage, 0, params<>::getter<>, 1,
                                      protocol_message_matcher<x11::WM_DELETE_WINDOW>>;

    using destroy_event = event_handler<DestroyNotify, SubstructureNotifyMask>;

    using any_key_down_event = event_handler<KeyPress, KeyPressMask,
                                             params<os::key_state, os::key_symbol, std::string>::
                                             getter<get_key_state, get_key_symbol, get_key_chars>>;

    using any_key_up_event = event_handler<KeyRelease, KeyReleaseMask,
                                           params<os::key_state, os::key_symbol>::
                                           getter<get_key_state,get_key_symbol>>;

    template <os::key_symbol symbol, os::key_state state>
    using key_down_event = event_handler<KeyPress, KeyPressMask, params<>::getter<>,
                                         0, key_matcher<KeyPress, symbol, state>>;


    template <os::key_symbol symbol, os::key_state state>
    using key_up_event = event_handler<KeyRelease, KeyPressMask, params<>::getter<>,
                                       0, key_matcher<KeyRelease, symbol, state>>;

    using mouse_move_event = event_handler<MotionNotify, PointerMotionMask,
                                           params<os::key_state, core::point>::
                                           getter<get_state<XMotionEvent>,
                                                  get_param<core::point, XMotionEvent>>>;

    using mouse_move_abs_event = event_handler<MotionNotify, PointerMotionMask,
                                               params<os::key_state, core::point>::
                                               getter<get_state<XMotionEvent>,
                                                      get_root_mouse_pos>>;

    using left_btn_down_event = event_handler<ButtonPress, ButtonPressMask,
                                              params<os::key_state, core::point>::
                                              getter<get_state<XButtonEvent>,
                                                     get_param<core::point, XButtonEvent>>,
                                              0,
                                              event_button_match<ButtonPress, Button1>>;

    using left_btn_up_event = event_handler<ButtonRelease, ButtonReleaseMask|ButtonPressMask,
                                            params<os::key_state, core::point>::
                                            getter<get_state<XButtonEvent>,
                                                   get_param<core::point, XButtonEvent>>,
                                            0,
                                            event_button_match<ButtonRelease, Button1>>;

    using right_btn_down_event = event_handler<ButtonPress, ButtonPressMask,
                                               params<os::key_state, core::point>::
                                               getter<get_state<XButtonEvent>,
                                                      get_param<core::point, XButtonEvent>>,
                                               0,
                                               event_button_match<ButtonPress, Button3>>;

    using right_btn_up_event = event_handler<ButtonRelease, ButtonReleaseMask|ButtonPressMask,
                                             params<os::key_state, core::point>::
                                             getter<get_state<XButtonEvent>,
                                                    get_param<core::point, XButtonEvent>>,
                                             0,
                                             event_button_match<ButtonRelease, Button3>>;

    using middle_btn_down_event = event_handler<ButtonPress, ButtonPressMask,
                                                params<os::key_state, core::point>::
                                                getter<get_state<XButtonEvent>,
                                                       get_param<core::point, XButtonEvent>>,
                                                0,
                                                event_button_match<ButtonPress, Button2>>;

    using middle_btn_up_event = event_handler<ButtonRelease, ButtonReleaseMask|ButtonPressMask,
                                              params<os::key_state, core::point>::
                                              getter<get_state<XButtonEvent>,
                                                     get_param<core::point, XButtonEvent>>,
                                              0,
                                              event_button_match<ButtonRelease, Button2>>;

    using btn_down_event = event_handler<ButtonPress, ButtonPressMask,
                                         params<os::key_state, core::point>::
                                         getter<get_state<XButtonEvent>,
                                                get_param<core::point, XButtonEvent>>>;

    using btn_up_event = event_handler<ButtonRelease, ButtonReleaseMask|ButtonPressMask,
                                       params<os::key_state, core::point>::
                                       getter<get_state<XButtonEvent>,
                                              get_param<core::point, XButtonEvent>>>;

    using left_btn_dblclk_event = event_handler<ButtonRelease, ButtonReleaseMask,
                                                params<os::key_state, core::point>::
                                                getter<get_state<XButtonEvent>,
                                                       get_param<core::point, XButtonEvent>>,
                                                0,
                                                double_click_matcher<Button1>>;

    using right_btn_dblclk_event = event_handler<ButtonRelease, ButtonReleaseMask,
                                                 params<os::key_state, core::point>::
                                                 getter<get_state<XButtonEvent>,
                                                        get_param<core::point, XButtonEvent>>,
                                                 0,
                                                 double_click_matcher<Button3>>;
    using middle_btn_dblclk_event = event_handler<ButtonRelease, ButtonReleaseMask,
                                                  params<os::key_state, core::point>::
                                                  getter<get_state<XButtonEvent>,
                                                         get_param<core::point, XButtonEvent>>,
                                                  0,
                                                  double_click_matcher<Button2>>;

    using wheel_x_event = event_handler<ButtonPress, ButtonPressMask,
                                        params<core::point_type, core::point>::
                                        getter<get_wheel_delta<6, 7>,
                                               get_param<core::point, XButtonEvent>>,
                                        0,
                                        wheel_button_match<6, 7>>;
    using wheel_y_event = event_handler<ButtonPress, ButtonPressMask,
                                        params<core::point_type,  core::point>::
                                        getter<get_wheel_delta<Button4, Button5>,
                                               get_param<core::point, XButtonEvent>>,
                                        0,
                                        wheel_button_match<Button4, Button5>>;

    using show_event = event_handler<MapNotify, StructureNotifyMask>;
    using hide_event = event_handler<UnmapNotify, StructureNotifyMask>;

    using set_focus_event = event_handler<FocusIn, FocusChangeMask,
                                          params<window*>::
                                          getter<get_current_focus_window>>;
    using lost_focus_event = event_handler<FocusOut, FocusChangeMask,
                                           params<window*>::
                                           getter<get_current_focus_window>>;

    using mouse_enter_event = event_handler<EnterNotify, EnterWindowMask,
                                            params<>::getter<>,
                                            0,
                                            mode_matcher<NotifyNormal, EnterNotify, XCrossingEvent>>;
    using mouse_leave_event = event_handler<LeaveNotify, LeaveWindowMask,
                                            params<>::getter<>,
                                            0,
                                            mode_matcher<NotifyNormal, LeaveNotify, XCrossingEvent>>;

    using move_event = event_handler<ConfigureNotify, StructureNotifyMask,
                                     params<core::point>::
                                     getter<get_param<core::point, XConfigureEvent>>,
                                     0,
                                     move_size_matcher<core::point,
                                                       ConfigureNotify,
                                                       XConfigureEvent>>;
    using size_event = event_handler<ConfigureNotify, StructureNotifyMask,
                                     params<core::size>::
                                     getter<get_param<core::size, XConfigureEvent>>,
                                     0,
                                     move_size_matcher<core::size,
                                                       ConfigureNotify,
                                                       XConfigureEvent>>;
    using place_event = event_handler<ConfigureNotify, StructureNotifyMask,
                                      params<core::rectangle>::
                                      getter<get_param<core::rectangle, XConfigureEvent>>,
                                      0,
                                      move_size_matcher<core::rectangle,
                                                        ConfigureNotify,
                                                        XConfigureEvent>>;

    using moving_event = event_handler<ConfigureRequest, SubstructureRedirectMask,
                                       params<core::point>::
                                       getter<get_param<core::point, XConfigureRequestEvent>>,
                                       0,
                                       move_size_matcher<core::point,
                                                         ConfigureRequest,
                                                         XConfigureRequestEvent>>;

    using sizing_event = event_handler<ConfigureRequest, SubstructureRedirectMask,
                                       params<core::size>::
                                       getter<get_param<core::size, XConfigureRequestEvent>>,
                                       0,
                                       move_size_matcher<core::size,
                                                         ConfigureRequest,
                                                         XConfigureRequestEvent>>;
    using placing_event = event_handler<ConfigureRequest, SubstructureRedirectMask,
                                        params<core::rectangle>::
                                        getter<get_param<core::rectangle, XConfigureRequestEvent>>,
                                        0,
                                        move_size_matcher<core::rectangle,
                                                          ConfigureRequest,
                                                          XConfigureRequestEvent>>;

    // --------------------------------------------------------------------------
    using os_paint_event = event_handler<Expose, ExposureMask,
                                         params<os::graphics>::
                                         getter<get_param<0, os::graphics>>>;

    // --------------------------------------------------------------------------
    void send_client_message (const window* win, Atom message, long l1 = 0, long l2 = 0);

    // --------------------------------------------------------------------------
#endif // X11

  } // gui

} // gui
