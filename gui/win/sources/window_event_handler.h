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
#include "event.h"
#include "easy_bind.h"
#include "gui_types.h"
#include "graphics.h"
#include "logger.h"


namespace gui {

  namespace win {
    class window;

    // --------------------------------------------------------------------------
    template<typename R, typename ... Args>
    struct event_handlerT : std::function<core::event_handler> {
      typedef std::function<R(Args...)> function;

      event_handlerT(function callback_)
        : callback(callback_) {}

      template<class T>
      event_handlerT(T* t, R(T::*callback_)(Args...))
        : callback(gui::core::easy_bind(t, callback_)) {}

    protected:
      function callback;
    };

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
    template<>
    window* get_param1<window*>(const core::event& e);
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
#endif // WIN32
    
     // --------------------------------------------------------------------------
    template <typename T>
    const T& cast_event_type(const core::event& e) {
      return *((const T*)&e);
    }
    
    // --------------------------------------------------------------------------
#ifdef WIN32
    template <core::event_id id>
    bool event_type_match(const core::event& e) {
      return (e.msg == id);
    }
#elif X11
    template <core::event_id id>
    bool event_type_match(const core::event& e) {
      return (e.type == id);
    }
#endif // X11

    // --------------------------------------------------------------------------
    template<core::event_id M,
             bool(matcher)(const core::event&) = event_type_match<M>,
             core::event_result R = 0>
    struct no_param_event_handler : std::function<core::event_handler> {
      typedef std::function<void()> function;

      no_param_event_handler(function fn)
        : callback(fn) {
      }

      template<class T>
      no_param_event_handler(T* win, void(T::*fn)())
        : callback(win, fn) {
      }

      bool operator()(const core::event& e, core::event_result& result) {
         if (matcher(e) && callback) {
          callback();
          result = R;
          return true;
        }
        return false;
      }

    private:
      function callback;
      
    };

    // --------------------------------------------------------------------------
    template<core::event_id M,
             typename P,
             P(F)(const core::event&) = get_param1<P>,
             bool(matcher)(const core::event&) = event_type_match<M>,
             core::event_result R = 0>
    struct one_param_event_handler : std::function<core::event_handler> {
      typedef std::function<void(P)> function;
      
      one_param_event_handler(function fn)
        : callback(fn)
      {}

      template<class T>
      one_param_event_handler(T* win, void(T::*fn)(T))
        : callback(win, fn) 
      {}

      bool operator()(const core::event& e, core::event_result& result) {
         if (matcher(e) && callback) {
          callback(F(e));
          result = R;
          return true;
        }
        return false;
      }

    private:
      function callback;

    };

    // --------------------------------------------------------------------------
    template<core::event_id M,
             typename P1,
             typename P2,
             P1(F1)(const core::event&) = get_param1<P1>,
             P2(F2)(const core::event&) = get_param2<P2>,
             bool(matcher)(const core::event&) = event_type_match<M>,
             core::event_result R = 0>
    struct two_param_event_handler : std::function<core::event_handler> {
      typedef std::function<void(P1, P2)> function;

      two_param_event_handler(function fn)
        : callback(fn)
      {}

      template<class T>
      two_param_event_handler(T* win, void(T::*fn)(P1, P2))
        : callback(win, fn)
      {}

      bool operator()(const core::event& e, core::event_result& result) {
         if (matcher(e) && callback) {
          callback(F1(e), F2(e));
          result = R;
          return true;
        }
        return false;
      }

    private:
      function callback;
      
    };

    // --------------------------------------------------------------------------
    template<core::event_id M,
             typename P1,
             typename P2,
             typename P3,
             P1(F1)(const core::event&) = get_param1_low<P1>,
             P2(F2)(const core::event&) = get_param1_high<P2>,
             P3(F3)(const core::event&) = get_param2<P3>,
             bool(matcher)(const core::event&) = event_type_match<M>,
             core::event_result R = 0>
    struct three_param_event_handler : std::function<core::event_handler> {
      typedef std::function<void(P1, P2, P3)> function;

      three_param_event_handler(function fn)
        : callback(fn)
      {}

      template<class T>
      three_param_event_handler(T* win, void(T::*fn)(P1, P2, P3))
        : callback(win, fn)
      {}

      bool operator()(const core::event& e, core::event_result& result) {
         if (matcher(e) && callback) {
          callback(F1(e), F2(e), F3(e));
          result = R;
          return true;
        }
        return false;
      }

    private:
      function callback;
      
    };

#ifdef WIN32
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
                                    get_param2<core::point>>      move_event;
    typedef one_param_event_handler<WM_MOVING,
                                    core::rectangle,
                                    get_param2<core::rectangle>,
                                    event_type_match<WM_MOVING>, TRUE>       moving_event;

    typedef two_param_event_handler<WM_SIZE, unsigned int, core::size>  size_event;
    typedef two_param_event_handler<WM_SIZING,
                                    unsigned int,
                                    core::rectangle,
                                    get_param1<unsigned int>,
                                    get_param2<core::rectangle>,
                                    event_type_match<WM_SIZING>, TRUE>       sizing_event;

    typedef one_param_event_handler<WM_LBUTTONDOWN,
                                    core::point, get_param2<core::point>>  left_btn_down_event;
    typedef one_param_event_handler<WM_LBUTTONUP,
                                    core::point, get_param2<core::point>>  left_btn_up_event;
    typedef one_param_event_handler<WM_LBUTTONDBLCLK,
                                    core::point, get_param2<core::point>> left_btn_dblclk_event;

