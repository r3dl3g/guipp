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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window_event.h"
#include "easy_bind.h"
#include "gui_types.h"
#include "graphics.h"


namespace gui {

  namespace win {
    class window;

    // --------------------------------------------------------------------------
    template<typename R, typename ... Args>
    struct event_handlerT : std::function<event_handler> {
      typedef std::function<R(Args...)> function;

      event_handlerT(function callback_)
        : callback(callback_) {}

      template<class T>
      event_handlerT(T* t, R(T::*callback_)(Args...))
        : callback(util::easy_bind(t, callback_)) {}

    protected:
      function callback;
    };

    // --------------------------------------------------------------------------
    inline bool event_type_match(const window_event& e, core::event_id id) {
#ifdef WIN32
        return (e.msg == id);
#elif X11
        return (e.type == id);
#endif // WIN32
    }

    // --------------------------------------------------------------------------
    template<core::event_id M, core::event_result R = 0>
    struct no_param_event_handler : event_handlerT<void> {
      no_param_event_handler(function fn)
        : event_handlerT(fn) {}

      template<class T>
      no_param_event_handler(T* win, void(T::*fn)())
        : event_handlerT(win, fn) {}

      bool operator()(const window_event& e, core::event_result& result) {
        if (event_type_match(e, M) && callback) {
          callback();
          result = R;
          return true;
        }
        return false;
      }
    };

    // --------------------------------------------------------------------------
    template<core::event_id M,
             typename P1,
             typename F = get_param1<P1>,
             core::event_result R = 0>
    struct one_param_event_handler : event_handlerT <void, P1> {

      one_param_event_handler(function fn)
        : event_handlerT(fn) {}

      template<class T>
      one_param_event_handler(T* win, void(T::*fn)(T))
        : event_handlerT(win, fn) {}

      bool operator()(const window_event& e, core::event_result& result) {
        if (event_type_match(e, M) && callback) {
          callback(F()(e));
          result = R;
          return true;
        }
        return false;
      }
    };

    // --------------------------------------------------------------------------
    template<core::event_id M,
             typename P1,
             typename P2,
             typename F1 = get_param1<P1>,
             typename F2 = get_param2<P2>,
             core::event_result R = 0>
    struct two_param_event_handler : event_handlerT<void, P1, P2> {

      two_param_event_handler(function fn)
        : event_handlerT(fn) {}

      template<class T>
      two_param_event_handler(T* win, void(T::*fn)(P1, P2))
        : event_handlerT(win, fn) {}

      bool operator()(const window_event& e, core::event_result& result) {
        if (event_type_match(e, M) && callback) {
          callback(F1()(e), F2()(e));
          result = R;
          return true;
        }
        return false;
      }
    };

    // --------------------------------------------------------------------------
    template<core::event_id M,
             typename P1,
             typename P2,
             typename P3,
             typename F1 = get_param1_low<P1>,
             typename F2 = get_param1_high<P2>,
             typename F3 = get_param2<P3>,
             core::event_result R = 0>
    struct three_param_event_handler : event_handlerT<void, P1, P2, P3> {

      three_param_event_handler(function fn)
        : event_handlerT(fn) {}

      template<class T>
      three_param_event_handler(T* win, void(T::*fn)(P1, P2, P3))
        : event_handlerT(win, fn) {}

      bool operator()(const window_event& e, core::event_result& result) {
        if (event_type_match(e, M) && callback) {
          callback(F1()(e), F2()(e), F3()(e));
          result = R;
          return true;
        }
        return false;
      }
    };

    // --------------------------------------------------------------------------
    template<typename T> struct get_param1 {
      T operator()(const window_event& e) const {
        return T(e.param_1);
      }
    };
    // --------------------------------------------------------------------------
    template<typename T> struct get_param2 {
      T operator()(const window_event& e) const {
        return T(e.param_2);
      }
    };
    // --------------------------------------------------------------------------
    template<typename T> struct get_param1_low {
      T operator()(const window_event& e) const {
        return static_cast<T>((SHORT)LOWORD(e.param_1));
      }
    };
    // --------------------------------------------------------------------------
    template<typename T> struct get_param1_high {
      T operator()(const window_event& e) const {
        return static_cast<T>((SHORT)HIWORD(e.param_1));
      }
    };
    // --------------------------------------------------------------------------
    template<> struct get_param1<bool> {
      bool operator()(const window_event& e) const;
    };
    // --------------------------------------------------------------------------
    template<> struct get_param1<window*> {
      window* operator()(const window_event& e) const;
    };
    // --------------------------------------------------------------------------
    template<> struct get_param1<draw::graphics> {
      draw::graphics operator()(const window_event& e) const;
    };
    // --------------------------------------------------------------------------
    template<> struct get_param2<window*> {
      window* operator()(const window_event& e) const;
    };
    // --------------------------------------------------------------------------
    template<typename T> struct get_rect {
      core::rectangle operator()(const window_event& e) const {
        T& p = *reinterpret_cast<T*>(e.param_2);
        return core::rectangle(p.x, p.y, p.cx, p.cy);
      }
    };
    // --------------------------------------------------------------------------
    struct get_window_from_cs {
      window* operator()(const window_event& e) const;
    };
    // --------------------------------------------------------------------------
    struct get_flags_from_wp {
      unsigned int operator()(const window_event& e) const;
    };

    // --------------------------------------------------------------------------
    typedef two_param_event_handler<WM_CREATE,
                                    window*,
                                    core::rectangle,
                                    get_window_from_cs,
                                    get_rect<CREATESTRUCT>>       create_event;

