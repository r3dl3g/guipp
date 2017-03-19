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
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "scroll_view.h"


namespace gui {

  namespace win {

    window_class scroll_view::clazz;

    scroll_view::scroll_view () {
      if (!clazz.is_valid()) {
#ifdef WIN32
        clazz = win::window_class::custom_class("MyScrollView",
                                                CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW,
                                                WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                                                WS_EX_NOPARENTNOTIFY,
                                                nullptr,
                                                LoadCursor(nullptr, IDC_ARROW),
                                                (HBRUSH)(COLOR_WINDOW + 1));
#endif // WIN32
#ifdef X11
        clazz = win::window_class::custom_class("MyScrollView");
#endif //X11
      }

      layout.init(&vscroll, &hscroll, &edge);

      vscroll.register_event_handler(scroll_event([&](int y) {
        move_children(core::point(0, y - layout.get_current_pos().y()));
      }));

      hscroll.register_event_handler(scroll_event([&](int x) {
        move_children(core::point(x - layout.get_current_pos().x(), 0));
      }));
    }

    void scroll_view::create (const container& parent,
                              const core::rectangle& place) {
      super::create(clazz, parent, place);
      core::size sz = place.size();
      vscroll.create(*this, scroll_view_layout::get_vscroll_area(sz, true));
      hscroll.create(*this, scroll_view_layout::get_hscroll_area(sz, true));
      edge.create(*this, scroll_view_layout::get_edge_area(sz));
    }

    void scroll_view::move_children (const core::point& delta) {
      std::vector<window*> children = get_children();
      for(window* win : children) {
        if ((win != &vscroll) && (win != &hscroll) && (win != &edge)) {
          win->move(win->position() - delta);
        }
      }
      layout.set_current_pos(layout.get_current_pos() + delta);
    }

    void scroll_view::set_scroll_pos (const core::point& pt) {
      hscroll.set_value(pt.x());
      vscroll.set_value(pt.y());
    }

    core::point scroll_view::get_scroll_pos () const {
      return { core::point::type(hscroll.get_value()),
               core::point::type(vscroll.get_value()) };
    }

    void scroll_view::enable_vscroll_bar (bool enable) {
      vscroll.set_visible(enable);
      if (enable) {
        bool hscroll_bar_enabled = is_hscroll_bar_enabled();
        vscroll.place(scroll_view_layout::get_vscroll_area(size(), hscroll_bar_enabled));
        vscroll.to_front();
        if (hscroll_bar_enabled) {;
          hscroll.place(scroll_view_layout::get_hscroll_area(size(), enable));
          edge.place(scroll_view_layout::get_edge_area(size()));
          edge.set_visible();
          edge.to_front();
        }
      }
    }

