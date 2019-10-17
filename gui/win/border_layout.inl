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


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

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
      get_top_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::all_symmetric>::
      get_bottom_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::all_symmetric>::
      get_left_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::all_symmetric>::
      get_right_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto border_layout_geometrie<border_layout_type::top_bottom_maximize>::
      get_top_position (const core::rectangle&, const core::rectangle& sz) -> points {
        return detail::make_points(0, sz.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_bottom_maximize>::
      get_bottom_position (const core::rectangle&, const core::rectangle& sz) -> points {
        return detail::make_points(0, sz.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_bottom_maximize>::
      get_left_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_bottom_maximize>::
      get_right_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto border_layout_geometrie<border_layout_type::left_right_maximize>::
      get_top_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::left_right_maximize>::
      get_bottom_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::left_right_maximize>::
      get_left_position (const core::rectangle&, const core::rectangle& sz) -> points {
        return detail::make_points(0, sz.height());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::left_right_maximize>::
      get_right_position (const core::rectangle&, const core::rectangle& sz) -> points {
        return detail::make_points(0, sz.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto border_layout_geometrie<border_layout_type::top_left_maximize>::
      get_top_position (const core::rectangle& r, const core::rectangle& sz) -> points {
        return detail::make_points(r.x(), sz.width() - r.x());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_left_maximize>::
      get_bottom_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_left_maximize>::
      get_left_position (const core::rectangle& r, const core::rectangle& sz) -> points {
        return detail::make_points(r.y(), sz.height() - r.y());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::top_left_maximize>::
      get_right_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto border_layout_geometrie<border_layout_type::bottom_right_maximize>::
      get_top_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::bottom_right_maximize>::
      get_bottom_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(0, r.width() + r.x());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::bottom_right_maximize>::
      get_left_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      template<>
      inline auto border_layout_geometrie<border_layout_type::bottom_right_maximize>::
      get_right_position (const core::rectangle& r, const core::rectangle&) -> points {
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
    void border_layout<type>::layout (const core::rectangle& sz) {
      LogTrace << "border_layout::layout(" << sz << ")";
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
    }

    template<border_layout_type type>
    void border_layout<type>::init () {
      super::init(basepp::bind_method(this, &border_layout::layout), [&](){
        layout(get_layout_area());
      });
    }

  } // namespace layout

} // namespace gui
