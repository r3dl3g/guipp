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
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui_types.h"
#include "bind_method.h"


namespace gui {

  namespace win {
    class container;
    class window;
  }

  namespace layout {

    // --------------------------------------------------------------------------
    class standard_layout {
    public:
      standard_layout (win::container*)
      {}
    };

    class layout_base {
    public:
      typedef void (callback)(const core::size& sz);
      typedef std::function<callback> callback_function;


      layout_base (win::container* m)
        : main(m)
      {}

      void init(callback_function f);

      static std::vector<win::window*> get_children (win::container*);
      static void place_child (win::window*, const core::rectangle&);
      static void hide_children (std::vector<win::window*>&);

    protected:
      win::container* main;
    };

    template<unsigned short width, unsigned short border = 0, unsigned short gap = 0>
    class horizontal_lineup : protected layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      horizontal_lineup (win::container* m)
        : super(m)
      {
        super::init(core::bind_method(this, &horizontal_lineup::layout));
      }

      void layout (const core::size& sz) {
        std::vector<win::window*> children = get_children(main);
        const type border2 = (border * 2);
        const type space = sz.width() - border2;

        if (children.size()) {
          if (space > 0) {
            const type height = sz.height() - border2;
            const type offset = width + gap;
            core::rectangle area(border, border, width, height);
            for(win::window* win : children) {
              place_child(win, area);
              area.move_x(offset);
            }
          } else {
            hide_children(children);
          }
        }
      }
    };

    template<unsigned short height, unsigned short border = 0, unsigned short gap = 0>
    class vertical_lineup : protected layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      vertical_lineup (win::container* m)
        :super(m)
      {
        super::init(core::bind_method(this, &vertical_lineup::layout));
      }

      void layout (const core::size& sz) {
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
    class grid_lineup : protected layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      grid_lineup (win::container* m)
        : super(m)
      {
        super::init(core::bind_method(this, &grid_lineup::layout));
      }

      void layout (const core::size& sz) {
        std::vector<win::window*> children = get_children(main);
        const type xmax = sz.width() - border;
        const type ymax = sz.height() - border;

        if (children.size()) {
          if ((xmax > border) && (ymax > border)) {
            const type xoffset = width + gap;
            const type yoffset = height + gap;
            core::rectangle area(border, border, width, height);
            for(win::window* win : children) {
              place_child(win, area);
              area.move_x(xoffset);
              if (area.x2() > xmax) {
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

    template<unsigned short border = 0, unsigned short gap = 0>
    class horizontal_adaption : protected layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      horizontal_adaption (win::container* m)
        : super(m)
      {
        super::init(core::bind_method(this, &horizontal_adaption::layout));
      }

      void layout (const core::size& sz) {
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
            for(win::window* win : children) {
              place_child(win, area);
              area.move_x(offset);
            }
          } else {
            hide_children(children);
          }
        }
      }
    };

    template<unsigned short border = 0, unsigned short gap = 0>
    class vertical_adaption : protected layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      vertical_adaption (win::container* m)
        : super(m)
      {
        super::init(core::bind_method(this, &vertical_adaption::layout));
      }

      void layout (const core::size& sz) {
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
    class grid_adaption : protected layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      grid_adaption (win::container* m)
        : super(m)
      {
        super::init(core::bind_method(this, &grid_adaption::layout));
      }

      void layout (const core::size& sz) {
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


    enum class What {
      left,
      right,
      top,
      bottom
    };

    enum class Where {
      x,
      x2,
      y,
      y2,
      width,
      height
    };

    namespace detail {

      template<int O, int R>
      core::point_type scale (core::point_type a) {
        return core::point_type(a * core::point_type(R) / 10000.0 + O);
      }

      template<Where W, int O, int R>
      struct source {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::x, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>(outer.x());
        }
      };

      template<int O, int R>
      struct source<Where::x2, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>(outer.x2());
        }
      };

      template<int O, int R>
      struct source<Where::y, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>(outer.y());
        }
      };

      template<int O, int R>
      struct source<Where::y2, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>(outer.y2());
        }
      };

      template<int O, int R>
      struct source<Where::width, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>(sz.width());
        }
      };

      template<int O, int R>
      struct source<Where::height, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>(sz.height());
        }
      };

      template<What T, Where W, int O, int R>
      struct target {
        void operator() (core::rectangle&,
                         const core::size&,
                         const core::rectangle&);
      };

      template<Where W, int O, int R>
      struct target<What::left, W, O, R> {
        void operator() (core::rectangle& rect,
                         const core::size& sz,
                         const core::rectangle& outer) {
          rect.x(source<W, O, R>::calc(sz, outer));
        }
      };

      template<Where W, int O, int R>
      struct target<What::right, W, O, R> {
        void operator() (core::rectangle& rect,
                         const core::size& sz,
                         const core::rectangle& outer) {
          rect.x2(source<W, O, R>::calc(sz, outer));
        }
      };

      template<Where W, int O, int R>
      struct target<What::top, W, O, R> {
        void operator() (core::rectangle& rect,
                         const core::size& sz,
                         const core::rectangle& outer) {
          rect.y(source<W, O, R>::calc(sz, outer));
        }
      };

      template<Where W, int O, int R>
      struct target<What::bottom, W, O, R> {
        void operator() (core::rectangle& rect,
                         const core::size& sz,
                         const core::rectangle& outer) {
          rect.y2(source<W, O, R>::calc(sz, outer));
        }
      };

      struct attachment {
        typedef void(adjust_function) (core::rectangle&,
                                     const core::size&,
                                     const core::rectangle&);

        win::window* target;
        win::window* source;
        std::function<adjust_function> adjust;
      };

    }

    class attach {
    public:
      attach (win::container*);

      template<What what, Where where, int offset>
      void abs (win::window* target, win::window* source) {
        attachments.push_back({target, source, detail::target<what, where, offset, 10000>()});
      }

      template<What what, int relativ, int offset = 0>
      void rel (win::window* target, win::window* source) {
        const Where where = (what == What::left) || (what == What::right) ? Where::width : Where::height;
        attachments.push_back({target, source, detail::target<what, where, offset, relativ>()});
      }

      void layout (const core::size& sz);

    private:
      std::vector<detail::attachment> attachments;
    };

  }
}
