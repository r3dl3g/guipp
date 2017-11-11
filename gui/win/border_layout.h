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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/layout.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    enum class border_layout_type {
      all_symmetric,
      top_bottom_maximize,
      left_right_maximize,
      top_left_maximize,
      bottom_right_maximize
    };

    // --------------------------------------------------------------------------
    namespace detail {

      class border_layout_base : protected layout_base {
      public:
        typedef layout_base super;

        border_layout_base (win::container* m,
                            float top_height = 0,
                            float bottom_height = 0,
                            float left_width = 0,
                           float right_width = 0);
        border_layout_base (win::container* m, const border_layout_base& rhs);
        border_layout_base (win::container* m, border_layout_base&& rhs);

        win::window* get_center () const;
        win::window* get_top () const;
        win::window* get_bottom () const;
        win::window* get_left () const;
        win::window* get_right () const;

        void set_center (win::window* center);
        void set_top (win::window* top);
        void set_bottom (win::window* bottom);
        void set_left (win::window* left);
        void set_right (win::window* right);

        void set_center_top_bottom_left_right (win::window* center,
                                               win::window* top,
                                               win::window* bottom,
                                               win::window* left,
                                               win::window* right);

        float get_top_height () const;
        float get_bottom_height () const;
        float get_left_width () const;
        float get_right_width () const;

        void set_top_height (float v);
        void set_bottom_height (float v);
        void set_left_width (float v);
        void set_right_width (float v);

      protected:
        struct border_layout_data {
          border_layout_data (float top_height = 0,
                              float bottom_height = 0,
                              float left_width = 0,
                              float right_width = 0);

          win::window* center;
          win::window* top;
          win::window* bottom;
          win::window* left;
          win::window* right;

          float top_height;
          float bottom_height;
          float left_width;
          float right_width;

        } data;

      };

      // --------------------------------------------------------------------------
      template<typename T1, typename T2>
      std::pair<core::point::type, core::point::type> make_points (const T1& t1, const T2& t2);

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
    } // namespace detail

    // --------------------------------------------------------------------------
    template<border_layout_type type = border_layout_type::top_bottom_maximize>
    class border_layout : public detail::border_layout_base {
    public:
      typedef detail::border_layout_base super;
      typedef detail::border_layout_geometrie<type> geometrie;

      border_layout (win::container* m,
                     float top_height,
                     float bottom_height,
                     float left_width,
                     float right_width);
      border_layout (win::container* m, const border_layout& rhs);
      border_layout (win::container* m, border_layout&& rhs);

      void layout (const core::size& sz);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    //
    // inlines
    //
    namespace detail {

      inline border_layout_base::border_layout_base (win::container* m,
                                                     float top_height,
                                                     float bottom_height,
                                                     float left_width,
                                                     float right_width)
        : super(m)
        , data(top_height, bottom_height, left_width, right_width)
      {}

      inline border_layout_base::border_layout_base (win::container* m,
                                                     const border_layout_base& rhs)
        : super(m)
        , data(rhs.data)
      {}

      inline border_layout_base::border_layout_base (win::container* m,
                                                     border_layout_base&& rhs)
        : super(m)
        , data(std::move(rhs.data))
      {}

      inline win::window* border_layout_base::get_center () const {
        return data.center;
      }

      inline win::window* border_layout_base::get_top () const {
        return data.top;
      }

      inline win::window* border_layout_base::get_bottom () const {
        return data.bottom;
      }

      inline win::window* border_layout_base::get_left () const {
        return data.left;
      }

      inline win::window* border_layout_base::get_right () const {
        return data.right;
      }

      inline void border_layout_base::set_center (win::window* center) {
        data.center = center;
      }

      inline void border_layout_base::set_top (win::window* top) {
        data.top = top;
      }

      inline void border_layout_base::set_bottom (win::window* bottom) {
        data.bottom = bottom;
      }

      inline void border_layout_base::set_left (win::window* left) {
        data.left = left;
      }

      inline void border_layout_base::set_right (win::window* right) {
        data.right = right;
      }

      inline void border_layout_base::set_center_top_bottom_left_right (win::window* center,
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

      inline float border_layout_base::get_top_height () const {
        return data.top_height;
      }

      inline float border_layout_base::get_bottom_height () const {
        return data.bottom_height;
      }

      inline float border_layout_base::get_left_width () const {
        return data.left_width;
      }

      inline float border_layout_base::get_right_width () const {
        return data.right_width;
      }

      inline void border_layout_base::set_top_height (float v) {
        data.top_height = v;
      }

      inline void border_layout_base::set_bottom_height (float v) {
        data.bottom_height = v;
      }

      inline void border_layout_base::set_left_width (float v) {
        data.left_width = v;
      }

      inline void border_layout_base::set_right_width (float v) {
        data.right_width = v;
      }

      inline border_layout_base::border_layout_data::border_layout_data (float top_height,
                                                                         float bottom_height,
                                                                         float left_width,
                                                                         float right_width)
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

      // --------------------------------------------------------------------------
      template<typename T1, typename T2>
      inline std::pair<core::point::type, core::point::type> make_points (const T1& t1, const T2& t2) {
        return std::make_pair(static_cast<core::point::type>(t1), static_cast<core::point::type>(t2));
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto border_layout_geometrie<border_layout_type::all_symmetric>::
      get_top_position (const core::rectangle& r, const core::size&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::all_symmetric>::
      get_bottom_position (const core::rectangle& r, const core::size&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::all_symmetric>::
      get_left_position (const core::rectangle& r, const core::size&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::all_symmetric>::
      get_right_position (const core::rectangle& r, const core::size&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto border_layout_geometrie<border_layout_type::top_bottom_maximize>::
      get_top_position (const core::rectangle&, const core::size& sz) -> points {
        return detail::make_points(0, sz.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_bottom_maximize>::
      get_bottom_position (const core::rectangle&, const core::size& sz) -> points {
        return detail::make_points(0, sz.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_bottom_maximize>::
      get_left_position (const core::rectangle& r, const core::size&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_bottom_maximize>::
      get_right_position (const core::rectangle& r, const core::size&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto border_layout_geometrie<border_layout_type::left_right_maximize>::
      get_top_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::left_right_maximize>::
      get_bottom_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::left_right_maximize>::
      get_left_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(0, sz.height());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::left_right_maximize>::
      get_right_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(0, sz.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto border_layout_geometrie<border_layout_type::top_left_maximize>::
      get_top_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(r.x(), sz.width() - r.x());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_left_maximize>::
      get_bottom_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_left_maximize>::
      get_left_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(r.y(), sz.height() - r.y());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_left_maximize>::
      get_right_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(r.y(), r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto border_layout_geometrie<border_layout_type::bottom_right_maximize>::
      get_top_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::bottom_right_maximize>::
      get_bottom_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(0, r.width() + r.x());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::bottom_right_maximize>::
      get_left_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(r.y(), r.height());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::bottom_right_maximize>::
      get_right_position (const core::rectangle& r, const core::size& sz) -> points {
        return detail::make_points(0, r.height() + r.y());
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<border_layout_type type>
    border_layout<type>::border_layout (win::container* m,
                                        float top_height,
                                        float bottom_height,
                                        float left_width,
                                        float right_width)
      : super(m, top_height, bottom_height, left_width, right_width)
    {
      init();
    }

    template<border_layout_type type>
    border_layout<type>::border_layout (win::container* m, const border_layout& rhs)
      : super(m, rhs)
    {
      init();
    }

    template<border_layout_type type>
    border_layout<type>::border_layout (win::container* m, border_layout&& rhs)
      : super(m, std::move(rhs))
    {
      init();
    }

    template<border_layout_type type>
    void border_layout<type>::layout (const core::size& sz) {
      core::rectangle r = core::rectangle(core::point(super::get_left_width(), super::get_top_height()),
                                          core::point(sz.width() - super::get_right_width(), sz.height() - super::get_bottom_height()));
      if (super::get_top() && is_child_visible(super::get_top())) {
        const typename geometrie::points pt = geometrie::get_top_position(r, sz);
        place_child(super::get_top(), core::rectangle(pt.first, 0, pt.second, super::get_top_height()));
      }
      if (super::get_bottom() && is_child_visible(super::get_bottom())) {
        const typename geometrie::points pt = geometrie::get_bottom_position(r, sz);
        place_child(super::get_bottom(), core::rectangle(pt.first, r.y2(), pt.second, super::get_bottom_height()));
      }
      if (super::get_left() && is_child_visible(super::get_left())) {
        const typename geometrie::points pt = geometrie::get_left_position(r, sz);
        place_child(super::get_left(), core::rectangle(0, pt.first, super::get_left_width(), pt.second));
      }
      if (super::get_right() && is_child_visible(super::get_right())) {
        const typename geometrie::points pt = geometrie::get_right_position(r, sz);
        place_child(super::get_right(), core::rectangle(r.x2(), pt.first, super::get_right_width(), pt.second));
      }
      if (super::get_center() && is_child_visible(super::get_center())) {
        place_child(super::get_center(), r);
      }
      update();
    }

    template<border_layout_type type>
    void border_layout<type>::init () {
      super::init(core::bind_method(this, &border_layout::layout), [&](){
        layout(get_main_size());
      });
    }

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui
