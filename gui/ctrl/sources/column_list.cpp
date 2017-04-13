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
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "column_list.h"


namespace gui {

#ifdef WIN32
#endif // WIN32

#ifdef X11
#endif // X11

  namespace layout {

    namespace detail {
      // --------------------------------------------------------------------------
      column_size_type column_list_layout::get_column_left_pos(std::size_t i) const {
        column_size_type w = 0;
        for (int j = 0; j < i; ++j) {
          w += get_column_width(j);
        }
        return w;
      }

      column_size_type column_list_layout::get_column_right_pos(std::size_t i) const {
        return get_column_left_pos(i + 1);
      }

      core::size::type column_list_layout::get_available_width(const core::size& sz) const {
        return sz.width() - (list && list->is_scroll_bar_visible() ? win::scroll_bar::get_scroll_bar_width() : 0);
      }

      core::size::type column_list_layout::get_available_width() const {
        if (list) {
          return get_available_width(main->client_size());
        }
        return 0;
      }

      void column_list_layout::set_column_count(std::size_t i) {
        widths.resize(i);
        aligns.resize(i);
        sliders.clear(); // forece rebuild
      }

      void column_list_layout::set_columns(std::initializer_list<column_info> infos, bool update) {
        set_column_count(infos.size());

        typedef std::initializer_list<column_info>::iterator iterator;
        int idx = 0;
        for (iterator i = infos.begin(), e = infos.end(); i != e; ++i, ++idx) {
          aligns[idx] = i->align;
          widths[idx] = i->width;
        }
        if (update) {
          update_views();
        }
      }

      void column_list_layout::set_column_width(std::size_t i, column_size_type w, bool update) {
        if (widths[i] != w) {
          widths[i] = w;
          if (update) {
            update_views();
          }
        }
      }

      void column_list_layout::set_column_align(std::size_t i, draw::text_origin a) {
        aligns[i] = a;
        list->redraw_later();
      }

      void column_list_layout::set_slider(std::size_t i, win::detail::slider* s) {
        sliders[i] = s;
      }

      void column_list_layout::set_column_info(std::size_t i, const column_info& info, bool update) {
        aligns[i] = info.align;
        set_column_width(i, info.width, update);
      }

      void column_list_layout::set_slider_creator(std::function<create_sliders> sc) {
        slider_creator = sc;
      }

      void column_list_layout::layout(const core::size& new_size) {
        core::rectangle r(1, 0, 2, new_size.height());
        std::size_t count = get_column_count();
        if (count != sliders.size()) {
          sliders = slider_creator(count);
        }
        for (std::size_t i = 0; i < count; ++i) {
          slider* s = sliders[i];
          r.move_x(get_column_width(i) - 2);
          s->place(r);
          r.move_x(2);
        }
      }

      void column_list_layout::update_views() {
        layout(main->size());
        redraw_views();
      }

      void column_list_layout::redraw_views() {
        main->redraw_later();
        list->redraw_later();
      }
    }

    // --------------------------------------------------------------------------
    void simple_column_list_layout::set_columns(std::initializer_list<simple_column_info> infos, bool update) {
      set_column_count(infos.size());

      typedef std::initializer_list<simple_column_info>::iterator iterator;
      int idx = 0;
      for (iterator i = infos.begin(), e = infos.end(); i != e; ++i, ++idx) {
        aligns[idx] = i->align;
        widths[idx] = i->width;
        min_widths[idx] = i->min_width;
      }
      if (update) {
        update_views();
      }
    }

    // --------------------------------------------------------------------------
    void weight_column_list_layout::set_columns(std::initializer_list<weight_column_info> infos, bool update) {
      set_column_count(infos.size());

      typedef std::initializer_list<weight_column_info>::iterator iterator;
      int idx = 0;
      for (iterator i = infos.begin(), e = infos.end(); i != e; ++i, ++idx) {
        aligns[idx] = i->align;
        widths[idx] = i->width;
        min_widths[idx] = i->min_width;
        weights[idx] = i->weight;
      }
      if (update) {
        update_views();
      }
    }

    void weight_column_list_layout::set_column_width(std::size_t i, column_size_type w, bool update) {
      std::size_t count = get_column_count();

      // adjust only the columns on the right

      float right_weigth = 0.0F;
      column_size_type right_width = 0;
      column_size_type right_min_width = 0;

      for (std::size_t j = i + 1; j < count; ++j) {
        right_weigth += get_column_weight(j);
        right_width += get_column_width(j);
        right_min_width += get_column_min_width(j);
      }

      if (0.0F == right_weigth) {
        right_weigth = 1.0F / (count - i);
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

        for (std::size_t j = i + 1; j < count; ++j) {
          column_size_type cw = get_column_width(j) + static_cast<column_size_type>(open_space * get_column_weight(j) / right_weigth);
          super::set_column_width(j, cw, false);
        }
      }

      if (update) {
        update_views();
      }
    }

    void weight_column_list_layout::layout (const core::size& sz) {
      std::size_t count = get_column_count();

      float full_weigth = 0.0F;
      column_size_type full_width = 0;

      for (std::size_t i = 0; i < count; ++i) {
        full_weigth += get_column_weight(i);
        full_width += get_column_width(i);
      }

      if (0.0F == full_weigth) {
        full_weigth = 1.0F;
      }

      core::size::type available_width = get_available_width(sz);
      column_size_type space = available_width - full_width;


      for (std::size_t i = 0; i < count; ++i) {
        column_size_type w = get_column_width(i) + static_cast<column_size_type>(space * get_column_weight(i) / full_weigth);
        super::set_column_width(i, w, false);
      }

      super::layout(sz);
      get_slider(count - 1)->disable();
      list->redraw_later();
    }

  }

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      window_class base_column_list_clazz(
#ifdef WIN32
        win::window_class::custom_class("column_list",
                                        CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW,
                                        WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                                        WS_EX_NOPARENTNOTIFY,
                                        0,
                                        IDC_ARROW,
                                        (os::color)(COLOR_WINDOW + 1))
#endif // WIN32
#ifdef X11
        win::window_class::custom_class("column_list")
#endif //X11
      );

    }

  } // win

} // gui
