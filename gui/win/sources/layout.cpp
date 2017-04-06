/**
* @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
* @brief     C++ API: layout
*
* @file
*/

// --------------------------------------------------------------------------
//
// Common includes
//
#include <map>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "layout.h"
#include "window.h"

namespace gui {

  namespace layout {

    namespace detail {

      void horizontal_lineup_base::layout (const core::size& sz) {
        std::vector<win::window*> children = main->get_children();

        const int count = static_cast<int>(children.size());
        const int space = sz.width() - (border * 2);
        if (count && (space > 0)) {
          const core::size::type width = core::size::type((space - (gap * (count - 1))) / count);
          const core::size::type height = core::size::type(sz.height() - (border * 2));
          core::point::type offset = width + gap;
          core::point::type padding = core::point::type (space - (offset * count - gap));

          core::rectangle area(border, border, width + padding, height);
          offset += padding;
          for(win::window* win : children) {
            win->place(area);
            area.move_x(offset);

            area.width(width);
            offset -= padding;
            padding = 0;
          }
        }
      }

      void vertical_lineup_base::layout (const core::size& sz) {
        std::vector<win::window*> children = main->get_children();

        const int count = static_cast<int>(children.size());
        const int space = sz.height() - (border * 2);
        if (count && (space > 0)) {
          const core::size::type width = core::size::type(sz.width() - (border * 2));
          const core::size::type height = core::size::type((space - (gap * (count - 1))) / count);
          core::point::type offset = height + gap;
          core::point::type padding = core::point::type(space - (offset * count - gap));

          core::rectangle area(border, border, width, height + padding);
          offset += padding;
          for(win::window* win : children) {
            win->place(area);
            area.move_y(offset);

            area.height(height);
            offset -= padding;
            padding = 0;
          }
        }
      }

    } // detail

    void attach::layout (const core::size& sz) {
      typedef std::map<win::window*, core::rectangle> window_places;
      typedef window_places::iterator iterator;
      window_places places;

      for(attachment a : attachments) {
        iterator i = places.find(a.target);
        if (i != places.end()) {
          a.adjust(i->second, a.source->client_size(), a.source->place());
        } else {
          core::rectangle& r = places[a.target];
          r = a.target->place();
          a.adjust(r, a.source->client_size(), a.source->place());
        }
      }
      for (auto i : places) {
        i.first->place(i.second);
      }
    }

    core::point::type attach::attachment::adjust (const core::size& sz,
                                                  const core::rectangle& outer) const {
      switch( where ) {
        case Where::width:
          return calc(sz.width());
        case Where::x:
          return calc(outer.x());
        case Where::x2:
          return calc(outer.x2());
        case Where::height:
          return calc(sz.height());
        case Where::y:
          return calc(outer.y());
        case Where::y2:
          return calc(outer.y2());
      }
      return 0;
    }

    void attach::attachment::adjust (core::rectangle& rect,
                                     const core::size& sz,
                                     const core::rectangle& outer) const {
      switch (what) {
        case What::left:
          rect.x(adjust(sz, outer));
          break;
        case What::right:
          rect.x2(adjust(sz, outer));
          break;
        case What::top:
          rect.y(adjust(sz, outer));
          break;
        case What::bottom:
          rect.y2(adjust(sz, outer));
          break;
      }

    }

  } // layout

} // gui