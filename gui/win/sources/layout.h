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

      layout_base (win::container* m, const layout_base&)
        : main(m)
      {}

      layout_base (win::container* m, layout_base&&)
        : main(m)
      {}

      layout_base (const layout_base&) = delete;
      layout_base (layout_base&&) = delete;

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
        typedef layout_base super;
        typedef core::size::type type;

        base (win::container* m)
          : super(m)
        {}

        base (win::container* m, const base& rhs)
          : super(m, rhs)
        {}

        base (win::container* m, base&& rhs)
          : super(m, std::move(rhs))
        {}

        type get_dimension1 (const core::size& sz);
        type get_dimension2 (const core::size& sz);
        core::size make_size (type dim1, type dim2);
        core::rectangle get_sep_area (const core::rectangle& area, unsigned short s);
        void move_area (core::rectangle& area, type offs);
      };

      // --------------------------------------------------------------------------
      template<>
      inline base<orientation::horizontal>::type base<orientation::horizontal>::get_dimension1 (const core::size& sz) {
        return sz.width();
      }

      template<>
      inline base<orientation::horizontal>::type base<orientation::horizontal>::get_dimension2 (const core::size& sz) {
        return sz.height();
      }

      template<>
      inline core::size base<orientation::horizontal>::make_size (type dim1, type dim2) {
        return core::size(dim1, dim2);
      }

      template<>
      inline core::rectangle base<orientation::horizontal>::get_sep_area (const core::rectangle& area, unsigned short s) {
        return area.with_width(s);
      }

      template<>
      inline void base<orientation::horizontal>::move_area (core::rectangle& area, type offs) {
        area.move_x(offs);
      }

      // --------------------------------------------------------------------------
      template<>
      inline base<orientation::vertical>::type base<orientation::vertical>::get_dimension1 (const core::size& sz) {
        return sz.height();
      }

      template<>
      inline base<orientation::vertical>::type base<orientation::vertical>::get_dimension2 (const core::size& sz) {
        return sz.width();
      }

      template<>
      inline core::size base<orientation::vertical>::make_size (type dim1, type dim2) {
        return core::size(dim2, dim1);
      }

      template<>
      inline core::rectangle base<orientation::vertical>::get_sep_area (const core::rectangle& area, unsigned short s) {
        return area.with_height(s);
      }

      template<>
      inline void base<orientation::vertical>::move_area (core::rectangle& area, type offs) {
        area.move_y(offs);
      }

      // --------------------------------------------------------------------------
      template<orientation H, origin R>
      class lineup_base : public base<H> {
      public:
        typedef base<H> super;
        typedef core::size::type type;

        lineup_base (win::container* m)
          : super(m)
        {}

        lineup_base (win::container* m, const lineup_base& rhs)
          : super(m, rhs)
        {}

        lineup_base (win::container* m, lineup_base&& rhs)
          : super(m, std::move(rhs))
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

        lineup_base (win::container* m, const lineup_base& rhs)
          : super(m, rhs)
        {}

        lineup_base (win::container* m, lineup_base&& rhs)
          : super(m, std::move(rhs))
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

        lineup_base (win::container* m, const lineup_base& rhs)
          : super(m, rhs)
        {}

        lineup_base (win::container* m, lineup_base&& rhs)
          : super(m, std::move(rhs))
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

        lineup_base (win::container* m, const lineup_base& rhs)
          : super(m, rhs)
        {}

        lineup_base (win::container* m, lineup_base&& rhs)
          : super(m, std::move(rhs))
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

        lineup_base (win::container* m, const lineup_base& rhs)
          : super(m, rhs)
        {}

        lineup_base (win::container* m, lineup_base&& rhs)
          : super(m, std::move(rhs))
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

        lineup_base (win::container* m, const lineup_base& rhs)
          : super(m, rhs)
        {}

        lineup_base (win::container* m, lineup_base&& rhs)
          : super(m, std::move(rhs))
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
        init();
      }

      lineup_layout (win::container* m, const lineup_layout& rhs)
        : super(m, rhs)
      {
        init();
      }

      lineup_layout (win::container* m, lineup_layout&& rhs)
        : super(m, std::move(rhs))
      {
        init();
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

    private:
      void init () {
        super::init(core::bind_method(this, &lineup_layout::layout), [&](){
          layout(super::get_main_size());
        });
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
        init();
      }

      adaption_layout (win::container* m, const adaption_layout& rhs)
        : super(m, rhs)
      {
        init();
      }

      adaption_layout (win::container* m, adaption_layout&& rhs)
        : super(m, std::move(rhs))
      {
        init();
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

    private:
      void init () {
        super::init(core::bind_method(this, &adaption_layout::layout), [&](){
          layout(super::get_main_size());
        });
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
        init();
      }

      grid_lineup (win::container* m, const grid_lineup& rhs)
        : super(m, rhs)
      {
        init();
      }

      grid_lineup (win::container* m, grid_lineup&& rhs)
        : super(m, std::move(rhs))
      {
        init();
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

    private:
      void init () {
        super::init(core::bind_method(this, &grid_lineup::layout), [&](){
          layout(get_main_size());
        });
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
        init();
      }

      grid_adaption (win::container* m, const grid_adaption& rhs)
        : super(m, rhs)
      {
        init();
      }

      grid_adaption (win::container* m, grid_adaption&& rhs)
        : super(m, std::move(rhs))
      {
        init();
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

    private:
      void init () {
        super::init(core::bind_method(this, &grid_adaption::layout), [&](){
          layout(get_main_size());
        });
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
      typedef std::pair<core::point_type, core::point_type> points;
      static points get_top_position (const core::rectangle&, const core::size&);
      static points get_bottom_position (const core::rectangle&, const core::size&);
      static points get_left_position (const core::rectangle&, const core::size&);
      static points get_right_position (const core::rectangle&, const core::size&);
    };

    namespace detail {

      template<typename T1, typename T2>
      inline std::pair<core::point_type, core::point_type> make_points (const T1& t1, const T2& t2) {
        return std::make_pair(static_cast<core::point_type>(t1), static_cast<core::point_type>(t2));
      }

    } // detail

    // --------------------------------------------------------------------------
    template<>
    struct border_layout_geometrie<border_layout_type::all_symmetric> {
      typedef std::pair<core::point::type, core::point::type> points;

      static points get_top_position (const core::rectangle& r, const core::size&) {
        return detail::make_points(r.x(), r.width());
      }

      static points get_bottom_position (const core::rectangle& r, const core::size&) {
        return detail::make_points(r.x(), r.width());
      }

      static points get_left_position (const core::rectangle& r, const core::size&) {
        return detail::make_points(r.y(), r.height());
      }

      static points get_right_position (const core::rectangle& r, const core::size&) {
        return detail::make_points(r.y(), r.height());
      }
    };

    // --------------------------------------------------------------------------
    template<>
    struct border_layout_geometrie<border_layout_type::top_bottom_maximize> {
      typedef std::pair<core::point::type, core::point::type> points;

      static points get_top_position (const core::rectangle&, const core::size& sz) {
        return detail::make_points(0, sz.width());
      }

      static points get_bottom_position (const core::rectangle&, const core::size& sz) {
        return detail::make_points(0, sz.width());
      }

      static points get_left_position (const core::rectangle& r, const core::size&) {
        return detail::make_points(r.y(), r.height());
      }

      static points get_right_position (const core::rectangle& r, const core::size&) {
        return detail::make_points(r.y(), r.height());
      }
    };

    // --------------------------------------------------------------------------
    template<>
    struct border_layout_geometrie<border_layout_type::left_right_maximize> {
      typedef std::pair<core::point::type, core::point::type> points;

      static points get_top_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(r.x(), r.width());
      }

      static points get_bottom_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(r.x(), r.width());
      }

      static points get_left_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(0, sz.height());
      }

      static points get_right_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(0, sz.height());
      }
    };

    // --------------------------------------------------------------------------
    template<>
    struct border_layout_geometrie<border_layout_type::top_left_maximize> {
      typedef std::pair<core::point::type, core::point::type> points;

      static points get_top_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(r.x(), sz.width() - r.x());
      }

      static points get_bottom_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(r.x(), r.width());
      }

      static points get_left_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(r.y(), sz.height() - r.y());
      }

      static points get_right_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(r.y(), r.height());
      }
    };

    // --------------------------------------------------------------------------
    template<>
    struct border_layout_geometrie<border_layout_type::bottom_right_maximize> {
      typedef std::pair<core::point::type, core::point::type> points;

      static points get_top_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(r.x(), r.width());
      }

      static points get_bottom_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(0, r.width() + r.x());
      }

      static points get_left_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(r.y(), r.height());
      }

      static points get_right_position (const core::rectangle& r, const core::size& sz) {
        return detail::make_points(0, r.height() + r.y());
      }
    };

    struct border_layout_data {
      border_layout_data (float top_height = 0,
                          float bottom_height = 0,
                          float left_width = 0,
                          float right_width = 0)
        : center(nullptr)
        , top(nullptr)
        , bottom(nullptr)
        , left(nullptr)
        , right(nullptr)
        , top_height(top_height)
        , bottom_height(bottom_height)
        , left_width(left_width)
        , right_width(right_width)
      {}

      win::window* center;
      win::window* top;
      win::window* bottom;
      win::window* left;
      win::window* right;

      float top_height;
      float bottom_height;
      float left_width;
      float right_width;
    };

    // --------------------------------------------------------------------------
    template<border_layout_type type = border_layout_type::top_bottom_maximize>
    class border_layout : public layout_base {
    public:
      typedef layout_base super;
      typedef border_layout_geometrie<type> geometrie;

      border_layout (win::container* m,
                     float top_height,
                     float bottom_height,
                     float left_width,
                     float right_width)
        : super(m)
        , data(top_height, bottom_height, left_width, right_width)
      {
        init();
      }

      border_layout (win::container* m, const border_layout& rhs)
        : super(m, rhs)
        , data(rhs.data)
      {
        init();
      }

      border_layout (win::container* m, border_layout&& rhs)
        : super(m, std::move(rhs))
        , data(std::move(rhs.data))
      {
        init();
      }

      void set_center (win::window* center) {
        data.center = center;
      }

      void set_top (win::window* top) {
        data.top = top;
      }

      void set_bottom (win::window* bottom) {
        data.bottom = bottom;
      }

      void set_left (win::window* left) {
        data.left = left;
      }

      void set_right (win::window* right) {
        data.right = right;
      }

      void set_center_top_bottom_left_right (win::window* center,
                                             win::window* top,
                                             win::window* bottom,
                                             win::window* left,
                                             win::window* right) {
        data.center = center;
        data.top = top;
        data.bottom = bottom;
        data.left = left;
        data.right = right;
      }

      void layout (const core::size& sz) {
        core::rectangle r = core::rectangle(core::point(data.left_width, data.top_height),
                                            core::point(sz.width() - data.right_width, sz.height() - data.bottom_height));
        if (data.top && is_child_visible(data.top)) {
          const typename geometrie::points pt = geometrie::get_top_position(r, sz);
          place_child(data.top, core::rectangle(pt.first, 0, pt.second, data.top_height));
        }
        if (data.bottom && is_child_visible(data.bottom)) {
          const typename geometrie::points pt = geometrie::get_bottom_position(r, sz);
          place_child(data.bottom, core::rectangle(pt.first, r.y2(), pt.second, data.bottom_height));
        }
        if (data.left && is_child_visible(data.left)) {
          const typename geometrie::points pt = geometrie::get_left_position(r, sz);
          place_child(data.left, core::rectangle(0, pt.first, data.left_width, pt.second));
        }
        if (data.right && is_child_visible(data.right)) {
          const typename geometrie::points pt = geometrie::get_right_position(r, sz);
          place_child(data.right, core::rectangle(r.x2(), pt.first, data.right_width, pt.second));
        }
        if (data.center && is_child_visible(data.center)) {
          place_child(data.center, r);
        }
        update();
      }

      inline float get_top_height () const    { return data.top_height; }
      inline float get_bottom_height () const { return data.bottom_height; }
      inline float get_left_width () const    { return data.left_width; }
      inline float get_right_width () const   { return data.right_width; }

      inline void set_top_height (float v)     { data.top_height = v; }
      inline void set_bottom_height (float v)  { data.bottom_height = v; }
      inline void set_left_width (float v)     { data.left_width = v; }
      inline void set_right_width (float v)    { data.right_width = v; }

    private:
      void init () {
        super::init(core::bind_method(this, &border_layout::layout), [&](){
          layout(get_main_size());
        });
      }

      border_layout_data data;
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
      attach (win::container* m) {
        init(m);
      }

      attach (win::container* m, const attach&) {
        init(m);
      }

      attach (win::container* m, attach&&) {
        init(m);
      }

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
      void init (win::container*);

      std::vector<detail::attachment> attachments;
    };

  }
}