    typedef one_param_event_handler<WM_RBUTTONDOWN,
                                    core::point, get_param2<core::point>> right_btn_down_event;
    typedef one_param_event_handler<WM_RBUTTONUP,
                                    core::point, get_param2<core::point>> right_btn_up_event;
    typedef one_param_event_handler<WM_RBUTTONDBLCLK,
                                    core::point, get_param2<core::point>> right_btn_dblclk_event;

    typedef one_param_event_handler<WM_MBUTTONDOWN,
                                    core::point, get_param2<core::point>> middle_btn_down_event;
    typedef one_param_event_handler<WM_MBUTTONUP,
                                    core::point, get_param2<core::point>> middle_btn_up_event;
    typedef one_param_event_handler<WM_MBUTTONDBLCLK,
                                    core::point, get_param2<core::point>> middle_btn_dblclk_event;

    typedef two_param_event_handler<WM_MOUSEMOVE,
                                    unsigned int, core::point> mouse_move_event;
    typedef two_param_event_handler<WM_MOUSEHOVER,
                                    unsigned int, core::point> mouse_hover_event;
    typedef no_param_event_handler<WM_MOUSELEAVE>               mouse_leave_event;

    typedef three_param_event_handler<WM_MOUSEHWHEEL,
                                      unsigned int, int, core::point> wheel_x_event;
    typedef three_param_event_handler<WM_MOUSEWHEEL,
                                      unsigned int, int, core::point> wheel_y_event;

    // --------------------------------------------------------------------------
    typedef two_param_event_handler<WM_WINDOWPOSCHANGED,
                                    unsigned int,
                                    core::rectangle,
                                    get_flags_from_wp,
                                    get_rect<WINDOWPOS>> pos_changed_event;

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

      bool operator()(const core::event& e, core::event_result& result);
    };

    // --------------------------------------------------------------------------
    struct pos_changing_event : event_handlerT<void, unsigned int&, core::rectangle &> {
      pos_changing_event(function changingfn)
        : event_handlerT(changingfn)
      {}

      template<class T>
      pos_changing_event(T* win, void(T::*changingfn)(unsigned int&, core::rectangle&))
        : event_handlerT(t, changingfn)
      {}

      bool operator()(const core::event& e, core::event_result& result);
    };

    // --------------------------------------------------------------------------
    struct get_minmax_event : event_handlerT<void, const core::size&, const core::point&, core::size&, core::size&> {
      get_minmax_event(function fn)
        : event_handlerT(fn)
      {}

      template<class T>
      get_minmax_event(T* win, void(T::*fn)(const core::size&, const core::point&, core::size&, core::size&))
        : event_handlerT(win, fn)
      {}

      bool operator()(const core::event& e, core::event_result& result);
    };
#endif // WIN32
#ifdef X11
    // --------------------------------------------------------------------------
    template <core::event_id id, core::event_id btn, int sts>
    bool event_button_match(const XEvent& e) {
      /*if (e.type == id) {
        LogDebug << "event_button_match id:" << e.type << " && (" 
                 << e.xbutton.button << " == " << btn << ") && ("
                 << e.xbutton.state << " & " << sts << " = " << (e.xbutton.state & sts) << ")";
      }*/
      return (e.type == id) && (e.xbutton.button == btn) && ((e.xbutton.state & sts) == sts);
    }
    // --------------------------------------------------------------------------
    template <>
    inline const XButtonEvent& cast_event_type<XButtonEvent>(const core::event& e) {
      return e.xbutton;
    }
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
    inline core::point get_point(const core::event& e) {
      const T& be = cast_event_type<T>(e);
      return core::point(be.x, be.y);
    }
    // --------------------------------------------------------------------------
    typedef no_param_event_handler<DestroyNotify> destroy_event;
    
    typedef one_param_event_handler<ButtonPress,
                                    core::point,
                                    get_point<XButtonEvent>,
                                    event_button_match<ButtonPress, Button1, 0>> left_btn_down_event;
    typedef one_param_event_handler<ButtonRelease,
                                    core::point,
                                    get_point<XButtonEvent>,
                                    event_button_match<ButtonRelease, Button1, Button1Mask>> left_btn_up_event;

    typedef one_param_event_handler<ButtonPress,
                                    core::point,
                                    get_point<XButtonEvent>,
                                    event_button_match<ButtonPress, Button3, 0>> right_btn_down_event;
    typedef one_param_event_handler<ButtonRelease,
                                    core::point,
                                    get_point<XButtonEvent>,
                                    event_button_match<ButtonRelease, Button3, Button3Mask>> right_btn_up_event;

    typedef three_param_event_handler<ButtonPress,
                                     unsigned int, unsigned int, core::point,
                                     get_state<XButtonEvent>,
                                     get_button<XButtonEvent>,
                                     get_point<XButtonEvent>> button_event;
    // --------------------------------------------------------------------------
    struct paint_event : std::function<core::event_handler> {
      typedef std::function<void(draw::graphics&)> function;

      paint_event(function changingfn)
        : callback(changingfn)
      {}

      template<class T>
      paint_event(T* t, void(T::*changingfn)(draw::graphics&))
        : callback(t, changingfn)
      {}

      bool operator()(const core::event& e, core::event_result& result);

    private:
      function callback;
      
    };

#endif // X11

  } // gui

} // gui
