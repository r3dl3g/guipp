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
#include "window_event_proc.h"


namespace gui {

  namespace win {

    class window;

    namespace detail {
      void set_id(window* w, core::window_id id);
    }

    class window : public core::event_container {
    public:

      window();
      ~window();

      inline core::window_id get_id() const {
        return id;
      }

      bool is_valid() const;
      bool is_visible() const;
      bool is_enabled() const;
      bool has_focus() const;
      bool is_child() const;
      bool is_popup() const;
      bool is_toplevel() const;
      bool is_top_most() const;
      bool is_minimized() const;
      bool is_maximized() const;

      bool has_border() const;

      void destroy();
      void quit();

      void set_parent(const window& parent);
      window* get_parent() const;

      bool is_parent_of(const window& parent) const;
      bool is_child_of(const window& parent) const;

      void hide();
      void show();

      void minimize();
      void maximize();
      void restore();

      void set_top_most(bool toplevel);

      void enable(bool on = true);
      inline void disable() {
        enable(false);
      }

      void take_focus ();

      void enable_redraw(bool on = true);
      void redraw_now();
      void redraw_later();

      core::size size() const;
      core::point position() const;
      core::rectangle absolute_place() const;
      core::rectangle place() const;
      core::point absolute_position() const;
      core::rectangle client_area() const;

      void move(const core::point&, bool repaint = true);
      void resize(const core::size&, bool repaint = true);
      void place(const core::rectangle&, bool repaint = true);

      core::point window_to_screen(const core::point&) const;
      core::point screen_to_window(const core::point&) const;
      core::point client_to_screen(const core::point&) const;
      core::point screen_to_client(const core::point&) const;

      const window_class* get_window_class() const;

      static window* get(core::window_id id);

    protected:
      void create(const window_class& type,
                  const window& parent,
                  const core::rectangle& place = core::rectangle::default_rectangle);

      void create(const window_class& type,
                  const core::rectangle& place = core::rectangle::default_rectangle);

    private:
      void create (const window_class& type,
                   core::window_id parent_id,
                   const core::rectangle& place);

      friend void detail::set_id(window*, core::window_id);
      core::window_id id;
      const window_class* cls;

#ifdef X11
      bool redraw_disabled;
      bool window_disabled;
#endif // X11
    };

    template<window_class& clazz>
    class windowT : public window {
    public:

      void create(const window& parent,
                  const core::rectangle& place = core::rectangle::default_rectangle) {
        window::create(clazz, parent, place);
      }

      void create(const core::rectangle& place = core::rectangle::default_rectangle) {
        window::create(clazz, place);
      }

    };

    class window_with_text : public window {
    public:
      void set_text(const std::string&);
      std::string get_text() const;

    protected:
#ifdef X11
      std::string text;
#endif // X11
    };

  } // win

} // gui
