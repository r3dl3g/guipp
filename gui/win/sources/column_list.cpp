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

    } // detail

    window_class column_list::clazz;

    column_list::column_list () {
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
      register_event_handler(size_event([&](const core::size& sz) {
        float full_weigth = 0.0F;
        int full_width = 0;
        for (const column& c : columns) {
          full_weigth += c.weight;
          full_width += c.min_width;
        }
        int diff = sz.width() - full_width;
        for (column& c : columns) {
          c.width = c.min_width + std::max((int)(diff * c.weight / full_weigth), 0);
        }
        this->list.resize(sz);
      }));
    }

    column_list::~column_list() {
      //dtor
    }

    void column_list::create (const container& parent,
                              const core::rectangle& place) {
      super::create(clazz, parent, place);
      list.create(*reinterpret_cast<container*>(this), core::rectangle(place.size()));
      list.set_visible(true);
    }

    void column_list::set_drawer (std::function<cell_draw> drawer,
                                  int item_height) {
      this->drawer = drawer;
      list.set_drawer(core::easy_bind(this, &column_list::draw_line), item_height);
    }

    void column_list::draw_line (draw::graphics& g,
                                 int idx,
                                 const core::rectangle& place,
                                 bool selected) {
      if (drawer) {
        int idy = 0;
        core::rectangle r = place;
        for (column c : columns) {
          r.width(c.width);
          drawer(g, idx, idy, r, selected);
          r.move_x(c.width);
          ++idy;
        }
      }
    }


  } // win

} // gui
#include "column_list.h"
