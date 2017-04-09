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

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <vector>
#include <algorithm>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui_types.h"

namespace gui {

  namespace win {
    class container;
    class window;
  }

  namespace layout {

    // --------------------------------------------------------------------------
    class standard_layout {
    public:
      void layout (const core::size& new_size, win::container*) {}
    };

    namespace detail {

      class layout_base {
      public:
        static std::vector<win::window*> get_children (win::container*);
        static void place_child (win::window*, const core::rectangle&);
        static void hide_children (std::vector<win::window*>&);
      };
    }

    template<unsigned short width, unsigned short border = 0, unsigned short gap = 0>
    class horizontal_lineup : protected detail::layout_base {
    public:
      typedef core::size::type type;

      void layout (const core::size& sz, win::container* main) {
        std::vector<win::window*> children = get_children(main);
        const type border2 = (border * 2);
        const type space = sz.width() - border2;

        if (children.size()) {
          if (space > 0) {
            const type height = sz.height() - border2;
            const type offset = width + gap;
            core::rectangle area(border, border, width, height);
            std::for_each(children.begin(), children.end(), [&](win::window* win) {
              place_child(win, area);
              area.move_x(offset);
            });
          } else {
            hide_children(children);
          }
        }
      }
    };

    template<unsigned short height, unsigned short border = 0, unsigned short gap = 0>
    class vertical_lineup : protected detail::layout_base {
    public:
      typedef core::size::type type;

      inline void layout (const core::size& sz, win::container* main) {
        std::vector<win::window*> children = get_children(main);
        const type border2 = (border * 2);
        const type space = sz.height() - border2;

        if (children.size()) {
          if (space > 0) {
            const type width = sz.width() - (border * 2);
            const type offset = height + gap;

            core::rectangle area(border, border, width, height);
            for(win::window* win : children) {
              place_child(win, area);
              area.move_y(offset);
            }
          } else {
            hide_children(children);
          }
        }
      }
    };

    template<unsigned short width, unsigned short height, unsigned short border = 0, unsigned short gap = 0>
    class grid_lineup : protected detail::layout_base {
    public:
      typedef core::size::type type;

      void layout (const core::size& sz, win::container* main) {
        std::vector<win::window*> children = get_children(main);
        const type xmax = sz.width() - border;
        const type ymax = sz.height() - border;

        if (children.size()) {
          if ((xmax > border) && (ymax > border)) {
            const type xoffset = width + gap;
            const type yoffset = height + gap;
            core::rectangle area(border, border, width, height);
            std::for_each(children.begin(), children.end(), [&](win::window* win) {
              place_child(win, area);
              area.move_x(xoffset);
              if (area.x2() > xmax) {
                area.move_to_x(border);
                area.move_y(yoffset);
              }
            });
          } else {
            hide_children(children);
          }
        }
      }
    };

    template<unsigned short border = 0, unsigned short gap = 0>
    class horizontal_adaption : protected detail::layout_base {
    public:
      typedef core::size::type type;

      void layout (const core::size& sz, win::container* main) {
        std::vector<win::window*> children = get_children(main);
        const std::size_t count = children.size();
        const type border2 = (border * 2);
        const type space = sz.width() - border2;

        if (count) {
          if (space > 0) {
            const type width = (space - (gap * (count - 1))) / count;
            const type height = sz.height() - border2;
            const type offset = width + gap;

            core::rectangle area(border, border, width, height);
            std::for_each(children.begin(), children.end(), [&](win::window* win) {
              place_child(win, area);
              area.move_x(offset);
            });
          } else {
            hide_children(children);
          }
        }
      }
    };

    template<unsigned short border = 0, unsigned short gap = 0>
    class vertical_adaption : protected detail::layout_base {
    public:
      typedef core::size::type type;

      void layout (const core::size& sz, win::container* main) {
        std::vector<win::window*> children = get_children(main);
        const std::size_t count = static_cast<int>(children.size());
        const type border2 = (border * 2);
        const type space = sz.height() - border2;

        if (count) {
          if (space > 0) {
            const type width = sz.width() - border2;
            const type height = (space - (gap * (count - 1))) / count;
            const type offset = height + gap;

            core::rectangle area(border, border, width, height);
            for(win::window* win : children) {
              place_child(win, area);
              area.move_y(offset);
            }
          } else {
            hide_children(children);
          }
        }
      }
    };

    template<unsigned short columns, unsigned short rows, unsigned short border = 0, unsigned short gap = 0>
    class grid_adaption : protected detail::layout_base {
    public:
      typedef core::size::type type;

      void layout (const core::size& sz, win::container* main) {
        std::vector<win::window*> children = get_children(main);
        const type border2 = (border * 2);
        const type xspace = sz.width() - border2;
        const type yspace = sz.height() - border2;

        if (children.size()) {
          if ((xspace > 0) && (yspace > 0)) {
            const type width = (xspace - (gap * (columns - 1))) / columns;
            const type height = (yspace - (gap * (rows - 1))) / rows;
            const type xoffset = width + gap;
            const type yoffset = height + gap;

            core::rectangle area(border, border, width, height);
            int column = 0;
            for(win::window* win : children) {
              place_child(win, area);
              column++;
              if (column < columns) {
                area.move_x(xoffset);
              } else {
                column = 0;
                area.move_to_x(border);
                area.move_y(yoffset);
              }
            }
          } else {
            hide_children(children);
          }
        }
      }
    };


    enum class What : char {
      left,
      right,
      top,
      bottom
    };

    enum class Where : char {
      x,
      x2,
      y,
      y2,
      width,
      height
    };

    class attach {
    public:
      void abs (win::window* target, win::window* source, What what, Where where, short offset) {
        attachments.push_back({target, source, what, where, offset, double_to_short(1.0)});
      }

      void rel (win::window* target, win::window* source, What what, double relative, short offset = 0) {
        attachments.push_back({target, source, what,
                               (what == What::left) || (what == What::right) ? Where::width : Where::height,
                               offset, double_to_short(relative)});
      }

      void layout (const core::size& sz, win::container* main);

    private:
      struct attachment {
        win::window* target;
        win::window* source;
        What what;
        Where where;
        short offset;
        short relative;

        void adjust (core::rectangle& rect,
                     const core::size& sz,
                     const core::rectangle& outer) const;

        core::point::type adjust (const core::size& sz,
                                  const core::rectangle& outer) const;

        inline core::point::type calc (core::point::type a) const {
          return core::point::type ((double)a * short_to_double(relative)) + offset;
        }
      };

      static inline double short_to_double (short offset) {
        return (double)offset / 10000.0;
      }

      static inline short double_to_short (double rel) {
        return short(rel * 10000.0);
      }

      std::vector<attachment> attachments;
    };
  }
}
