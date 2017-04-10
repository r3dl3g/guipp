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

    // --------------------------------------------------------------------------
    window_class scroll_view::clazz(
    #ifdef WIN32
      win::window_class::custom_class("MyScrollView",
                                      CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW,
                                      WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                                      WS_EX_NOPARENTNOTIFY,
                                      nullptr,
                                      IDC_ARROW,
                                      (HBRUSH)(COLOR_WINDOW + 1))
    #endif // WIN32
    #ifdef X11
      win::window_class::custom_class("MyScrollView")
    #endif //X11
    );

    // --------------------------------------------------------------------------
    scroll_view::scroll_view () {
      layout.init(this, &vscroll, &hscroll, &edge);

      vscroll.register_event_handler(scroll_event([&](core::point::type y) {
                                       move_children(core::point(0, y - layout.get_current_pos().y()));
                                     }));

      hscroll.register_event_handler(scroll_event([&](core::point::type x) {
                                       move_children(core::point(x - layout.get_current_pos().x(), 0));
                                     }));
    }

    void scroll_view::create (const container& parent,
                              const core::rectangle& place) {
      super::create(clazz, parent, place);
      core::size sz = place.size();
      vscroll.create(*this, layout::scroll_view::get_vscroll_area(sz, true));
      hscroll.create(*this, layout::scroll_view::get_hscroll_area(sz, true));
      edge.create(*this, layout::scroll_view::get_edge_area(sz));
    }

    void scroll_view::move_children (const core::point& delta) {
      std::vector<window*> children = get_children();
      for (window* win : children) {
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
      return {core::point::type(hscroll.get_value()),
            core::point::type(vscroll.get_value())};
    }

    void scroll_view::enable_vscroll_bar (bool enable) {
      vscroll.set_visible(enable);
      if (enable) {
        bool hscroll_bar_enabled = is_hscroll_bar_enabled();
        vscroll.place(layout::scroll_view::get_vscroll_area(size(), hscroll_bar_enabled));
        vscroll.to_front();
        if (hscroll_bar_enabled) { ;
          hscroll.place(layout::scroll_view::get_hscroll_area(size(), enable));
          edge.place(layout::scroll_view::get_edge_area(size()));
          edge.set_visible();
          edge.to_front();
        }
      }
    }

    void scroll_view::enable_hscroll_bar (bool enable) {
      hscroll.set_visible(enable);
      if (enable) {
        bool vscroll_bar_enabled = is_vscroll_bar_enabled();
        hscroll.place(layout::scroll_view::get_hscroll_area(size(), vscroll_bar_enabled));
        hscroll.to_front();
        if (vscroll_bar_enabled) {
          vscroll.place(layout::scroll_view::get_vscroll_area(size(), enable));
          edge.place(layout::scroll_view::get_edge_area(size()));
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
      layout.layout(size(), this);
    }
  } // win

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    scroll_view::scroll_view ()
      : vscroll(nullptr)
      , hscroll(nullptr)
      , edge(nullptr)
      , me(core::bind_method(this, &scroll_view::handle_child_move))
      , se(core::bind_method(this, &scroll_view::handle_child_size))
    {}

    void scroll_view::init (win::container* main,
                            win::vscroll_bar* vscroll,
                            win::hscroll_bar* hscroll,
                            win::client_window* edge) {
      this->main = main;
      this->vscroll = vscroll;
      this->hscroll = hscroll;
      this->edge = edge;
    }

    void scroll_view::set_current_pos (const core::point& pt) {
      current_pos = pt;
    }

    core::point scroll_view::get_current_pos () const {
      return current_pos;
    }

    core::rectangle scroll_view::get_vscroll_area (const core::size& sz, bool hscroll_bar_enabled) {
      core::rectangle r(sz);
      r.x(r.x2() - win::scroll_bar::get_scroll_bar_width());
      r.width(win::scroll_bar::get_scroll_bar_width());
      if (hscroll_bar_enabled) {
        r.height(r.height() - win::scroll_bar::get_scroll_bar_width());
      }
      return r;
    }

    core::rectangle scroll_view::get_hscroll_area (const core::size& sz, bool vscroll_bar_enabled) {
      core::rectangle r(sz);
      r.y(r.y2() - win::scroll_bar::get_scroll_bar_width());
      r.height(win::scroll_bar::get_scroll_bar_width());
      if (vscroll_bar_enabled) {
        r.width(r.width() - win::scroll_bar::get_scroll_bar_width());
      }
      return r;
    }

    core::rectangle scroll_view::get_visible_area (const core::size& sz) {
      return core::rectangle(sz - core::size{ win::scroll_bar::get_scroll_bar_width(),
                                              win::scroll_bar::get_scroll_bar_width() });
    }

    core::rectangle scroll_view::get_edge_area (const core::size& sz) {
      return core::rectangle(sz.width() - win::scroll_bar::get_scroll_bar_width(),
                             sz.height() - win::scroll_bar::get_scroll_bar_width(),
                             win::scroll_bar::get_scroll_bar_width(),
                             win::scroll_bar::get_scroll_bar_width());
    }

    void scroll_view::layout (const core::size& new_size, win::container* main) {
      core::rectangle space(new_size);

      std::vector<win::window*> children = main->get_children();
      core::rectangle required = get_visible_area(new_size);
      for(win::window* win : children) {
        if ((win != vscroll) && (win != hscroll) && (win != edge)) {
          required |= win->place() + core::size(1, 1);
          win->unregister_event_handler(me);
          win->register_event_handler(me);
          win->unregister_event_handler(se);
          win->register_event_handler(se);
        }
      }

      LogDebug << "Space:" << space << ", Required:" << required;

      bool show_h = hscroll && (required.x() < space.x()) || (required.x2() > space.x2());
      if (show_h) {
        space.height(space.height() - win::scroll_bar::get_scroll_bar_width());
      }

      bool show_v = vscroll && (required.y() < space.y()) || (required.y2() > space.y2());
      if (show_v) {
        space.width(space.width() - win::scroll_bar::get_scroll_bar_width());

        if (!show_h) {
          // re-check h
          show_h = hscroll && (required.x() < space.x()) || (required.x2() > space.x2());
          if (show_h) {
            space.height(space.height() - win::scroll_bar::get_scroll_bar_width());
          }
        }

        core::point::type ymin = std::min(core::point::type(required.y() - space.y()), core::point::type(0));
        core::point::type ymax = std::max(core::point::type(required.y2() - space.y2()), core::point::type(0));
        core::point::type ypos = 0;

        LogDebug << "Y:{ min:" << ymin << ", pos:" << ypos << ", max:" << ymax << " }";

        vscroll->set_min_max(ymin, ymax);
#ifdef WIN32
        vscroll->set_step(std::min(ymax - ymin, space.height()) / 2);
#endif // WIN32
#ifdef X11
        vscroll->set_step(std::min(ymax - ymin, space.height()));
#endif // X11
        vscroll->set_value(ypos);
        current_pos.y(ypos);
      }

      if (show_h) {
        core::point::type xmin = std::min(core::point::type(required.x() - space.x()), core::point::type(0));
        core::point::type xmax = std::max(core::point::type(required.x2() - space.x2()), core::point::type(0));
        core::point::type xpos = 0;

        LogDebug << "X:{ min:" << xmin << ", pos:" << xpos << ", max:" << xmax << " }";

        hscroll->set_min_max(xmin, xmax);
#ifdef WIN32
        hscroll->set_step(std::min(xmax - xmin, space.width()) / 2);
#endif // WIN32
#ifdef X11
        hscroll->set_step(std::min(xmax - xmin, space.width()));
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

    void scroll_view::handle_child_move (const core::point&) {
      if ((vscroll->get_state() == win::scroll_bar::Nothing_pressed) &&
          (hscroll->get_state() == win::scroll_bar::Nothing_pressed)) {
        layout(main->size(), main);
      }
    }

    void scroll_view::handle_child_size (const core::size&) {
      layout(main->size(), main);
    }
    // --------------------------------------------------------------------------

  } // layout

} // gui
