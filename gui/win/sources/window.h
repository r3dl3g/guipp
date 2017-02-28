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
      bool is_active() const;
      bool is_child() const;
      bool is_popup() const;
      bool is_toplevel() const;
      bool is_top_most() const;
      bool is_minimized() const;
      bool is_maximized() const;

      bool has_border() const;

      void create(const window_class& type,
                  const window& parent,
                  const core::rectangle& place = core::rectangle::default_rectangle);
      void create(const window_class& type,
                  const core::rectangle& place = core::rectangle::default_rectangle);

      void close();
      void destroy();
      void quit();

      void setParent(const window& parent);
      window* getParent() const;

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

      void enableRedraw(bool on = true);
      void redraw_now();
      void redraw_later();

      void setText(const std::string&);
      std::string getText() const;

      core::size size() const;
      core::point position() const;
      core::rectangle absolute_place() const;
      core::rectangle place() const;
      core::point absolute_position() const;
      core::rectangle client_area() const;

      void move(const core::point&, bool repaint = true);
      void resize(const core::size&, bool repaint = true);
      void place(const core::rectangle&, bool repaint = true);

      core::point windowToScreen(const core::point&) const;
      core::point screenToWindow(const core::point&) const;
      core::point clientToScreen(const core::point&) const;
      core::point screenToClient(const core::point&) const;

      static window* get(core::window_id id);

    private:
      friend void detail::set_id(window*, core::window_id);
      core::window_id id;

    };

    template<window_class& cls>
    class windowT : public window {
    public:
      void create(const window& parent,
                  const core::rectangle& place = core::rectangle::default_rectangle) {
        window::create(cls, parent, place);
      }
      void create(const core::rectangle& place = core::rectangle::default_rectangle) {
        window::create(cls, place);
      }
    };

  } // win

} // gui
