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
 * @brief     C++ API: scroll view container
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/scroll_view.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    scroll_view_base::scroll_view_base ()
      : vscroll(nullptr)
      , hscroll(nullptr)
      , edge(nullptr)
    {}

    void scroll_view_base::init (ctrl::vertical_scroll_bar* vscroll,
                                 ctrl::horizontal_scroll_bar* hscroll,
                                 win::window* edge) {
      this->vscroll = vscroll;
      this->hscroll = hscroll;
      this->edge = edge;
    }

    core::rectangle scroll_view_base::get_vscroll_area (const core::rectangle& sz, bool hscroll_bar_enabled) {
      core::rectangle r(sz);
      r.x(r.x2() - ctrl::scroll_bar::get_scroll_bar_width());
      r.width(static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width()));
      if (hscroll_bar_enabled) {
        r.height(r.height() - ctrl::scroll_bar::get_scroll_bar_width());
      }
      return r;
    }

    core::rectangle scroll_view_base::get_hscroll_area (const core::rectangle& sz, bool vscroll_bar_enabled) {
      core::rectangle r(sz);
      r.y(r.y2() - ctrl::scroll_bar::get_scroll_bar_width());
      r.height(static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width()));
      if (vscroll_bar_enabled) {
        r.width(r.width() - ctrl::scroll_bar::get_scroll_bar_width());
      }
      return r;
    }

    core::rectangle scroll_view_base::get_client_area (const core::rectangle& sz) {
      return core::rectangle(sz - core::size {static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width()),
                                              static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width())});
    }

    core::rectangle scroll_view_base::get_edge_area (const core::rectangle& sz) {
      return core::rectangle(sz.x2() - ctrl::scroll_bar::get_scroll_bar_width(),
                             sz.y2() - ctrl::scroll_bar::get_scroll_bar_width(),
                             static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width()),
                             static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width()));
    }

    core::rectangle scroll_view_base::layout (const core::rectangle& new_size, const core::rectangle& required) const {
      core::rectangle space(new_size);

      //LogDebug << "Space:" << space << ", Required:" << required;

      bool show_h = hscroll && ((required.x() < space.x()) || (required.x2() > space.x2()));
      if (show_h) {
        space.height(space.height() - ctrl::scroll_bar::get_scroll_bar_width());
      }

      bool show_v = vscroll && ((required.y() < space.y()) || (required.y2() > space.y2()));
      if (show_v) {
        space.width(space.width() - ctrl::scroll_bar::get_scroll_bar_width());

        if (!show_h) {
          // re-check h
          bool show_h = hscroll && ((required.x() < space.x()) || (required.x2() > space.x2()));
          if (show_h) {
            space.height(space.height() - ctrl::scroll_bar::get_scroll_bar_width());
          }
        }

        core::point::type ypos = vscroll->get_value();
        core::point::type ymin = std::min(core::point::type(required.y() - space.y()), core::point::type(0)) + ypos;
        core::point::type ymax = std::max(core::point::type(required.y2() - space.y2()), core::point::type(0)) + ypos;
        core::point::type st = std::min(ymax - ymin, space.height());

//        LogDebug << *vscroll << " Y:{ min:" << ymin << ", pos:" << ypos << ", max:" << ymax << ", step:" << st << " }";

        vscroll->set_min_max_step_page(ymin, ymax, 1, st);
      }

      if (show_h) {
        core::point::type xpos = hscroll->get_value();
        core::point::type xmin = std::min(core::point::type(required.x() - space.x()), core::point::type(0)) + xpos;
        core::point::type xmax = std::max(core::point::type(required.x2() - space.x2()), core::point::type(0)) + xpos;
        core::point::type st = std::min(xmax - xmin, space.width());

//        LogDebug << *vscroll << " X:{ min:" << xmin << ", pos:" << xpos << ", max:" << xmax << ", step:" << st << " }";

        hscroll->set_min_max_step_page(xmin, xmax, 1, st);
      }

      if (vscroll) {
        vscroll->set_visible(show_v);
      }
      if (hscroll) {
        hscroll->set_visible(show_h);
      }
      if (edge) {
        edge->set_visible(show_h && show_v);
      }

      core::rectangle available(new_size);

      if (edge && show_h && show_v) {
        edge->place(get_edge_area(new_size));
        edge->to_front();
      }

      if (show_v) {
        auto area = get_vscroll_area(new_size, show_h);
        vscroll->place(area);
        vscroll->to_front();
        available.width(area.x());
      }

      if (show_h) {
        auto area = get_hscroll_area(new_size, show_v);
        hscroll->place(area);
        hscroll->to_front();
        available.height(area.y());
      }

      return available;
    }

    // --------------------------------------------------------------------------
    scroll_view::scroll_view ()
      : me(util::bind_method(this, &scroll_view::handle_child_move))
      , se(util::bind_method(this, &scroll_view::handle_child_size))
      , main(nullptr)
      , in_scroll_event(false)
    {}

    scroll_view::scroll_view (const scroll_view& rhs)
      : super(rhs)
      , me(util::bind_method(this, &scroll_view::handle_child_move))
      , se(util::bind_method(this, &scroll_view::handle_child_size))
      , main(nullptr)
      , in_scroll_event(false)
    {}

    scroll_view::scroll_view (scroll_view&& rhs)
      : super(std::move(rhs))
      , me(util::bind_method(this, &scroll_view::handle_child_move))
      , se(util::bind_method(this, &scroll_view::handle_child_size))
      , main(nullptr)
      , in_scroll_event(false)
    {}

    void scroll_view::set_main (win::container* main) {
      this->main = main;
    }

    void scroll_view::layout (const core::rectangle& new_size) const {
      if (main) {
        std::vector<win::window*> children = main->get_children();
        core::rectangle required = get_client_area(new_size);
        for (win::window* win : children) {
          if ((win != vscroll) && (win != hscroll) && (win != edge)) {
            required |= win->place();
            win->unregister_event_handler<win::move_event>(me);
            win->unregister_event_handler<win::size_event>(se);
          }
        }

        super::layout(new_size, required);

        for (win::window* win : children) {
          if ((win != vscroll) && (win != hscroll) && (win != edge)) {
            win->on<win::move_event>(me);
            win->on<win::size_event>(se);
          }
        }
      }
    }

    void scroll_view::set_in_scroll_event (bool b) {
      in_scroll_event = b;
    }

    void scroll_view::handle_child_move (const core::point&) const {
      if (!in_scroll_event && main) {
        layout(main->client_area());
      }
    }

    void scroll_view::handle_child_size (const core::size&) const {
      if (main) {
        layout(main->client_area());
      }
    }

    // --------------------------------------------------------------------------

  } // layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    scroll_view::scroll_view () {
      get_layout().set_main(this);
      get_layout().init(&vscroll, &hscroll, &edge);

      vscroll.on_scroll([&](core::point::type y) {
        move_children(core::size(0, y - current_pos.y()));
      });

      hscroll.on_scroll([&](core::point::type x) {
        move_children(core::size(x - current_pos.x(), 0));
      });
    }

    void scroll_view::create (const container& parent,
                              const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
      vscroll.create(*this, layout::scroll_view::get_vscroll_area(place, true));
      hscroll.create(*this, layout::scroll_view::get_hscroll_area(place, true));
      edge.create(*this, layout::scroll_view::get_edge_area(place));
    }

    void scroll_view::move_children (const core::size& delta) {
      if (delta == core::size::zero) {
        return;
      }
      get_layout().set_in_scroll_event(true);
      std::vector<window*> children = get_children();
      for (window* win : children) {
        if ((win != &vscroll) && (win != &hscroll) && (win != &edge)) {
          win->move(win->position() - delta);
        }
      }
      current_pos = current_pos + delta;
      get_layout().set_in_scroll_event(false);
    }

    void scroll_view::set_scroll_pos (const core::point& pt) {
      hscroll.set_value(pt.x());
      vscroll.set_value(pt.y());
    }

    core::point scroll_view::get_scroll_pos () const {
      return {core::point::type(hscroll.get_value()),
              core::point::type(vscroll.get_value())};
    }

    void scroll_view::enable_vscroll_bar (bool enable) {
      vscroll.set_visible(enable);
      if (enable) {
        bool hscroll_bar_enabled = is_hscroll_bar_enabled();
        const auto area = client_area();
        vscroll.place(layout::scroll_view::get_vscroll_area(area, hscroll_bar_enabled));
        vscroll.to_front();
        if (hscroll_bar_enabled) {
          hscroll.place(layout::scroll_view::get_hscroll_area(area, enable));
          edge.place(layout::scroll_view::get_edge_area(area));
          edge.set_visible();
          edge.to_front();
        }
      }
    }

    void scroll_view::enable_hscroll_bar (bool enable) {
      hscroll.set_visible(enable);
      if (enable) {
        bool vscroll_bar_enabled = is_vscroll_bar_enabled();
        const auto area = client_area();
        hscroll.place(layout::scroll_view::get_hscroll_area(area, vscroll_bar_enabled));
        hscroll.to_front();
        if (vscroll_bar_enabled) {
          vscroll.place(layout::scroll_view::get_vscroll_area(area, enable));
          edge.place(layout::scroll_view::get_edge_area(area));
          edge.set_visible();
          edge.to_front();
        }
      }
    }

    bool scroll_view::is_vscroll_bar_enabled () const {
      return vscroll.is_visible();
    }

    bool scroll_view::is_hscroll_bar_enabled () const {
      return hscroll.is_visible();
    }

    vertical_scroll_bar& scroll_view::get_vscroll () {
      return vscroll;
    }

    horizontal_scroll_bar& scroll_view::get_hscroll () {
      return hscroll;
    }

    win::window& scroll_view::get_edge () {
      return edge;
    }

  } // ctrl

} // gui
