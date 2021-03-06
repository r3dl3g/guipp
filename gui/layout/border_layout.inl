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
        static left_width get_top (const core::rectangle& r) {
          return {r.x() + LE, r.width() - (RI + LE)};
        }

        static left_width get_bottom (const core::rectangle& r) {
          return {r.x() + LE, r.width() - (RI + LE)};
        }

        static top_height get_left (const core::rectangle& r) {
          return {r.y() + TO, r.height() - (BO + TO)};
        }

        static top_height get_right (const core::rectangle& r) {
          return {r.y() + TO, r.height() - (BO + TO)};
        }
      };

      // --------------------------------------------------------------------------
      // top_bottom_maximize
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::top_bottom_maximize> {
        static left_width get_top (const core::rectangle& r) {
          return {r.x(), r.width()};
        }

        static left_width get_bottom (const core::rectangle& r) {
          return {r.x(), r.width()};
        }

        static top_height get_left (const core::rectangle& r) {
          return {r.y() + TO, r.height() - (BO + TO)};
        }

        static top_height get_right (const core::rectangle& r) {
          return {r.y() + TO, r.height() - (BO + TO)};
        }
      };

      // --------------------------------------------------------------------------
      // left_right_maximize
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::left_right_maximize> {
        static left_width get_top (const core::rectangle& r) {
          return {r.x() + LE, r.width() - (RI + LE)};
        }

        static left_width get_bottom (const core::rectangle& r) {
          return {r.x() + LE, r.width() - (RI + LE)};
        }

        static top_height get_left (const core::rectangle& r) {
          return {r.y(), r.height()};
        }

        static top_height get_right (const core::rectangle& r) {
          return {r.y(), r.height()};
        }
      };

      // --------------------------------------------------------------------------
      // bottom_max_top_min
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::bottom_max_top_min> {
        static left_width get_top (const core::rectangle& r) {
          return {r.x() + LE, r.width() - (RI + LE)};
        }

        static left_width get_bottom (const core::rectangle& r) {
          return {r.x(), r.width()};
        }

        static top_height get_left (const core::rectangle& r) {
          return {r.y(), r.height() - BO};
        }

        static top_height get_right (const core::rectangle& r) {
          return {r.y(), r.height() - BO};
        }
      };

      // --------------------------------------------------------------------------
      // top_max_bottom_min
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::top_max_bottom_min> {
        static left_width get_top (const core::rectangle& r) {
          return {r.x(), r.width()};
        }

        static left_width get_bottom (const core::rectangle& r) {
          return {r.x() + LE, r.width() - (RI + LE)};
        }

        static top_height get_left (const core::rectangle& r) {
          return {r.y() + TO, r.height() - TO};
        }

        static top_height get_right (const core::rectangle& r) {
          return {r.y() + TO, r.height() - TO};
        }
      };

      // --------------------------------------------------------------------------
      // left_max_right_min
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::left_max_right_min> {
        static left_width get_top (const core::rectangle& r) {
          return {r.x() + LE, r.width() - LE};
        }

        static left_width get_bottom (const core::rectangle& r) {
          return {r.x() + LE, r.width() - LE};
        }

        static top_height get_left (const core::rectangle& r) {
          return {r.y(), r.height()};
        }

        static top_height get_right (const core::rectangle& r) {
          return {r.y() + TO, r.height() - (BO + TO)};
        }
      };

      // --------------------------------------------------------------------------
      // right_max_left_min
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::right_max_left_min> {
        static left_width get_top (const core::rectangle& r) {
          return {r.x(), r.width() - RI};
        }

        static left_width get_bottom (const core::rectangle& r) {
          return {r.x(), r.width() - RI};
        }

        static top_height get_left (const core::rectangle& r) {
          return {r.y() + TO, r.height() - (BO + TO)};
        }

        static top_height get_right (const core::rectangle& r) {
          return {r.y(), r.height()};
        }
      };

      // --------------------------------------------------------------------------
      // top_left_maximize
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::top_left_maximize> {
        static left_width get_top (const core::rectangle& r) {
          return {r.x() + LE, r.width() - LE};
        }

        static left_width get_bottom (const core::rectangle& r) {
          return {r.x() + LE, r.width() - (LE + RI)};
        }

        static top_height get_left (const core::rectangle& r) {
          return {r.y() + TO, r.height() - TO};
        }

        static top_height get_right (const core::rectangle& r) {
          return {r.y() + TO, r.height() - (TO + BO)};
        }
      };

      // --------------------------------------------------------------------------
      // bottom_right_maximize
      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type_t::bottom_right_maximize> {
        static left_width get_top (const core::rectangle& r) {
          return {r.x() + LE, r.width() - (LE + RI)};
        }

        static left_width get_bottom (const core::rectangle& r) {
          return {r.x(), r.width() - RI};
        }

        static top_height get_left (const core::rectangle& r) {
          return {r.y() + TO, r.height() - (TO + BO)};
        }

        static top_height get_right (const core::rectangle& r) {
          return {r.y(), r.height() - BO};
        }
      };

      // --------------------------------------------------------------------------
      inline const layout_function& layout_base::get_center() const {
        return data.center;
      }

      inline const layout_function& layout_base::get_top() const {
        return data.top;
      }

      inline const layout_function& layout_base::get_bottom() const {
        return data.bottom;
      }

      inline const layout_function& layout_base::get_left() const {
        return data.left;
      }

      inline const layout_function& layout_base::get_right() const {
        return data.right;
      }

      inline void layout_base::set_center(const layout_function& center) {
        data.center = center;
      }

      inline void layout_base::set_top(const layout_function& top) {
        data.top = top;
      }

      inline void layout_base::set_bottom(const layout_function& bottom) {
        data.bottom = bottom;
      }

      inline void layout_base::set_left(const layout_function& left) {
        data.left = left;
      }

      inline void layout_base::set_right(const layout_function& right) {
        data.right = right;
      }

      inline void layout_base::set_center_top_bottom_left_right(const layout_function& center,
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

      inline void layout_base::add(const std::vector<std::reference_wrapper<win::window>>& list) {
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
      layouter<TO, BO, LE, RI, T>::layouter (win::container*)
      {}

      template<int TO, int BO, int LE, int RI, type_t T>
      inline int layouter<TO, BO, LE, RI, T>::get_top_height () const {
        return TO;
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline int layouter<TO, BO, LE, RI, T>::get_bottom_height () const {
        return BO;
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline int layouter<TO, BO, LE, RI, T>::get_left_width () const {
        return LE;
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline int layouter<TO, BO, LE, RI, T>::get_right_width () const {
        return RI;
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline core::rectangle layouter<TO, BO, LE, RI, T>::get_center_geometry (const core::rectangle& r) const {
        return r.shrinked(TO, BO, LE, RI);
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline core::rectangle layouter<TO, BO, LE, RI, T>::get_top_geometry (const core::rectangle& r) const {
        const left_width pt = geometrie::get_top(r);
        return core::rectangle(pt.left, r.y(), pt.width, TO);
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline core::rectangle layouter<TO, BO, LE, RI, T>::get_bottom_geometry (const core::rectangle& r) const {
        const left_width pt = geometrie::get_bottom(r);
        return core::rectangle(pt.left, r.y2() - BO, pt.width, BO);
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline core::rectangle layouter<TO, BO, LE, RI, T>::get_left_geometry (const core::rectangle& r) const {
        const top_height pt = geometrie::get_left(r);
        return core::rectangle(r.x(), pt.top, LE, pt.height);
      }

      template<int TO, int BO, int LE, int RI, type_t T>
      inline core::rectangle layouter<TO, BO, LE, RI, T>::get_right_geometry (const core::rectangle& r) const {
        const top_height pt = geometrie::get_right(r);
        return core::rectangle(r.x2() - RI, pt.top, RI, pt.height);
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
      inline const layout_function& sym_layouter<TO, BO, LE, RI>::get_top_left () const {
        return data.top_left;
      }

      template<int TO, int BO, int LE, int RI>
      inline const layout_function& sym_layouter<TO, BO, LE, RI>::get_top_right () const {
        return data.top_right;
      }

      template<int TO, int BO, int LE, int RI>
      inline const layout_function& sym_layouter<TO, BO, LE, RI>::get_bottom_left () const {
        return data.bottom_left;
      }

      template<int TO, int BO, int LE, int RI>
      inline const layout_function& sym_layouter<TO, BO, LE, RI>::get_bottom_right () const {
        return data.bottom_right;
      }

      template<int TO, int BO, int LE, int RI>
      inline void sym_layouter<TO, BO, LE, RI>::set_top_left (const layout_function& top_left) {
        data.top_left = top_left;
      }

      template<int TO, int BO, int LE, int RI>
      inline void sym_layouter<TO, BO, LE, RI>::set_top_right (const layout_function& top_right) {
        data.top_right = top_right;
      }

      template<int TO, int BO, int LE, int RI>
      inline void sym_layouter<TO, BO, LE, RI>::set_bottom_left (const layout_function& bottom_left) {
        data.bottom_left = bottom_left;
      }

      template<int TO, int BO, int LE, int RI>
      inline void sym_layouter<TO, BO, LE, RI>::set_bottom_right (const layout_function& bottom_right) {
        data.bottom_right = bottom_right;
      }

      template<int TO, int BO, int LE, int RI>
      void sym_layouter<TO, BO, LE, RI>::layout (const core::rectangle& r) {
        logging::trace() << "border::sym_layouter(" << r << ")";
        if (get_top_left()) {
          get_top_left()(core::rectangle(r.x(), r.y(), LE, TO));
        }
        if (get_top_right()) {
          get_top_right()(core::rectangle(r.x2() - RI, r.y(), RI, TO));
        }
        if (get_bottom_left()) {
          get_bottom_left()(core::rectangle(r.x(), r.y2() - BO, LE, BO));
        }
        if (get_bottom_right()) {
          get_bottom_right()(core::rectangle(r.x2() - RI, r.y2() - BO, RI, BO));
        }
        super::layout(r);
      }

    } // namespace border

  } // namespace layout

} // namespace gui