    typedef no_param_event_handler<WM_DESTROY>                    destroy_event;
    typedef no_param_event_handler<WM_CLOSE>                      close_event;
    typedef no_param_event_handler<WM_QUIT>                       quit_event;

    typedef one_param_event_handler<WM_ERASEBKGND, draw::graphics> erase_event;
    typedef one_param_event_handler<WM_PRINT, draw::graphics>      print_event;
    typedef one_param_event_handler<WM_PRINTCLIENT, draw::graphics> print_client_event;

    typedef one_param_event_handler<WM_SETREDRAW, bool>           redraw_changed_event;

    typedef one_param_event_handler<WM_ENABLE, bool>              enable_event;
    typedef two_param_event_handler<WM_ACTIVATE, bool, window*>   activate_event;
    typedef one_param_event_handler<WM_ACTIVATEAPP, bool>         activate_app_event;
    typedef one_param_event_handler<WM_SETFOCUS, window*>         set_focus_event;
    typedef one_param_event_handler<WM_KILLFOCUS, window*>        lost_focus_event;

    typedef no_param_event_handler<WM_ENTERSIZEMOVE>              begin_size_or_move_event;
    typedef no_param_event_handler<WM_EXITSIZEMOVE>               end_size_or_move_event;

    typedef one_param_event_handler<WM_MOVE,
                                    core::point,
                                    get_param2<core::point>>   move_event;
    typedef one_param_event_handler<WM_MOVING,
                                    core::rectangle,
                                    get_param2<core::rectangle>, TRUE > moving_event;

    typedef two_param_event_handler<WM_SIZE, unsigned int, core::size>  size_event;
    typedef two_param_event_handler<WM_SIZING,
                                    unsigned int,
                                    core::rectangle,
                                    get_param1<unsigned int>,
                                    get_param2<core::rectangle>, TRUE > sizing_event;

#ifdef WIN32
    typedef two_param_event_handler<WM_LBUTTONDOWN,
                                    unsigned int,core::point>  left_btn_down_event;
#elif X11
    typedef two_param_event_handler<ButtonPress,
                                    unsigned int,core::point>  left_btn_down_event;
#endif // WIN32
    typedef two_param_event_handler<WM_LBUTTONUP,
                                    unsigned int,core::point>  left_btn_up_event;
    typedef two_param_event_handler<WM_LBUTTONDBLCLK,
                                    unsigned int, core::point> left_btn_dblclk_event;

    typedef two_param_event_handler<WM_RBUTTONDOWN,
                                    unsigned int, core::point> right_btn_down_event;
    typedef two_param_event_handler<WM_RBUTTONUP,
                                    unsigned int, core::point> right_btn_up_event;
    typedef two_param_event_handler<WM_RBUTTONDBLCLK,
                                    unsigned int, core::point> right_btn_dblclk_event;

    typedef two_param_event_handler<WM_MBUTTONDOWN,
                                    unsigned int, core::point> middle_btn_down_event;
    typedef two_param_event_handler<WM_MBUTTONUP,
                                    unsigned int, core::point> middle_btn_up_event;
    typedef two_param_event_handler<WM_MBUTTONDBLCLK,
                                    unsigned int, core::point> middle_btn_dblclk_event;

    typedef two_param_event_handler<WM_MOUSEMOVE,
                                    unsigned int, core::point> mouse_move_event;
    typedef two_param_event_handler<WM_MOUSEHOVER,
                                    unsigned int, core::point> mouse_hover_event;
    typedef no_param_event_handler<WM_MOUSELEAVE>                 mouse_leave_event;

    typedef three_param_event_handler<WM_MOUSEHWHEEL,
                                      unsigned int, int, core::point> wheel_x_event;
    typedef three_param_event_handler<WM_MOUSEWHEEL,
                                      unsigned int, int, core::point> wheel_y_event;

    // --------------------------------------------------------------------------
    typedef two_param_event_handler<WM_WINDOWPOSCHANGED,
                                    unsigned int,
                                    core::rectangle,
                                    get_flags_from_wp,
                                    get_rect<WINDOWPOS>>          pos_changed_event;

    typedef two_param_event_handler<WM_SHOWWINDOW,
                                    bool,
                                    unsigned int> show_event;

    // --------------------------------------------------------------------------
    struct paint_event : event_handlerT<void, draw::graphics&> {
      paint_event(function changingfn)
        : event_handlerT(changingfn)
      {}

      template<class T>
      paint_event(T* win, void(T::*changingfn)(draw::graphics&))
        : event_handlerT(t, changingfn)
      {}

      bool operator()(const window_event& e, core::event_result& result);
    };

    // --------------------------------------------------------------------------
    struct pos_changing_event : event_handlerT<void, unsigned int&, core::rectangle &> {
      pos_changing_event(function changingfn)
        : event_handlerT(changingfn) {}

      template<class T>
      pos_changing_event(T* win,
        void(T::*changingfn)(unsigned int&, core::rectangle&))
        : event_handlerT(t, changingfn) {}

      bool operator()(const window_event& e, core::event_result& result);
    };

    // --------------------------------------------------------------------------
    struct get_minmax_event : event_handlerT<void, const core::size&, const core::point&, core::size&, core::size&> {
      get_minmax_event(function fn)
        : event_handlerT(fn) {}

      template<class T>
      get_minmax_event(T* win, void(T::*fn)(const core::size&, const core::point&, core::size&, core::size&))
        : event_handlerT(win, fn) {}

      bool operator()(const window_event& e, core::event_result& result);
    };

  } // gui

} // gui
