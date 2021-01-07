/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic window
 *
 * @file
 */


// --------------------------------------------------------------------------
//
// Common includes
//
#include <gui/win/window_event_proc.h>
#include <gui/win/receiver.h>


namespace gui {

  namespace win {

    void receiver::register_event_handler (event_handler_function&& f,
                                           os::event_id mask) {
      events.register_event_handler(std::move(f));
#ifdef GUIPP_X11
      x11::prepare_win_for_event(this, mask);
#else
      (void)mask;
#endif // GUIPP_X11
    }

    void receiver::on_create (create_event::function&& f) {
      on<create_event>(std::move(f));
    }

    void receiver::on_close (close_event::function&& f) {
      on<close_event>(std::move(f));
    }

    void receiver::on_destroy (destroy_event::function&& f) {
      on<destroy_event>(std::move(f));
    }

    void receiver::on_any_key_down (any_key_down_event::function&& f) {
      on<any_key_down_event>(std::move(f));
    }

    void receiver::on_any_key_up (any_key_up_event::function&& f) {
      on<any_key_up_event>(std::move(f));
    }

    void receiver::on_mouse_move (mouse_move_event::function&& f) {
      on<mouse_move_event>(std::move(f));
    }

    void receiver::on_mouse_move_abs (mouse_move_abs_event::function&& f) {
      on<mouse_move_abs_event>(std::move(f));
    }

    void receiver::on_left_btn_down (left_btn_down_event::function&& f) {
      on<left_btn_down_event>(std::move(f));
    }

    void receiver::on_left_btn_up (left_btn_up_event::function&& f) {
      on<left_btn_up_event>(std::move(f));
    }

    void receiver::on_right_btn_down (right_btn_down_event::function&& f) {
      on<right_btn_down_event>(std::move(f));
    }

    void receiver::on_right_btn_up (right_btn_up_event::function&& f) {
      on<right_btn_up_event>(std::move(f));
    }

    void receiver::on_middle_btn_down (middle_btn_down_event::function&& f) {
      on<middle_btn_down_event>(std::move(f));
    }

    void receiver::on_middle_btn_up (middle_btn_up_event::function&& f) {
      on<middle_btn_up_event>(std::move(f));
    }

    void receiver::on_btn_down (btn_down_event::function&& f) {
      on<btn_down_event>(std::move(f));
    }

    void receiver::on_btn_up (btn_up_event::function&& f) {
      on<btn_up_event>(std::move(f));
    }

    void receiver::on_left_btn_dblclk (left_btn_dblclk_event::function&& f) {
      on<left_btn_dblclk_event>(std::move(f));
    }

    void receiver::on_right_btn_dblclk (right_btn_dblclk_event::function&& f) {
      on<right_btn_dblclk_event>(std::move(f));
    }

    void receiver::on_middle_btn_dblclk (middle_btn_dblclk_event::function&& f) {
      on<middle_btn_dblclk_event>(std::move(f));
    }

    void receiver::on_wheel_x (wheel_x_event::function&& f) {
      on<wheel_x_event>(std::move(f));
    }

    void receiver::on_wheel_y (wheel_y_event::function&& f) {
      on<wheel_y_event>(std::move(f));
    }

    void receiver::on_show (show_event::function&& f) {
      on<show_event>(std::move(f));
    }

    void receiver::on_hide (hide_event::function&& f) {
      on<hide_event>(std::move(f));
    }

    void receiver::on_set_focus (set_focus_event::function&& f) {
      on<set_focus_event>(std::move(f));
    }

    void receiver::on_lost_focus (lost_focus_event::function&& f) {
      on<lost_focus_event>(std::move(f));
    }

    void receiver::on_mouse_enter (mouse_enter_event::function&& f) {
      on<mouse_enter_event>(std::move(f));
    }

    void receiver::on_mouse_leave (mouse_leave_event::function&& f) {
      on<mouse_leave_event>(std::move(f));
    }

    void receiver::on_move (move_event::function&& f) {
      on<move_event>(std::move(f));
    }

    void receiver::on_size (size_event::function&& f) {
      on<size_event>(std::move(f));
    }

    void receiver::on_layout (layout_event::function&& f) {
      on<layout_event>(std::move(f));
    }

    void receiver::on_paint (paint_event::function&& f) {
      on<paint_event>(std::move(f));
    }

  } // namespace win

} // namespace gui
