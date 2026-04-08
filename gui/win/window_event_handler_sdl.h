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
 * @brief     X11 specific window events
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

#ifndef GUIPP_SDL
# error Include window_event_handler.h instead window_event_handler_sdl.h!
#endif // GUIPP_SDL

namespace gui {

  namespace win {

    class window;
    class overlapped_window;

    // --------------------------------------------------------------------------
    template<typename T>
    const T& event_type_cast (const core::event& e);
    // --------------------------------------------------------------------------
    template<>
    inline const SDL_WindowEvent& event_type_cast<SDL_WindowEvent>(const core::event& e) {
      return e.window;
    }

    template<>
    inline const SDL_KeyboardEvent& event_type_cast<SDL_KeyboardEvent>(const core::event& e) {
      return e.key;
    }

    template<>
    inline const SDL_MouseMotionEvent& event_type_cast<SDL_MouseMotionEvent>(const core::event& e) {
      return e.motion;
    }

    template<>
    inline const SDL_TextInputEvent& event_type_cast<SDL_TextInputEvent>(const core::event& e) {
      return e.text;
    }

    template<>
    inline const SDL_MouseButtonEvent& event_type_cast<SDL_MouseButtonEvent>(const core::event& e) {
      return e.button;
    }

    template<>
    inline const SDL_MouseWheelEvent& event_type_cast<SDL_MouseWheelEvent>(const core::event& e) {
      return e.wheel;
    }

    template<>
    inline const SDL_DisplayEvent& event_type_cast<SDL_DisplayEvent>(const core::event& e) {
      return e.display;
    }

    template<>
    inline const SDL_QuitEvent& event_type_cast<SDL_QuitEvent>(const core::event& e) {
      return e.quit;
    }

    template<>
    inline const SDL_UserEvent& event_type_cast<SDL_UserEvent>(const core::event& e) {
      return e.user;
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
        return core::size(core::global::scale_from_native<core::size::type>(event_type_cast<C>(e).data1),
                          core::global::scale_from_native<core::size::type>(event_type_cast<C>(e).data2));
      }
    };
    // --------------------------------------------------------------------------
    template<typename C>
    struct get<core::point, C> {
      static core::point param (const core::event& e) {
        return core::point(core::global::scale_from_native<core::point::type>(event_type_cast<C>(e).data1),
                           core::global::scale_from_native<core::point::type>(event_type_cast<C>(e).data2));
      }
    };
    // --------------------------------------------------------------------------
    template<typename C>
    struct get<core::native_size, C> {
      static core::native_size param (const core::event& e) {
        return core::native_size(static_cast<core::native_size::type>(event_type_cast<C>(e).data1),
                                 static_cast<core::native_size::type>(event_type_cast<C>(e).data2));
      }
    };
    // --------------------------------------------------------------------------
    template<typename C>
    struct get<core::native_point, C> {
      static core::native_point param (const core::event& e) {
        return core::native_point(static_cast<core::native_point::type>(event_type_cast<C>(e).x),
                                  static_cast<core::native_point::type>(event_type_cast<C>(e).y));
      }
    };
    // --------------------------------------------------------------------------
    template<os::event_id id, os::event_id btn>
    bool event_button_matcher (const core::event& e) {
      return (e.type == id) && (event_type_cast<SDL_MouseButtonEvent>(e).button == btn);
    }

