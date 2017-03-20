/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
#include <map>
#ifdef X11
#include <X11/Xlib.h>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include "event.h"
#include "easy_bind.h"
#include "gui_types.h"
#include "graphics.h"
#include "window_event_proc.h"
#include "logger.h"

namespace gui {

  namespace win {
    class window;

#ifdef WIN32
    // --------------------------------------------------------------------------
    template<typename T>
    T get_param1 (const core::event& e) {
      return T(e.param_1);
    }
    // --------------------------------------------------------------------------
    template<typename T>
    T get_param2 (const core::event& e) {
      return T(e.param_2);
    }
    // --------------------------------------------------------------------------
    template<typename T>
    T get_param1_low (const core::event& e) {
      return static_cast<T>((SHORT)LOWORD(e.param_1));
    }
    // --------------------------------------------------------------------------
    template<typename T>
    T get_param1_high (const core::event& e) {
      return static_cast<T>((SHORT)HIWORD(e.param_1));
    }
    // --------------------------------------------------------------------------
    template<>
    bool get_param1<bool>(const core::event& e);
    // --------------------------------------------------------------------------
    // --------------------------------------------------------------------------
    template<>
    draw::graphics get_param1<draw::graphics>(const core::event& e);
    // --------------------------------------------------------------------------
    template<>
    window* get_param2<window*>(const core::event& e);
    // --------------------------------------------------------------------------
    template<typename T>
    core::rectangle get_rect(const core::event& e) {
      T& p = *reinterpret_cast<T*>(e.param_2);
      return core::rectangle(p.x, p.y, p.cx, p.cy);
    }
    // --------------------------------------------------------------------------
    window* get_window_from_cs(const core::event& e);
    // --------------------------------------------------------------------------
    unsigned int get_flags_from_wp(const core::event& e);

#elif X11
    // --------------------------------------------------------------------------
    template<typename T>
    T get_param1(const core::event& e) {
      return T(0);
    }
    // --------------------------------------------------------------------------
    template<typename T>
    T get_param2(const core::event& e) {
      return T(0);
    }
    // --------------------------------------------------------------------------
    template<typename T>
    T get_param1_low(const core::event& e) {
      return T(0);
    }
    // --------------------------------------------------------------------------
    template<typename T>
    T get_param1_high(const core::event& e) {
      return T(0);
    }
#endif // X11

    template<>
    window* get_param1<window*>(const core::event& e);

     // --------------------------------------------------------------------------
    template <typename T>
    const T& cast_event_type(const core::event& e);

    // --------------------------------------------------------------------------
    template <core::event_id id>
    struct event_type_match {
      bool operator() (const core::event& e) {
        return (e.type == id);
      }
    };

    // --------------------------------------------------------------------------
    struct no_param_caller {
      typedef std::function<void()> function;

      no_param_caller (const function cb)
        :callback(cb)
      {}

      template<class T>
      no_param_caller (T* t, void(T::*callback_)())
        : callback(gui::core::easy_bind(t, callback_))
      {}

      operator bool () const {
        return callback.operator bool();
      }

      void operator() (const core::event& e) {
        callback();
      }

    private:
      function callback;
    };

    // --------------------------------------------------------------------------
    template<typename P1,
             P1(F1)(const core::event&) = get_param1<P1>>
    struct one_param_caller {
      typedef std::function<void(P1)> function;

      one_param_caller (const function cb)
        :callback(cb)
      {}

      template<class T>
      one_param_caller (T* t, void(T::*callback_)(P1))
        : callback(gui::core::easy_bind(t, callback_))
      {}

      operator bool () const {
        return callback.operator bool();
      }

      void operator() (const core::event& e) {
        callback(F1(e));
      }

    protected:
      function callback;
    };

    // --------------------------------------------------------------------------
    template<typename P1,
             typename P2,
             P1(F1)(const core::event&) = get_param1<P1>,
             P2(F2)(const core::event&) = get_param2<P2>>
    struct two_param_caller {
      typedef std::function<void(P1, P2)> function;

      two_param_caller (const function cb)
        :callback(cb)
      {}

