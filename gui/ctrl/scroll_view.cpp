/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     view to scroll child views
 *
 * @license   MIT license. See accompanying file LICENSE.
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

    core::rectangle scroll_view_base::get_vscroll_area (const core::rectangle& r, bool hscroll_bar_enabled) {
      core::rectangle r2(r);
      r2.x(r2.x2() - ctrl::scroll_bar::get_scroll_bar_width());
      r2.width(static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width()));
      if (hscroll_bar_enabled) {
        r2.height(r2.height() - ctrl::scroll_bar::get_scroll_bar_width());
      }
      return r2;
    }

    core::rectangle scroll_view_base::get_hscroll_area (const core::rectangle& r, bool vscroll_bar_enabled) {
      core::rectangle r2(r);
      r2.y(r2.y2() - ctrl::scroll_bar::get_scroll_bar_width());
      r2.height(static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width()));
      if (vscroll_bar_enabled) {
        r2.width(r2.width() - ctrl::scroll_bar::get_scroll_bar_width());
      }
      return r2;
    }

    core::rectangle scroll_view_base::get_client_area (const core::rectangle& r) {
      return core::rectangle(r - core::size {static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width()),
                                             static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width())});
    }

    core::rectangle scroll_view_base::get_edge_area (const core::rectangle& r) {
      return core::rectangle(r.x2() - ctrl::scroll_bar::get_scroll_bar_width(),
                             r.y2() - ctrl::scroll_bar::get_scroll_bar_width(),
                             static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width()),
                             static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width()));
    }

    core::rectangle scroll_view_base::layout (const core::rectangle& new_size, const core::rectangle& required) const {
      core::rectangle space(new_size);

      //logging::debug() << "Space:" << space << ", Required:" << required;

      bool show_h = hscroll && ((required.x() < 0) || (required.x2() > space.width()));
      if (show_h) {
        space.height(space.height() - ctrl::scroll_bar::get_scroll_bar_width());
      }

      bool show_v = vscroll && ((required.y() < 0) || (required.y2() > space.height()));
      if (show_v) {
        space.width(space.width() - ctrl::scroll_bar::get_scroll_bar_width());

        if (!show_h) {
          // re-check h
          show_h = hscroll && ((required.x() < 0) || (required.x2() > space.width()));
          if (show_h) {
            space.height(space.height() - ctrl::scroll_bar::get_scroll_bar_width());
          }
        }

        typedef core::point::type type;
        type ymin = std::min(required.y(), type(0));
        type ymax = std::max(required.y2() - space.height(), type(0));
        type ypage = std::min(ymax - ymin, space.height());

//        logging::debug() << *vscroll << " Y:{ min:" << ymin << ", pos:" << ypos << ", max:" << ymax << ", step:" << ypage << " }";

        vscroll->set_min_max_page(ymin, ymax, ypage);
      } else if (vscroll) {
        vscroll->set_min_max(0, 0);
      }

      if (show_h) {
        typedef core::point::type type;
        type xmin = std::min(required.x(), type(0));
        type xmax = std::max(required.x2() - space.width(), type(0));
        type xpage = std::min(xmax - xmin, space.width());

//        logging::debug() << *vscroll << " X:{ min:" << xmin << ", pos:" << xpos << ", max:" << xmax << ", step:" << xpage << " }";

        hscroll->set_min_max_page(xmin, xmax, xpage);
      } else if (hscroll) {
        hscroll->set_min_max(0, 0);
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
        edge->geometry(get_edge_area(new_size));
        edge->to_front();
      }

      if (show_v) {
        auto area = get_vscroll_area(new_size, show_h);
        vscroll->geometry(area);
        vscroll->to_front();
        available.x2(area.x());
      }

      if (show_h) {
        auto area = get_hscroll_area(new_size, show_v);
        hscroll->geometry(area);
        hscroll->to_front();
        available.y2(area.y());
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

    scroll_view::scroll_view (scroll_view&& rhs) noexcept
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
            required |= win->geometry();
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
        layout(main->client_geometry());
      }
    }

    void scroll_view::handle_child_size (const core::size&) const {
      if (main) {
        layout(main->client_geometry());
      }
    }

    // --------------------------------------------------------------------------

  } // layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    scroll_view_base::scroll_view_base () {
      get_layout().set_main(this);
      get_layout().init(&vscroll, &hscroll, &edge);

      vscroll.on_scroll([&](core::point::type y) {
        move_children(0, y - current_pos.y());
      });

      hscroll.on_scroll([&](core::point::type x) {
        move_children(x - current_pos.x(), 0);
      });
    }

    void scroll_view_base::move_children (type dx, type dy) {
      if ((dx == 0.0F) && (dy == 0.0F)) {
        return;
      }
      core::point delta(dx, dy);
      get_layout().set_in_scroll_event(true);
      std::vector<window*> children = get_children();
      for (window* win : children) {
        if ((win != &vscroll) && (win != &hscroll) && (win != &edge)) {
          win->position(win->position() - delta);
        }
      }
      current_pos = current_pos + delta;
      get_layout().set_in_scroll_event(false);
      invalidate();
    }

    void scroll_view_base::set_scroll_pos (const core::point& pt) {
      hscroll.set_value(pt.x());
      vscroll.set_value(pt.y());
      current_pos = pt;
    }

    core::point scroll_view_base::get_scroll_pos () const {
      return {core::point::type(hscroll.get_value()),
              core::point::type(vscroll.get_value())};
    }

    void scroll_view_base::enable_vscroll_bar (bool enable) {
      vscroll.set_visible(enable);
      if (enable) {
        bool hscroll_bar_enabled = is_hscroll_bar_enabled();
        const auto area = client_geometry();
        vscroll.geometry(layout::scroll_view::get_vscroll_area(area, hscroll_bar_enabled));
        vscroll.to_front();
        if (hscroll_bar_enabled) {
          hscroll.geometry(layout::scroll_view::get_hscroll_area(area, enable));
          edge.geometry(layout::scroll_view::get_edge_area(area));
          edge.set_visible();
          edge.to_front();
        }
      }
    }

    void scroll_view_base::enable_hscroll_bar (bool enable) {
      hscroll.set_visible(enable);
      if (enable) {
        bool vscroll_bar_enabled = is_vscroll_bar_enabled();
        const auto area = client_geometry();
        hscroll.geometry(layout::scroll_view::get_hscroll_area(area, vscroll_bar_enabled));
        hscroll.to_front();
        if (vscroll_bar_enabled) {
          hscroll.geometry(layout::scroll_view::get_hscroll_area(area, enable));
          edge.geometry(layout::scroll_view::get_edge_area(area));
          edge.set_visible();
          edge.to_front();
        }
      }
    }

    bool scroll_view_base::is_vscroll_bar_enabled () const {
      return vscroll.is_visible();
    }

    bool scroll_view_base::is_hscroll_bar_enabled () const {
      return hscroll.is_visible();
    }

    vertical_scroll_bar& scroll_view_base::get_vscroll () {
      return vscroll;
    }

    horizontal_scroll_bar& scroll_view_base::get_hscroll () {
      return hscroll;
    }

    win::window& scroll_view_base::get_edge () {
      return edge;
    }

    void scroll_view_base::create_children (const core::rectangle& place) {
      vscroll.create(*this, layout::scroll_view::get_vscroll_area(place, true));
      hscroll.create(*this, layout::scroll_view::get_hscroll_area(place, true));
      edge.create(*this, layout::scroll_view::get_edge_area(place));
    }

  } // ctrl

} // gui
