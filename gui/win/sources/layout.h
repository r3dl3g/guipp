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
      typedef void (size_callback)(const core::size& sz);
      typedef void (show_callback)();

      layout_base (win::container* m)
        : main(m)
      {}

      void init(std::function<size_callback> f1);
      void init(std::function<size_callback> f1, std::function<show_callback> f2);

      static std::vector<win::window*> get_children (win::container*);
      static void place_child (win::window*, const core::rectangle&);
      static bool is_child_visible (win::window*);
      static void hide_children (std::vector<win::window*>&);

      void update ();
      core::size get_main_size () const;

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
        super::init(core::bind_method(this, &horizontal_lineup::layout), [&](){
          layout(get_main_size());
        });
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
        update();
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
        super::init(core::bind_method(this, &vertical_lineup::layout), [&](){
          layout(get_main_size());
        });
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
        update();
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
        super::init(core::bind_method(this, &grid_lineup::layout), [&](){
          layout(get_main_size());
        });
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
        update();
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
        super::init(core::bind_method(this, &horizontal_adaption::layout), [&](){
          layout(get_main_size());
        });
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
        update();
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
        super::init(core::bind_method(this, &vertical_adaption::layout), [&](){
          layout(get_main_size());
        });
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
        update();
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
        super::init(core::bind_method(this, &grid_adaption::layout), [&](){
          layout(get_main_size());
        });
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
        update();
      }
    };


    template<int top_height, int bottom_height, int left_width, int right_width>
    class border_layout : protected layout_base {
    public:
      typedef layout_base super;

      border_layout (win::container* m)
        : super(m)
      {
        super::init(core::bind_method(this, &border_layout::layout), [&](){
          layout(get_main_size());
        });
      }

      void set_center (win::window* center) {
        this->center = center;
      }

      void set_top (win::window* top) {
        this->top = top;
      }

      void set_bottom (win::window* bottom) {
        this->bottom = bottom;
      }

      void set_left (win::window* left) {
        this->left = left;
      }

      void set_right (win::window* right) {
        this->right = right;
      }

      void set_center_top_bottom_left_right (win::window* center,
                                             win::window* top,
                                             win::window* bottom,
                                             win::window* left,
                                             win::window* right) {
        this->center = center;
        this->top = top;
        this->bottom = bottom;
        this->left = left;
        this->right = right;
      }

      void layout (const core::size& sz) {
        core::rectangle r(core::point::zero, sz);
        if (top && is_child_visible(top)) {
          place_child(top, core::rectangle(0, 0, sz.width(), top_height));
          r.y(top_height);
        }
        if (bottom && is_child_visible(bottom)) {
          place_child(bottom, core::rectangle(0, sz.height() - bottom_height, sz.width(), bottom_height));
          r.y2(r.y2() - bottom_height);
        }
        if (left && is_child_visible(left)) {
          place_child(left, core::rectangle(r.top_left(), core::point(left_width, r.y2())));
          r.x(left_width);
        }
        if (right && is_child_visible(right)) {
          place_child(right, core::rectangle(core::point(r.x2() - right_width, r.y()), r.bottom_right()));
          r.x2(r.x2() - right_width);
        }
        if (center && is_child_visible(center)) {
          place_child(center, r);
        }
        update();
      }

    private:
      win::window* center;
      win::window* top;
      win::window* bottom;
      win::window* left;
      win::window* right;
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
      struct scale {
        static core::point_type calc (core::point_type a) {
          return core::point_type(a * core::point_type(R) / 10000.0 + O);
        }
      };

      template<int O>
      struct scale<O, 10000> {
        static core::point_type calc (core::point_type a) {
          return core::point_type(a + O);
        }
      };

      template<int O>
      struct scale<O, 0> {
        constexpr static core::point_type calc (core::point_type a) {
          return core::point_type(O);
        }
      };

      template<Where W, int O, int R = 10000>
      struct source {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::x, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>::calc(outer.x());
        }
      };

      template<int O, int R>
      struct source<Where::x2, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>::calc(outer.x2());
        }
      };

      template<int O, int R>
      struct source<Where::y, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>::calc(outer.y());
        }
      };

      template<int O, int R>
      struct source<Where::y2, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>::calc(outer.y2());
        }
      };

      template<int O, int R>
      struct source<Where::width, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>::calc(sz.width());
        }
      };

      template<int O, int R>
      struct source<Where::height, O, R> {
        static core::point_type calc (const core::size& sz,
                                      const core::rectangle& outer) {
          return scale<O, R>::calc(sz.height());
        }
      };

      template<What T, Where W, int O, int R = 10000>
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

    constexpr int make_relative (double r) {
      return static_cast<int>(r * 10000.0);
    }

    class attach {
    public:
      attach (win::container*);

      template<What what, Where where, int offset = 0>
      void attach_fix (win::window* target, win::window* source) {
        attachments.push_back({target, source, detail::target<what, where, offset, 10000>()});
      }

      template<What what, int relativ, int offset = 0>
      void attach_relative (win::window* target, win::window* source) {
        const Where where = (what == What::left) || (what == What::right) ? Where::width : Where::height;
        attachments.push_back({target, source, detail::target<what, where, offset, relativ>()});
      }

      void layout (const core::size& sz);

    private:
      std::vector<detail::attachment> attachments;
    };

  }
}
