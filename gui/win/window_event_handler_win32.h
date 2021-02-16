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

#ifndef GUIPP_WIN
# error Include window_event_handler.h instead window_event_handler_win32.h!
#endif //GUIPP_WIN

namespace gui {

  namespace win {

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
    template<> GUIPP_WIN_EXPORT core::native_point get_param<1>(const core::event& e);
    template<> GUIPP_WIN_EXPORT core::native_size get_param<1>(const core::event& e);
    template<> GUIPP_WIN_EXPORT core::native_rect get_param<1>(const core::event& e);
    template<> GUIPP_WIN_EXPORT core::native_rect* get_param<1>(const core::event& e);
    // --------------------------------------------------------------------------
    template<typename T>
    core::native_rect get_rect (const core::event& e) {
      T& p = *reinterpret_cast<T*>(e.lParam);
      return core::native_rect(p.x, p.y, p.cx, p.cy);
    }

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT window* get_window_from_cs (const core::event& e);
    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT window* get_window_from_id (const core::event& e);
    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT unsigned int get_flags_from_wp (const core::event& e);
    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT core::native_point::type get_wheel_delta (const core::event& e);
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
    struct GUIPP_WIN_EXPORT pos_changing_getter {
      typedef std::function<void (core::rectangle&)> function;

      pos_changing_getter (const function cb)
        : callback(cb)
      {}

      template<class T>
      pos_changing_getter (T* win, void(T::*changingfn)(core::rectangle &))
        : callback(win, changingfn)
      {}

      void operator() (const core::event& e);

    private:
      function callback;
    };
    // --------------------------------------------------------------------------
    struct GUIPP_WIN_EXPORT minmax_getter {
      typedef std::function<void (const core::size&, const core::point&, core::size&, core::size&)> function;

      minmax_getter (const function cb)
        : callback(cb)
      {}

      template<class T>
      minmax_getter (T* win, void(T::*fn)(const core::size&, const core::point&, core::size&, core::size&))
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
    using create_event = core::event_handler<core::WM_CREATE_WINDOW>;

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
                                                    core::params<os::key_state, core::native_point>::
                                                    getter<get_param<0, os::key_state>,
                                                           get_param<1, core::native_point>>>;
    using left_btn_up_event = core::event_handler<WM_LBUTTONUP, 0,
                                                  core::params<os::key_state, core::native_point>::
                                                  getter<get_param<0, os::key_state>,
                                                         get_param<1, core::native_point>>>;
    using left_btn_dblclk_event = core::event_handler<WM_LBUTTONDBLCLK, 0,
                                                      core::params<os::key_state, core::native_point>::
                                                      getter<get_param<0, os::key_state>,
                                                             get_param<1, core::native_point>>>;

    using right_btn_down_event = core::event_handler<WM_RBUTTONDOWN, 0,
                                                     core::params<os::key_state, core::native_point>::
                                                     getter<get_param<0, os::key_state>,
                                                            get_param<1, core::native_point>>>;
    using right_btn_up_event = core::event_handler<WM_RBUTTONUP, 0,
                                                   core::params<os::key_state, core::native_point>::
                                                   getter<get_param<0, os::key_state>,
                                                          get_param<1, core::native_point>>>;
    using right_btn_dblclk_event = core::event_handler<WM_RBUTTONDBLCLK, 0,
                                                       core::params<os::key_state, core::native_point>::
                                                       getter<get_param<0, os::key_state>,
                                                              get_param<1, core::native_point>>>;

    using middle_btn_down_event = core::event_handler<WM_MBUTTONDOWN, 0,
                                                      core::params<os::key_state, core::native_point>::
                                                      getter<get_param<0, os::key_state>,
                                                             get_param<1, core::native_point>>>;
    using middle_btn_up_event = core::event_handler<WM_MBUTTONUP, 0,
                                                    core::params<os::key_state, core::native_point>::
                                                    getter<get_param<0, os::key_state>,
                                                           get_param<1, core::native_point>>>;
    using middle_btn_dblclk_event = core::event_handler<WM_MBUTTONDBLCLK, 0,
                                                        core::params<os::key_state, core::native_point>::
                                                        getter<get_param<0, os::key_state>,
                                                               get_param<1, core::native_point>>>;

    using btn_down_event = core::event_handler<WM_LBUTTONDOWN, 0,
                                               core::params<os::key_state, core::native_point>::
                                               getter<get_param<0, os::key_state>,
                                                      get_param<1, core::native_point>>,
                                               0,
                                               event::functor<any_button_matcher<true>>>;
    using btn_up_event = core::event_handler<WM_LBUTTONUP, 0,
                                             core::params<os::key_state, core::native_point>::
                                             getter<get_param<0, os::key_state>,
                                                    get_param<1, core::native_point>>,
                                             0,
                                             event::functor<any_button_matcher<false>>>;

    using mouse_move_event = core::event_handler<WM_MOUSEMOVE, 0,
                                                 core::params<os::key_state, core::native_point>::
                                                 getter<get_param<0, os::key_state>,
                                                        get_param<1, core::native_point>>>;

    using mouse_move_abs_event = core::event_handler<WM_MOUSEMOVE, 0,
                                                     core::params<os::key_state, core::native_point>::
                                                     getter<get_param<0, os::key_state>,
                                                            get_root_mouse_pos>>;

    using mouse_enter_event = core::event_handler<WM_MOUSEMOVE, 0,
                                                  core::params<>::getter<>,
                                                  0,
                                                  event::functor<mouse_enter_matcher>>;
    using mouse_leave_event = core::event_handler<WM_MOUSELEAVE>;

    using wheel_x_event = core::event_handler<WM_MOUSEHWHEEL, 0,
                                              core::params<core::native_point::type, core::native_point>::
                                              getter<get_wheel_delta,
                                                     get_param<1, core::native_point>>>;
    using wheel_y_event = core::event_handler<WM_MOUSEWHEEL, 0,
                                              core::params<core::native_point::type, core::native_point>::
                                              getter<get_wheel_delta,
                                                     get_param<1, core::native_point>>>;

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

    using paint_event = core::event_handler<core::WM_PAINT_WINDOW, 0,
                                            core::params<core::context*, core::native_rect*>::
                                            getter<get_context, get_param<1, core::native_rect*>>>;

    using expose_event = core::event_handler<WM_PAINT>;
    // --------------------------------------------------------------------------

  } // namespace win

} // namespace gui