    // --------------------------------------------------------------------------
    template<int M, os::event_id E, typename C>
    bool mode_matcher (const core::event& e) {
      return (e.type == E) && (event_type_cast<C>(e).mode == M);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline os::key_state get_event_state (const core::event& e) {
      return static_cast<os::key_state>(event_type_cast<T>(e).state & ~0x10);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline unsigned int get_button (const core::event& e) {
      return event_type_cast<T>(e).button;
    }

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT const core::native_rect* get_paint_rect (const core::event& e);
    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT core::rectangle get_client_data_rect (const core::event& e);
    GUIPP_WIN_EXPORT core::size get_client_data_size (const core::event& e);
    // --------------------------------------------------------------------------
    template<typename T>
    inline window* get_window (const core::event& e) {
      return native::get_window(event_type_cast<T>(e).window);
    }

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT window* get_current_focus_window (const core::event&);
    // --------------------------------------------------------------------------
//    GUIPP_WIN_EXPORT os::graphics get_graphics (const core::event&);
    // --------------------------------------------------------------------------
//    GUIPP_WIN_EXPORT os::window get_draw_window (const core::event&);
    // --------------------------------------------------------------------------
    template<int M>
    inline bool user_message_matcher (const core::event& e) {
      return (e.type == SDL_USEREVENT) && (e.user.code == M);
    }

    template<int M>
    inline bool window_message_matcher (const core::event& e) {
      return (e.type == SDL_WINDOWEVENT) && (e.window.event == M);
    }

    // --------------------------------------------------------------------------
    template<os::event_id E, os::key_symbol symbol, os::key_state state>
    inline bool key_symbol_matcher (const core::event& e) {
      return (e.type == E) &&
             (get_key_symbol(e) == symbol) &&
             core::bit_mask<os::key_state, state>::is_set(get_key_state(e));
    }

    // --------------------------------------------------------------------------

    template<int B>
    struct double_click_matcher {
      inline bool operator() (const core::event& e) {
        if ((e.type == SDL_MOUSEBUTTONUP) && (e.button.button == B)) {
          Uint32 diff = e.button.timestamp - last_up;
          last_up = e.button.timestamp;
          return (diff < 300);
        }
        return false;
      }

      Uint32 last_up;
    };

    // --------------------------------------------------------------------------
    inline core::native_point::type get_wheel_delta_x (const core::event& e) {
      return e.wheel.x > 0 ? 1 : -1;
    }

    inline core::native_point::type get_wheel_delta_y (const core::event& e) {
      return e.wheel.y > 0 ? 1 : -1;
    }
    // --------------------------------------------------------------------------
    inline bool wheel_matcher_x (const core::event& e) {
      return (e.type == SDL_MOUSEWHEEL) && (e.wheel.x != 0);
    }

    inline bool wheel_matcher_y (const core::event& e) {
      return (e.type == SDL_MOUSEWHEEL) && (e.wheel.y != 0);
    }

    // --------------------------------------------------------------------------
    namespace detail {
      template<int E>
      using window_event = core::event_handler<SDL_WINDOWEVENT, 0, 
                                               core::params<>::getter<>, 0, 
                                               event::functor<window_message_matcher<E>>>;

      template<int E, int B>
      using mouse_event = core::event_handler<E, 0,
                                              core::params<os::key_state, core::native_point>::
                                              getter<get_event_state<SDL_MouseButtonEvent>,
                                                      get<core::native_point, SDL_MouseButtonEvent>::param>,
                                              0,
                                              event::functor<event_button_matcher<E, B>>>;

      template<int B>
      using mouse_dblclk_event = core::event_handler<SDL_MOUSEBUTTONUP, 0,
                                                     core::params<os::key_state, core::native_point>::
                                                     getter<get_event_state<SDL_MouseButtonEvent>,
                                                            get<core::native_point, SDL_MouseButtonEvent>::param>,
                                                     0,
                                                     double_click_matcher<B>>;

    template<int B, typename G = core::params<>::getter<> >
    using user_event = core::event_handler<SDL_USEREVENT, 0, G, 0,
                                           event::functor<user_message_matcher<B>>>;
    }

    // --------------------------------------------------------------------------
    using create_event = detail::user_event<core::WM_CREATE_WINDOW>;

    using close_event = detail::window_event<SDL_WINDOWEVENT_CLOSE>;

    using destroy_event = core::event_handler<SDL_APP_TERMINATING>;

    using any_key_down_event = core::event_handler<SDL_KEYDOWN, 0,
                                                   core::params<os::key_state, os::key_symbol, std::string>::
                                                   getter<get_key_state, get_key_symbol, get_key_chars>>;

    using any_key_up_event = core::event_handler<SDL_KEYUP, 0,
                                                 core::params<os::key_state, os::key_symbol>::
                                                 getter<get_key_state, get_key_symbol>>;

    template<os::key_symbol symbol, os::key_state state>
    using key_down_event = core::event_handler<SDL_KEYDOWN, 0, core::params<>::getter<>, 0,
                                               event::functor<key_symbol_matcher<SDL_KEYDOWN, symbol, state>>>;


    template<os::key_symbol symbol, os::key_state state>
    using key_up_event = core::event_handler<SDL_KEYUP, 0, core::params<>::getter<>, 0,
                                             event::functor<key_symbol_matcher<SDL_KEYUP, symbol, state>>>;

    using mouse_move_event = core::event_handler<SDL_MOUSEMOTION, 0,
                                                 core::params<os::key_state, core::native_point>::
                                                 getter<get_event_state<SDL_MouseMotionEvent>,
                                                        get<core::native_point, SDL_MouseMotionEvent>::param>>;

    using left_btn_down_event = detail::mouse_event<SDL_MOUSEBUTTONDOWN, 1>;

    using left_btn_up_event = detail::mouse_event<SDL_MOUSEBUTTONUP, 1>;

    using right_btn_down_event = detail::mouse_event<SDL_MOUSEBUTTONDOWN, 3>;

    using right_btn_up_event = detail::mouse_event<SDL_MOUSEBUTTONUP, 3>;

    using middle_btn_down_event = detail::mouse_event<SDL_MOUSEBUTTONDOWN, 2>;

    using middle_btn_up_event = detail::mouse_event<SDL_MOUSEBUTTONUP, 2>;

    using btn_down_event = core::event_handler<SDL_MOUSEBUTTONDOWN, 0,
                                               core::params<os::key_state, core::native_point>::
                                               getter<get_event_state<SDL_MouseButtonEvent>,
                                                      get<core::native_point, SDL_MouseButtonEvent>::param>>;

    using btn_up_event = core::event_handler<SDL_MOUSEBUTTONUP, 0,
                                             core::params<os::key_state, core::native_point>::
                                             getter<get_event_state<SDL_MouseButtonEvent>,
                                                    get<core::native_point, SDL_MouseButtonEvent>::param>>;

    using left_btn_dblclk_event = detail::mouse_dblclk_event<1>;

    using right_btn_dblclk_event = detail::mouse_dblclk_event<2>;
    using middle_btn_dblclk_event = detail::mouse_dblclk_event<3>;

    using wheel_x_event = core::event_handler<SDL_MOUSEWHEEL, 0,
                                              core::params<core::native_point::type, core::native_point>::
                                              getter<get_wheel_delta_x,
                                              get<core::native_point, SDL_MouseButtonEvent>::param>,
                                              0, event::functor<wheel_matcher_x>>;

    using wheel_y_event = core::event_handler<SDL_MOUSEWHEEL, 0,
                                              core::params<core::native_point::type, core::native_point>::
                                              getter<get_wheel_delta_y,
                                                     get<core::native_point, SDL_MouseButtonEvent>::param>,
                                              0, event::functor<wheel_matcher_y>>;

    template<orientation_t O>
    struct wheel_event {};

    template<>
    struct wheel_event<orientation_t::horizontal> : public wheel_x_event {};

    template<>
    struct wheel_event<orientation_t::vertical> : public wheel_y_event {};

    using show_event = detail::window_event<SDL_WINDOWEVENT_SHOWN>;
    using hide_event = detail::window_event<SDL_WINDOWEVENT_HIDDEN>;

    using set_focus_event = detail::window_event<SDL_WINDOWEVENT_FOCUS_GAINED>;
    using lost_focus_event = detail::window_event<SDL_WINDOWEVENT_FOCUS_LOST>;

    using mouse_enter_event = detail::window_event<SDL_WINDOWEVENT_ENTER>;
    using mouse_leave_event = detail::window_event<SDL_WINDOWEVENT_LEAVE>;

    using move_event = core::event_handler<SDL_WINDOWEVENT, 0,
                                           core::params<core::point>::getter<get<core::point, SDL_WindowEvent>::param>,
                                           0,
                                           event::functor<window_message_matcher<SDL_WINDOWEVENT_MOVED>>>;
    using size_event = core::event_handler<SDL_WINDOWEVENT, 0,
                                           core::params<core::size>::getter<get<core::size, SDL_WindowEvent>::param>,
                                           0,
                                           event::functor<window_message_matcher<SDL_WINDOWEVENT_RESIZED>>>;

    using layout_event = detail::user_event<core::WM_LAYOUT_WINDOW,
                                            core::params<core::rectangle>::getter<get_client_data_rect>>;

    using paint_event = detail::user_event<core::WM_PAINT_WINDOW,
                                           core::params<core::context*, const core::native_rect*>::
                                           getter<get_context, get_paint_rect> >;

    // --------------------------------------------------------------------------

  } // namespace win

} // namespace gui
