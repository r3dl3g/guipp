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
#include <gui/core/window_state.h>
#include <gui/win/window_class.h>
#include <gui/win/receiver.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    // --------------------------------------------------------------------------
    class window;
    class container;
    class overlapped_window;

    // --------------------------------------------------------------------------
    namespace detail {

      void set_os_window (window* w, os::window id);

    }

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT window : public receiver {
    public:

      window ();
      virtual ~window ();

      void close ();

      void set_parent (container& parent);
      container* get_parent () const;
      const window& get_root_window () const;
      overlapped_window& get_overlapped_window () const;
      bool is_child_of (const container& parent) const;

      bool is_valid () const;

      core::window_state::is get_state () const;
      core::window_state::set set_state ();

      void set_visible (bool s = true); /// set window visible
      bool is_visible () const;         /// window is visible

      void enable (bool on = true);     /// enable of disable this window
      void disable ();                  /// disable this window
      bool is_enabled () const;         /// return if window is enabled

      void set_disable_redraw (bool on = true); /// disable automatic redraw on any change
      bool is_redraw_disabled () const;         /// return if automatic redraw is desabled

      bool is_focus_accepting () const; /// window type is general able to accept focus (like edits)
      bool is_focused () const;

      bool can_accept_focus () const;   /// this specific window will accept focus (is_focus_accepting and visible and enabled)
      void take_focus ();               /// grabs the input focus
      void focus_lost ();               /// focus was given an other window

      static window* get_current_focus_window ();

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

      void set_cursor (const os::cursor&);

      void capture_pointer ();
      void uncapture_pointer ();

      bool handle_event (const core::event&, gui::os::event_result&) const;

      void notify_event (os::message_type message, long l1 = 0, long l2 = 0);
      void notify_event (os::message_type message, const core::rectangle&);
      void notify_event_double (os::message_type message, double d1);

      operator os::drawable() const;

      static core::size screen_size ();
      static core::rectangle screen_area ();

      window& operator= (const window&) = delete;
      window& operator= (window&&) = delete;

    protected:
      window (const window&);
      window (window&&) noexcept;

      void create (const class_info&,
                   container&,
                   const core::rectangle& = core::rectangle::def);

      void create_internal (const class_info&,
                            os::window parent,
                            const core::rectangle&);

      void set_accept_focus (bool a);

    private:
      friend void detail::set_os_window (window*, os::window);
      friend os::window detail::get_os_window (const window&);

      os::window get_os_window () const;
      void set_os_window (os::window);

    protected:
      void destroy ();

      core::rectangle area;
      gui::core::state_type flags;

    private:
      void init ();

      os::window id;
      container* parent;

    };

    // --------------------------------------------------------------------------
    template<os::color background = color::very_light_gray>
    class client_window : public window {
    public:
      typedef window super;
      typedef window_class<client_window, background> clazz;

      void create (container& parent,
                   const core::rectangle& r = core::rectangle::def);
    };

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui

namespace std {

  inline ostream& operator<< (ostream& out, const gui::win::window& t) {
    out << "[" << gui::win::detail::get_os_window(t) << "] (" << typeid(t).name() << ')';
    return out;
  }

  inline ostream& operator<< (ostream& out, const gui::win::window* t) {
    out << "[" << gui::win::detail::get_os_window(*t) << "] (" << typeid(*t).name() << ')';
    return out;
  }

} // namespace std

#include <gui/win/window.inl>
