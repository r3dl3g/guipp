/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <cstddef>
#include <memory>
#include <bitset>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/event_container.h>
#include <gui/core/gui_types.h>
#include <gui/win/window_state.h>
#include <gui/win/window_class.h>
#include <gui/win/window_event_handler.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    // --------------------------------------------------------------------------
    class window;
    class container;
    class overlapped_window;

    // --------------------------------------------------------------------------
    namespace detail {
      void set_id (window* w, os::window id);
    }

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT window {
    public:
      typedef core::event_container::event_handler_function event_handler_function;

      window ();
      ~window ();

      os::window get_id () const;

      operator os::drawable() const;

      void destroy ();
      void close ();

      void set_parent (const container& parent);
      container* get_parent () const;
      container* get_root_window () const;
      overlapped_window* get_overlapped_window () const;
      bool is_child_of (const container& parent) const;

      bool is_valid () const;
      bool is_child () const;
      bool is_popup () const;
      bool is_toplevel () const;
      bool has_border () const;

      const window_state get_state () const;
      window_state get_state ();

      bool is_visible () const;
      void set_visible (bool s = true);

      bool is_enabled () const;
      void enable (bool on = true);
      void disable ();

      bool is_focus_accepting () const;
      bool can_accept_focus () const;
      bool has_focus () const;

      bool is_redraw_disabled () const;
      void disable_redraw (bool on = true);

      void shift_focus (bool backward = false) const;

      void take_focus () const;

      void to_front ();
      void to_back ();

      void invalidate () const;
      void redraw () const;

      core::size size () const;
      core::point position () const;
      core::rectangle absolute_place () const;
      core::rectangle place () const;
      core::point absolute_position () const;
      core::size client_size () const;
      core::rectangle client_area () const;

      void move (const core::point&, bool repaint = false);
      void resize (const core::size&, bool repaint = true);
      void place (const core::rectangle&, bool repaint = true);

      core::point window_to_screen (const core::point&) const;
      core::point screen_to_window (const core::point&) const;

      core::point client_to_screen (const core::point&) const;
      core::point screen_to_client (const core::point&) const;

      std::string get_class_name () const;
      const class_info& get_window_class () const;

      void set_cursor (os::cursor);

      void capture_pointer ();
      void uncapture_pointer ();

      typedef void(create_fn)(window*, core::rectangle);
      typedef void(notification_fn)();
      typedef void(mouse_fn)(os::key_state, core::point);
      typedef void(wheel_fn)(core::point::type, core::point);
      typedef void(window_fn)(window*);
      typedef void(move_fn)(core::point);
      typedef void(size_fn)(core::size);
      typedef void(place_fn)(core::rectangle);

      void on_create (std::function<create_fn>&& f);

      void on_close (std::function<notification_fn>&& f);
      void on_destroy (std::function<window_fn>&& f);

      void on_any_key_down (std::function<void(os::key_state, os::key_symbol, std::string)>&& f);
      void on_any_key_up (std::function<void(os::key_state, os::key_symbol)>&& f);

      template<os::key_symbol symbol, os::key_state state = state::none>
      void on_key_down (std::function<notification_fn>&& f);

      template<os::key_symbol symbol, os::key_state state = state::none>
      void on_key_up (std::function<notification_fn>&& f);

      void on_mouse_move (std::function<mouse_fn>&& f);
      void on_mouse_move_abs (std::function<mouse_fn>&& f);

      void on_left_btn_down (std::function<mouse_fn>&& f);
      void on_left_btn_up (std::function<mouse_fn>&& f);
      void on_right_btn_down (std::function<mouse_fn>&& f);
      void on_right_btn_up (std::function<mouse_fn>&& f);
      void on_middle_btn_down (std::function<mouse_fn>&& f);
      void on_middle_btn_up (std::function<mouse_fn>&& f);

      void on_btn_down (std::function<mouse_fn>&& f);
      void on_btn_up (std::function<mouse_fn>&& f);

      void on_left_btn_dblclk (std::function<mouse_fn>&& f);
      void on_right_btn_dblclk (std::function<mouse_fn>&& f);
      void on_middle_btn_dblclk (std::function<mouse_fn>&& f);

      template<orientation V>
      void on_wheel (std::function<wheel_fn>&& f);

      void on_wheel_x (std::function<wheel_fn>&& f);
      void on_wheel_y (std::function<wheel_fn>&& f);

      void on_show (std::function<notification_fn>&& f);
      void on_hide (std::function<notification_fn>&& f);

      void on_set_focus (std::function<window_fn>&& f);
      void on_lost_focus (std::function<window_fn>&& f);

      void on_mouse_enter (std::function<notification_fn>&& f);
      void on_mouse_leave (std::function<notification_fn>&& f);

      void on_move (std::function<move_fn>&& f);
      void on_size (std::function<size_fn>&& f);
      void on_place (std::function<place_fn>&& f);

      void on_moving (std::function<move_fn>&& f);
      void on_sizing (std::function<size_fn>&& f);
      void on_placing (std::function<place_fn>&& f);

      void on_layout (std::function<place_fn>&& f);

      void on_paint (std::function<void(os::window, os::graphics)>&& f);

      template<typename H>
      void on (typename H::function&& f);

      template<typename H>
      void on (const typename H::function& f);

      template<typename H>
      void unregister_event_handler (const typename H::function& f);

      bool handle_event (const core::event&, gui::os::event_result&) const;

    protected:
      window (const window&);
      window (window&&);

      window& operator= (const window&) = delete;
      window& operator= (window&&) = delete;

      void create (const class_info&,
                   const container&,
                   const core::rectangle& = core::rectangle::def);

      void create (const class_info&,
                   os::window parent,
                   const core::rectangle&);

      void set_accept_focus (bool a);

      void register_event_handler (event_handler_function&& f, os::event_id mask);

    private:
      friend struct window_state;
      bool get_flag (byte bit) const;
      void set_flag (byte bit, bool a);

    private:
      friend void detail::set_id (window*, os::window);

      static os::window create_window (const class_info&,
                                       const core::rectangle& r,
                                       os::window parent_id,
                                       window* data);

      void init ();

      os::window id;
      core::event_container events;
      std::bitset<sizeof(long) * 8> flags;
    };

    // --------------------------------------------------------------------------
    template<os::color background = color::very_light_gray>
    class client_window : public window {
    public:
      typedef window super;
      typedef window_class<client_window, background> clazz;

      void create (const container& parent,
                   const core::rectangle& r = core::rectangle::def);
    };

    // --------------------------------------------------------------------------
    template<class T>
    typename std::enable_if<std::is_base_of<window, T>::value, std::ostream&>::type
    operator<< (std::ostream& out, const T& t) {
      out << "[" << t.get_id() << "]" << typeid(T).name();
      return out;
    }

  } // namespace win

} // namespace gui

#include <gui/win/window.inl>
