/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     layout to position windows on top, bottm, left, right or center of a parent
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    namespace border {

      // --------------------------------------------------------------------------
      // all_symmetric
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::all_symmetric> {

        typedef core::size::type type;

        type top = TO;
        type bottom = BO;
        type left = LE;
        type right = RI;

        left_width get_top (const core::rectangle& r) const {
          return {r.x() + left, r.width() - (right + left)};
        }

        left_width get_bottom (const core::rectangle& r) const {
          return {r.x() + left, r.width() - (right + left)};
        }

        top_height get_left (const core::rectangle& r) const {
          return {r.y() + top, r.height() - (bottom + top)};
        }

        top_height get_right (const core::rectangle& r) const {
          return {r.y() + top, r.height() - (bottom + top)};
        }
      };

      // --------------------------------------------------------------------------
      // top_bottom_maximize
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::top_bottom_maximize> {

        typedef core::size::type type;

        type top = TO;
        type bottom = BO;
        type left = LE;
        type right = RI;

        left_width get_top (const core::rectangle& r) const {
          return {r.x(), r.width()};
        }

        left_width get_bottom (const core::rectangle& r) const {
          return {r.x(), r.width()};
        }

        top_height get_left (const core::rectangle& r) const {
          return {r.y() + top, r.height() - (bottom + top)};
        }

        top_height get_right (const core::rectangle& r) const {
          return {r.y() + top, r.height() - (bottom + top)};
        }
      };

      // --------------------------------------------------------------------------
      // left_right_maximize
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::left_right_maximize> {

        typedef core::size::type type;

        type top = TO;
        type bottom = BO;
        type left = LE;
        type right = RI;

        left_width get_top (const core::rectangle& r) const {
          return {r.x() + left, r.width() - (right + left)};
        }

        left_width get_bottom (const core::rectangle& r) const {
          return {r.x() + left, r.width() - (right + left)};
        }

        top_height get_left (const core::rectangle& r) const {
          return {r.y(), r.height()};
        }

        top_height get_right (const core::rectangle& r) const {
          return {r.y(), r.height()};
        }
      };

      // --------------------------------------------------------------------------
      // bottom_max_top_min
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::bottom_max_top_min> {

        typedef core::size::type type;

        type top = TO;
        type bottom = BO;
        type left = LE;
        type right = RI;

        left_width get_top (const core::rectangle& r) const {
          return {r.x() + left, r.width() - (right + left)};
        }

        left_width get_bottom (const core::rectangle& r) const {
          return {r.x(), r.width()};
        }

        top_height get_left (const core::rectangle& r) const {
          return {r.y(), r.height() - bottom};
        }

        top_height get_right (const core::rectangle& r) const {
          return {r.y(), r.height() - bottom};
        }
      };

      // --------------------------------------------------------------------------
      // top_max_bottom_min
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::top_max_bottom_min> {

        typedef core::size::type type;

        type top = TO;
        type bottom = BO;
        type left = LE;
        type right = RI;

        left_width get_top (const core::rectangle& r) const {
          return {r.x(), r.width()};
        }

        left_width get_bottom (const core::rectangle& r) const {
          return {r.x() + left, r.width() - (right + left)};
        }

        top_height get_left (const core::rectangle& r) const {
          return {r.y() + top, r.height() - top};
        }

        top_height get_right (const core::rectangle& r) const {
          return {r.y() + top, r.height() - top};
        }
      };

      // --------------------------------------------------------------------------
      // left_max_right_min
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::left_max_right_min> {

        typedef core::size::type type;

        type top = TO;
        type bottom = BO;
        type left = LE;
        type right = RI;

        left_width get_top (const core::rectangle& r) const {
          return {r.x() + left, r.width() - left};
        }

        left_width get_bottom (const core::rectangle& r) const {
          return {r.x() + left, r.width() - left};
        }

        top_height get_left (const core::rectangle& r) const {
          return {r.y(), r.height()};
        }

        top_height get_right (const core::rectangle& r) const {
          return {r.y() + top, r.height() - (bottom + top)};
        }
      };

      // --------------------------------------------------------------------------
      // right_max_left_min
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::right_max_left_min> {

        typedef core::size::type type;

        type top = TO;
        type bottom = BO;
        type left = LE;
        type right = RI;

        left_width get_top (const core::rectangle& r) const {
          return {r.x(), r.width() - right};
        }

        left_width get_bottom (const core::rectangle& r) const {
          return {r.x(), r.width() - right};
        }

        top_height get_left (const core::rectangle& r) const {
          return {r.y() + top, r.height() - (bottom + top)};
        }

        top_height get_right (const core::rectangle& r) const {
          return {r.y(), r.height()};
        }
      };

      // --------------------------------------------------------------------------
      // top_left_maximize
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::top_left_maximize> {

        typedef core::size::type type;

        type top = TO;
        type bottom = BO;
        type left = LE;
        type right = RI;

        left_width get_top (const core::rectangle& r) const {
          return {r.x() + left, r.width() - left};
        }

        left_width get_bottom (const core::rectangle& r) const {
          return {r.x() + left, r.width() - (left + right)};
        }

        top_height get_left (const core::rectangle& r) const {
          return {r.y() + top, r.height() - top};
        }

        top_height get_right (const core::rectangle& r) const {
          return {r.y() + top, r.height() - (top + bottom)};
        }
      };

      // --------------------------------------------------------------------------
      // bottom_right_maximize
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::bottom_right_maximize> {

        typedef core::size::type type;

        type top = TO;
        type bottom = BO;
        type left = LE;
        type right = RI;

        left_width get_top (const core::rectangle& r) const {
          return {r.x() + left, r.width() - (left + right)};
        }

        left_width get_bottom (const core::rectangle& r) const {
          return {r.x(), r.width() - right};
        }

        top_height get_left (const core::rectangle& r) const {
          return {r.y() + top, r.height() - (top + bottom)};
        }

        top_height get_right (const core::rectangle& r) const {
          return {r.y(), r.height() - bottom};
        }
      };

      // --------------------------------------------------------------------------
      inline const layout_element& layout_base::get_center () const {
        return data.center;
      }

      inline const layout_element& layout_base::get_top () const {
        return data.top;
      }

      inline const layout_element& layout_base::get_bottom () const {
        return data.bottom;
      }

      inline const layout_element& layout_base::get_left () const {
        return data.left;
      }

      inline const layout_element& layout_base::get_right () const {
        return data.right;
      }

      inline void layout_base::set_center (const layout_element& center) {
        data.center = center;
      }

      inline void layout_base::set_top (const layout_element& top) {
        data.top = top;
      }

      inline void layout_base::set_bottom (const layout_element& bottom) {
        data.bottom = bottom;
      }

      inline void layout_base::set_left (const layout_element& left) {
        data.left = left;
      }

      inline void layout_base::set_right (const layout_element& right) {
        data.right = right;
      }

      inline void layout_base::set_center_top_bottom_left_right (
        const layout_element& center,
        const layout_element& top,
        const layout_element& bottom,
        const layout_element& left,
        const layout_element& right
      ) {
        data.center = center;
        data.top = top;
        data.bottom = bottom;
        data.left = left;
        data.right = right;
      }

      inline void layout_base::add (const std::vector<std::reference_wrapper<win::window>>& list) {
        if (list.size() > 0) {
          data.center = lay(list[0].get());
          if (list.size() > 1) {
            data.top = lay(list[1].get());
            if (list.size() > 2) {
              data.bottom = lay(list[2].get());;
              if (list.size() > 3) {
                data.left = lay(list[3].get());;
                if (list.size() > 4) {
                  data.right = lay(list[4].get());;
                }
              }
            }
          }
        }
      }

      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI, type_t T>
      layouter<TO, BO, LE, RI, T>::layouter (type top_height,
                                             type bottom_height,
                                             type left_width,
                                             type right_width)
        : geometrie{top_height, bottom_height, left_width, right_width}
      {}

      template<int TO, int BO, int LE, int RI, type_t T>
      inline auto layouter<TO, BO, LE, RI, T>::get_top_height () const -> type {
        return geometrie.top;
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline auto layouter<TO, BO, LE, RI, T>::get_bottom_height () const -> type {
        return geometrie.bottom;
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline auto layouter<TO, BO, LE, RI, T>::get_left_width () const -> type {
        return geometrie.left;
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline auto layouter<TO, BO, LE, RI, T>::get_right_width () const -> type {
        return geometrie.right;
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline void layouter<TO, BO, LE, RI, T>::set_top_height (type v) {
        geometrie.top = v;
      }
      template<int TO, int BO, int LE, int RI, type_t T>
      inline void layouter<TO, BO, LE, RI, T>::set_bottom_height (type v) {
        geometrie.bottom = v;
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline void layouter<TO, BO, LE, RI, T>::set_left_width (type v) {
        geometrie.left = v;
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline void layouter<TO, BO, LE, RI, T>::set_right_width (type v) {
        geometrie.right = v;
      }



      template<int TO, int BO, int LE, int RI, type_t T>
      inline core::rectangle layouter<TO, BO, LE, RI, T>::get_center_geometry (const core::rectangle& r) const {
        return r.shrinked(get_top_height(), get_bottom_height(), get_left_width(), get_right_width());
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline core::rectangle layouter<TO, BO, LE, RI, T>::get_top_geometry (const core::rectangle& r) const {
        const left_width pt = geometrie.get_top(r);
        return core::rectangle(pt.left, r.y(), pt.width, get_top_height());
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline core::rectangle layouter<TO, BO, LE, RI, T>::get_bottom_geometry (const core::rectangle& r) const {
        const left_width pt = geometrie.get_bottom(r);
        return core::rectangle(pt.left, r.y2() - get_bottom_height(), pt.width, get_bottom_height());
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline core::rectangle layouter<TO, BO, LE, RI, T>::get_left_geometry (const core::rectangle& r) const {
        const top_height pt = geometrie.get_left(r);
        return core::rectangle(r.x(), pt.top, get_left_width(), pt.height);
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline core::rectangle layouter<TO, BO, LE, RI, T>::get_right_geometry (const core::rectangle& r) const {
        const top_height pt = geometrie.get_right(r);
        return core::rectangle(r.x2() - get_right_width(), pt.top, get_right_width(), pt.height);
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      void layouter<TO, BO, LE, RI, T>::layout (const core::rectangle& r) {
        logging::trace() << "border::layouter(" << r << ")";
        if (get_top()) {
          get_top()(get_top_geometry(r));
        }
        if (get_bottom()) {
          get_bottom()(get_bottom_geometry(r));
        }
        if (get_left()) {
          get_left()(get_left_geometry(r));
        }
        if (get_right()) {
          get_right()(get_right_geometry(r));
        }
        if (get_center()) {
          get_center()(get_center_geometry(r));
        }
      }

      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      sym_layouter<TO, BO, LE, RI>::sym_layouter (win::container* p)
        : super(p)
      {}

      template<int TO, int BO, int LE, int RI>
      inline const layout_element& sym_layouter<TO, BO, LE, RI>::get_top_left () const {
        return data.top_left;
      }

      template<int TO, int BO, int LE, int RI>
      inline const layout_element& sym_layouter<TO, BO, LE, RI>::get_top_right () const {
        return data.top_right;
      }

      template<int TO, int BO, int LE, int RI>
      inline const layout_element& sym_layouter<TO, BO, LE, RI>::get_bottom_left () const {
        return data.bottom_left;
      }

      template<int TO, int BO, int LE, int RI>
      inline const layout_element& sym_layouter<TO, BO, LE, RI>::get_bottom_right () const {
        return data.bottom_right;
      }

      template<int TO, int BO, int LE, int RI>
      inline void sym_layouter<TO, BO, LE, RI>::set_top_left (const layout_element& top_left) {
        data.top_left = top_left;
      }

      template<int TO, int BO, int LE, int RI>
      inline void sym_layouter<TO, BO, LE, RI>::set_top_right (const layout_element& top_right) {
        data.top_right = top_right;
      }

      template<int TO, int BO, int LE, int RI>
      inline void sym_layouter<TO, BO, LE, RI>::set_bottom_left (const layout_element& bottom_left) {
        data.bottom_left = bottom_left;
      }

      template<int TO, int BO, int LE, int RI>
      inline void sym_layouter<TO, BO, LE, RI>::set_bottom_right (const layout_element& bottom_right) {
        data.bottom_right = bottom_right;
      }

      template<int TO, int BO, int LE, int RI>
      void sym_layouter<TO, BO, LE, RI>::layout (const core::rectangle& r) {
        logging::trace() << "border::sym_layouter(" << r << ")";
        if (get_top_left()) {
          get_top_left()(core::rectangle(r.x(), r.y(), super::get_left_width(), super::get_top_height()));
        }
        if (get_top_right()) {
          get_top_right()(core::rectangle(r.x2() - super::get_right_width(), r.y(), super::get_right_width(), super::get_top_height()));
        }
        if (get_bottom_left()) {
          get_bottom_left()(core::rectangle(r.x(), r.y2() - super::get_bottom_height(), super::get_left_width(), super::get_bottom_height()));
        }
        if (get_bottom_right()) {
          get_bottom_right()(core::rectangle(r.x2() - super::get_right_width(), r.y2() - super::get_bottom_height(), super::get_right_width(), super::get_bottom_height()));
        }
        super::layout(r);
      }

    } // namespace border

  } // namespace layout

} // namespace gui
