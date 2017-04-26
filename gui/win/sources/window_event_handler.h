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
#include <map>
#ifdef X11
#include <X11/Xlib.h>
#endif // X11

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

    namespace detail {

      template<typename T, T mask>
      struct bit_mask {
        static bool is_set (T value) {
          return (value & mask) == mask;
        }

        static bool is_not_set (T value) {
          return (value & mask) == 0;
        }
      };

    }

#ifdef WIN32
#ifndef MK_MENU
# define MK_MENU 0x0080
#endif // MK_MENU
#ifndef MK_SYTEM
# define MK_SYTEM 0x0100
#endif // MK_MENU

    using left_button_bit_mask = detail::bit_mask<os::key_state, MK_LBUTTON>;
    using middle_button_bit_mask = detail::bit_mask<os::key_state, MK_MBUTTON>;
    using right_button_bit_mask = detail::bit_mask<os::key_state, MK_RBUTTON>;

    using shift_key_bit_mask = detail::bit_mask<os::key_state, MK_SHIFT>;
    using control_key_bit_mask = detail::bit_mask<os::key_state, MK_CONTROL>;
    using alt_key_bit_mask = detail::bit_mask<os::key_state, MK_MENU>;
    using system_key_bit_mask = detail::bit_mask<os::key_state, MK_SYTEM>;

    namespace keys {
      const os::key_symbol left = VK_LEFT;
      const os::key_symbol right = VK_RIGHT;
      const os::key_symbol up = VK_UP;
      const os::key_symbol down = VK_DOWN;

      const os::key_symbol page_up = VK_PRIOR;
      const os::key_symbol page_down = VK_NEXT;
      const os::key_symbol home = VK_HOME;
      const os::key_symbol end = VK_END;

      namespace numpad {
        const os::key_symbol left = VK_LEFT | 0xFF00;
        const os::key_symbol right = VK_RIGHT | 0xFF00;
        const os::key_symbol up = VK_UP | 0xFF00;
        const os::key_symbol down = VK_DOWN | 0xFF00;

        const os::key_symbol page_up = VK_PRIOR | 0xFF00;
        const os::key_symbol page_down = VK_NEXT | 0xFF00;
        const os::key_symbol home = VK_HOME | 0xFF00;
        const os::key_symbol end = VK_END | 0xFF00;

        const os::key_symbol del = VK_DELETE | 0xFF00;
      }

      const os::key_symbol del = VK_DELETE;
      const os::key_symbol insert = VK_INSERT;
      const os::key_symbol escape = VK_ESCAPE;
      const os::key_symbol enter = VK_RETURN;
      const os::key_symbol space = VK_SPACE;
      const os::key_symbol clear = 0xFFFF;

      const os::key_symbol back_space = VK_BACK;
      const os::key_symbol tab = VK_TAB;
      const os::key_symbol print = VK_PRINT;
    }

#endif // WIN32

#ifdef X11
    typedef detail::bit_mask<os::key_state, Button1Mask> left_button_bit_mask;
    typedef detail::bit_mask<os::key_state, Button2Mask> middle_button_bit_mask;
    typedef detail::bit_mask<os::key_state, Button3Mask> right_button_bit_mask;

    typedef detail::bit_mask<os::key_state, ShiftMask> shift_key_bit_mask;
    typedef detail::bit_mask<os::key_state, ControlMask> control_key_bit_mask;
    typedef detail::bit_mask<os::key_state, Mod1Mask> alt_key_bit_mask;
    typedef detail::bit_mask<os::key_state, Mod2Mask> system_key_bit_mask;

    namespace keys {
      const os::key_symbol left = XK_Left;
      const os::key_symbol right = XK_Right;
      const os::key_symbol up = XK_Up;
      const os::key_symbol down = XK_Down;

      const os::key_symbol page_up = XK_Page_Up;
      const os::key_symbol page_down = XK_Page_Down;
      const os::key_symbol home = XK_Home;
      const os::key_symbol end = XK_End;

      namespace numpad {
        const os::key_symbol left = XK_KP_Left;
        const os::key_symbol right = XK_KP_Right;
        const os::key_symbol up = XK_KP_Up;
        const os::key_symbol down = XK_KP_Down;

        const os::key_symbol page_up = XK_KP_Page_Up;
        const os::key_symbol page_down = XK_KP_Page_Down;
        const os::key_symbol home = XK_KP_Home;
        const os::key_symbol end = XK_KP_End;

        const os::key_symbol del = XK_KP_Delete;
      }

      const os::key_symbol del = XK_Delete;
      const os::key_symbol insert = XK_Insert;
      const os::key_symbol escape = XK_Escape;
      const os::key_symbol enter = XK_Return;
      const os::key_symbol space = XK_space;
      const os::key_symbol clear = XK_Clear;

      const os::key_symbol back_space = XK_BackSpace;
      const os::key_symbol tab = XK_Tab;
      const os::key_symbol print = XK_Print;
  }
