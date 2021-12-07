/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     table control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace table {

      // --------------------------------------------------------------------------
      namespace data {

        // --------------------------------------------------------------------------
        template<typename T>
        inline vector<T>::vector (const T& default_data)
          : default_data(default_data)
        {}

        template<typename T>
        inline auto vector<T>::get (std::size_t idx) const->const T & {
          if (idx < super::size()) {
            return super::at(idx);
          }
          return default_data;
        }

        template<typename T>
        inline auto vector<T>::operator[] (std::size_t idx) const->const T & {
          return get(idx);
        }

        template<typename T>
        inline void vector<T>::set (std::size_t idx, const T& t) {
          if (super::size() <= idx) {
            super::resize(idx + 1, default_data);
          }
          super::set(idx, t);
        }

        template<typename T>
        inline auto vector<T>::operator[] (std::size_t idx)->T & {
          if (super::size() <= idx) {
            super::resize(idx + 1, default_data);
          }
          return super::operator[](idx);
        }

        template<typename T>
        inline const T& vector<T>::get_default_data () const {
          return default_data;
        }

        // --------------------------------------------------------------------------
        template<typename T>
        inline matrix<T>::matrix (const T& default_data)
          : column_data(default_data)
          , row_data(default_data)
        {}

        template<typename T>
        auto matrix<T>::get_cell (const position &cell) const->const T &{
          if (cell.x() < data.size()) {
            const std::vector<T>& c = data[cell.x()];
            if (cell.y() < c.size()) {
              return c[cell.y()];
            }
          }
          return get_column_row_cell(cell);
        }

        template<typename T>
        void matrix<T>::set_cell (const position& cell, const T& t) {
          const std::size_t data_size = data.size();
          if (data_size <= cell.x()) {
            data.resize(cell.x() + 1);
          }
          std::vector<T>& c = data[cell.x()];
          const int rows = static_cast<int>(c.size());
          if (rows <= cell.y()) {
            c.resize(cell.y() + 1, column_data.get(cell.x()));
            for (int r = rows; r < cell.y(); ++r) {
              c[r] = get_column_row_cell(position(cell.x(), r));
            }
          }
          c[cell.y()] = t;
        }

        template<typename T>
        void matrix<T>::set_column (std::size_t column, const T& t) {
          column_data[column] = t;
          if (column < data.size()) {
            data[column].clear();
          }
        }

        template<typename T>
        void matrix<T>::set_row (std::size_t row, const T& t) {
          row_data[row] = t;
          for (std::vector<T>& column : data) {
            if (row < column.size()) {
              column[row] = t;
            }
          }
          const int column_data_size = static_cast<int>(column_data.size());
          for (int c = 0; c < column_data_size; ++c) {
            set_cell(position(c, static_cast<int>(row)), t);
          }
        }

        template<typename T>
        inline void matrix<T>::clear () {
          data.clear();
          column_data.clear();
          row_data.clear();
        }

        template<typename T>
        inline auto matrix<T>::size () const->position {
          return position {column_data.size(), row_data.size()};
        }

        template<typename T>
        inline auto matrix<T>::get_default_data () const->const T &{
          return column_data.get_default_data();
        }

        template<typename T>
        inline auto matrix<T>::get_column_row_cell (const position &cell) const->const T &{
          if (cell.x() < column_data.size()) {
            return column_data[cell.x()];
          }
          return row_data[cell.y()];
        }

        // --------------------------------------------------------------------------
        inline bool spawn::operator== (const spawn& rhs) const {
          return (x == rhs.x) && (y == rhs.y);
        }

        inline bool spawn::is_hidden () const {
          return (x < 0) || (y < 0);
        }

        inline bool spawn::is_empty () const {
          return (x == 0) && (y == 0);
        }

        inline bool spawn::is_spawn () const {
          return (x > 0) || (y > 0);
        }

      } // data

      // --------------------------------------------------------------------------
      inline layout::layout (core::size::type default_size)
        : first_idx(0)
        , offset(0)
        , first_offset(0)
        , sizes(default_size)
      {}

      inline core::size::type layout::get_default_size () const {
        return sizes.get_default_data();
      }

      inline core::size::type layout::get_size (int idx) const {
        return idx > -1 ? sizes[idx] : 0;
      }

      inline std::size_t layout::get_first_idx () const {
        return first_idx;
      }

      inline core::point::type layout::get_offset () const {
        return offset;
      }

      inline core::point::type layout::get_first_offset () const {
        return first_offset;
      }

      // --------------------------------------------------------------------------
      inline metric::metric (core::size::type default_width,
                             core::size::type default_height)
        : widths(default_width)
        , heights(default_height)
      {}

      inline core::point metric::position_of (const position& cell) const {
        return core::point(widths.position_of(cell.x()), heights.position_of(cell.y()));
      }

      inline position metric::index_at (const core::point& pt) const {
        return position(widths.index_at(pt.x()), heights.index_at(pt.y()));
      }

      inline core::size metric::get_default_size () const {
        return core::size(widths.get_default_size(), heights.get_default_size());
      }

      inline position metric::get_first_idx () const {
        return position(static_cast<position::type>(widths.get_first_idx()),
                        static_cast<position::type>(heights.get_first_idx()));
      }

      inline core::point metric::get_offset () const {
        return core::point(widths.get_offset(), heights.get_offset());
      }

      inline void metric::set_offset (const core::point& pos) {
        widths.set_offset(pos.x());
        heights.set_offset(pos.y());
      }

      inline core::point metric::get_first_offset () const {
        return core::point(widths.get_first_offset(), heights.get_first_offset());
      }

      // --------------------------------------------------------------------------
      namespace filter {

        // --------------------------------------------------------------------------
        inline bool data_selection (const position& cell, const metric& geometrie) {
          return geometrie.selection == cell;
        }

        inline bool data_hilite (const position& cell, const metric& geometrie) {
          return (geometrie.hilite == cell) ||
                 (geometrie.selection.x() == cell.x()) ||
                 (geometrie.selection.y() == cell.y());
        }

        // --------------------------------------------------------------------------
        inline bool column_selection (const position& cell, const metric& geometrie) {
          return geometrie.selection.x() == cell.x();
        }

        inline bool column_hilite (const position& cell, const metric& geometrie) {
          return (geometrie.hilite.x() == cell.x()) || (geometrie.selection.x() == cell.x());
        }

        // --------------------------------------------------------------------------
        inline bool row_selection (const position& cell, const metric& geometrie) {
          return geometrie.selection.y() == cell.y();
        }

        inline bool row_hilite (const position& cell, const metric& geometrie) {
          return (geometrie.hilite.y() == cell.y()) || (geometrie.selection.y() == cell.y());
        }

      } // namespace filter

      // --------------------------------------------------------------------------
      template<template<typename U> class T>
      cell_view<T>::cell_view (metric& geometrie,
                               text_origin_t align,
                               os::color foreground,
                               os::color background,
                               const std::function<filter::selection_and_hilite>& selection_filter,
                               const std::function<filter::selection_and_hilite>& hilite_filter)
        : geometrie(geometrie)
        , aligns(align)
        , foregrounds(foreground)
        , backgrounds(background)
        , selection_filter(selection_filter)
        , hilite_filter(hilite_filter)
      {}

      template<template<typename U> class T>
      cell_view<T>::cell_view (metric& geometrie,
                               const cell_view& rhs)
        : geometrie(geometrie)
        , aligns(rhs.aligns)
        , foregrounds(rhs.foregrounds)
        , backgrounds(rhs.backgrounds)
        , selection_filter(rhs.selection_filter)
        , hilite_filter(rhs.hilite_filter)
      {}

      template<template<typename U> class T>
      cell_view<T>::cell_view (metric& geometrie,
                               cell_view&& rhs)
        : geometrie(geometrie)
        , aligns(std::move(rhs.aligns))
        , foregrounds(std::move(rhs.foregrounds))
        , backgrounds(std::move(rhs.backgrounds))
        , selection_filter(std::move(rhs.selection_filter))
        , hilite_filter(std::move(rhs.hilite_filter))
      {}

      template<template<typename U> class T>
      void cell_view<T>::create (win::container& parent,
                                 const core::rectangle& place) {
        create(clazz::get(), parent, place);
      }

      template<template<typename U> class T>
      inline void cell_view<T>::set_drawer (const std::function<cell_drawer>& drawer) {
        this->drawer = drawer;
      }

      template<template<typename U> class T>
      inline void cell_view<T>::set_drawer (std::function<cell_drawer>&& drawer) {
        this->drawer = std::move(drawer);
      }

      template<template<typename U> class T>
      inline const std::function<cell_drawer>& cell_view<T>::get_drawer () const {
        return drawer;
      }

      template<template<typename U> class T>
      inline text_origin_t cell_view<T>::get_default_align () const {
        return aligns.get_default_data();
      }

      template<template<typename U> class T>
      inline os::color cell_view<T>::get_default_foreground () const {
        return foregrounds.get_default_data();
      }

      template<template<typename U> class T>
      inline os::color cell_view<T>::get_default_background () const {
        return backgrounds.get_default_data();
      }

      template<template<typename U> class T>
      inline const std::function<filter::selection_and_hilite>& cell_view<T>::get_selection_filter () const {
        return selection_filter;
      }

      template<template<typename U> class T>
      inline const std::function<filter::selection_and_hilite>& cell_view<T>::get_hilite_filter () const {
        return hilite_filter;
      }

      template<template<typename U> class T>
      inline void cell_view<T>::set_selection_filter (const std::function<filter::selection_and_hilite>& f) {
        selection_filter = f;
      }

      template<template<typename U> class T>
      inline void cell_view<T>::set_hilite_filter (const std::function<filter::selection_and_hilite>& f) {
        hilite_filter = f;
      }

    } // table

    // --------------------------------------------------------------------------
    inline void table_edit::set_enable_edit (bool enable) {
      enable_edit = enable;
    }

    inline bool table_edit::is_edit_enabled () const {
      return enable_edit;
    }

  } // ctrl

} // gui
