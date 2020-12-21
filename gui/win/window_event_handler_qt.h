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

#ifndef GUIPP_QT
# error Include window_event_handler.h instead window_event_handler_qt.h!
#endif // GUIPP_QT

// --------------------------------------------------------------------------
//
// Common includes
//
#include <QtGui/qevent.h>

namespace gui {

  namespace win {

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

  } // namespace win

} // namespace gui
