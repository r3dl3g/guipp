/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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

#ifndef GUIPP_X11
# error Include window_event_handler.h instead window_event_handler_x11.h!
#endif // GUIPP_X11

namespace gui {

  namespace win {

    class window;
    class overlapped_window;

    // --------------------------------------------------------------------------
    namespace x11 {

      GUIPP_WIN_EXPORT void send_client_message (window* win, Atom message,
                                                 long l1 = 0, long l2 = 0, long l3 = 0, long l4 = 0, long l5 = 0);

//      GUIPP_WIN_EXPORT void send_client_message (window* win, Atom message, const window* w, const core::rectangle& r);

      GUIPP_WIN_EXPORT void prepare_win_for_event (const window& win, os::event_id mask);
      GUIPP_WIN_EXPORT void unprepare_win (const window& win);

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
    inline unsigned int get_event_state (const core::event& e) {
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
    template<>
    GUIPP_WIN_EXPORT core::point::type get_client_data<0, core::point::type>(const core::event& e);
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
                                                 getter<get_event_state<XMotionEvent>,
                                                        get<core::point, XMotionEvent>::param>>;

    using mouse_move_abs_event = core::event_handler<MotionNotify, PointerMotionMask,
                                                     core::params<os::key_state, core::point>::
                                                     getter<get_event_state<XMotionEvent>,
                                                            get_root_mouse_pos>>;

    using left_btn_down_event = core::event_handler<ButtonPress, ButtonPressMask,
                                                    core::params<os::key_state, core::point>::
                                                    getter<get_event_state<XButtonEvent>,
                                                           get<core::point, XButtonEvent>::param>,
                                                    0,
                                                    event::functor<event_button_matcher<ButtonPress, Button1>>>;

    using left_btn_up_event = core::event_handler<ButtonRelease, ButtonReleaseMask | ButtonPressMask,
                                                  core::params<os::key_state, core::point>::
                                                  getter<get_event_state<XButtonEvent>,
                                                         get<core::point, XButtonEvent>::param>,
                                                  0,
                                                  event::functor<event_button_matcher<ButtonRelease, Button1>>>;

    using right_btn_down_event = core::event_handler<ButtonPress, ButtonPressMask,
                                                     core::params<os::key_state, core::point>::
                                                     getter<get_event_state<XButtonEvent>,
                                                            get<core::point, XButtonEvent>::param>,
                                                     0,
                                                     event::functor<event_button_matcher<ButtonPress, Button3>>>;

    using right_btn_up_event = core::event_handler<ButtonRelease, ButtonReleaseMask | ButtonPressMask,
                                                   core::params<os::key_state, core::point>::
                                                   getter<get_event_state<XButtonEvent>,
                                                          get<core::point, XButtonEvent>::param>,
                                                   0,
                                                   event::functor<event_button_matcher<ButtonRelease, Button3>>>;

    using middle_btn_down_event = core::event_handler<ButtonPress, ButtonPressMask,
                                                      core::params<os::key_state, core::point>::
                                                      getter<get_event_state<XButtonEvent>,
                                                             get<core::point, XButtonEvent>::param>,
                                                      0,
                                                      event::functor<event_button_matcher<ButtonPress, Button2>>>;

    using middle_btn_up_event = core::event_handler<ButtonRelease, ButtonReleaseMask | ButtonPressMask,
                                                    core::params<os::key_state, core::point>::
                                                    getter<get_event_state<XButtonEvent>,
                                                           get<core::point, XButtonEvent>::param>,
                                                    0,
                                                    event::functor<event_button_matcher<ButtonRelease, Button2>>>;

    using btn_down_event = core::event_handler<ButtonPress, ButtonPressMask,
                                               core::params<os::key_state, core::point>::
                                               getter<get_event_state<XButtonEvent>,
                                                      get<core::point, XButtonEvent>::param>>;

    using btn_up_event = core::event_handler<ButtonRelease, ButtonReleaseMask | ButtonPressMask,
                                             core::params<os::key_state, core::point>::
                                             getter<get_event_state<XButtonEvent>,
                                                    get<core::point, XButtonEvent>::param>>;

    using left_btn_dblclk_event = core::event_handler<ButtonRelease, ButtonPressMask | ButtonReleaseMask,
                                                      core::params<os::key_state, core::point>::
                                                      getter<get_event_state<XButtonEvent>,
                                                             get<core::point, XButtonEvent>::param>,
                                                      0,
                                                      double_click_matcher<Button1>>;

    using right_btn_dblclk_event = core::event_handler<ButtonRelease, ButtonPressMask | ButtonReleaseMask,
                                                       core::params<os::key_state, core::point>::
                                                       getter<get_event_state<XButtonEvent>,
                                                              get<core::point, XButtonEvent>::param>,
                                                       0,
                                                       double_click_matcher<Button3>>;
    using middle_btn_dblclk_event = core::event_handler<ButtonRelease, ButtonPressMask | ButtonReleaseMask,
                                                        core::params<os::key_state, core::point>::
                                                        getter<get_event_state<XButtonEvent>,
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

  } // namespace win

} // namespace gui
