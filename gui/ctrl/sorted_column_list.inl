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
 * @brief     C++ API: separator view
 *
 * @file
 */

#pragma once

namespace gui {

  namespace ctrl {

    template<typename L, typename ... A>
    sorted_column_list_t<L, A...>::sorted_column_list_t (gui::core::size::type item_size,
                                                      gui::os::color background,
                                                      bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      init();
    }

    template<typename L, typename ... A>
    sorted_column_list_t<L, A...>::sorted_column_list_t (const sorted_column_list_t& rhs)
      : super(rhs)
    {
      init();
    }

    template<typename L, typename ... A>
    sorted_column_list_t<L, A...>::sorted_column_list_t (sorted_column_list_t&& rhs)
      : super(std::move(rhs))
    {
      init();
    }

    template<typename L, typename ... A>
    void sorted_column_list_t<L, A...>::on_sort (std::function<sort_callback> s) {
      sorter = s;
    }

    template<typename L, typename ... A>
    void sorted_column_list_t<L, A...>::sort () {
      if (sorter) {
        if (!sorter(sort_dir, sort_column)) {
          sort_column = -1;
        }
      }
      super::list.invalidate();
      super::header.invalidate();
    }

    template<typename L, typename ... A>
    void sorted_column_list_t<L, A...>::init () {
      using namespace gui;
      using namespace gui::draw;

      mouse_down_point = core::point::undefined;
      sort_column = -1;
      sort_dir = util::sort::order::up;

      super::header.set_cell_drawer([&] (std::size_t i,
                                         const gui::draw::graphics& g,
                                         const gui::core::rectangle& r,
                                         const draw::brush& background) {
        g.fill(draw::rectangle(r), background);
        frame::raised_deep_relief(g, r);
        g.text(text_box(header_label[i], r, text_origin_t::center), draw::font::system(), color::windowTextColor());
        if (sort_column == i) {
          core::rectangle s = r.right_width(r.height()).shrinked({7, 8});
          if (sort_dir == util::sort::order::up) {
            draw_arrow_up(g, s, color::black);
          } else {
            gui::ctrl::draw_arrow_down(g, s, color::black);
          }
        }
      });
      super::header.on_left_btn_down([&] (gui::os::key_state, const core::point& pt) {
        mouse_down_point = pt;
      });
      super::header.on_left_btn_up([&] (gui::os::key_state, const core::point& pt) {
        if (mouse_down_point == pt) {
          int i = super::get_column_layout().index_at(pt.x());
          if (i < 6) {
            int new_sort_column = i;
            if (sort_column == new_sort_column) {
              sort_dir = ~sort_dir;
            } else {
              sort_column = new_sort_column;
              sort_dir = util::sort::order::up;
            }
            sort();
          }
        }
        mouse_down_point = core::point::undefined;
      });
    }

  } // ctrl

} // gui
