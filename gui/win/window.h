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

    // --------------------------------------------------------------------------
    namespace detail {
      void set_id (window* w, os::window id);
    }

    // --------------------------------------------------------------------------
    class window {
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
      container* get_root () const;
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

      void redraw_now () const;
      void redraw_later () const;

      core::size size () const;
      core::point position () const;
      core::rectangle absolute_place () const;
      core::rectangle place () const;
      core::point absolute_position () const;
      core::size client_size () const;
      core::rectangle client_area () const;

      void move (const core::point&, bool repaint = true);
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

      void register_event_handler (char const name[], const event_handler_function& f, os::event_id mask);
      void register_event_handler (char const name[], event_handler_function&& f, os::event_id mask);

      void unregister_event_handler (const event_handler_function& f);

      template<typename H>
      void register_event_handler (char const name[], const H& h);

      template<typename H>
      void register_event_handler (char const name[], H&& h);

      template<typename T>
      void register_event_handler (char const name[], T * t, bool (T::*method)(const core::event &, os::event_result & result), os::event_id mask);

      bool handle_event (const core::event&, os::event_result&) const;

      void prepare_for_event (os::event_id mask);

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

      client_window ();
      client_window (const client_window& rhs);
      client_window (client_window&& rhs);

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
