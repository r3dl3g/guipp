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
      vscroll.register_event_handler(scroll_event([&](int y) {
        move_children(core::point(0, y - current_pos.y()));
      }));
      hscroll.register_event_handler(scroll_event([&](int x) {
        move_children(core::point(x - current_pos.x(), 0));
      }));
    }

    void scroll_view::move_children (const core::point& delta) {
      std::vector<window*> children = get_children();
      for(window* win : children) {
        if ((win != &vscroll) && (win != &hscroll) && (win != &edge)) {
          win->move(win->position() - delta);
        }
      }
      current_pos += delta;
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
        vscroll.place(get_vscroll_area());
        vscroll.to_front();
        if (is_hscroll_bar_enabled()) {
          hscroll.place(get_hscroll_area());
          edge.place(get_edge_area());
          edge.set_visible();
          edge.to_front();
        }
      }
    }

    void scroll_view::enable_hscroll_bar (bool enable) {
      hscroll.set_visible(enable);
      if (enable) {
        hscroll.place(get_hscroll_area());
        hscroll.to_front();
        if (is_vscroll_bar_enabled()) {
          vscroll.place(get_vscroll_area());
          edge.place(get_edge_area());
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
    
    void scroll_view::calc_area () {
      core::rectangle space = client_area();

      std::vector<window*> children = get_children();
      core::rectangle required= get_visible_area(true);
      for(window* win : children) {
        if ((win != &vscroll) && (win != &hscroll) && (win != &edge)) {
          required |= win->place() + core::size(1, 1);
        }
      }

      LogDebug << "Space:" << space << ", Required:" << required;

      bool show_h = (required.x() < space.x()) || (required.x2() > space.x2());
      if (show_h) {
        space.height(space.height() - scroll_bar::get_scroll_bar_width());
      }

      bool show_v = (required.y() < space.y()) || (required.y2() > space.y2());
      if (show_v) {
        space.width(space.width() - scroll_bar::get_scroll_bar_width());

        if (!show_h) {
          // re-check h
          show_h = (required.x() < space.x()) || (required.x2() > space.x2());
          if (show_h) {
            space.height(space.height() - scroll_bar::get_scroll_bar_width());
          }
        }

        int ymin = std::min(core::point::type(required.y() - space.y()), core::point::type(0));
        int ymax = std::max(core::point::type(required.y2() - space.y2()), core::point::type(0));
        int ypos = 0;

        LogDebug << "Y:{ min:" << ymin << ", pos:" << ypos << ", max:" << ymax << " }";

        vscroll.set_min_max(ymin, ymax);

        vscroll.set_value(ypos);
        current_pos.y(ypos);
      }

      if (show_h) {
        int xmin = std::min(core::point::type(required.x() - space.x()), core::point::type(0));
        int xmax = std::max(core::point::type(required.x2() - space.x2()), core::point::type(0));
        int xpos = 0;

        LogDebug << "X:{ min:" << xmin << ", pos:" << xpos << ", max:" << xmax << " }";

        hscroll.set_min_max(xmin, xmax);

        hscroll.set_value(xpos);
        current_pos.x(xpos);
      }

      vscroll.set_visible(show_v);
      hscroll.set_visible(show_h);
      edge.set_visible(show_h && show_v);

      if (show_h && show_v) {
        edge.place(get_edge_area());
        edge.to_front();
      }

      if (show_v) {
        vscroll.place(get_vscroll_area());
        vscroll.to_front();
      }

      if (show_h) {
        hscroll.place(get_hscroll_area());
        hscroll.to_front();
      }

    }

    core::rectangle scroll_view::get_vscroll_area () const {
      core::rectangle r(size());
      r.x(r.x2() - scroll_bar::get_scroll_bar_width());
      r.width(scroll_bar::get_scroll_bar_width());
      if (is_hscroll_bar_enabled()) {
        r.height(r.height() - scroll_bar::get_scroll_bar_width());
      }
      return r;
    }

    core::rectangle scroll_view::get_hscroll_area () const {
      core::rectangle r(size());
      r.y(r.y2() - scroll_bar::get_scroll_bar_width());
      r.height(scroll_bar::get_scroll_bar_width());
      if (is_vscroll_bar_enabled()) {
        r.width(r.width() - scroll_bar::get_scroll_bar_width());
      }
      return r;
    }

    core::rectangle scroll_view::get_edge_area () const {
      core::size sz = size();
      return core::rectangle(sz.width() - scroll_bar::get_scroll_bar_width(),
                             sz.height() - scroll_bar::get_scroll_bar_width(),
                             scroll_bar::get_scroll_bar_width(),
                             scroll_bar::get_scroll_bar_width());
    }

    core::rectangle scroll_view::get_visible_area (bool without_scrolls) const {
      core::rectangle r = client_area();
      if (without_scrolls || is_vscroll_bar_enabled()) {
        r.width(r.width() - scroll_bar::get_scroll_bar_width());
      }
      if (without_scrolls || is_hscroll_bar_enabled()) {
        r.height(r.height() - scroll_bar::get_scroll_bar_width());
      }
      return r;
    }

  } // win

} // gui
