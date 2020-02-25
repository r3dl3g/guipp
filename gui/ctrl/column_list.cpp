/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic controls
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/column_list.h>


namespace gui {

  namespace layout {

    namespace detail {
      // --------------------------------------------------------------------------
      column_size_type column_list_layout::get_column_left_pos (std::size_t i) const {
        column_size_type w = 0;
        for (decltype(i) j = 0; j < i; ++j) {
          w += get_column_width(j);
        }
        return w;
      }

      int column_list_layout::index_at (core::point::type pt) const {
        core::point::type pos = 0;
        int idx = 0;
        while (pos <= pt) {
          pos += get_column_width(idx);
          ++idx;
        }
        return idx - 1;
      }

      int column_list_layout::split_idx_at (core::point::type pt, core::size::type delta) const {
        const core::point::type lower = pt - delta;
        const core::point::type upper = pt + delta;
        core::point::type pos = 0;
        int idx = 0;
        while (pos < lower) {
          pos += get_column_width(idx);
          ++idx;
        }
        if ((lower <= pos) && (pos < upper)) {
          return idx - 1;
        }
        return -1;
      }

      column_size_type column_list_layout::get_column_right_pos (std::size_t i) const {
        return get_column_left_pos(i + 1);
      }

      core::size::type column_list_layout::get_available_width () const {
        if (list) {
          return list->content_size(list->client_size()).width();
        }
        return 0;
      }

      void column_list_layout::set_column_count (std::size_t i) {
        widths.resize(i);
        aligns.resize(i);
      }

      void column_list_layout::set_columns (std::initializer_list<column_info> infos, bool update) {
        set_column_count(infos.size());

        int idx = 0;
        for (const column_info& i : infos) {
          aligns[idx] = i.align;
          widths[idx] = i.width;
          ++idx;
        }
        if (update) {
          redraw_views();
        }
      }

      void column_list_layout::set_column_width (std::size_t i, column_size_type w, bool update) {
        if (widths[i] != w) {
          widths[i] = w;
          if (update) {
            redraw_views();
          }
        }
      }

      void column_list_layout::set_column_align (std::size_t i, text_origin a) {
        aligns[i] = a;
      }

      void column_list_layout::set_column_info (std::size_t i, const column_info& info, bool update) {
        aligns[i] = info.align;
        set_column_width(i, info.width, update);
      }

      void column_list_layout::redraw_views () {
        main->invalidate();
        list->invalidate();
      }

      void column_list_layout::set_default_width (column_size_type w) {
        for (column_size_type& width  : widths) {
          width = w;
        }
      }

      void column_list_layout::set_default_align (text_origin a) {
        for (text_origin& align  : aligns) {
          align = a;
        }
      }

      void column_list_layout::layout (const core::rectangle&) {
      }

    } // detail

    // --------------------------------------------------------------------------
    void simple_column_list_layout::set_columns (std::initializer_list<simple_column_info> infos, bool update) {
      set_column_count(infos.size());

      int idx = 0;
      for (const simple_column_info& i : infos) {
        aligns[idx] = i.align;
        widths[idx] = i.width;
        min_widths[idx] = i.min_width;
        ++idx;
      }
      if (update) {
        redraw_views();
      }
    }

    // --------------------------------------------------------------------------
    void weight_column_list_layout::set_columns (std::initializer_list<weight_column_info> infos, bool update) {
      set_column_count(infos.size());

      int idx = 0;
      for (const weight_column_info& i : infos) {
        aligns[idx] = i.align;
        widths[idx] = i.width;
        min_widths[idx] = i.min_width;
        weights[idx] = i.weight;
        ++idx;
      }
      if (update) {
        redraw_views();
      }
    }

    void weight_column_list_layout::set_column_width (std::size_t i, column_size_type w, bool update) {
      auto count = get_column_count();

      // adjust only the columns on the right

      float right_weight = 0.0F;
      column_size_type right_width = 0;
      column_size_type right_min_width = 0;

      for (decltype(count) j = i + 1; j < count; ++j) {
        right_weight += get_column_weight(j);
        right_width += get_column_width(j);
        right_min_width += get_column_min_width(j);
      }

      if (0.0F == right_weight) {
        right_weight = 1.0F / (count - i);
      }

      const core::size::type available_width = get_available_width();
      const column_size_type left = get_column_left_pos(i);

      const column_size_type space = available_width - (right_min_width + left);
      if (space < 1) {
        // no space left
        return;
      }

      const column_size_type old_w = get_column_width(i);
      super::set_column_width(i, std::min(w, space), false);
      const column_size_type new_w = get_column_width(i);

      if (old_w != new_w) {
        const column_size_type open_space = available_width - left - new_w - right_width;

        for (decltype(count) j = i + 1; j < count; ++j) {
          column_size_type cw = get_column_width(j) + static_cast<column_size_type>(open_space * get_column_weight(j) / right_weight);
          super::set_column_width(j, cw, false);
        }
      }

      if (update) {
        redraw_views();
      }
    }

    void weight_column_list_layout::layout (const core::rectangle& sz) {
      log_trace << "weight_column_list_layout::layout(" << sz << ")";
      auto count = get_column_count();

      float full_weight = 0.0F;
      column_size_type full_width = 0;

      for (decltype(count) i = 0; i < count; ++i) {
        full_weight += get_column_weight(i);
        full_width += get_column_width(i);
      }

      if (0.0F == full_weight) {
        full_weight = 1.0F;
      }

      core::size::type available_width = sz.width();
      column_size_type space = available_width - full_width;


      for (decltype(count) i = 0; i < count; ++i) {
        column_size_type w = get_column_width(i) + static_cast<column_size_type>(space * get_column_weight(i) / full_weight);
        super::set_column_width(i, w, false);
      }
    }

  } // layout

  namespace ctrl {

    void default_header_cell_drawer (std::size_t i,
                                     const draw::graphics& g,
                                     const core::rectangle& r,
                                     const draw::brush& background) {
      using namespace draw;
      const core::rectangle r2 = frame::raised_relief(g, r);
      g.fill(rectangle(r2), background);
      g.text(text_box(ostreamfmt((i + 1) << '.'), r2, text_origin::center), font::system(), color::windowTextColor());
    }


  } // ctrl

} // gui