#endif // X11

    class window;

#ifdef WIN32
    // --------------------------------------------------------------------------
    template<int I, typename T>
    T get_param (const core::event& e);

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
    bool get_param<0, bool>(const core::event& e);

    // --------------------------------------------------------------------------
    template<>
    unsigned int get_param<0, unsigned int>(const core::event& e);

    // --------------------------------------------------------------------------
    template<>
    int get_param<0, int>(const core::event& e);

    // --------------------------------------------------------------------------
    template<>
    window* get_param<1, window*>(const core::event& e);

    // --------------------------------------------------------------------------
    template<>
    core::point get_param<1, core::point>(const core::event& e);

    // --------------------------------------------------------------------------
    template<>
    core::size get_param<1, core::size>(const core::event& e);

    // --------------------------------------------------------------------------
    template<typename T>
    core::rectangle get_rect(const core::event& e) {
      T& p = *reinterpret_cast<T*>(e.param_2);
      return core::rectangle(static_cast<core::point::type>(p.x),
                             static_cast<core::point::type>(p.y),
                             static_cast<core::size::type>(p.cx),
                             static_cast<core::size::type>(p.cy));
    }

    // --------------------------------------------------------------------------
    window* get_window_from_cs(const core::event& e);

    // --------------------------------------------------------------------------
    unsigned int get_flags_from_wp(const core::event& e);

#endif // Win32
#ifdef X11

    // --------------------------------------------------------------------------
    template<int I, typename T>
    T get_param(const core::event& e);

    // --------------------------------------------------------------------------
    template <typename T>
    const T& cast_event_type(const core::event& e);

