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

    // --------------------------------------------------------------------------
    class layout_base {
    public:
      inline void add_separator (const win::window* w) {
        separators.push_back(w);
      }

      inline bool is_separator (const win::window* w) const {
        separator_list::const_iterator end = separators.end();
        return std::find(separators.begin(), end, w) != end;
      }

      inline std::size_t separator_count () const {
        return separators.size();
      }

    protected:
      typedef void (size_callback)(const core::size& sz);
      typedef void (show_callback)();

      layout_base (win::container* m)
        : main(m)
      {}

      void init(std::function<size_callback> f1);
      void init(std::function<size_callback> f1, std::function<show_callback> f2);

      std::vector<win::window*> get_children ();
      static void place_child (win::window*, const core::rectangle&);
      static bool is_child_visible (win::window*);
      static void hide_children (std::vector<win::window*>&);

      void update ();
      core::size get_main_size () const;

    protected:
      win::container* main;

      typedef std::vector<const win::window*> separator_list;
      separator_list separators;
    };

    namespace detail {

      // --------------------------------------------------------------------------
      template<orientation>
      class base : public layout_base {
      public:
        typedef core::size::type type;

        base (win::container* m)
          :layout_base(m)
        {}

        type get_dimension1 (const core::size& sz);
        type get_dimension2 (const core::size& sz);
        core::size make_size (type dim1, type dim2);
        core::rectangle get_sep_area (const core::rectangle& area, unsigned short s);
        void move_area (core::rectangle& area, type offs);
      };

      // --------------------------------------------------------------------------
      template<>
      class base<orientation::horizontal> : public layout_base {
      public:
        typedef core::size::type type;

        base (win::container* m)
          :layout_base(m)
        {}

        type get_dimension1 (const core::size& sz) {
          return sz.width();
        }

        type get_dimension2 (const core::size& sz) {
          return sz.height();
        }

        core::size make_size (type dim1, type dim2) {
          return core::size(dim1, dim2);
        }

        core::rectangle get_sep_area (const core::rectangle& area, unsigned short s) {
          return area.with_width(s);
        }

        void move_area (core::rectangle& area, type offs) {
          area.move_x(offs);
        }
      };

      // --------------------------------------------------------------------------
      template<>
      class base<orientation::vertical> : public layout_base {
      public:
        typedef core::size::type type;

        base (win::container* m)
          :layout_base(m)
        {}

        type get_dimension1 (const core::size& sz) {
          return sz.height();
        }

        type get_dimension2 (const core::size& sz) {
          return sz.width();
        }

        core::size make_size (type dim1, type dim2) {
          return core::size(dim2, dim1);
        }

        core::rectangle get_sep_area (const core::rectangle& area, unsigned short s) {
          return area.with_height(s);
        }

        void move_area (core::rectangle& area, type offs) {
          area.move_y(offs);
        }
      };

      // --------------------------------------------------------------------------
      template<orientation H, origin R>
      class lineup_base : public base<H> {
      public:
        typedef base<H> super;
        typedef core::size::type type;

        lineup_base (win::container* m)
          : super(m)
        {}

        core::rectangle init_area (type border, const core::size&, const core::size&, int gap, std::size_t count);
        void move_area (core::rectangle& area, type offs);
      };

      template<orientation H>
      class lineup_base<H, origin::start> : public base<H> {
      public:
        typedef base<H> super;
        typedef core::size::type type;

        lineup_base (win::container* m)
          : super(m)
        {}

        core::rectangle init_area (type border, const core::size& is, const core::size& sz, int, std::size_t) {
          return core::rectangle(core::point(border, border), is);
        }

        void move_area (core::rectangle& area, type offs) {
          super::move_area(area, offs);
        }
      };

      template<>
      class lineup_base<orientation::horizontal, origin::end> : public base<orientation::horizontal> {
      public:
        typedef base<orientation::horizontal> super;
        typedef core::size::type type;

        lineup_base (win::container* m)
          : super(m)
        {}

        core::rectangle init_area (type border, const core::size& is, const core::size& sz, int, std::size_t) {
          return core::rectangle(core::point(sz.width() - is.width() - border, border), is);
        }

        void move_area (core::rectangle& area, type offs) {
          super::move_area(area, -offs);
        }
      };

      template<>
      class lineup_base<orientation::vertical, origin::end> : public base<orientation::vertical> {
      public:
        typedef base<orientation::vertical> super;
        typedef core::size::type type;

        lineup_base (win::container* m)
          : super(m)
        {}

        core::rectangle init_area (type border, const core::size& is, const core::size& sz, int, std::size_t) {
          return core::rectangle(core::point(border, sz.height() - is.height() - border), is);
        }

        void move_area (core::rectangle& area, type offs) {
          super::move_area(area, -offs);
        }
      };

      template<>
      class lineup_base<orientation::horizontal, origin::center> : public base<orientation::horizontal> {
      public:
        typedef base<orientation::horizontal> super;
        typedef core::size::type type;

        lineup_base (win::container* m)
          : super(m)
        {}

        core::rectangle init_area (type border, const core::size& is, const core::size& sz, int gap, std::size_t count) {
          return core::rectangle(core::point((sz.width() - (is.width() * count + (count - 1) * gap)) / 2, border), is);
        }

        void move_area (core::rectangle& area, type offs) {
          super::move_area(area, offs);
        }
      };

      template<>
      class lineup_base<orientation::vertical, origin::center> : public base<orientation::vertical> {
      public:
        typedef base<orientation::vertical> super;
        typedef core::size::type type;

        lineup_base (win::container* m)
          : super(m)
        {}

        core::rectangle init_area (type border, const core::size& is, const core::size& sz, int gap, std::size_t count) {
          return core::rectangle(core::point(border, (sz.height() - (is.height() * count + (count - 1) * gap)) / 2), is);
        }

        void move_area (core::rectangle& area, type offs) {
          super::move_area(area, offs);
        }
      };

    }

    // --------------------------------------------------------------------------
    template<orientation H,
             unsigned short dim1,
             unsigned short border = 0,
             unsigned short gap = 0,
             unsigned short sep = 2,
             origin R = origin::start>
    class lineup_layout : public detail::lineup_base<H, R> {
    public:
      typedef core::size::type type;
      typedef detail::lineup_base<H, R> super;

      lineup_layout (win::container* m)
        : super(m)
      {
        super::init(core::bind_method(this, &lineup_layout::layout), [&](){
          layout(super::get_main_size());
        });
      }

      void layout (const core::size& sz) {
        std::vector<win::window*> children = super::get_children();
        const type border2 = (border * 2);
        const type space = super::get_dimension1(sz) - border2;

        auto count = children.size();
        if (count) {
          if (space > 0) {
            const type dim2 = super::get_dimension2(sz) - border2;
            const type offset = dim1 + gap;
            const type sep_offset = gap + sep;

            core::rectangle area = super::init_area(border, super::make_size(dim1, dim2), sz, gap, count);
            for(win::window* win : children) {
              if (super::is_separator(win)) {
                super::place_child(win, super::get_sep_area(area, sep));
                super::move_area(area, sep_offset);
              } else {
                super::place_child(win, area);
                super::move_area(area, offset);
              }
            }
          } else {
            super::hide_children(children);
          }
        }
        super::update();
      }
    };

    // --------------------------------------------------------------------------
    template<unsigned short dim1,
             unsigned short border = 0,
             unsigned short gap = 0,
             unsigned short sep = 2,
             origin R = origin::start>
    using horizontal_lineup = lineup_layout<orientation::horizontal, dim1, border, gap, sep, R>;

    // --------------------------------------------------------------------------
    template<unsigned short dim1,
             unsigned short border = 0,
             unsigned short gap = 0,
             unsigned short sep = 2,
             origin R = origin::start>
    using vertical_lineup = lineup_layout<orientation::vertical, dim1, border, gap, sep, R>;

    // --------------------------------------------------------------------------
    template<alignment a,
             unsigned short dim1,
             unsigned short border = 0,
             unsigned short gap = 0,
             unsigned short sep = 2>
    using lineup = lineup_layout<alignment_orientation<a>::value, dim1, border, gap, sep, alignment_origin<a>::value>;

    // --------------------------------------------------------------------------
    template<orientation H, unsigned short border = 0, unsigned short gap = 0, unsigned short sep = 2>
    class adaption_layout : public detail::base<H> {
    public:
      typedef core::size::type type;
      typedef detail::base<H> super;

      adaption_layout (win::container* m)
        : super(m)
      {
        super::init(core::bind_method(this, &adaption_layout::layout), [&](){
          layout(super::get_main_size());
        });
      }

      void layout (const core::size& sz) {
        std::vector<win::window*> children = super::get_children();
        const std::size_t count = children.size();
        const std::size_t sep_count = super::separator_count();
        const type border2 = (border * 2);
        const type space = super::get_dimension1(sz) - border2 - sep_count * sep;

        if (count) {
          const type dim1 = (space - (gap * (count - 1))) / (count - sep_count);
          if (dim1 >= 1) {
            const type dim2 = super::get_dimension2(sz) - border2;
            const type offset = dim1 + gap;
            const type sep_offset = gap + sep;

            core::rectangle area(core::point(border, border), super::make_size(dim1, dim2));
            for(win::window* win : children) {
              if (super::is_separator(win)) {
                super::place_child(win, super::get_sep_area(area, sep));
                super::move_area(area, sep_offset);
              } else {
                super::place_child(win, area);
                super::move_area(area, offset);
              }
            }
          } else {
            super::hide_children(children);
          }
        }
        super::update();
      }
    };

    // --------------------------------------------------------------------------
    template<unsigned short border = 0, unsigned short gap = 0, unsigned short sep = 2>
    using horizontal_adaption = adaption_layout<orientation::horizontal, border, gap, sep>;

    // --------------------------------------------------------------------------
    template<unsigned short border = 0, unsigned short gap = 0, unsigned short sep = 2>
    using vertical_adaption = adaption_layout<orientation::vertical, border, gap, sep>;

    // --------------------------------------------------------------------------
    template<unsigned short width, unsigned short height, unsigned short border = 0, unsigned short gap = 0>
    class grid_lineup : public layout_base {
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
        std::vector<win::window*> children = get_children();
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

    // --------------------------------------------------------------------------
    template<unsigned short columns, unsigned short rows, unsigned short border = 0, unsigned short gap = 0>
    class grid_adaption : public layout_base {
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
        std::vector<win::window*> children = get_children();
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

    // --------------------------------------------------------------------------
    enum class border_layout_type {
      all_symmetric,
      top_bottom_maximize,
      left_right_maximize,
      top_left_maximize,
      bottom_right_maximize
    };

    // --------------------------------------------------------------------------
    template<border_layout_type T>
    struct border_layout_geometrie {
      typedef std::pair<core::point::type, core::point::type> points;
      static points get_top_position (const core::rectangle&, const core::size&);
      static points get_bottom_position (const core::rectangle&, const core::size&);
      static points get_left_position (const core::rectangle&, const core::size&);
      static points get_right_position (const core::rectangle&, const core::size&);
    };

    // --------------------------------------------------------------------------
    template<>
    struct border_layout_geometrie<border_layout_type::all_symmetric> {
      typedef std::pair<core::point::type, core::point::type> points;

      static points get_top_position (const core::rectangle& r, const core::size&) {
        return std::make_pair(r.x(), r.width());
      }

      static points get_bottom_position (const core::rectangle& r, const core::size&) {
        return std::make_pair(r.x(), r.width());
      }

      static points get_left_position (const core::rectangle& r, const core::size&) {
        return std::make_pair(r.y(), r.height());
      }

      static points get_right_position (const core::rectangle& r, const core::size&) {
        return std::make_pair(r.y(), r.height());
      }
    };

    // --------------------------------------------------------------------------
    template<>
    struct border_layout_geometrie<border_layout_type::top_bottom_maximize> {
      typedef std::pair<core::point::type, core::point::type> points;

      static points get_top_position (const core::rectangle&, const core::size& sz) {
        return std::make_pair(0, sz.width());
      }

      static points get_bottom_position (const core::rectangle&, const core::size& sz) {
        return std::make_pair(0, sz.width());
      }

      static points get_left_position (const core::rectangle& r, const core::size&) {
        return std::make_pair(r.y(), r.height());
      }

      static points get_right_position (const core::rectangle& r, const core::size&) {
        return std::make_pair(r.y(), r.height());
      }
    };

    // --------------------------------------------------------------------------
    template<>
    struct border_layout_geometrie<border_layout_type::left_right_maximize> {
      typedef std::pair<core::point::type, core::point::type> points;

      static points get_top_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(r.x(), r.width());
      }

      static points get_bottom_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(r.x(), r.width());
      }

      static points get_left_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(0, sz.height());
      }

      static points get_right_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(0, sz.height());
      }
    };

    // --------------------------------------------------------------------------
    template<>
    struct border_layout_geometrie<border_layout_type::top_left_maximize> {
      typedef std::pair<core::point::type, core::point::type> points;

      static points get_top_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(r.x(), sz.width() - r.x());
      }

      static points get_bottom_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(r.x(), r.width());
      }

      static points get_left_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(r.y(), sz.height() - r.y());
      }

      static points get_right_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(r.y(), r.height());
      }
    };

    // --------------------------------------------------------------------------
    template<>
    struct border_layout_geometrie<border_layout_type::bottom_right_maximize> {
      typedef std::pair<core::point::type, core::point::type> points;

      static points get_top_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(r.x(), r.width());
      }

      static points get_bottom_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(0, r.width() + r.x());
      }

      static points get_left_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(r.y(), r.height());
      }

      static points get_right_position (const core::rectangle& r, const core::size& sz) {
        return std::make_pair(0, r.height() + r.y());
      }
    };

    // --------------------------------------------------------------------------
    template<border_layout_type type = border_layout_type::top_bottom_maximize>
    class border_layout : public layout_base {
    public:
      typedef layout_base super;
      typedef border_layout_geometrie<type> geometrie;

      border_layout (win::container* m,
                     int top_height,
                     int bottom_height,
                     int left_width,
                     int right_width)
        : super(m)
        , center(nullptr)
        , top(nullptr)
        , bottom(nullptr)
        , left(nullptr)
        , right(nullptr)
        , top_height(top_height)
        , bottom_height(bottom_height)
        , left_width(left_width)
        , right_width(right_width)
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
        core::rectangle r = core::rectangle(core::point(left_width, top_height),
                                            core::point(sz.width() - right_width, sz.height() - bottom_height));
        if (top && is_child_visible(top)) {
          const typename geometrie::points pt = geometrie::get_top_position(r, sz);
          place_child(top, core::rectangle(pt.first, 0, pt.second, top_height));
        }
        if (bottom && is_child_visible(bottom)) {
          const typename geometrie::points pt = geometrie::get_bottom_position(r, sz);
          place_child(bottom, core::rectangle(pt.first, r.y2(), pt.second, bottom_height));
        }
        if (left && is_child_visible(left)) {
          const typename geometrie::points pt = geometrie::get_left_position(r, sz);
          place_child(left, core::rectangle(0, pt.first, left_width, pt.second));
        }
        if (right && is_child_visible(right)) {
          const typename geometrie::points pt = geometrie::get_right_position(r, sz);
          place_child(right, core::rectangle(r.x2(), pt.first, right_width, pt.second));
        }
        if (center && is_child_visible(center)) {
          place_child(center, r);
        }
        update();
      }

      inline int get_top_height () const    { return top_height; }
      inline int get_bottom_height () const { return bottom_height; }
      inline int get_left_width () const    { return left_width; }
      inline int get_right_width () const   { return right_width; }

      inline void set_top_height (int v)     { top_height = v; }
      inline void set_bottom_height (int v)  { bottom_height = v; }
      inline void set_left_width (int v)     { left_width = v; }
      inline void set_right_width (int v)    { right_width = v; }

    private:
      win::window* center;
      win::window* top;
      win::window* bottom;
      win::window* left;
      win::window* right;

      int top_height;
      int bottom_height;
      int left_width;
      int right_width;
    };


    // --------------------------------------------------------------------------
    enum class What {
      left,
      right,
      top,
      bottom
    };

    // --------------------------------------------------------------------------
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

    // --------------------------------------------------------------------------
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
