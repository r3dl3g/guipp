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
#include "column_list.h"


namespace gui {

#ifdef WIN32
#endif // WIN32

#ifdef X11
#endif // X11

  namespace layout {
  }

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------

      void column_list_header::init () {
        core::rectangle r = client_area();
        r.width(2);
        std::size_t count = main.get_columns().size();
        sliders.resize(count);
        for (int i = 0; i < count; ++i) {
          slider& s = sliders[i];
          column_info& c = main.get_column(i);
          r.move_x(c.width - 1);
          s.create(*this, r);
          s.set_visible();
          s.register_event_handler(win::slider_event([=, &c] (int dx) {
            c.width += dx;
            c.min_width = c.width;
            main.list.redraw_now();
            do_layout();
          }));
          r.move_x(2);
        }
        register_event_handler(win::paint_event([&](draw::graphics& g) {
          using namespace draw;

          core::rectangle r = client_area();
          g.fill(rectangle(r), color::buttonColor);

          int idy = 1;
          for (const column_info& c : main.get_columns()) {
            r.width(c.width - 1);
            frame::raised_relief(g, r);
            g.text(text_box(ostreamfmt(idy << '.'), r, center), font::system(), color::windowTextColor);
            r.move_x(c.width);
            ++idy;
          }

        }));

      }

      void column_list_header::do_layout () {
        core::rectangle r(1, 0, 2, client_size().height());
        std::size_t count = main.get_columns().size();
        if (count == sliders.size()) {
          for (int i = 0; i < count; ++i) {
            slider& s = sliders[i];
            const column_info& c = main.get_column(i);
            r.move_x(c.width - 2);
            s.place(r);
            r.move_x(2);
          }
        }
      }

      // --------------------------------------------------------------------------
      window_class base_column_list::clazz;

      base_column_list::base_column_list ()
        : header(*this)
      {
        if (!clazz.is_valid()) {
#ifdef WIN32
          clazz = win::window_class::custom_class("column_list",
            CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW,
            WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
            WS_EX_NOPARENTNOTIFY,
            nullptr,
            LoadCursor(nullptr, IDC_ARROW),
            (HBRUSH)(COLOR_WINDOW + 1));
#endif // WIN32
#ifdef X11
          clazz = win::window_class::custom_class("column_list");
#endif //X11
        }
        register_event_handler(size_event(core::easy_bind(this, &base_column_list::do_layout_intern)));
      }

      base_column_list::~base_column_list() {
        //dtor
      }

      void base_column_list::set_columns (const columns_t& val) {
        columns = val;
        header.init();
      }

      void base_column_list::set_column (columns_t::size_type i, const column_info& c) {
        columns[i] = c;
        header.do_layout();
      }

      void base_column_list::do_layout () {
        do_layout_intern(client_size());
      }

      void base_column_list::do_layout_intern (const core::size& sz) {
        float full_weigth = 0.0F;
        int full_width = 0;
        for (const column_info& c : columns) {
          full_weigth += c.weight;
          full_width += c.min_width;
        }

        int diff = sz.width() - full_width;
        if (list.is_scroll_bar_visible()) {
          diff -= scroll_bar::get_scroll_bar_width();
        }
        for (column_info& c : columns) {
          c.width = c.min_width + std::max((int)(diff * c.weight / full_weigth), 0);
        }
        header.resize(core::size(sz.width(), 20));
        header.do_layout();
        this->list.resize(sz - core::size(0, 20));
      }

      void base_column_list::create (const container& parent,
                                const core::rectangle& place) {
        super::create(clazz, parent, place);
        header.create(*reinterpret_cast<container*>(this), core::rectangle(0, 0, place.width(), 20));
        header.set_visible();
        list.create(*reinterpret_cast<container*>(this), core::rectangle(0, 20, place.width(), place.height() - 20));
        list.set_visible();
      }

    }

    void simple_column_list::set_drawer (std::function<cell_draw> drawer,
                                  core::size::type item_height) {
      this->drawer = drawer;
      list.set_drawer(core::easy_bind(this, &simple_column_list::draw_cells), { item_height, item_height });
    }

    void simple_column_list::draw_cells (int idx,
                                         draw::graphics& g,
                                         const core::rectangle& place,
                                         bool selected) {
      if (drawer) {
        int idy = 0;
        core::rectangle r = place;
        for (const column_info& c : get_columns()) {
          r.width(c.width - 1);
          drawer(idx, idy, c, g, r, selected);
          r.move_x(c.width);
          ++idy;
        }
      }
    }

  } // win

} // gui
#include "column_list.h"
