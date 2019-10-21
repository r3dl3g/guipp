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
 * @brief     C++ API: tile view
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    template<typename T>
    inline void default_tile_item_drawer (const T& t,
                                          const draw::graphics& g,
                                          const core::rectangle& place,
                                          const draw::brush& background,
                                          bool selected,
                                          bool) {
      paint::text_item(g, place, background, convert_to_string<T>(t), selected, text_origin::center);
    }

    // --------------------------------------------------------------------------
    template<>
    inline core::size::type tile_list_traits<orientation::horizontal>::get_line_size () const {
      return item_size.width() + spacing.width();
    }

    template<>
    inline core::size::type tile_list_traits<orientation::horizontal>::get_item_dimension () const {
      return item_size.height();
    }

    template<>
    inline core::size::type tile_list_traits<orientation::horizontal>::get_item_border () const {
      return border.height();
    }

    template<>
    inline core::size::type tile_list_traits<orientation::horizontal>::get_line_border () const {
      return border.width();
    }

    template<>
    inline core::size::type tile_list_traits<orientation::horizontal>::get_item_spacing () const {
      return spacing.height();
    }

    template<>
    inline core::size::type tile_list_traits<orientation::horizontal>::get_line_spacing () const {
      return spacing.width();
    }

    // --------------------------------------------------------------------------
    template<>
    inline core::size::type tile_list_traits<orientation::vertical>::get_line_size () const {
      return item_size.height() + spacing.height();
    }

    template<>
    inline core::size::type tile_list_traits<orientation::vertical>::get_item_dimension () const {
      return item_size.width();
    }

    template<>
    inline core::size::type tile_list_traits<orientation::vertical>::get_item_border () const {
      return border.width();
    }

    template<>
    inline core::size::type tile_list_traits<orientation::vertical>::get_line_border () const {
      return border.height();
    }

    template<>
    inline core::size::type tile_list_traits<orientation::vertical>::get_item_spacing () const {
      return spacing.width();
    }

    template<>
    inline core::size::type tile_list_traits<orientation::vertical>::get_line_spacing () const {
      return spacing.height();
    }

    // --------------------------------------------------------------------------
    template<orientation V>
    inline tile_list_traits<V>::tile_list_traits (const size_type& item_size)
      : item_size(item_size)
    {}

    template<orientation V>
    inline auto tile_list_traits<V>::get_invisible_size (const core::size& list_size,
                                                         size_t count) const -> dim_type {
      const auto ipl = get_items_per_line(list_size);
      const auto lines = (ipl > 0 ? basepp::div_ceil(count, ipl) : 1);
      return std::max(dim_type(0), ((get_line_size()) * lines + get_line_border() * 2) - super::get(list_size));
    }

    template<orientation V>
    int tile_list_traits<V>::get_index_at_point (const core::size& list_size,
                                                 const core::point& pt,
                                                 dim_type scroll_pos,
                                                 size_t count) const {
      const auto per_line = static_cast<int>(get_items_per_line(list_size));
      const auto line = static_cast<int>((super::get(pt) + scroll_pos - get_line_border()) / get_line_size());
      const auto offs = static_cast<int>((super::get_other(pt) - get_item_border()) / (get_item_dimension() + get_item_spacing()));
      const auto idx = (line * per_line) + offs;
      return (idx < count) && get_place_of_index(list_size, idx, scroll_pos).is_inside(pt) ? idx : -1;
    }

    template<orientation V>
    core::rectangle tile_list_traits<V>::get_place_of_index (const core::size& list_size,
                                                             int idx,
                                                             dim_type scroll_pos) const {
      const auto per_line = get_items_per_line(list_size);
      const auto line = per_line > 0 ? static_cast<std::size_t>(idx) / per_line : 0;
      const auto offs = idx - (line * per_line);

      const auto lsz = get_line_size();
      const auto isz = get_item_dimension();

      core::rectangle place;
      super::set(place, lsz * line - scroll_pos + get_line_border(), lsz - get_line_spacing());
      super::set_other(place, (isz + get_item_spacing()) * offs + get_item_border(), isz);
      return place;
    }

    template<orientation V>
    inline auto tile_list_traits<V>::get_offset_of_index (const core::size& list_size, int idx) const -> dim_type {
      const int line = idx / static_cast<int>(get_items_per_line(list_size));
      return(get_line_size() * line) + get_line_border();
    }

    template<orientation V>
    inline std::size_t tile_list_traits<V>::get_items_per_line (const core::size& list_size) const {
      const auto isp = get_item_spacing();
      const auto avail = (super::get_other(list_size) - get_line_spacing() - get_item_border() * 2 + isp);
      const auto need = (get_item_dimension() + isp);
      return static_cast<std::size_t>(std::max(avail / need, 1.0f));
    }

    template<orientation V>
    inline std::size_t tile_list_traits<V>::get_line_count (size_t count,
                                                            const core::size& list_size) const {
      return div_ceil(count, get_items_per_line(list_size));
    }

    // --------------------------------------------------------------------------
    template<orientation V>
    inline basic_tile_view<V>::basic_tile_view (const core::size& item_size,
                                                os::color background,
                                                bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      init();
    }

    template<orientation V>
    inline basic_tile_view<V>::basic_tile_view (const basic_tile_view& rhs)
      : super(rhs)
    {
      init();
    }

    template<orientation V>
    inline basic_tile_view<V>::basic_tile_view (basic_tile_view&& rhs)
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation V>
    inline std::size_t basic_tile_view<V>::get_line_count () const {
      return super::traits.get_line_count(super::get_count(), super::content_size());
    }

    template<orientation V>
    core::rectangle basic_tile_view<V>::get_full_place_of_index (int idx) {
      const auto per_line = super::traits.get_items_per_line(super::content_size());
      const auto line = per_line > 0 ? static_cast<std::size_t>(idx) / per_line : 0;
      const auto offs = idx - (line * per_line);

      const auto lsz = super::traits.get_line_size();
      const auto isz = super::traits.get_item_dimension() + super::traits.get_item_spacing();

      core::rectangle place;
      super::traits.set(place, lsz * line - super::get_scroll_pos() + super::traits.get_line_border(), lsz);
      super::traits.set_other(place, isz * offs + super::traits.get_item_border(), isz);
      return place;
    }

    template<orientation V>
    void basic_tile_view<V>::paint (const draw::graphics& graph) {
      const auto list_size = super::content_size();
      const core::rectangle area(list_size);

      draw::brush back_brush(super::get_background());

      const auto last = super::get_count();
      const auto lsz = super::traits.get_line_size();
      if ((last < 1) || (lsz < 1)) {
        graph.fill(draw::rectangle(area), back_brush);
      } else {
        const auto list_sz = super::traits.get(list_size);
        const auto isp = super::traits.get_item_spacing();
        const auto lsp = super::traits.get_line_spacing();
        const auto scp = super::get_scroll_pos();
        const auto lb = super::traits.get_line_border();
        const int per_line = static_cast<int>(super::traits.get_items_per_line(list_size));

        const int first_line = static_cast<int>((scp - lb + lsp) / lsz);

        int idx = first_line * per_line;
        core::rectangle place = super::traits.get_place_of_index(list_size, idx, scp);

        const auto start = super::traits.get(place.top_left());

        for (; (idx < last) && (super::traits.get(place.top_left()) < list_sz); ++idx) {
          super::draw_item(idx, graph, place, back_brush, super::get_selection() == idx, super::get_hilite() == idx);
          if (isp > 0) {
            super::traits.set_other(place, super::traits.get_other(place.bottom_right()), isp);
            graph.fill(draw::rectangle(place), back_brush);
          }
          place = super::traits.get_place_of_index(list_size, idx + 1, scp);
        }

        const int last_line = basepp::div_ceil(idx, per_line);

        place = get_full_place_of_index(idx);
        for (; (super::traits.get(place.top_left()) < list_sz); ++idx) {
          graph.fill(draw::rectangle(place), back_brush);
          place = get_full_place_of_index(idx + 1);
        }

        const auto ib = super::traits.get_item_border();
        const auto isz = super::traits.get_item_dimension();
        if (lsp > 0) {
          const auto lw = (isz + isp) * per_line;
          for (int line = first_line; line < last_line; ++line) {
            super::traits.set(place, lsz * line - scp + lb + lsz - lsp, lsp);
            super::traits.set_other(place, ib, lw);
            graph.fill(draw::rectangle(place), back_brush);
          }
        }

        const auto width = (isz + isp) * per_line + ib;
        const auto max_width = super::traits.get_other(area.bottom_right());
        if (max_width > width) {
          core::rectangle space = area;
          super::traits.set_other(space, width, max_width - width);
          graph.fill(draw::rectangle(space), back_brush);
        }

        if (ib > 0) {
          core::rectangle space = area;
          super::traits.set_other(space, super::traits.get(area.top_left()), ib);
          graph.fill(draw::rectangle(space), back_brush);
        }

        if (start > 0) {
          core::rectangle space = area;
          super::traits.set(space, super::traits.get_other(area.top_left()), start);
          graph.fill(draw::rectangle(space), back_brush);
        }
      }

      if (super::has_focus()) {
        draw::frame::dots(graph, area);
      }

    }

    // --------------------------------------------------------------------------
    template<orientation V>
    void basic_tile_view<V>::handle_key (os::key_state,
                                         os::key_symbol key,
                                         const std::string&) {
      handle_direction_key(key);
      switch (key) {
      case win::keys::page_up:
      case win::keys::numpad::page_up:
        super::try_to_select(super::get_selection() - static_cast<int>((super::get_list_size() + super::traits.get_line_spacing()) / super::traits.get_line_size()), event_source::keyboard);
        break;
      case win::keys::page_down:
      case win::keys::numpad::page_down:
        super::try_to_select(super::get_selection() + static_cast<int>((super::get_list_size() + super::traits.get_line_spacing()) / super::traits.get_line_size()), event_source::keyboard);
        break;
      case win::keys::home:
      case win::keys::numpad::home:
        super::try_to_select(0, event_source::keyboard);
        break;
      case win::keys::end:
      case win::keys::numpad::end:
        super::try_to_select(static_cast<int>(super::get_count()) - 1,
          event_source::keyboard);
        break;
      case win::keys::enter:
        send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
        break;
      }
    }

    template<orientation V>
    void basic_tile_view<V>::init () {
      super::on_paint(draw::buffered_paint(this, &basic_tile_view::paint));
      super::on_any_key_down(basepp::bind_method(this, &basic_tile_view::handle_key));
    }

    template<orientation V>
    void basic_tile_view<V>::set_border (const core::size& sz) {
      super::traits.border = sz;
      super::adjust_scroll_bar();
    }
    
    template<orientation V>
    void basic_tile_view<V>::set_spacing (const core::size& sz) {
      super::traits.spacing = sz;
      super::adjust_scroll_bar();
    }

    template<orientation V>
    const core::size& basic_tile_view<V>::get_border () const {
      return super::traits.border;
    }

    template<orientation V>
    const core::size& basic_tile_view<V>::get_spacing () const {
      return super::traits.spacing;
    }

  } // ctrl

} // gui