      template<class T>
      two_param_caller (T* t, void(T::*callback_)(P1, P2))
        : callback(gui::core::easy_bind(t, callback_))
      {}

      operator bool () const {
        return callback.operator bool();
      }

      void operator() (const core::event& e) {
        callback(F1(e), F2(e));
      }

    protected:
      function callback;
    };

    // --------------------------------------------------------------------------
    template<typename P1,
             typename P2,
             typename P3,
             P1(F1)(const core::event&) = get_param1_low<P1>,
             P2(F2)(const core::event&) = get_param1_high<P2>,
             P3(F3)(const core::event&) = get_param2<P3>>
    struct three_param_caller {
      typedef std::function<void(P1, P2, P3)> function;

      three_param_caller (const function cb)
        :callback(cb)
      {}

      template<class T>
      three_param_caller (T* t, void(T::*callback_)(P1, P2, P3))
        : callback(gui::core::easy_bind(t, callback_))
      {}

      operator bool () const {
        return callback.operator bool();
      }

      void operator() (const core::event& e) {
        callback(F1(e), F2(e), F3(e));
      }

    protected:
      function callback;
    };

    // --------------------------------------------------------------------------
    template<core::event_id E,
             typename C = no_param_caller,
             core::event_result R = 0,
             typename M = event_type_match<E>>
    struct event_handlerT {
      typedef typename C::function function;

      event_handlerT (const function cb)
        : caller(cb)
      {}

      bool operator() (const core::event& e, core::event_result& result) {
        if (matcher(e) && caller) {
          caller(e);
          result = R;
          return true;
        }
        return false;
      }

      core::event_id get_event_id () const {
        return E;
      }

    protected:
      C caller;
      M matcher;
    };

#ifdef WIN32
    // --------------------------------------------------------------------------
    typedef event_handlerT<WM_CREATE,
                           two_param_caller<window*,
                                            core::rectangle,
                                            get_window_from_cs,
                                            get_rect<CREATESTRUCT>>>          create_event;

    typedef event_handlerT<WM_DESTROY>                                        destroy_event;
    typedef event_handlerT<WM_CLOSE>                                          close_event;
    typedef event_handlerT<WM_QUIT>                                           quit_event;

    typedef event_handlerT<WM_ERASEBKGND, one_param_caller<draw::graphics>>   erase_event;
    typedef event_handlerT<WM_PRINT, one_param_caller<draw::graphics>>        print_event;
    typedef event_handlerT<WM_PRINTCLIENT, one_param_caller<draw::graphics>>  print_client_event;

    typedef event_handlerT<WM_SETREDRAW, one_param_caller<bool>>              redraw_changed_event;

    typedef event_handlerT<WM_ENABLE, one_param_caller<bool>>                 enable_event;
    typedef event_handlerT<WM_ACTIVATE, two_param_caller<bool, window*>>      activate_event;
    typedef event_handlerT<WM_ACTIVATEAPP, one_param_caller<bool>>            activate_app_event;
    typedef event_handlerT<WM_SETFOCUS, one_param_caller<window*>>            set_focus_event;
    typedef event_handlerT<WM_KILLFOCUS, one_param_caller<window*>>           lost_focus_event;

    typedef event_handlerT<WM_ENTERSIZEMOVE>                                  begin_size_or_move_event;
    typedef event_handlerT<WM_EXITSIZEMOVE>                                   end_size_or_move_event;

    typedef event_handlerT<WM_MOVE,
                           one_param_caller<core::point,
                                            get_param2<core::point>>>         move_event;
    typedef event_handlerT<WM_MOVING,
                           one_param_caller<core::point,
                                            get_param2<core::point>>,
                           TRUE>                                              moving_event;

    typedef event_handlerT<WM_SIZE,
                           one_param_caller<core::size,
                                            get_param2<core::size>>>          size_event;
    typedef event_handlerT<WM_SIZING,
                           one_param_caller<core::size,
                                            get_param2<core::size>>,
                           TRUE>                                              sizing_event;

