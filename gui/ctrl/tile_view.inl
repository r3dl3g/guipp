/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: tile view
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    template<typename T>
    inline void default_tile_item_drawer (const T& t,
                                          draw::graphics& g,
                                          const core::rectangle& place,
                                          const draw::brush& background,
                                          item_state state) {
      gui::look::text_item(g, place, background,
                           util::string::convert::from<T>(t),
                           state, text_origin_t::center);
    }

    // --------------------------------------------------------------------------
    template<>
    inline core::size::type tile_list_traits<orientation_t::horizontal>::get_line_size () const {
      return item_size.width() + spacing.width();
    }

    template<>
    inline core::size::type tile_list_traits<orientation_t::horizontal>::get_item_dimension () const {
      return item_size.height();
    }

    template<>
    inline core::size::type tile_list_traits<orientation_t::horizontal>::get_item_border () const {
      return border.height();
    }

    template<>
    inline core::size::type tile_list_traits<orientation_t::horizontal>::get_line_border () const {
      return border.width();
    }

    template<>
    inline core::size::type tile_list_traits<orientation_t::horizontal>::get_item_spacing () const {
      return spacing.height();
    }

    template<>
    inline core::size::type tile_list_traits<orientation_t::horizontal>::get_line_spacing () const {
      return spacing.width();
    }

    // --------------------------------------------------------------------------
    template<>
    inline core::size::type tile_list_traits<orientation_t::vertical>::get_line_size () const {
      return item_size.height() + spacing.height();
    }

    template<>
    inline core::size::type tile_list_traits<orientation_t::vertical>::get_item_dimension () const {
      return item_size.width();
    }

    template<>
    inline core::size::type tile_list_traits<orientation_t::vertical>::get_item_border () const {
      return border.width();
    }

    template<>
    inline core::size::type tile_list_traits<orientation_t::vertical>::get_line_border () const {
      return border.height();
    }

    template<>
    inline core::size::type tile_list_traits<orientation_t::vertical>::get_item_spacing () const {
      return spacing.width();
    }

    template<>
    inline core::size::type tile_list_traits<orientation_t::vertical>::get_line_spacing () const {
      return spacing.height();
    }

    // --------------------------------------------------------------------------
    template<orientation_t V>
    inline tile_list_traits<V>::tile_list_traits (const size_type& item_size)
      : item_size(item_size)
    {}

    template<orientation_t V>
    inline auto tile_list_traits<V>::get_invisible_size (const core::size& list_size,
                                                         size_t count) const -> dim_type {
      const auto ipl = get_items_per_line(list_size);
      const auto lines = (ipl > 0 ? core::div_ceil(count, ipl) : 1);
      return std::max(dim_type(0), ((get_line_size()) * lines + get_line_border() * 2) - super::get_1(list_size));
    }

    template<orientation_t V>
    int tile_list_traits<V>::get_index_at_point (const core::rectangle& list_area,
                                                 const core::point& pt,
                                                 dim_type scroll_pos,
                                                 size_t count) const {
      const auto per_line = static_cast<int>(get_items_per_line(list_area.size()));
      const auto line = static_cast<int>((super::get_1(pt) + scroll_pos - get_line_border()) / get_line_size());
      const auto offs = static_cast<int>((super::get_2(pt) - get_item_border()) / (get_item_dimension() + get_item_spacing()));
      const auto idx = (line * per_line) + offs;
      return (idx < count) && get_place_of_index(list_area, idx, scroll_pos).is_inside(pt) ? idx : -1;
    }

    template<orientation_t V>
    core::rectangle tile_list_traits<V>::get_place_of_index (const core::rectangle& list_area,
                                                             int idx,
                                                             dim_type scroll_pos) const {
      const auto per_line = get_items_per_line(list_area.size());
      const auto line = per_line > 0 ? static_cast<std::size_t>(idx) / per_line : 0;
      const auto offs = idx - (line * per_line);

      const auto lsz = get_line_size();
      const auto isz = get_item_dimension();
      const auto pos = list_area.position();

      core::rectangle place;
      super::set_1(place, super::get_1(pos) + lsz * line - scroll_pos + get_line_border(), lsz - get_line_spacing());
      super::set_2(place, super::get_2(pos) + (isz + get_item_spacing()) * offs + get_item_border(), isz);
      return place;
    }

    template<orientation_t V>
    inline auto tile_list_traits<V>::get_offset_of_index (const core::size& list_size, int idx) const -> dim_type {
      const int line = idx / static_cast<int>(get_items_per_line(list_size));
      return(get_line_size() * line) + get_line_border();
    }

    template<orientation_t V>
    inline std::size_t tile_list_traits<V>::get_items_per_line (const core::size& list_size) const {
      const auto isp = get_item_spacing();
      const auto avail = (super::get_2(list_size) - get_item_border() * 2 + isp);
      const auto need = (get_item_dimension() + isp);
      return static_cast<std::size_t>(std::max(avail / need, 1.0f));
    }

    template<orientation_t V>
    inline std::size_t tile_list_traits<V>::get_line_count (size_t count,
                                                            const core::size& list_size) const {
      return core::div_ceil(count, get_items_per_line(list_size));
    }

    // --------------------------------------------------------------------------
    template<orientation_t V>
    inline basic_tile_view<V>::basic_tile_view (const core::size& item_size,
                                                os::color background,
                                                bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      init();
    }

    template<orientation_t V>
    inline basic_tile_view<V>::basic_tile_view (const basic_tile_view& rhs)
      : super(rhs)
    {
      init();
    }

    template<orientation_t V>
    inline basic_tile_view<V>::basic_tile_view (basic_tile_view&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation_t V>
    inline std::size_t basic_tile_view<V>::get_line_count () const {
      return super::traits.get_line_count(super::get_count(), super::content_size(super::client_size(), true));
    }

    template<orientation_t V>
    core::rectangle basic_tile_view<V>::get_full_place_of_index (int idx) {
      const core::rectangle list_area = super::content_area(super::surface_area());

      const auto per_line = super::traits.get_items_per_line(list_area.size());
      const auto line = per_line > 0 ? static_cast<std::size_t>(idx) / per_line : 0;
      const auto offs = idx - (line * per_line);

      const auto lsz = super::traits.get_line_size();
      const auto isz = super::traits.get_item_dimension() + super::traits.get_item_spacing();
      const auto pos = list_area.position();

      core::rectangle place;
      super::traits.set_1(place, super::traits.get_1(pos) + lsz * line - super::get_scroll_pos() + super::traits.get_line_border(), lsz + 1);
      super::traits.set_2(place, super::traits.get_2(pos) + isz * offs + super::traits.get_item_border(), isz + 1);
      return place;
    }

    template<orientation_t V>
    void basic_tile_view<V>::paint (draw::graphics& graph) {
      const core::rectangle area = super::content_area(super::surface_area());
      draw::clip clp(graph, area);

      draw::brush back_brush(super::get_background());

      const auto last = super::get_count();
      const auto lsz = super::traits.get_line_size();
      if ((last < 1) || (lsz < 1)) {
        graph.fill(draw::rectangle(area), back_brush);
      } else {
        const auto list_max = super::traits.get_1(area.x2y2());
        const auto isp = super::traits.get_item_spacing();
        const auto lsp = super::traits.get_line_spacing();
        const auto scp = super::get_scroll_pos();
        const auto lb = super::traits.get_line_border();
        const int per_line = static_cast<int>(super::traits.get_items_per_line(area.size()));

        const int first_line = static_cast<int>((scp - lb + lsp) / lsz);

        int idx = first_line * per_line;
        core::rectangle place = super::traits.get_place_of_index(area, idx, scp);

        const auto start = super::traits.get_1(place.top_left());

        for (; (idx < last) && (super::traits.get_1(place.top_left()) < list_max); ++idx) {
          super::draw_item(idx, graph, place, back_brush, super::get_item_state(idx));
          if (isp > 0) {
            // fill space after each item in fill direction
            super::traits.set_2(place, super::traits.get_2(place.x2y2()), isp);
            graph.fill(draw::rectangle(place), back_brush);
          }
          place = super::traits.get_place_of_index(area, idx + 1, scp);
        }

        const int last_line = core::div_ceil(idx, per_line);

        // draw "empty items" until space is filled up
        place = get_full_place_of_index(idx);
        for (; super::traits.get_1(place.top_left()) < list_max; ++idx) {
          graph.fill(draw::rectangle(place), back_brush);
          place = get_full_place_of_index(idx + 1);
        }

        // fill space between lines

        // item begin position
        const auto ib = super::traits.get_2(area.top_left()) + super::traits.get_item_border();

        // item size
        const auto isz = super::traits.get_item_dimension();

        // line width
        const auto lw = (isz + isp) * per_line;

        if (lsp > 0) {
          // offset of each line
          const auto offs = super::traits.get_1(area.top_left()) - scp + lb + lsz - lsp;
          for (int line = first_line; line < last_line; ++line) {
            super::traits.set_1(place, lsz * line + offs, lsp);
            super::traits.set_2(place, ib, lw);
            graph.fill(draw::rectangle(place), back_brush);
          }
        }

        const auto width = lw + ib;
        const auto max_width = super::traits.get_2(area.x2y2());
        if (max_width > width) {
          core::rectangle space = area;
          super::traits.set_2(space, width, max_width - width);
          graph.fill(draw::rectangle(space), back_brush);
        }

        if (ib > 0) {
          // top/left
          const auto tl = super::traits.get_2(area.top_left());
          core::rectangle space = area;
          super::traits.set_2(space, tl, ib - tl);
          graph.fill(draw::rectangle(space), back_brush);
        }

        if (start > 0) {
          const auto tl = super::traits.get_1(area.top_left());
          core::rectangle space = area;
          super::traits.set_1(space, tl, start - tl);
          graph.fill(draw::rectangle(space), back_brush);
        }
      }

      if (super::is_focused()) {
        draw::frame::dots(graph, area);
      }

    }

    // --------------------------------------------------------------------------
    template<orientation_t V>
    void basic_tile_view<V>::handle_key (os::key_state,
                                         os::key_symbol key,
                                         const std::string&) {
      handle_direction_key(key);
      switch (key) {
      case core::keys::page_up:
      case core::keys::numpad::page_up:
        super::try_to_select(super::get_selection() - static_cast<int>((super::get_list_size() + super::traits.get_line_spacing()) / super::traits.get_line_size()), event_source::keyboard);
        break;
      case core::keys::page_down:
      case core::keys::numpad::page_down:
        super::try_to_select(super::get_selection() + static_cast<int>((super::get_list_size() + super::traits.get_line_spacing()) / super::traits.get_line_size()), event_source::keyboard);
        break;
      case core::keys::home:
      case core::keys::numpad::home:
        super::try_to_select(0, event_source::keyboard);
        break;
      case core::keys::end:
      case core::keys::numpad::end:
        super::try_to_select(static_cast<int>(super::get_count()) - 1,
          event_source::keyboard);
        break;
      case core::keys::enter:
        super::notify_selection_commit();
        break;
      }
    }

    template<orientation_t V>
    void basic_tile_view<V>::init () {
      super::on_paint(draw::paint(this, &basic_tile_view::paint));
      super::on_any_key_down(util::bind_method(this, &basic_tile_view::handle_key));
    }

    template<orientation_t V>
    void basic_tile_view<V>::set_border (const core::size& sz) {
      super::traits.border = sz;
      super::adjust_scroll_bar();
    }
    
    template<orientation_t V>
    void basic_tile_view<V>::set_spacing (const core::size& sz) {
      super::traits.spacing = sz;
      super::adjust_scroll_bar();
    }

    template<orientation_t V>
    const core::size& basic_tile_view<V>::get_border () const {
      return super::traits.border;
    }

    template<orientation_t V>
    const core::size& basic_tile_view<V>::get_spacing () const {
      return super::traits.spacing;
    }

  } // ctrl

} // gui
