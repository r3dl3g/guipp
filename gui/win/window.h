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
    class GUIPP_WIN_EXPORT window : public receiver {
    public:
      typedef receiver super;

      window ();
      virtual ~window ();

      void close ();

      void set_parent (container& parent);
      container* get_parent () const;
      void remove_from_parent ();

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

      virtual window* get_current_focus_window () const;

      void to_front ();
      void to_back ();

      void invalidate () const;
      void redraw () const;

      core::size size () const;
      virtual core::size client_size () const;

      virtual core::point position () const;
      core::point absolute_position () const;

      virtual core::native_point surface_position () const;
      core::native_rect surface_geometry () const;

      core::rectangle geometry () const;
      core::rectangle absolute_geometry () const;
      core::rectangle client_geometry () const;

      void position (const core::point&, bool repaint = false);
      void resize (const core::size&, bool repaint = true);
      void geometry (const core::rectangle&, bool repaint = true);

      core::point client_to_screen (const core::point&) const;
      core::point screen_to_client (const core::point&) const;

      core::native_point client_to_surface (const core::point&) const;
      core::point surface_to_client (const core::native_point&) const;

      const char* get_class_name () const;
      const class_info& get_window_class () const;

      virtual void set_cursor (const cursor&);
      void reset_cursor ();
      const cursor& get_cursor () const;

      void capture_pointer ();
      void uncapture_pointer ();

      virtual bool handle_event (const core::event&, gui::os::event_result&);

      void notify_event (os::message_type message, long l1 = 0, long l2 = 0);
      void notify_event (os::message_type message, void* v1, void* v2);
      void notify_event (os::message_type message, const core::rectangle&);
      void notify_event_float (os::message_type message, float d1);
      void notify_mouse_event (bool enter);
      void notify_visibility (bool visible);

      static core::size screen_size ();
      static core::rectangle screen_area ();

      window& operator= (const window&) = delete;
      window& operator= (window&&) = delete;

      void add_event_mask (os::event_id mask) override;
      virtual os::event_id collect_event_mask () const;

      void create (const class_info&,
                   container&,
                   const core::rectangle& = core::rectangle::def);

    protected:
      window (const window&);
      window (window&&) noexcept;

      void create_internal (const class_info&,
                            const core::rectangle&);

      void set_accept_focus (bool a);

    protected:
      friend class container;
      void remove_parent ();

    protected:
      gui::core::state_type flags;
      core::rectangle area;

      virtual void move_native (const core::point&);
      virtual void resize_native (const core::size&);
      virtual void geometry_native (const core::rectangle&);

      bool has_overlapped_window () const;

    private:
      container* parent;
      cursor cursor_;
      const char* class_name;

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
    out << "[" << &t << "] (" << typeid(t).name() << ')';
    return out;
  }

//  inline ostream& operator<< (ostream& out, const gui::win::window* t) {
//    out << "[" << t << "] (" << typeid(*t).name() << ')';
//    return out;
//  }

} // namespace std

#include <gui/win/window.inl>