    typedef event_handlerT<WM_LBUTTONDOWN,
                           one_param_caller<core::point,
                                            get_param2<core::point>>>         left_btn_down_event;
    typedef event_handlerT<WM_LBUTTONUP,
                           one_param_caller<core::point,
                                            get_param2<core::point>>>         left_btn_up_event;
    typedef event_handlerT<WM_LBUTTONDBLCLK,
                           one_param_caller<core::point,
                                            get_param2<core::point>>>         left_btn_dblclk_event;

    typedef event_handlerT<WM_RBUTTONDOWN,
                           one_param_caller<core::point,
                                            get_param2<core::point>>>         right_btn_down_event;
    typedef event_handlerT<WM_RBUTTONUP,
                           one_param_caller<core::point,
                                            get_param2<core::point>>>         right_btn_up_event;
    typedef event_handlerT<WM_RBUTTONDBLCLK,
                           one_param_caller<core::point,
                                            get_param2<core::point>>>         right_btn_dblclk_event;

    typedef event_handlerT<WM_MBUTTONDOWN,
                           one_param_caller<core::point,
                                            get_param2<core::point>>>         middle_btn_down_event;
    typedef event_handlerT<WM_MBUTTONUP,
                           one_param_caller<core::point,
                                            get_param2<core::point>>>         middle_btn_up_event;
    typedef event_handlerT<WM_MBUTTONDBLCLK,
                           one_param_caller<core::point,
                                            get_param2<core::point>>>         middle_btn_dblclk_event;

    typedef event_handlerT<WM_MOUSEMOVE,
                           two_param_caller<unsigned int,
                                            core::point>>                     mouse_move_event;


    struct mouse_enter_matcher {
      bool operator() (const core::event& e);
    };

    typedef event_handlerT<WM_MOUSEMOVE,
                           no_param_caller,
                           0,
                           mouse_enter_matcher>                               mouse_enter_event;
    typedef event_handlerT<WM_MOUSELEAVE>                                     mouse_leave_event;

    typedef event_handlerT<WM_MOUSEHWHEEL,
                           two_param_caller<int, core::point,
                                            get_param1_high<int>,
                                            get_param2<core::point>>>         wheel_x_event;
    typedef event_handlerT<WM_MOUSEWHEEL,
                           two_param_caller<int, core::point,
                                            get_param1_high<int>,
                                            get_param2<core::point>>>         wheel_y_event;

    // --------------------------------------------------------------------------
    template <core::event_id id, bool show>
    struct visibility_event_type_match {
      bool operator() (const core::event& e) {
        return (e.type == id) && (get_param1<bool>(e) == show);
      }
    };

    typedef event_handlerT<WM_SHOWWINDOW,
                           no_param_caller,
                           0,
                           visibility_event_type_match<WM_SHOWWINDOW, true>>  show_event;
    typedef event_handlerT<WM_SHOWWINDOW,
                           no_param_caller,
                           0,
                           visibility_event_type_match<WM_SHOWWINDOW, false>> hide_event;

    // --------------------------------------------------------------------------
    typedef event_handlerT<WM_WINDOWPOSCHANGED,
                           one_param_caller<core::rectangle,
                                            get_rect<WINDOWPOS>>>             place_event;

    // --------------------------------------------------------------------------
    struct pos_changing_caller : one_param_caller<core::rectangle &> {
      pos_changing_caller (const function cb)
        : one_param_caller(cb)
      {}

      template<class T>
      pos_changing_caller (T* win, void(T::*changingfn)(core::rectangle&))
        : event_handlerT(t, changingfn)
      {}

      void operator()(const core::event& e);
    };

    typedef event_handlerT<WM_WINDOWPOSCHANGING, pos_changing_caller>          placing_event;

    // --------------------------------------------------------------------------
    struct get_minmax_caller {
      typedef std::function<void(const core::size&, const core::point&, core::size&, core::size&)> function;

      get_minmax_caller (const function cb)
        : callback(cb)
      {}

      template<class T>
      get_minmax_caller (T* win, void(T::*fn)(const core::size&, const core::point&, core::size&, core::size&))
        : callback(win, fn)
      {}