#endif // X11

    // --------------------------------------------------------------------------
    os::key_state get_key_state (const core::event& e);
    // --------------------------------------------------------------------------
    os::key_symbol get_key_symbol (const core::event& e);
    // --------------------------------------------------------------------------
    std::string get_key_chars (const core::event& e);

    // --------------------------------------------------------------------------
    template<>
    os::graphics get_param<0, os::graphics>(const core::event& e);

    // --------------------------------------------------------------------------
    template<>
    window* get_param<0, window*>(const core::event& e);

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
    template<typename... Ts> struct Params {
      template<param_getter<Ts>... Fs> struct caller {
        using callback = void(Ts...);
        typedef std::function<callback> function;

        caller (const function f)
          :f(f)
        {}

        template<class C>
        caller (C* t, void(C::*method)(Ts...))
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
             typename Caller = Params<>::caller<>,
             os::event_result R = 0,
             typename Matcher = event_type_match<E>>
    struct event_handler {
      static const os::event_id mask = Mask;

      typedef typename Caller::function function;

      event_handler (const function cb)
        : caller(cb)
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
    typedef event_handler<WM_CREATE, 0,
                           Params<window*, core::rectangle>::
                           caller<get_window_from_cs,
                                  get_rect<CREATESTRUCT>>>                    create_event;

    typedef event_handler<WM_DESTROY>                                         destroy_event;
    typedef event_handler<WM_CLOSE>                                           close_event;
    typedef event_handler<WM_QUIT>                                            quit_event;

    typedef event_handler<WM_KEYDOWN, 0,
                          Params<os::key_state, os::key_symbol, std::string>::
                          caller<get_key_state,
                                 get_key_symbol,
                                 get_key_chars>>                              key_down_event;

    typedef event_handler<WM_KEYUP, 0,
                          Params<os::key_state, os::key_symbol>::
                          caller<get_key_state,
                                 get_key_symbol>>                             key_up_event;

    typedef event_handler<WM_CHAR, 0,
                          Params<std::string>::
                          caller<get_key_chars>>                              character_event;

    typedef event_handler<WM_ERASEBKGND, 0,
                           Params<os::graphics>::
                           caller<get_param<0, os::graphics>>>                erase_event;
    typedef event_handler<WM_PRINT, 0,
                           Params<os::graphics>::
                           caller<get_param<0, os::graphics>>>                print_event;
    typedef event_handler<WM_PRINTCLIENT, 0,
                           Params<os::graphics>::
                           caller<get_param<0, os::graphics>>>                print_client_event;

    typedef event_handler<WM_SETREDRAW, 0,
                           Params<bool>::caller<get_param<0, bool>>>          redraw_changed_event;

    typedef event_handler<WM_ENABLE, 0,
                           Params<bool>::caller<get_param<0, bool>>>          enable_event;
    typedef event_handler<WM_ACTIVATE, 0,
                          Params<bool, window*>::
                          caller<get_param<0, bool>,
                                 get_param<1, window*>>>                      activate_event;
    typedef event_handler<WM_ACTIVATEAPP, 0,
                           Params<bool>::caller<get_param<0, bool>>>          activate_app_event;
    typedef event_handler<WM_SETFOCUS, 0,
                           Params<window*>::caller<get_param<0, window*>>>    set_focus_event;
    typedef event_handler<WM_KILLFOCUS, 0,
                           Params<window*>::caller<get_param<0, window*>>>    lost_focus_event;

    typedef event_handler<WM_ENTERSIZEMOVE>                                   begin_size_or_move_event;
    typedef event_handler<WM_EXITSIZEMOVE>                                    end_size_or_move_event;

    typedef event_handler<WM_MOVE, 0,
                           Params<core::point>::
                           caller<get_param<1, core::point>>>                 move_event;
    typedef event_handler<WM_MOVING, 0,
                           Params<core::point>::
                           caller<get_param<1, core::point>>,
                           TRUE>                                              moving_event;

    typedef event_handler<WM_SIZE, 0,
                           Params<core::size>::
                           caller<get_param<1, core::size>>>                  size_event;
    typedef event_handler<WM_SIZING, 0,
                           Params<core::size>::
                           caller<get_param<1, core::size>>,
                           TRUE>                                              sizing_event;

    typedef event_handler<WM_LBUTTONDOWN, 0,
                           Params<os::key_state, core::point>::
                           caller<get_param<0, os::key_state>,
                                  get_param<1, core::point>>>                 left_btn_down_event;
    typedef event_handler<WM_LBUTTONUP, 0,
                           Params<os::key_state, core::point>::
                           caller<get_param<0, os::key_state>,
                                  get_param<1, core::point>>>                 left_btn_up_event;
    typedef event_handler<WM_LBUTTONDBLCLK, 0,
                           Params<os::key_state, core::point>::
                           caller<get_param<0, os::key_state>,
                                  get_param<1, core::point>>>                 left_btn_dblclk_event;

    typedef event_handler<WM_RBUTTONDOWN, 0,
                           Params<os::key_state, core::point>::
                           caller<get_param<0, os::key_state>,
                                  get_param<1, core::point>>>                 right_btn_down_event;
    typedef event_handler<WM_RBUTTONUP, 0,
                           Params<os::key_state, core::point>::
                           caller<get_param<0, os::key_state>,
                                  get_param<1, core::point>>>                 right_btn_up_event;
    typedef event_handler<WM_RBUTTONDBLCLK, 0,
                           Params<os::key_state, core::point>::
                           caller<get_param<0, os::key_state>,
                                  get_param<1, core::point>>>                 right_btn_dblclk_event;

    typedef event_handler<WM_MBUTTONDOWN, 0,
                           Params<os::key_state, core::point>::
                           caller<get_param<0, os::key_state>,
                                  get_param<1, core::point>>>                 middle_btn_down_event;
    typedef event_handler<WM_MBUTTONUP, 0,
                           Params<os::key_state, core::point>::
                           caller<get_param<0, os::key_state>,
                                  get_param<1, core::point>>>                 middle_btn_up_event;
    typedef event_handler<WM_MBUTTONDBLCLK, 0,
                           Params<os::key_state, core::point>::
                           caller<get_param<0, os::key_state>,
                                  get_param<1, core::point>>>                 middle_btn_dblclk_event;

    typedef event_handler<WM_MOUSEMOVE, 0,
                           Params<os::key_state, core::point>::
                           caller<get_param<0, os::key_state>,
                                  get_param<1, core::point>>>                 mouse_move_event;


    struct mouse_enter_matcher {
      bool operator() (const core::event& e);
    };

    typedef event_handler<WM_MOUSEMOVE, 0,
                           Params<>::caller<>,
                           0,
                           mouse_enter_matcher>                               mouse_enter_event;
    typedef event_handler<WM_MOUSELEAVE>                                      mouse_leave_event;

    typedef event_handler<WM_MOUSEHWHEEL, 0,
                           Params<core::point::type, core::point>::
                           caller<get_param1_high<core::point_type>,
                                  get_param<1, core::point>>>                 wheel_x_event;
    typedef event_handler<WM_MOUSEWHEEL, 0,
                           Params<core::point::type, core::point>::
                           caller<get_param1_high<core::point_type>,
                                  get_param<1, core::point>>>                 wheel_y_event;

    // --------------------------------------------------------------------------
    template <os::event_id id, bool show>
    struct visibility_event_type_match {
      bool operator() (const core::event& e) {
        return (e.type == id) && (get_param<0, bool>(e) == show);
      }
    };

    typedef event_handler<WM_SHOWWINDOW, 0,
                           Params<>::caller<>,
                           0,
                           visibility_event_type_match<WM_SHOWWINDOW, true>>  show_event;
    typedef event_handler<WM_SHOWWINDOW, 0,
                           Params<>::caller<>,
                           0,
                           visibility_event_type_match<WM_SHOWWINDOW, false>> hide_event;

    // --------------------------------------------------------------------------
    typedef event_handler<WM_WINDOWPOSCHANGED, 0,
                           Params<core::rectangle>::caller<
                                            get_rect<WINDOWPOS>>>             place_event;

    // --------------------------------------------------------------------------
    struct pos_changing_caller : Params<core::rectangle>::caller<get_param<0, core::rectangle>> {
      typedef Params<core::rectangle>::caller<get_param<0, core::rectangle>> super;

      pos_changing_caller (const function cb)
        : super(cb)
      {}

      template<class T>
      pos_changing_caller (T* win, void(T::*changingfn)(core::rectangle&))
        : event_handler(t, changingfn)
      {}

      void operator()(const core::event& e);
    };

    typedef event_handler<WM_WINDOWPOSCHANGING, 0, pos_changing_caller>          placing_event;

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

    typedef event_handler<WM_GETMINMAXINFO, 0, get_minmax_caller>                 get_minmax_event;

    // --------------------------------------------------------------------------
    struct os_paint_caller : Params<os::graphics>::caller<get_param<0, os::graphics>> {
      typedef Params<os::graphics>::caller<get_param<0, os::graphics>> super;

      os_paint_caller (const function cb)
        :super(cb)
      {}

      template<class T>
      os_paint_caller (T* t, void(T::*callback_)(os::graphics&))
        : super(gui::core::bind_method(t, callback_))
      {}

      void operator()(const core::event& e);
    };

    typedef event_handler<WM_PAINT, 0, os_paint_caller>                           os_paint_event;

    // --------------------------------------------------------------------------
    template <WORD N>
    struct command_matcher {
      bool operator() (const core::event& e) {
        return (e.type == WM_COMMAND) && (get_param1_high<WORD>(e) == N);
      }
    };

    void send_client_message (window* win, os::event_id message, long l1 = 0, long l2 = 0);


#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    namespace detail {
      extern Atom WM_CREATE_WINDOW;
      extern Atom WM_DELETE_WINDOW;
      extern std::map<Window, XIC> s_window_ic_map;

      void init_message (Atom& message, const char* name);
    }

    template <os::event_id id, os::event_id btn, int sts>
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
    template<typename T, os::event_id E, typename C>
    struct move_size_matcher {
      bool operator() (const core::event& e) {
        os::event_id t = e.type;
        if (t == E) {
          Window w = e.xany.window;
          T& o = s_last_place[w];
          T n = T(cast_event_type<C>(e));
          if (o != n) {
            //LogDebug << "move_size_matcher " << o << " -> " << n;
            o = n;
            return true;
          }
        }
        return false;
      }
    private:
      static std::map<Window, T> s_last_place;
    };

    template<typename T, os::event_id E, typename C>
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
    inline T* get_event_ptr(const core::event& e) {
      return const_cast<T*>(&(cast_event_type<T>(e)));
    }
    // --------------------------------------------------------------------------
    template<int I, typename T>
    inline T get_client_data(const core::event& e) {
      return (T)e.xclient.data.l[I];
    }
    // --------------------------------------------------------------------------
    core::rectangle get_client_data_rect(const core::event& e);
    // --------------------------------------------------------------------------
    template<>
    window* get_client_data<0, window*>(const core::event& e);
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
    typedef event_handler<ClientMessage, 0,
                           Params<window*, core::rectangle>::
                           caller<get_client_data<0, window*>,
                                  get_client_data_rect>,
                           0,
                           client_message_matcher<detail::WM_CREATE_WINDOW>>        create_event;

    typedef event_handler<DestroyNotify, SubstructureNotifyMask>                    destroy_event;

    typedef event_handler<KeyPress, KeyPressMask,
                           Params<os::key_state, os::key_symbol, std::string>::
                           caller<get_key_state,
                                  get_key_symbol,
                                  get_key_chars>>                                   key_down_event;

    typedef event_handler<KeyRelease, KeyReleaseMask,
                           Params<os::key_state, os::key_symbol>::
                           caller<get_key_state,
                                  get_key_symbol>>                                  key_up_event;

    typedef event_handler<MotionNotify, PointerMotionMask,
                           Params<os::key_state, core::point>::
                           caller<get_state<XMotionEvent>,
                                  get_param<core::point, XMotionEvent>>>            mouse_move_event;

    typedef event_handler<ButtonPress, ButtonPressMask,
                           Params<os::key_state, core::point>::
                           caller<get_state<XButtonEvent>,
                                  get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonPress, Button1, 0>>             left_btn_down_event;

    typedef event_handler<ButtonRelease, ButtonReleaseMask,
                           Params<os::key_state, core::point>::
                           caller<get_state<XButtonEvent>,
                                  get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonRelease, Button1, Button1Mask>> left_btn_up_event;

    typedef event_handler<ButtonPress, ButtonPressMask,
                           Params<os::key_state, core::point>::
                           caller<get_state<XButtonEvent>,
                                  get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonPress, Button3, 0>>             right_btn_down_event;

    typedef event_handler<ButtonRelease, ButtonReleaseMask,
                           Params<os::key_state, core::point>::
                           caller<get_state<XButtonEvent>,
                                  get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonRelease, Button3, Button3Mask>> right_btn_up_event;

    typedef event_handler<ButtonPress, ButtonPressMask,
                           Params<os::key_state, core::point>::
                           caller<get_state<XButtonEvent>,
                                  get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonPress, Button2, 0>>             middle_btn_down_event;

    typedef event_handler<ButtonRelease, ButtonReleaseMask,
                           Params<os::key_state, core::point>::
                           caller<get_state<XButtonEvent>,
                                  get_param<core::point, XButtonEvent>>,
                           0,
                           event_button_match<ButtonRelease, Button2, Button3Mask>> middle_btn_up_event;

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

    template<os::event_id B> std::map<Window, Time> double_click_matcher<B>::s_last_up;

    typedef event_handler<ButtonRelease, ButtonReleaseMask,
                           Params<os::key_state, core::point>::
                           caller<get_state<XButtonEvent>,
                                  get_param<core::point, XButtonEvent>>,
                           0,
                           double_click_matcher<Button1>>                           left_btn_dblclk_event;
    typedef event_handler<ButtonRelease, ButtonReleaseMask,
                           Params<os::key_state, core::point>::
                           caller<get_state<XButtonEvent>,
                                  get_param<core::point, XButtonEvent>>,
                           0,
                           double_click_matcher<Button3>>                           right_btn_dblclk_event;
    typedef event_handler<ButtonRelease, ButtonReleaseMask,
                           Params<os::key_state, core::point>::
                           caller<get_state<XButtonEvent>,
                                  get_param<core::point, XButtonEvent>>,
                           0,
                           double_click_matcher<Button2>>                           middle_btn_dblclk_event;

    template<int D, int U>
    inline core::point::type get_wheel_delta (const core::event& e) {
      switch (e.xbutton.button) {
        case D: return 1.0F;
        case U: return -1.0F;
      }
      return 0;
    }

    template <int D, int U>
    struct wheel_button_match {
      bool operator() (const core::event& e) {
        return (e.type == ButtonRelease) && ((e.xbutton.button == D) || (e.xbutton.button == U));
      }
    };

    typedef event_handler<ButtonRelease, ButtonReleaseMask,
                           Params<core::point::type, core::point>::
                           caller<get_wheel_delta<6, 7>,
                                  get_param<core::point, XButtonEvent>>,
                           0,
                           wheel_button_match<6, 7>>                                wheel_x_event;
    typedef event_handler<ButtonRelease, ButtonReleaseMask,
                           Params<core::point::type,  core::point>::
                           caller<get_wheel_delta<Button4, Button5>,
                                  get_param<core::point, XButtonEvent>>,
                           0,
                           wheel_button_match<Button4, Button5>>                    wheel_y_event;

    typedef event_handler<MapNotify, StructureNotifyMask>                           show_event;
    typedef event_handler<UnmapNotify, StructureNotifyMask>                         hide_event;

    typedef event_handler<FocusIn, FocusChangeMask,
                           Params<window*>::
                           caller<get_window<XFocusChangeEvent>>>                   set_focus_event;
    typedef event_handler<FocusOut, FocusChangeMask,
                           Params<window*>::
                           caller<get_window<XFocusChangeEvent>>>                   lost_focus_event;

    typedef event_handler<EnterNotify, EnterWindowMask>                             mouse_enter_event;
    typedef event_handler<LeaveNotify, LeaveWindowMask>                             mouse_leave_event;

    typedef event_handler<ConfigureNotify, StructureNotifyMask,
                           Params<core::point>::
                           caller<get_param<core::point, XConfigureEvent>>,
                           0,
                           move_size_matcher<core::point,
                                             ConfigureNotify,
                                             XConfigureEvent>>                      move_event;
    typedef event_handler<ConfigureNotify, StructureNotifyMask,
                           Params<core::size>::
                           caller<get_param<core::size, XConfigureEvent>>,
                           0,
                           move_size_matcher<core::size,
                                             ConfigureNotify,
                                             XConfigureEvent>>                      size_event;
    typedef event_handler<ConfigureNotify, StructureNotifyMask,
                           Params<core::rectangle>::
                           caller<get_param<core::rectangle, XConfigureEvent>>,
                           0,
                           move_size_matcher<core::rectangle,
                                             ConfigureNotify,
                                             XConfigureEvent>>                      place_event;

    typedef event_handler<ConfigureRequest, SubstructureRedirectMask,
                           Params<core::point>::
                           caller<get_param<core::point, XConfigureRequestEvent>>,
                           0,
                           move_size_matcher<core::point,
                                             ConfigureRequest,
                                             XConfigureRequestEvent>>               moving_event;

    typedef event_handler<ConfigureRequest, SubstructureRedirectMask,
                           Params<core::size>::
                           caller<get_param<core::size, XConfigureRequestEvent>>,
                           0,
                           move_size_matcher<core::size,
                                             ConfigureRequest,
                                             XConfigureRequestEvent>>               sizing_event;
    typedef event_handler<ConfigureRequest, SubstructureRedirectMask,
                           Params<core::rectangle>::
                           caller<get_param<core::rectangle, XConfigureRequestEvent>>,
                           0,
                           move_size_matcher<core::rectangle,
                                             ConfigureRequest,
                                             XConfigureRequestEvent>>               placing_event;

    // --------------------------------------------------------------------------
    typedef event_handler<Expose, ExposureMask,
                          Params<os::graphics>::
                          caller<get_param<0, os::graphics>>>                       os_paint_event;

    // --------------------------------------------------------------------------
    void send_client_message (window* win, Atom message,
                              long l1 = 0, long l2 = 0, long l3 = 0, long l4 = 0, long l5 = 0);
    void send_client_message (window* win, Atom message, window* w, const core::rectangle& r);

    // --------------------------------------------------------------------------
#endif // X11

  } // gui

} // gui
