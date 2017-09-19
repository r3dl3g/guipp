/**
* @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
* @brief     C++ API: basic controls
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "scroll_bar.h"
#include "container.h"


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    class scroll_view_base : protected layout_base {
    public:
      typedef layout_base super;

      scroll_view_base (win::container* main);

      void init (win::vscroll_bar* vscroll,
                 win::hscroll_bar* hscroll,
                 win::client_window* edge);

      core::rectangle layout (const core::size& new_size, const core::rectangle& required);

      static core::rectangle get_vscroll_area (const core::size&, bool hscroll_bar_enabled);
      static core::rectangle get_hscroll_area (const core::size&, bool vscroll_bar_enabled);
      static core::rectangle get_edge_area (const core::size&);
      static core::rectangle get_client_area (const core::size&);

    protected:
      void init(std::function<size_callback> f1) {
        super::init(f1);
      }

      win::vscroll_bar*   vscroll;
      win::hscroll_bar*   hscroll;
      win::client_window* edge;
    };

    // --------------------------------------------------------------------------
    class scroll_view : public scroll_view_base {
    public:
      typedef scroll_view_base super;

      scroll_view (win::container* main);

      void layout (const core::size& new_size);

      void set_in_scroll_event (bool);

      inline bool is_in_scroll_event () const {
        return in_scroll_event;
      }

    private:
      const win::move_event me;
      const win::size_event se;

      bool in_scroll_event;

      void handle_child_move (const core::point&);
      void handle_child_size (const core::size&);
    };

    // --------------------------------------------------------------------------
    template<typename T>
    class virtual_layout : public scroll_view_base {
    public:
      typedef scroll_view_base super;
      typedef T view_type;

      virtual_layout (win::container* main)
        :super(main)
      {
        super::init(core::bind_method(this, &virtual_layout::layout));
      }

      void init (win::vscroll_bar* vscroll,
                 win::hscroll_bar* hscroll,
                 win::client_window* edge,
                 view_type* client) {
        super::init(vscroll, hscroll, edge);
        this->client = client;
      }

      void layout (const core::size& new_size) {
        if (client && client->is_visible()) {
          auto available = super::layout(new_size, client->get_virtual_place());
          client->place(available);
        } else {
          super::layout(new_size, core::rectangle());
        }
      }

    private:
      view_type* client;

    };

    // --------------------------------------------------------------------------
  } // namespace layout

  namespace win {

    // --------------------------------------------------------------------------
    class scroll_view : public layout_container<layout::scroll_view> {
    public:
      typedef layout_container<layout::scroll_view> super;

      scroll_view ();

      void set_scroll_pos (const core::point& pt);
      core::point get_scroll_pos () const;

      void enable_vscroll_bar (bool enable);
      void enable_hscroll_bar (bool enable);
      bool is_vscroll_bar_enabled () const;
      bool is_hscroll_bar_enabled () const;

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void move_children (const core::point& delta);

    protected:
      vscroll_bar& get_vscroll ();
      hscroll_bar& get_hscroll ();
      client_window& get_edge ();

    private:
      core::point   current_pos;
      vscroll_bar   vscroll;
      hscroll_bar   hscroll;
      client_window edge;

      static window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<typename T, os::color background = color::white>
    class virtual_view : public group_window<layout::virtual_layout<T>, background> {
    public:
      typedef layout::virtual_layout<T> layout_type;
      typedef layout_container<layout_type> super;
      typedef T view_type;

      virtual_view () {
        super::register_event_handler(REGISTER_FUNCTION, create_event(this, &virtual_view::handle_create));
        super::get_layout().init(&vscroll, &hscroll, &edge, &view);

        vscroll.register_event_handler(REGISTER_FUNCTION, scroll_event([&](core::point::type y) {
          view.set_scroll_pos(core::point(hscroll.get_value(), y));
        }));
        hscroll.register_event_handler(REGISTER_FUNCTION, scroll_event([&](core::point::type x) {
          view.set_scroll_pos(core::point(x, vscroll.get_value()));
        }));
        view.register_event_handler(REGISTER_FUNCTION, content_changed_event([&]() {
          super::layout();
        }));
        view.register_event_handler(REGISTER_FUNCTION, selection_changed_event([&](event_source) {
          view.make_cursor_visible();
          const core::point& pos = view.get_scroll_pos();
          hscroll.set_value(pos.x());
          vscroll.set_value(pos.y());
          super::layout();
        }));
        view.register_event_handler(REGISTER_FUNCTION, wheel_x_event([&](const core::point_type delta, const core::point& pt) {
          hscroll.handle_wheel(delta, pt);
        }));
        view.register_event_handler(REGISTER_FUNCTION, wheel_y_event([&](const core::point_type delta, const core::point& pt) {
          vscroll.handle_wheel(delta, pt);
        }));
      }

      view_type     view;
      hscroll_bar   hscroll;
      vscroll_bar   vscroll;
      client_window edge;

    private:
      void handle_create(window*, const core::rectangle& r) {
        core::size sz = r.size();
        vscroll.create(*this, layout::scroll_view::get_vscroll_area(sz, true));
        hscroll.create(*this, layout::scroll_view::get_hscroll_area(sz, true));
        edge.create(*this, layout::scroll_view::get_edge_area(sz));
        view.create(*this, r);
        view.set_visible();
      }

    };

    // --------------------------------------------------------------------------
  } // namespace win

} // gui
