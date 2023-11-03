/**
 * @copyright (c) 2016-2023 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     list control with entries uneven in size
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace ctrl {

    namespace detail {

      template<orientation_t O>
      uneven_list_traits<O>::uneven_list_traits ()
      {}

      template<orientation_t O>
      auto uneven_list_traits<O>::get_offset_of_index (const core::size& list_size,
                                                       int idx) const -> dim_type {
        dim_type pos = 0;
        for (int i = 0; i < idx; ++i) {
          pos += get_item_dimension(i, list_size);
        }
        return pos;
      }

      template<orientation_t O>
      int uneven_list_traits<O>::get_index_at_point (const core::rectangle& list_area,
                                                     const core::point& pt,
                                                     const core::point& scroll_pos,
                                                     size_t count) const {
        const auto csz = list_area.size();
        dim_type pos = -otraits::get_1(scroll_pos);
        dim_type sz = 0;
        for (int i = 0; i < count; ++i) {
          sz = get_item_dimension(i, csz);
          if ((pos < otraits::get_1(pt)) && (pos + sz > otraits::get_1(pt))) {
            return i;
          }
          pos += sz;
        }
        return -1;
      }

      template<orientation_t O>
      core::rectangle 
      uneven_list_traits<O>::get_geometry_of_index (const core::rectangle& list_area,
                                                    int idx,
                                                    const core::point& scroll_pos) const {
        const auto csz = list_area.size();
        const auto pos = get_offset_of_index(csz, idx);
        const auto sz = get_item_dimension(idx, csz);

        core::rectangle place;
        otraits::set_1(place, otraits::get_1(list_area.position()) + otraits::get_1(scroll_pos) + pos, sz);
        otraits::set_2(place, otraits::get_2(list_area.position()) - otraits::get_2(scroll_pos), otraits::get_2(list_area.size()));
        return place;
      }

      template<orientation_t O>
      std::size_t uneven_list_traits<O>::get_items_per_page (const core::size& page_size, int idx) const {
        const auto sz_max = otraits::get_1(page_size);
        size_type pos = 0;
        for (; pos < sz_max; ++idx) {
          pos += get_item_dimension(idx, page_size);
        }
        return idx - 1;
      }

      template<orientation_t O>
      auto uneven_list_traits<O>::get_list_dimension (const list_base& list) const -> dim_type {
        const auto count = list.get_count();
        const auto csz = list.client_size();
        size_type sz = 0;
        for (int i = 0; i < count; ++i) {
          sz += get_item_dimension(i, csz);
        }
        return sz;
      }

      template<orientation_t O>
      auto uneven_list_traits<O>::get_line_size () const -> dim_type {
        return get_item_dimension(0, {100, 100});
      }

      template<orientation_t O>
      auto uneven_list_traits<O>::get_item_dimension (int idx, const core::size& sz) const -> dim_type {
        return size(idx, sz);
      }

      template<orientation_t O>
      int uneven_list_traits<O>::get_direction_step (os::key_symbol key, const core::size&) const {
        return linear_list_traits<O>::get_step(key);
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation_t O, typename S>
    uneven_list<O, S>::uneven_list (os::color background,
                                    bool grab_focus)
      : super(background, grab_focus)
    {
      init();
    }

    template<orientation_t O, typename S>
    uneven_list<O, S>::uneven_list (size_fn fn,
                                    os::color background,
                                    bool grab_focus)
      : super(background, grab_focus)
    {
      super::traits.size = fn;
      init();
    }

    template<orientation_t O, typename S>
    core::rectangle uneven_list<O, S>::get_virtual_geometry (const core::rectangle& area) const {
        core::rectangle place;
        super::otraits::set_1(place, 0, super::traits.get_list_dimension(*this));
        super::otraits::set_2(place, 0, super::otraits::get_2(area.size()));
        return place;
    }

    template<orientation_t O, typename S>
    core::size uneven_list<O, S>::get_scroll_steps () const {
      return super::client_size() / 50.0F;
    }

    template<orientation_t O, typename S>
    void uneven_list<O, S>::paint (draw::graphics& graph) {
        const core::rectangle area = super::client_geometry();
        core::rectangle place = area;

        draw::brush back_brush(super::get_background());

        const auto list_sz = super::otraits::get_1(area.x2y2());
        const auto last = super::get_count();
        const auto sp1 = super::otraits::get_1(super::get_scroll_pos());
        const auto sp2 = super::otraits::get_2(super::get_scroll_pos());

        super::otraits::set_2(place, -sp2, super::otraits::get_2(area.size()) + sp2);

        dim_type pos = -sp1;
        for (int idx = 0; (idx < last) && (pos < list_sz); ++idx) {
          const dim_type isz = super::traits.get_item_dimension(idx, area.size());
          super::otraits::set_1(place, pos, isz);
          pos += isz;
          if (pos > 0) {
            super::draw_item(idx, graph, place, back_brush, super::get_item_state(static_cast<int>(idx)));
          }
        }

        if (pos < list_sz) {
          super::otraits::set_1(place, pos, list_sz - pos);
          graph.fill(draw::rectangle(place), back_brush);
        }

        if (super::is_focused()) {
          draw::frame::dots(graph, area);
        }
    }

    template<orientation_t O, typename S>
    void uneven_list<O, S>::init () {
        super::on_paint(draw::paint(this, &uneven_list::paint));
        super::on_show([&] () {
          super::invalidate();
        });
    }

    template<orientation_t O, typename S>
    void uneven_list<O, S>::set_size_function (size_fn fn) {
      super::traits.size = fn;
      super::invalidate();
    }

  } // ctrl

} // gui
