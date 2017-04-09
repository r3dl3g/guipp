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
#include "layout.h"


namespace gui {

  namespace win {

    class window;

    namespace detail {
      void set_id (window* w,
                   os::window id);

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

      inline os::window get_id () const {
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

      core::size client_size () const;

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
                   const core::rectangle& place = core::rectangle::def,
                   os::menu menu = 0);

#ifdef WIN32
      void set_style (os::style mask, bool);
      os::style get_style (os::style mask =
                                     std::numeric_limits<os::style>::max()) const;
#endif // WIN32
      void create (const window_class& type,
                   os::window parent_id,
                   const core::rectangle& place,
                   os::menu menu = 0);

    private:
      friend void detail::set_id (window*,
                                  os::window);

      os::window id;
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
                   const core::rectangle& place = core::rectangle::def) {
        window::create(clazz, parent, place);
      }

    };

    // --------------------------------------------------------------------------
    class window_with_text : public window {
    public:
      void set_text (const std::string&);

      std::string get_text () const;

#ifdef X11
    protected:
      std::string text;
#endif // X11
    };

    // --------------------------------------------------------------------------
    class client_window : public window {
    public:
      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        window::create(clazz, parent, place);
      }

    private:
      static window_class clazz;
    };

    // --------------------------------------------------------------------------
    class container : public window {
    public:
      bool is_parent_of (const window& parent) const;

      std::vector<window*> get_children () const;
    };

    // --------------------------------------------------------------------------
    template<typename Layout = layout::standard_layout>
    class layout_container : public container {
    public:
      typedef Layout layout_type;

      layout_container () {
        register_event_handler(size_event(core::bind_method(this, &layout_container::do_layout_for_size)));
      }

      void do_layout () {
        do_layout_for_size(size());
      }

      void do_layout_for_size (const core::size& sz) {
        layout.layout(sz, this);
      }

      inline layout_type& get_layout() {
        return layout;
      };

      inline const layout_type& get_layout() const {
        return layout;
      };

    protected:
      layout_type layout;
    };

    window_class init_group_window_clazz (draw::color::value_type);

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, draw::color::value_type B = 0xFFFFFF>
    class group_window : public layout_container<L> {
    public:
      typedef layout_container<L> super;

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(clazz, parent, place);
      }

    private:
      static window_class clazz;
    };

    template<typename L, draw::color::value_type B>
    window_class group_window<L, B>::clazz(init_group_window_clazz(B));

    // --------------------------------------------------------------------------
    class main_window : public container {
    public:
      void set_title (const std::string&);

      std::string get_title () const;

      bool is_top_most () const;

      bool is_minimized () const;

      bool is_maximized () const;

      void minimize ();

      void maximize ();

      void restore ();

      void set_top_most (bool toplevel);

      void create (const core::rectangle& place = core::rectangle::def);

    private:
      static window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<typename Layout = layout::standard_layout>
    class layout_main_window : public main_window {
    public:
      layout_main_window () {
        register_event_handler(size_event(core::bind_method(this, &layout_main_window::do_layout_for_size)));
      }

      void do_layout () {
        do_layout_for_size(client_size());
      }

      void do_layout_for_size (const core::size& sz) {
        layout.layout(sz, this);
      }

      inline Layout& get_layout () {
        return layout;
      };

      inline const Layout& get_layout () const {
        return layout;
      };

    protected:
      Layout layout;
    };
    // --------------------------------------------------------------------------
  } // win

} // gui
