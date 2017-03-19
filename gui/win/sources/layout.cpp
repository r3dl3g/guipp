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

  } // layout

} // gui