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


// --------------------------------------------------------------------------
//
// Library includes
//
#include "event_container.h"
#include "gui_types.h"
#include "window_class.h"
#include "window_event_handler.h"


namespace gui {

  namespace win {

    class window;

    namespace detail {
      void set_id (window* w,
                   core::window_id id);

#ifdef X11
      extern Atom WM_CREATE_WINDOW;
#endif // X11
    }

    // --------------------------------------------------------------------------
    class container;

    // --------------------------------------------------------------------------
    class window : public core::event_container {
    public:

      window ();

      virtual ~window ();

      inline core::window_id get_id () const {
        return id;
      }

      bool is_valid () const;

      bool is_visible () const;

      bool is_enabled () const;

      bool has_focus () const;

      bool is_child () const;

      bool is_popup () const;

      bool is_toplevel () const;

      bool has_border () const;

      void destroy ();

      void quit ();

      void set_parent (const container& parent);

      container* get_parent () const;

      bool is_child_of (const container& parent) const;

      void set_visible (bool s = true);

      void enable (bool on = true);

      inline void disable () {
        enable(false);
      }

      void take_focus ();

      void to_front ();

      void to_back ();

      void enable_redraw (bool on = true);

      void redraw_now ();

      void redraw_later ();

      core::size size () const;

      core::point position () const;

      core::rectangle absolute_place () const;

      core::rectangle place () const;

      core::point absolute_position () const;

      core::rectangle client_area () const;

      void move (const core::point&,
                 bool repaint = true);

      void resize (const core::size&,
                   bool repaint = true);

      void place (const core::rectangle&,
                  bool repaint = true);

      core::point window_to_screen (const core::point&) const;

      core::point screen_to_window (const core::point&) const;

      core::point client_to_screen (const core::point&) const;

      core::point screen_to_client (const core::point&) const;

      const window_class* get_window_class () const;

    protected:
      void create (const window_class& type,
                   const container& parent,
                   const core::rectangle& place = core::rectangle::default_rectangle,
                   core::menu_id menu = 0);

#ifdef WIN32
      void set_style (core::windows_style mask, bool);
      core::windows_style get_style (core::windows_style mask = 
                                     std::numeric_limits<core::windows_style>::max()) const;
#endif // WIN32
      void create (const window_class& type,
                   core::window_id parent_id,
                   const core::rectangle& place,
                   core::menu_id menu = 0);

    private:
      friend void detail::set_id (window*,
                                  core::window_id);

      core::window_id id;
      const window_class* cls;

#ifdef X11
      bool redraw_disabled;
      bool window_disabled;
#endif // X11
    };

    // --------------------------------------------------------------------------
    template<window_class& clazz>
    class windowT : public window {
    public:

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::default_rectangle) {
        window::create(clazz, parent, place);
      }

    };

    // --------------------------------------------------------------------------
    class container : public window {
    public:
      bool is_parent_of (const window& parent) const;

      std::vector<window*> get_children () const;
    };

    // --------------------------------------------------------------------------
    class standard_layout {
    public:
      standard_layout(container*)
      {}

      void operator() (container&, const core::size& new_size)
      {}
    };

    // --------------------------------------------------------------------------
    template<typename Layout = standard_layout>
    class layout_container : public container {
    public:
      layout_container ()
        : layout(this) {
        register_event_handler(size_event(core::easy_bind(this, &layout_container::on_size_changed)));
      }

      void on_size_changed (const core::size& sz) {
        layout(*this, sz);
      }
    protected:
      Layout layout;
    };

    // --------------------------------------------------------------------------
    template<window_class& clazz, typename L = standard_layout>
    class containerT : public layout_container<L> {
    public:
      void create (const core::rectangle& place = core::rectangle::default_rectangle) {
        container::create(clazz, place);
      }

    };

    // --------------------------------------------------------------------------
    class window_with_text : public window {
    public:
      void set_text (const std::string&);

      std::string get_text () const;

    protected:
#ifdef X11
      std::string text;
#endif // X11
    };

    // --------------------------------------------------------------------------
    class main_window : public container {
    public:
      main_window ();
      
      void set_title (const std::string&);

      std::string get_title () const;

      bool is_top_most () const;

      bool is_minimized () const;

      bool is_maximized () const;

      void minimize ();

      void maximize ();

      void restore ();

      void set_top_most (bool toplevel);

      void create (const core::rectangle& place = core::rectangle::default_rectangle);

    private:
      static window_class clazz;
    };

    class client_window : public window {
    public:
      client_window ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::default_rectangle) {
        window::create(clazz, parent, place);
      }

    private:
      static window_class clazz;
    };

    // --------------------------------------------------------------------------
  } // win

} // gui
