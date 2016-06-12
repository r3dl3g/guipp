/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
#include <vector>
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window_event.h"
#include "gui_types.h"
#include "window_class.h"
#include "easy_bind.h"


namespace gui {

  namespace win {

    class window;

    namespace detail {
      void set_id(window* w, core::window_id id);
    }

    class window {
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
                  const core::rectangle& place = core::rectangle::default);
      void create(const window_class& type,
                  const core::rectangle& place = core::rectangle::default);

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

      void set_toplevel(bool toplevel);

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
      core::position position() const;
      core::rectangle absolute_place() const;
      core::rectangle place() const;
      core::position absolute_position() const;
      core::rectangle client_area() const;

      void move(const core::position&, bool repaint = true);
      void resize(const core::size&, bool repaint = true);
      void place(const core::rectangle&, bool repaint = true);

      core::position windowToScreen(const core::position&) const;
      core::position screenToWindow(const core::position&) const;
      core::position clientToScreen(const core::position&) const;
      core::position screenToClient(const core::position&) const;

      void register_event_handler(event_handler*);
      void unregister_event_handler(event_handler*);

      bool handle_event(const window_event& e, core::event_result& result);

      static window* get(core::window_id id);

    private:
      friend void detail::set_id(window*, core::window_id);

      core::window_id id;

    private:
      std::vector<event_handler*> event_handler_list;

    };

  } // win

} // gui
