/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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

      inline dynamic_border_layout_base::dynamic_border_layout_base (float top_height,
                                                     float bottom_height,
                                                     float left_width,
                                                     float right_width)
        : data(top_height, bottom_height, left_width, right_width)
      {}

      inline const layout_function& dynamic_border_layout_base::get_center () const {
        return data.center;
      }

      inline const layout_function& dynamic_border_layout_base::get_top () const {
        return data.top;
      }

      inline const layout_function& dynamic_border_layout_base::get_bottom () const {
        return data.bottom;
      }

      inline const layout_function& dynamic_border_layout_base::get_left () const {
        return data.left;
      }

      inline const layout_function& dynamic_border_layout_base::get_right () const {
        return data.right;
      }

      inline void dynamic_border_layout_base::set_center (const layout_function& center) {
        data.center = center;
      }

      inline void dynamic_border_layout_base::set_top (const layout_function& top) {
        data.top = top;
      }

      inline void dynamic_border_layout_base::set_bottom (const layout_function& bottom) {
        data.bottom = bottom;
      }

      inline void dynamic_border_layout_base::set_left (const layout_function& left) {
        data.left = left;
      }

      inline void dynamic_border_layout_base::set_right (const layout_function& right) {
        data.right = right;
      }

      inline void dynamic_border_layout_base::set_center_top_bottom_left_right (const layout_function& center,
                                                                                const layout_function& top,
                                                                                const layout_function& bottom,
                                                                                const layout_function& left,
                                                                                const layout_function& right) {
        data.center = center;
        data.top = top;
        data.bottom = bottom;
        data.left = left;
        data.right = right;
      }

      inline float dynamic_border_layout_base::get_top_height () const {
        return data.top_height;
      }

      inline float dynamic_border_layout_base::get_bottom_height () const {
        return data.bottom_height;
      }

      inline float dynamic_border_layout_base::get_left_width () const {
        return data.left_width;
      }

      inline float dynamic_border_layout_base::get_right_width () const {
        return data.right_width;
      }

      inline void dynamic_border_layout_base::set_top_height (float v) {
        data.top_height = v;
      }

      inline void dynamic_border_layout_base::set_bottom_height (float v) {
        data.bottom_height = v;
      }

      inline void dynamic_border_layout_base::set_left_width (float v) {
        data.left_width = v;
      }

      inline void dynamic_border_layout_base::set_right_width (float v) {
        data.right_width = v;
      }

      inline dynamic_border_layout_base::dynamic_border_layout_data::dynamic_border_layout_data (float top_height,
                                                                         float bottom_height,
                                                                         float left_width,
                                                                         float right_width)
        : top_height(top_height)
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
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::all_symmetric>::
      get_top_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::all_symmetric>::
      get_bottom_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::all_symmetric>::
      get_left_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::all_symmetric>::
      get_right_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::top_bottom_maximize>::
      get_top_position (const core::rectangle&, const core::rectangle& sz) -> points {
        return detail::make_points(0, sz.width());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::top_bottom_maximize>::
      get_bottom_position (const core::rectangle&, const core::rectangle& sz) -> points {
        return detail::make_points(0, sz.width());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::top_bottom_maximize>::
      get_left_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::top_bottom_maximize>::
      get_right_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::left_right_maximize>::
      get_top_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::left_right_maximize>::
      get_bottom_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::left_right_maximize>::
      get_left_position (const core::rectangle&, const core::rectangle& sz) -> points {
        return detail::make_points(0, sz.height());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::left_right_maximize>::
      get_right_position (const core::rectangle&, const core::rectangle& sz) -> points {
        return detail::make_points(0, sz.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::top_left_maximize>::
      get_top_position (const core::rectangle& r, const core::rectangle& sz) -> points {
        return detail::make_points(r.x(), sz.width() - r.x());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::top_left_maximize>::
      get_bottom_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::top_left_maximize>::
      get_left_position (const core::rectangle& r, const core::rectangle& sz) -> points {
        return detail::make_points(r.y(), sz.height() - r.y());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::top_left_maximize>::
      get_right_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::bottom_right_maximize>::
      get_top_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::bottom_right_maximize>::
      get_bottom_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(0, r.width() + r.x());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::bottom_right_maximize>::
      get_left_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.y(), r.height());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::bottom_right_maximize>::
      get_right_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(0, r.height() + r.y());
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::bottom_max_top_min>::
      get_top_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(r.x(), r.width());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::bottom_max_top_min>::
      get_bottom_position (const core::rectangle& /*r*/, const core::rectangle& sz) -> points {
        return detail::make_points(0, sz.width());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::bottom_max_top_min>::
      get_left_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(0, r.height() + r.y());
      }

      template<>
      inline auto dynamic_border_layout_geometrie<dynamic_border_layout_type::bottom_max_top_min>::
      get_right_position (const core::rectangle& r, const core::rectangle&) -> points {
        return detail::make_points(0, r.height() + r.y());
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<dynamic_border_layout_type type>
    dynamic_border_layout<type>::dynamic_border_layout (win::container*,
                                        float top_height,
                                        float bottom_height,
                                        float left_width,
                                        float right_width)
      : super(top_height, bottom_height, left_width, right_width)
    {}

    template<dynamic_border_layout_type type>
    dynamic_border_layout<type>::dynamic_border_layout (float top_height,
                                        float bottom_height,
                                        float left_width,
                                        float right_width)
      : super(top_height, bottom_height, left_width, right_width)
    {}

    template<dynamic_border_layout_type type>
    void dynamic_border_layout<type>::layout (const core::rectangle& sz) {
      clog::trace() << "dynamic_border_layout::layout(" << sz << ")";
      core::size top_left(super::get_left_width(), super::get_top_height());
      core::size bottom_right(super::get_right_width(), super::get_bottom_height());
      core::size size = sz.size() - top_left - bottom_right;
      core::rectangle r = core::rectangle(sz.top_left() + top_left, size);
      if (super::get_top()) {
        const typename geometrie::points pt = geometrie::get_top_position(r, sz);
        super::get_top()(core::rectangle(pt.first, 0, pt.second, top_left.height()));
      }
      if (super::get_bottom()) {
        const typename geometrie::points pt = geometrie::get_bottom_position(r, sz);
        super::get_bottom()(core::rectangle(pt.first, r.y2(), pt.second, bottom_right.height()));
      }
      if (super::get_left()) {
        const typename geometrie::points pt = geometrie::get_left_position(r, sz);
        super::get_left()(core::rectangle(0, pt.first, top_left.width(), pt.second));
      }
      if (super::get_right()) {
        const typename geometrie::points pt = geometrie::get_right_position(r, sz);
        super::get_right()(core::rectangle(r.x2(), pt.first, bottom_right.width(), pt.second));
      }
      if (super::get_center()) {
        super::get_center()(r);
      }
    }

  } // namespace layout

} // namespace gui
