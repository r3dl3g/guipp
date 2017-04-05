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
      void column_list_layout::set_column_width (columns_info_list::size_type i, core::size::type w) {
        columns[i].width = w;
        header->do_layout();
        list->redraw_now();
      }

      void column_list_layout::set_header_and_list (detail::column_list_header* h, win::list* l) {
        list = l;
        header = h;
        header->get_layout().set_column_list_layout(this);
      }

      void column_list_layout::layout (const core::size& sz) {
        float full_weigth = 0.0F;
        int full_width = 0;
        for (const column_info& c : columns) {
          full_weigth += c.weight;
          full_width += c.min_width;
        }

        int diff = sz.width() - full_width;
        if (list->is_scroll_bar_visible()) {
          diff -= scroll_bar::get_scroll_bar_width();
        }
        for (column_info& c : columns) {
          c.width = c.min_width + std::max((int)(diff * c.weight / full_weigth), 0);
        }
        header->resize(core::size(sz.width(), 20));
        header->do_layout();
        list->resize(sz - core::size(0, 20));
      }

      void column_list_layout::set_columns (const columns_info_list& val) {
        columns = val;
        header->init();
      }

      void column_list_layout::set_column (columns_info_list::size_type i, const column_info& c) {
        columns[i] = c;
        header->do_layout();
        list->redraw_now();
      }

      // --------------------------------------------------------------------------
      void column_list_header_layout::layout (const core::size& new_size) {
        if (column_layout && sliders) {
          core::rectangle r(1, 0, 2, new_size.height());
          std::size_t count = column_layout->get_column_count();
          if (count == sliders->size()) {
            for (int i = 0; i < count; ++i) {
              detail::slider& s = sliders->at(i);
              r.move_x(column_layout->get_column_width(i) - 2);
              s.place(r);
              r.move_x(2);
            }
          }
        }
      }

      // --------------------------------------------------------------------------
      column_list_header::column_list_header (column_list_layout& m)
        : main_layout(m)
      {
        register_event_handler(win::paint_event([&](draw::graphics& g) {
          using namespace draw;

          core::rectangle r = client_area();
          g.fill(rectangle(r), color::buttonColor);

          std::size_t count = main_layout.get_column_count();
          for (int i = 0; i < count; ++i) {
            core::size::type w = main_layout.get_column_width(i);
            r.width(w - 1);
            frame::raised_relief(g, r);
            g.text(text_box(ostreamfmt((i + 1) << '.'), r, center), font::system(), color::windowTextColor);
            r.move_x(w);
          }

        }));
      }

      // --------------------------------------------------------------------------
      void column_list_header::init () {
        get_layout().set_sliders(&sliders);

        core::rectangle r(0, 0, 2, size().height());
        std::size_t count = main_layout.get_column_count();
        sliders.resize(count);
        for (int i = 0; i < count; ++i) {
          slider& s = sliders[i];
          r.move_x(main_layout.get_column_width(i) - 2);
          s.create(*this, r);
          s.set_visible();
          s.register_event_handler(win::slider_event([=] (int dx) {
            main_layout.set_column_width(i, main_layout.get_column_width(i) + dx);
            do_layout();
            redraw_later();
          }));
          r.move_x(2);
        }

      }

      // --------------------------------------------------------------------------
      window_class base_column_list::clazz;

      base_column_list::base_column_list ()
        : header(get_layout())
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
        get_layout().set_header_and_list(&header, &list);
        register_event_handler(size_event(core::easy_bind((super*)this, &super::do_layout_for_size)));
      }

      base_column_list::~base_column_list() {
        //dtor
      }

      void base_column_list::create (const container& parent,
                                const core::rectangle& place) {
        super::create(clazz, parent, place);
        header.create(*reinterpret_cast<container*>(this), core::rectangle(0, 0, place.width(), 20));
        header.init();
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
        core::rectangle r = place;
        std::size_t count = get_layout().get_column_count();
        for (int i = 0; i < count; ++i) {
          core::size::type w = get_layout().get_column_width(i);
          r.width(w - 1);
          drawer(idx, i, get_layout().get_column(i), g, r, selected);
          r.move_x(w);
        }
      }
    }

  } // win

} // gui
#include "column_list.h"
