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

    namespace border {

      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type::all_symmetric> {
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
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type::top_bottom_maximize> {
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
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type::left_right_maximize> {
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
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type::bottom_max_top_min> {
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
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type::top_max_bottom_min> {
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
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type::left_max_right_min> {
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
      template<int TO, int BO, int LE, int RI>
      struct border_geometrie<TO, BO, LE, RI, type::right_max_left_min> {
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
      template<int TO, int BO, int LE, int RI, type T>
      layouter<TO, BO, LE, RI, T>::layouter (win::container*)
      {}

      template<int TO, int BO, int LE, int RI, type T>
      inline const layout_function& layouter<TO, BO, LE, RI, T>::get_center () const {
        return data.center;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline const layout_function& layouter<TO, BO, LE, RI, T>::get_top () const {
        return data.top;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline const layout_function& layouter<TO, BO, LE, RI, T>::get_bottom () const {
        return data.bottom;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline const layout_function& layouter<TO, BO, LE, RI, T>::get_left () const {
        return data.left;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline const layout_function& layouter<TO, BO, LE, RI, T>::get_right () const {
        return data.right;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline void layouter<TO, BO, LE, RI, T>::set_center (layout_function center) {
        data.center = center;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline void layouter<TO, BO, LE, RI, T>::set_top (layout_function top) {
        data.top = top;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline void layouter<TO, BO, LE, RI, T>::set_bottom (layout_function bottom) {
        data.bottom = bottom;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline void layouter<TO, BO, LE, RI, T>::set_left (layout_function left) {
        data.left = left;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline void layouter<TO, BO, LE, RI, T>::set_right (layout_function right) {
        data.right = right;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline int layouter<TO, BO, LE, RI, T>::get_top_height () const {
        return TO;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline int layouter<TO, BO, LE, RI, T>::get_bottom_height () const {
        return BO;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline int layouter<TO, BO, LE, RI, T>::get_left_width () const {
        return LE;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline int layouter<TO, BO, LE, RI, T>::get_right_width () const {
        return RI;
      }

      template<int TO, int BO, int LE, int RI, type T>
      inline void layouter<TO, BO, LE, RI, T>::set_center_top_bottom_left_right (layout_function center,
                                                                                 layout_function top,
                                                                                 layout_function bottom,
                                                                                 layout_function left,
                                                                                 layout_function right) {
        data.center = center;
        data.top = top;
        data.bottom = bottom;
        data.left = left;
        data.right = right;
      }

      template<int TO, int BO, int LE, int RI, type T>
      void layouter<TO, BO, LE, RI, T>::layout (const core::rectangle& r) {
        clog::trace() << "border::layouter(" << r << ")";
        if (get_top()) {
          const left_width pt = geometrie::get_top(r);
          get_top()(core::rectangle(pt.left, r.y(), pt.width, TO));
        }
        if (get_bottom()) {
          const left_width pt = geometrie::get_bottom(r);
          get_bottom()(core::rectangle(pt.left, r.y2() - BO, pt.width, BO));
        }
        if (get_left()) {
          const top_height pt = geometrie::get_left(r);
          get_left()(core::rectangle(r.x(), pt.top, LE, pt.height));
        }
        if (get_right()) {
          const top_height pt = geometrie::get_right(r);
          get_right()(core::rectangle(r.x2() - RI, pt.top, RI, pt.height));
        }
        if (get_center()) {
          get_center()(r.shrinked(TO, BO, LE, RI));
        }
      }

    } // namespace border

  } // namespace layout

} // namespace gui