      operator bool () const {
        return callback.operator bool();
      }

      void operator()(const core::event& e);

    private:
      function callback;
    };

    typedef event_handlerT<WM_GETMINMAXINFO, get_minmax_caller>                 get_minmax_event;

    // --------------------------------------------------------------------------
    struct paint_caller : one_param_caller<draw::graphics&> {
      paint_caller (const function cb)
        :one_param_caller(cb)
      {}

      template<class T>
      paint_caller (T* t, void(T::*callback_)(draw::graphics&))
        : one_param_caller(gui::core::easy_bind(t, callback_))
      {}

      void operator()(const core::event& e);
    };

    typedef event_handlerT<WM_PAINT, paint_caller>                                paint_event;

    // --------------------------------------------------------------------------
    template <WORD N>
    struct command_matcher {
      bool operator() (const core::event& e) {
        return (e.type == WM_COMMAND) && (get_param1_high<WORD>(e) == N);
      }
    };

#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    namespace detail {
      extern Atom WM_CREATE_WINDOW;

      struct window_and_rect {
        window* w;
        core::rectangle r;
      };

    }

    template <core::event_id id, core::event_id btn, int sts>
    struct event_button_match {
      bool operator() (const core::event& e) {
        return (e.type == id) && (e.xbutton.button == btn) && ((e.xbutton.state & sts) == sts);
      }
    };

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
    // --------------------------------------------------------------------------
    template <>
    inline const XKeyEvent& cast_event_type<XKeyEvent>(const core::event& e) {
      return e.xkey;
    }
    // --------------------------------------------------------------------------
    template <>
    inline const XMotionEvent& cast_event_type<XMotionEvent>(const core::event& e) {
      return e.xmotion;
    }
    // --------------------------------------------------------------------------
    template <>
    inline const XConfigureEvent& cast_event_type<XConfigureEvent>(const core::event& e) {
      return e.xconfigure;
    }
    // --------------------------------------------------------------------------
    template <>
    inline const XConfigureRequestEvent& cast_event_type<XConfigureRequestEvent>(const core::event& e) {
      return e.xconfigurerequest;
    }
    // --------------------------------------------------------------------------
    template<typename T, core::event_id E, typename C>
    struct move_size_matcher {
      bool operator() (const core::event& e) {
        if (e.type == E) {
          T& o = s_last_place[e.xany.window];
          T n = T(cast_event_type<C>(e));
          if (o != n) {
            LogDebug << "move_size_matcher " << o << " -> " << n;
            o = n;
            return true;
          }
        }
        return false;
      }
    private:
      static std::map<Window, T> s_last_place;
    };

    template<typename T, core::event_id E, typename C>
    std::map<Window, T> move_size_matcher<T, E, C>::s_last_place;

    // --------------------------------------------------------------------------
    template<typename T>
    inline unsigned int get_state(const core::event& e) {
      return cast_event_type<T>(e).state;
    }
    // --------------------------------------------------------------------------
    template<typename T>
    inline unsigned int get_button(const core::event& e) {
      return cast_event_type<T>(e).button;
    }
    // --------------------------------------------------------------------------
    template<typename T>
    inline unsigned int get_keycode(const core::event& e) {
      return cast_event_type<T>(e).keycode;
    }
    // --------------------------------------------------------------------------
    template<typename T, int I = 0>
    inline T get_client_data(const core::event& e) {
      return (T)e.xclient.data.l[I];
    }
    // --------------------------------------------------------------------------
    core::rectangle get_client_data_rect(const core::event& e);
    // --------------------------------------------------------------------------
    window* get_client_data_window(const core::event& e);
    // --------------------------------------------------------------------------
    template<typename T, typename C>
    inline T get_param (const core::event& e) {
      return T(cast_event_type<C>(e));
    }
    // --------------------------------------------------------------------------
    template<typename T>
    inline window* get_window(const core::event& e) {
      return detail::get_window(cast_event_type<T>(e).window);
    }
    // --------------------------------------------------------------------------
    template <Atom& M>
    struct client_message_matcher {
      bool operator() (const core::event& e) {
        return (e.type == ClientMessage) && (e.xclient.message_type == M);
      }
    };
    // --------------------------------------------------------------------------
    typedef event_handlerT<ClientMessage,
                           two_param_caller<window*,
                                            core::rectangle,
                                            get_client_data_window,
                                            get_client_data_rect>,
                            0,
                            client_message_matcher<detail::WM_CREATE_WINDOW>>       create_event;