    void scroll_view::enable_hscroll_bar (bool enable) {
      hscroll.set_visible(enable);
      if (enable) {
        bool vscroll_bar_enabled = is_vscroll_bar_enabled();
        hscroll.place(scroll_view_layout::get_hscroll_area(size(), vscroll_bar_enabled));
        hscroll.to_front();
        if (vscroll_bar_enabled) {
          vscroll.place(scroll_view_layout::get_vscroll_area(size(), enable));
          edge.place(scroll_view_layout::get_edge_area(size()));
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

    vscroll_bar& scroll_view::get_vscroll () {
      return vscroll;
    }

    hscroll_bar& scroll_view::get_hscroll () {
      return hscroll;
    }

    client_window& scroll_view::get_edge () {
      return edge;
    }

    void scroll_view::calc_area () {
      layout.layout(size());
    }

    scroll_view_layout::scroll_view_layout (container* main)
      : main(main)
      , vscroll(nullptr)
      , hscroll(nullptr)
      , edge(nullptr)
    {}

    void scroll_view_layout::init (vscroll_bar* vscroll,
                                   hscroll_bar* hscroll,
                                   client_window* edge) {
      this->vscroll = vscroll;
      this->hscroll = hscroll;
      this->edge = edge;
    }

    void scroll_view_layout::set_current_pos (const core::point& pt) {
      current_pos = pt;
    }
    core::point scroll_view_layout::get_current_pos () const {
      return current_pos;
    }

    core::rectangle scroll_view_layout::get_vscroll_area (const core::size& sz, bool hscroll_bar_enabled) {
      core::rectangle r(sz);
      r.x(r.x2() - scroll_bar::get_scroll_bar_width());
      r.width(scroll_bar::get_scroll_bar_width());
      if (hscroll_bar_enabled) {
        r.height(r.height() - scroll_bar::get_scroll_bar_width());
      }
      return r;
    }

    core::rectangle scroll_view_layout::get_hscroll_area (const core::size& sz, bool vscroll_bar_enabled) {
      core::rectangle r(sz);
      r.y(r.y2() - scroll_bar::get_scroll_bar_width());
      r.height(scroll_bar::get_scroll_bar_width());
      if (vscroll_bar_enabled) {
        r.width(r.width() - scroll_bar::get_scroll_bar_width());
      }
      return r;
    }

    core::rectangle scroll_view_layout::get_visible_area (const core::size& sz) {
      return core::rectangle(sz - core::size{ scroll_bar::get_scroll_bar_width() , scroll_bar::get_scroll_bar_width() });
    }

    core::rectangle scroll_view_layout::get_edge_area (const core::size& sz) {
      return core::rectangle(sz.width() - scroll_bar::get_scroll_bar_width(),
                             sz.height() - scroll_bar::get_scroll_bar_width(),
                             scroll_bar::get_scroll_bar_width(),
                             scroll_bar::get_scroll_bar_width());
    }

    void scroll_view_layout::layout (const core::size& new_size) {
      core::rectangle space(new_size);

      std::vector<window*> children = main->get_children();
      core::rectangle required = get_visible_area(new_size);
      for(window* win : children) {
        if ((win != vscroll) && (win != hscroll) && (win != edge)) {
          required |= win->place() + core::size(1, 1);
        }
      }

      LogDebug << "Space:" << space << ", Required:" << required;

      bool show_h = hscroll && (required.x() < space.x()) || (required.x2() > space.x2());
      if (show_h) {
        space.height(space.height() - scroll_bar::get_scroll_bar_width());
      }

      bool show_v = vscroll && (required.y() < space.y()) || (required.y2() > space.y2());
      if (show_v) {
        space.width(space.width() - scroll_bar::get_scroll_bar_width());

        if (!show_h) {
          // re-check h
          show_h = hscroll && (required.x() < space.x()) || (required.x2() > space.x2());
          if (show_h) {
            space.height(space.height() - scroll_bar::get_scroll_bar_width());
          }
        }

        int ymin = std::min(core::point::type(required.y() - space.y()), core::point::type(0));
        int ymax = std::max(core::point::type(required.y2() - space.y2()), core::point::type(0));
        int ypos = 0;

        LogDebug << "Y:{ min:" << ymin << ", pos:" << ypos << ", max:" << ymax << " }";

        vscroll->set_min_max(ymin, ymax);
#ifdef WIN32
        vscroll->set_step(std::min(ymax - ymin, (int)space.height()) / 2);
#endif // WIN32
#ifdef X11
        vscroll->set_step(std::min(ymax - ymin, (int)space.height()));
#endif // X11
        vscroll->set_value(ypos);
        current_pos.y(ypos);
      }

      if (show_h) {
        int xmin = std::min(core::point::type(required.x() - space.x()), core::point::type(0));
        int xmax = std::max(core::point::type(required.x2() - space.x2()), core::point::type(0));
        int xpos = 0;

        LogDebug << "X:{ min:" << xmin << ", pos:" << xpos << ", max:" << xmax << " }";

        hscroll->set_min_max(xmin, xmax);
#ifdef WIN32
        hscroll->set_step(std::min(xmax - xmin, (int)space.width()) / 2);
#endif // WIN32
#ifdef X11
        hscroll->set_step(std::min(xmax - xmin, (int)space.width()));
#endif // X11

        hscroll->set_value(xpos);
        current_pos.x(xpos);
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

      if (edge && show_h && show_v) {
        edge->place(get_edge_area(new_size));
        edge->to_front();
      }

      if (show_v) {
        vscroll->place(get_vscroll_area(new_size, show_h));
        vscroll->to_front();
      }

      if (show_h) {
        hscroll->place(get_hscroll_area(new_size, show_v));
        hscroll->to_front();
      }

    }

  } // win

} // gui
