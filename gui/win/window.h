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
#include <gui/win/window_class.h>
#include <gui/win/window_event_handler.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    class window;

    namespace detail {
      void set_id (window* w, os::window id);
    }

    // --------------------------------------------------------------------------
    class container;

    // --------------------------------------------------------------------------
    class window {
    public:
      typedef core::event_container::event_handler_function event_handler_function;

      window ();
      ~window ();

      os::window get_id () const;

      operator os::drawable() const;

      bool is_valid () const;
      bool is_visible () const;
      bool is_enabled () const;
      bool is_child () const;
      bool is_popup () const;
      bool is_toplevel () const;
      bool is_focus_accepting () const;
      bool is_redraw_disabled () const;

      bool has_focus () const;
      bool has_border () const;
      bool accept_focus () const;

      void destroy ();
      void close ();

      void set_parent (const container& parent);
      container* get_parent () const;
      container* get_root () const;
      bool is_child_of (const container& parent) const;

      void set_visible (bool s = true);
      void set_accept_focus (bool a);

      void enable (bool on = true);
      void disable ();

      void take_focus ();
      void shift_focus (bool backward = false);

      void to_front ();
      void to_back ();

      void disable_redraw (bool on = true);

      void redraw_now ();
      void redraw_later ();

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

      bool handle_event (const core::event&, os::event_result&);

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

    private:
      friend void detail::set_id (window*, os::window);

      static os::window create_window (const class_info&,
                                       const core::rectangle& r,
                                       os::window parent_id,
                                       window* data);

      void init ();

      os::window id;
      core::event_container events;
      std::bitset<16> flags;
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
  } // namespace win

} // namespace gui

#include <gui/win/window.inl>