    typedef event_handlerT<DestroyNotify>                                           destroy_event;

    typedef event_handlerT<KeyPress,
                           two_param_caller<unsigned int,
                                            unsigned int,
                                            get_state<XKeyEvent>,
                                            get_keycode<XKeyEvent>>>                key_down_event;

    typedef event_handlerT<KeyRelease,
                           two_param_caller<unsigned int,
                                            unsigned int,
                                            get_state<XKeyEvent>,
                                            get_keycode<XKeyEvent>>>                key_op_event;

    typedef event_handlerT<MotionNotify,
                           two_param_caller<unsigned int,
                                            core::point,
                                            get_state<XMotionEvent>,
                                            get_param<core::point,
                                                      XMotionEvent>>>               mouse_move_event;

    typedef event_handlerT<ButtonPress,
                           three_param_caller<unsigned int, unsigned int, core::point,
                                              get_state<XButtonEvent>,
                                              get_button<XButtonEvent>,
                                              get_param<core::point,
                                                        XButtonEvent>>>             button_event;

    typedef event_handlerT<ButtonPress,
                           one_param_caller<core::point,
                                            get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonPress, Button1, 0>>             left_btn_down_event;

    typedef event_handlerT<ButtonRelease,
                           one_param_caller<core::point,
                                            get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonRelease, Button1, Button1Mask>> left_btn_up_event;

    typedef event_handlerT<ButtonPress,
                           one_param_caller<core::point,
                                            get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonPress, Button3, 0>>             right_btn_down_event;

    typedef event_handlerT<ButtonRelease,
                           one_param_caller<core::point,
                                            get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonRelease, Button3, Button3Mask>> right_btn_up_event;

    typedef event_handlerT<ButtonPress,
                           one_param_caller<core::point,
                                            get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonPress, Button2, 0>>             middle_btn_down_event;

    typedef event_handlerT<ButtonRelease,
                           one_param_caller<core::point,
                                            get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonRelease, Button2, Button3Mask>> middle_btn_up_event;

    template<core::event_id B>
    struct double_click_matcher {

      bool operator() (const core::event& e) {
        if ((e.type == ButtonRelease) && (e.xbutton.button == B)) {
          Time last_up = s_last_up[e.xbutton.window];
          Time diff = e.xbutton.time - last_up;
          LogDebug << "double_click_matcher(" << e.xbutton.button
                   << ") at:" << e.xbutton.time << " last:" << last_up << " diff:" << diff;
          s_last_up[e.xbutton.window] = e.xbutton.time;
          return (diff < 300);
        }
        return false;
      }

    protected:
      static std::map<Window, Time> s_last_up;
    };

    template<core::event_id B> std::map<Window, Time> double_click_matcher<B>::s_last_up;

    typedef event_handlerT<ButtonRelease,
                           one_param_caller<core::point,
                                            get_param<core::point, XButtonEvent>>,
                           0,
                           double_click_matcher<Button1>>                           left_btn_dblclk_event;
    typedef event_handlerT<ButtonRelease,
                           one_param_caller<core::point,
                                            get_param<core::point, XButtonEvent>>,
                           0,
                           double_click_matcher<Button3>>                           right_btn_dblclk_event;
    typedef event_handlerT<ButtonRelease,
                           one_param_caller<core::point,
                                            get_param<core::point, XButtonEvent>>,
                           0,
                           double_click_matcher<Button2>>                           middle_btn_dblclk_event;

    template<int D, int U>
    inline int get_wheel_delta (const core::event& e) {
      switch (e.xbutton.button) {
        case D: return 1;
        case U: return -1;
      }
      return 0;
    }

