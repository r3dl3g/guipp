/**
 * @copyright (c) 2026-2026 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     browser specific window events
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

#ifndef GUIPP_JS
# error Include window_event_handler.h instead window_event_handler_js.h!
#endif //GUIPP_WIN

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    template<int I, typename T>
    struct get {
      T param (const core::event& e);
    };

    template<typename T>
    struct get<0, T> {
      static T param (const core::event& e) {
#ifndef NDEBUG
        try {
          return std::get<T>(e.param_0);
        } catch (std::exception& ex) {
          logging::fatal() << "exception " << ex << " when access param 0 of type " << typeid(T).name();
          throw ex;
        }
#else
        return std::get<T>(e.param_0);
#endif
      }
    };

    template<typename T>
    struct get<1, T> {
      static T param (const core::event& e) {
#ifndef NDEBUG
        try {
          return std::get<T>(e.param_1);
        } catch (std::exception& ex) {
          logging::fatal() << "exception " << ex << " when access param 1 of type " << typeid(T).name();
          throw ex;
        }
#else
        return std::get<T>(e.param_1);
#endif
      }
    };

    // --------------------------------------------------------------------------
    template<os::key_symbol sym, os::key_state state>
    bool key_down_matcher (const core::event& e) {
      return (e.type == gui::os::js::event_type::KeyDown)
        && (std::get<os::key_state>(e.param_0) == state)
        && (std::get<os::key_symbol>(e.param_1) == sym);
    }

    template<os::key_symbol sym, os::key_state state>
    bool key_up_matcher (const core::event& e) {
      return (e.type == gui::os::js::event_type::KeyUp)
        && (std::get<os::key_state>(e.param_0) == state)
        && (std::get<os::key_symbol>(e.param_1) == sym);
    }


    // --------------------------------------------------------------------------
    using create_event = core::event_handler<gui::os::js::event_type::Create>;

    using close_event = core::event_handler<gui::os::js::event_type::Close>;

    using destroy_event = core::event_handler<gui::os::js::event_type::Destroy>;

    using any_key_down_event = core::event_handler<gui::os::js::event_type::KeyDown, gui::os::js::event_type::None,
                                                   core::params<os::key_state, os::key_symbol, std::string>::
                                                   getter<get_key_state, get_key_symbol, get_key_chars>>;

    using any_key_up_event = core::event_handler<gui::os::js::event_type::KeyUp, gui::os::js::event_type::None,
                                                 core::params<os::key_state, os::key_symbol>::
                                                 getter<get_key_state, get_key_symbol>>;

    template<os::key_symbol symbol, os::key_state state>
    using key_down_event = core::event_handler<gui::os::js::event_type::KeyDown, gui::os::js::event_type::None,
                                               core::params<>::getter<>, 0,
                                               event::functor<key_down_matcher<symbol, state>>>;

    template<os::key_symbol symbol, os::key_state state>
    using key_up_event = core::event_handler<gui::os::js::event_type::KeyUp, gui::os::js::event_type::None,
                                             core::params<>::getter<>, 0,
                                             event::functor<key_up_matcher<symbol, state>>>;

    using set_focus_event = core::event_handler<gui::os::js::event_type::SetFocus>;
    using lost_focus_event = core::event_handler<gui::os::js::event_type::KillFocus>;

    using move_event = core::event_handler<gui::os::js::event_type::Move, gui::os::js::event_type::None,
                                           core::params<core::point>::
                                           getter<get<0, core::point>::param>>;

    using size_event = core::event_handler<gui::os::js::event_type::Size, gui::os::js::event_type::None,
                                           core::params<core::size>::
                                           getter<get<0, core::size>::param>>;

    using left_btn_down_event = core::event_handler<gui::os::js::event_type::LButtonDown, gui::os::js::event_type::None,
                                                    core::params<os::key_state, core::native_point>::
                                                    getter<get<0, os::key_state>::param,
                                                           get<1, core::native_point>::param>>;
    using left_btn_up_event = core::event_handler<gui::os::js::event_type::LButtonUp, gui::os::js::event_type::None,
                                                  core::params<os::key_state, core::native_point>::
                                                  getter<get<0, os::key_state>::param,
                                                         get<1, core::native_point>::param>>;
    using left_btn_dblclk_event = core::event_handler<gui::os::js::event_type::LButtonDblClk, gui::os::js::event_type::None,
                                                      core::params<os::key_state, core::native_point>::
                                                      getter<get<0, os::key_state>::param,
                                                             get<1, core::native_point>::param>>;

    using right_btn_down_event = core::event_handler<gui::os::js::event_type::RButtonDown, gui::os::js::event_type::None,
                                                     core::params<os::key_state, core::native_point>::
                                                     getter<get<0, os::key_state>::param,
                                                            get<1, core::native_point>::param>>;
    using right_btn_up_event = core::event_handler<gui::os::js::event_type::RButtonUp, gui::os::js::event_type::None,
                                                   core::params<os::key_state, core::native_point>::
                                                   getter<get<0, os::key_state>::param,
                                                          get<1, core::native_point>::param>>;
    using right_btn_dblclk_event = core::event_handler<gui::os::js::event_type::RButtonDblClk, gui::os::js::event_type::None,
                                                       core::params<os::key_state, core::native_point>::
                                                       getter<get<0, os::key_state>::param,
                                                              get<1, core::native_point>::param>>;

    using middle_btn_down_event = core::event_handler<gui::os::js::event_type::MButtonDown, gui::os::js::event_type::None,
                                                      core::params<os::key_state, core::native_point>::
                                                      getter<get<0, os::key_state>::param,
                                                             get<1, core::native_point>::param>>;
    using middle_btn_up_event = core::event_handler<gui::os::js::event_type::MButtonUp, gui::os::js::event_type::None,
                                                    core::params<os::key_state, core::native_point>::
                                                    getter<get<0, os::key_state>::param,
                                                           get<1, core::native_point>::param>>;
    using middle_btn_dblclk_event = core::event_handler<gui::os::js::event_type::MButtonDblClk, gui::os::js::event_type::None,
                                                        core::params<os::key_state, core::native_point>::
                                                        getter<get<0, os::key_state>::param,
                                                               get<1, core::native_point>::param>>;

    using btn_down_event = left_btn_down_event;
    using btn_up_event = left_btn_up_event;

    using mouse_move_event = core::event_handler<gui::os::js::event_type::MouseMove, gui::os::js::event_type::None,
                                                 core::params<os::key_state, core::native_point>::
                                                 getter<get<0, os::key_state>::param,
                                                        get<1, core::native_point>::param>>;

    using mouse_enter_event = core::event_handler<gui::os::js::event_type::MouseEnter>;
    using mouse_leave_event = core::event_handler<gui::os::js::event_type::MouseLeave>;

    using wheel_x_event = core::event_handler<gui::os::js::event_type::WheelH, gui::os::js::event_type::None,
                                              core::params<core::native_point::type, core::native_point>::
                                              getter<get<0, int>::param,
                                                     get<1, core::native_point>::param>>;
    using wheel_y_event = core::event_handler<gui::os::js::event_type::WheelV, gui::os::js::event_type::None,
                                              core::params<core::native_point::type, core::native_point>::
                                              getter<get<0, int>::param,
                                                     get<1, core::native_point>::param>>;

    using show_event = core::event_handler<gui::os::js::event_type::Show>;
    using hide_event = core::event_handler<gui::os::js::event_type::Hide>;

    using layout_event = core::event_handler<gui::os::js::event_type::Layout, gui::os::js::event_type::None,
                                             core::params<core::rectangle>::
                                             getter<get<0, core::rectangle>::param>>;

    using paint_event = core::event_handler<gui::os::js::event_type::Paint, gui::os::js::event_type::None,
                                            core::params<core::context*, const core::native_rect*>::
                                            getter<get_context, get<1, const core::native_rect*>::param>>;

    // --------------------------------------------------------------------------

  } // namespace win

} // namespace gui