    template <int D, int U>
    struct wheel_button_match {
      bool operator() (const core::event& e) {
        return (e.type == ButtonRelease) && ((e.xbutton.button == D) || (e.xbutton.button == U));
      }
    };

    typedef event_handlerT<ButtonRelease,
                           two_param_caller<int,
                                            core::point,
                                            get_wheel_delta<6, 7>,
                                            get_param<core::point, XButtonEvent>>,
                           0,
                           wheel_button_match<6, 7>>                                wheel_x_event;
    typedef event_handlerT<ButtonRelease,
                           two_param_caller<int,
                                            core::point,
                                            get_wheel_delta<Button4, Button5>,
                                            get_param<core::point, XButtonEvent>>,
                           0,
                           wheel_button_match<Button4, Button5>>                    wheel_y_event;

    typedef event_handlerT<MapNotify, no_param_caller>                              show_event;
    typedef event_handlerT<UnmapNotify, no_param_caller>                            hide_event;

    typedef event_handlerT<FocusIn,
                           one_param_caller<window*,
                                            get_window<XFocusChangeEvent>>>         set_focus_event;
    typedef event_handlerT<FocusOut,
                           one_param_caller<window*,
                                            get_window<XFocusChangeEvent>>>         lost_focus_event;

    typedef event_handlerT<EnterNotify>                                             mouse_enter_event;
    typedef event_handlerT<LeaveNotify>                                             mouse_leave_event;

    typedef event_handlerT<ConfigureNotify,
                           one_param_caller<core::point,
                                            get_param<core::point, XConfigureEvent>>,
                           0,
                           move_size_matcher<core::point,
                                             ConfigureNotify,
                                             XConfigureEvent>>                      move_event;
    typedef event_handlerT<ConfigureNotify,
                           one_param_caller<core::size,
                                            get_param<core::size, XConfigureEvent>>,
                           0,
                           move_size_matcher<core::size,
                                             ConfigureNotify,
                                             XConfigureEvent>>                      size_event;
    typedef event_handlerT<ConfigureNotify,
                           one_param_caller<core::rectangle,
                                            get_param<core::rectangle, XConfigureEvent>>,
                           0,
                           move_size_matcher<core::rectangle,
                                             ConfigureNotify,
                                             XConfigureEvent>>                      place_event;

    typedef event_handlerT<ConfigureRequest,
                           one_param_caller<core::point,
                                            get_param<core::point, XConfigureRequestEvent>>,
                           0,
                           move_size_matcher<core::point,
                                             ConfigureRequest,
                                             XConfigureRequestEvent>>               moving_event;

    typedef event_handlerT<ConfigureRequest,
                           one_param_caller<core::size,
                                            get_param<core::size, XConfigureRequestEvent>>,
                           0,
                           move_size_matcher<core::size,
                                             ConfigureRequest,
                                             XConfigureRequestEvent>>               sizing_event;
    typedef event_handlerT<ConfigureRequest,
                           one_param_caller<core::rectangle,
                                            get_param<core::rectangle, XConfigureRequestEvent>>,
                           0,
                           move_size_matcher<core::rectangle,
                                             ConfigureRequest,
                                             XConfigureRequestEvent>>               placing_event;

    // --------------------------------------------------------------------------
    struct paint_caller {
      typedef void(F)(draw::graphics&);
      typedef std::function<F> function;

      paint_caller (const function cb)
        : callback(cb)
        , gc(0)
      {}

      template<class T>
      paint_caller (T* t, void(T::*callback_)(draw::graphics&))
        : callback(gui::core::easy_bind(t, callback_))
        , gc(0)
      {}

      operator bool () const {
        return callback.operator bool();
      }

      ~paint_caller ();

      void operator()(const core::event& e);

    private:
      function callback;
      core::graphics_id gc;
    };

    typedef event_handlerT<Expose, paint_caller> paint_event;

    void send_client_message (window* win, Atom message,
                              long l1 = 0, long l2 = 0, long l3 = 0, long l4 = 0, long l5 = 0);
    void send_client_message (window* win, Atom message, window* w, const core::rectangle& r);

#endif // X11

  } // gui

} // gui
