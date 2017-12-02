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

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//


namespace gui {

  namespace layout {

    namespace detail {

      // --------------------------------------------------------------------------
      inline column_list_layout::column_list_layout (win::window* main)
        : main(main)
        , list(nullptr)
      {}

      inline column_list_layout::column_list_layout (win::window* main, const column_list_layout& rhs)
        : main(main)
        , list(nullptr)
        , widths(rhs.widths)
        , aligns(rhs.aligns)
      {}

      inline column_list_layout::column_list_layout (win::window* main, column_list_layout&& rhs)
        : main(main)
        , list(std::move(rhs.list))
        , widths(std::move(rhs.widths))
        , aligns(std::move(rhs.aligns))
      {}

      inline void column_list_layout::init_auto_layout () {
      }

      inline std::size_t column_list_layout::get_column_count () const {
        return widths.size();
      }

      inline text_origin column_list_layout::get_column_align (std::size_t i) const {
        return aligns[i];
      }

      inline column_size_type column_list_layout::get_column_width (std::size_t i) const {
        return widths[i];
      }

      inline void column_list_layout::set_list (list_type* l) {
        list = l;
      }

      // --------------------------------------------------------------------------
      inline base_column_list_layout::base_column_list_layout (win::container* m)
        : super(m)
      {
        init();
      }

      inline base_column_list_layout::base_column_list_layout (win::container* m, const base_column_list_layout& rhs)
        : super(m, rhs)
        , data(rhs.data)
      {
        init();
      }

      inline base_column_list_layout::base_column_list_layout (win::container* m, base_column_list_layout&& rhs)
        : super(m, std::move(rhs))
        , data(std::move(rhs.data))
      {
        init();
      }

      inline void base_column_list_layout::layout (const core::size& sz) {
        LogTrace << "base_column_list_layout::layout(" << sz << ")";
        data.header->resize(core::size(sz.width(), 20), false);
        data.list->resize(sz - core::size(0, 20), false);
      }

      inline void base_column_list_layout::set_header_and_list (win::window* header, list_type* list) {
        data.list = list;
        data.header = header;
      }

      inline void base_column_list_layout::init () {
        super::init(core::bind_method(this, &base_column_list_layout::layout));
      }

      // --------------------------------------------------------------------------
    } // detail

    // --------------------------------------------------------------------------
    inline simple_column_list_layout::simple_column_list_layout (win::window* main)
      : super(main)
    {}

    inline simple_column_list_layout::simple_column_list_layout (win::window* main, const simple_column_list_layout& rhs)
      : super(main, rhs)
      , min_widths(rhs.min_widths)
    {}

    inline simple_column_list_layout::simple_column_list_layout (win::window* main, simple_column_list_layout&& rhs)
      : super(main, std::move(rhs))
      , min_widths(std::move(rhs.min_widths))
    {}

    inline void simple_column_list_layout::set_column_width (std::size_t i, column_size_type w, bool update) {
      super::set_column_width(i, std::max(w, get_column_min_width(i)), update);
    }

    inline void simple_column_list_layout::set_column_count (std::size_t i) {
      min_widths.resize(i);
      super::set_column_count(i);
    }

    inline void simple_column_list_layout::set_column_min_width (std::size_t i, const column_size_type w) {
      min_widths[i] = w;
      if (get_column_width(i) < get_column_min_width(i)) {
        set_column_width(i, w);
      }
    }

    inline const column_size_type simple_column_list_layout::get_column_min_width (std::size_t i) const {
      return min_widths[i];
    }

    // --------------------------------------------------------------------------
    inline weight_column_list_layout::weight_column_list_layout (win::window* main)
      : super(main)
    {}

    inline weight_column_list_layout::weight_column_list_layout (win::window* main, const weight_column_list_layout& rhs)
      : super(main, rhs)
      , weights(rhs.weights)
    {}

    inline weight_column_list_layout::weight_column_list_layout (win::window* main, weight_column_list_layout&& rhs)
      : super(main, std::move(rhs))
      , weights(std::move(rhs.weights))
    {}

    inline void weight_column_list_layout::init_auto_layout () {
      main->register_event_handler(REGISTER_FUNCTION, win::layout_event(this, &weight_column_list_layout::layout));
      main->register_event_handler(REGISTER_FUNCTION, win::show_event([&] () {
        layout(list->client_size());
      }));
    }

    inline void weight_column_list_layout::set_column_count (std::size_t i) {
      weights.resize(i);
      super::set_column_count(i);
    }

    inline void weight_column_list_layout::set_column_weight (std::size_t i, float w) {
      weights[i] = w;
    }

    inline float weight_column_list_layout::get_column_weight (std::size_t i) const {
      return weights[i];
    }

    // --------------------------------------------------------------------------
  } // layout

  namespace win {

    // --------------------------------------------------------------------------
    template<typename Layout, os::color background>
    column_list_header<Layout, background>::column_list_header ()
      : down_idx(-1)
      , layouter(this)
    {
      super::set_accept_focus(false);
      set_cell_drawer(default_header_cell_drawer);
      layouter.init_auto_layout();
      this->register_event_handler(REGISTER_FUNCTION, paint_event(draw::buffered_paint(this, &column_list_header::paint)));
      this->register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &column_list_header::handle_mouse_move));
      this->register_event_handler(REGISTER_FUNCTION, left_btn_down_event(this, &column_list_header::handle_left_btn_down));
      this->register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &column_list_header::handle_left_btn_up));
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::paint (const draw::graphics& g) {
      using namespace draw;

      core::rectangle area = this->client_area();
      core::rectangle r = area;
      draw::brush back_brush(background);

      auto count = layouter.get_column_count();
      for (decltype(count) i = 0;i < count;++i) {
        layout::column_size_type w = layouter.get_column_width(i);
        r.width(w);
        if (cell_drawer) {
          cell_drawer(i, g, r, back_brush);
        }
        r.move_x(w);
      }

      if (r.x() < area.x2()) {
        g.fill(rectangle(r.top_left(), area.bottom_right()), back_brush);
      }
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::create (const container& parent,
                                                         const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::set_cell_drawer (std::function<cell_draw> cd) {
      cell_drawer = cd;
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::handle_left_btn_down (os::key_state, const core::point& pt) {
      last_mouse_point = pt;
      down_idx = layouter.split_idx_at(pt.x(), 2.0F);
      super::set_cursor(down_idx > -1 ? cursor::size_h() : cursor::arrow());
      super::capture_pointer();
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::handle_left_btn_up (os::key_state keys, const core::point& pt) {
      last_mouse_point = core::point::undefined;
      down_idx = -1;
      super::set_cursor(cursor::arrow());
      super::uncapture_pointer();
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::handle_mouse_move (os::key_state keys, const core::point& pt) {
      if (left_button_bit_mask::is_set(keys)) {
        if (last_mouse_point != core::point::undefined) {
          auto delta = pt.x() - last_mouse_point.x();
          if (down_idx > -1) {
            layouter.set_column_width(down_idx, layouter.get_column_width(down_idx) + delta);
          }
        }
        last_mouse_point = pt;
      } else {
        const int idx = layouter.split_idx_at(pt.x(), 2.0F);
        super::set_cursor(idx > -1 ? cursor::size_h() : cursor::arrow());
      }
    }

    template<typename Layout, os::color background>
    inline auto column_list_header<Layout, background>::get_column_layout () -> layout_type& {
      return layouter;
    }

    template<typename Layout, os::color background>
    inline auto column_list_header<Layout, background>::get_column_layout () const -> const layout_type& {
      return layouter;
    }

    template<typename Layout, os::color background>
    inline void column_list_header<Layout, background>::layout () {
      LogTrace << "column_list_header::layout()";
      layouter.layout(client_size());
    }

    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      template<typename Layout>
      base_column_list<Layout>::base_column_list (core::size::type item_size,
                                                  os::color background,
                                                  bool grab_focus)
        : list(item_size, background, grab_focus)
      {
        init();
      }

      template<typename Layout>
      base_column_list<Layout>::base_column_list (const base_column_list& rhs)
        : super(rhs)
        , header(rhs.header)
        , list(rhs.list)
      {
        init();
      }

      template<typename Layout>
      base_column_list<Layout>::base_column_list (base_column_list&& rhs)
        : super(std::move(rhs))
        , header(std::move(rhs.header))
        , list(std::move(rhs.list))
      {
        init();
      }

      template<typename Layout>
      auto base_column_list<Layout>::get_column_layout() -> layout_type & {
        return header.get_column_layout();
      }

      template<typename Layout>
      auto base_column_list<Layout>::get_column_layout() const->const layout_type &{
        return header.get_column_layout();
      }

      template<typename Layout>
      void base_column_list<Layout>::create (const container& parent,
                                             const core::rectangle& place) {
        super::create(clazz::get(), parent, place);
        header.create(*this, core::rectangle(0, 0, place.width(), 20));
        header.set_visible();
        list.create(*this, core::rectangle(0, 20, place.width(), place.height() - 20));
        list.set_visible();
      }

      template<typename Layout>
      void base_column_list<Layout>::init () {
        super::get_layout().set_header_and_list(&header, &list);
        get_column_layout().set_list(&list);
        super::register_event_handler(REGISTER_FUNCTION, win::layout_event([&] (const core::size&) {
          header.layout();
        }));
      }

      // --------------------------------------------------------------------------
    } // detail

    // --------------------------------------------------------------------------
    template<typename T, draw::frame::drawer F>
    simple_column_list_data<T, F>::simple_column_list_data ()
    {}

    template<typename T, draw::frame::drawer F>
    simple_column_list_data<T, F>::simple_column_list_data (std::initializer_list<row> args)
      : super(args)
    {}

    template<typename T, draw::frame::drawer F>
    simple_column_list_data<T, F>::simple_column_list_data (iterator b, iterator e)
      : super(b, e)
    {}

    template<typename T, draw::frame::drawer F>
    template<size_t N>
    simple_column_list_data<T, F>::simple_column_list_data (const row(&t)[N])
      : super(t, t + N)
    {}

    template<typename T, draw::frame::drawer F>
    template<typename L>
    void simple_column_list_data<T, F>::update_list (L& l) {
      l.set_count(super::size());
    }

    template<typename T, draw::frame::drawer F>
    void simple_column_list_data<T, F>::operator() (std::size_t row_id,
                                                    std::size_t col_id,
                                                    const draw::graphics& graph,
                                                    const core::rectangle& place,
                                                    const draw::brush& background,
                                                    bool selected,
                                                    bool,
                                                    text_origin align) {
      paint::text_item(graph, place, background, convert_to_string(super::at(row_id).at(col_id)), selected, align);
      if (!selected) {
        F(graph, place);
      }
    }

    template<typename L>
    simple_column_list<L>::simple_column_list (core::size::type item_size,
                                               os::color background,
                                               bool grab_focus)
      : super(item_size, background, grab_focus)
    {}

    template<typename L>
    void simple_column_list<L>::set_drawer (std::function<cell_draw> drawer) {
      this->drawer = drawer;
      this->list.set_drawer(core::bind_method(this, &simple_column_list::draw_cells));
    }

    template<typename L>
    void simple_column_list<L>::create (const container& parent,
                                        const core::rectangle& place) {
      super::create(parent, place);
    }

    template<typename L>
    template<typename T, draw::frame::drawer F>
    void simple_column_list<L>::set_data (simple_column_list_data<T, F> data) {
      set_drawer(data);
      this->list.set_count(data.size());
    }

    template<typename L>
    void simple_column_list<L>::draw_cells (std::size_t idx,
                                            const draw::graphics& g,
                                            const core::rectangle& place,
                                            const draw::brush& background,
                                            bool selected,
                                            bool hilited) {
      if (drawer) {
        core::rectangle r = place;
        auto count = this->get_column_layout().get_column_count();
        for (decltype(count) i = 0; i < count; ++i) {
          core::size::type w = this->get_column_layout().get_column_width(i);
          r.width(w);
          drawer(idx, i, g, r, background, selected, hilited, this->get_column_layout().get_column_align(i));
          r.move_x(w);
        }
        if (r.x() < place.x2()) {
          g.fill(draw::rectangle(r.top_left(), place.bottom_right()), background);
        }
      }
    }

    // --------------------------------------------------------------------------
    template<typename ... A>
    static_column_list_data_t<A ...>::static_column_list_data_t ()
    {}

    template<typename ... A>
    static_column_list_data_t<A ...>::static_column_list_data_t (std::initializer_list<row> args)
      : super(args)
    {}

    template<typename ... A>
    static_column_list_data_t<A ...>::static_column_list_data_t (iterator b, iterator e)
      : super(b, e)
    {}

    template<typename ... A>
    template<size_t N>
    static_column_list_data_t<A ...>::static_column_list_data_t (const row(&t)[N])
      : super(t, t + N)
    {}

    template<typename ... A>
    template<typename L>
    void static_column_list_data_t<A ...>::update_list (L& l) {
      l.set_count(super::size());
    }

    template<typename ... A>
    auto static_column_list_data_t<A ...>::operator() (std::size_t row_id) const->row {
      return super::at(row_id);
    }

    // --------------------------------------------------------------------------
    template<typename T, draw::frame::drawer F>
    void cell_drawer (const T& t,
                      const draw::graphics& graph,
                      const core::rectangle& place,
                      const draw::brush& background,
                      bool selected,
                      bool,
                      text_origin align) {
      paint::text_item(graph, place, background, convert_to_string(t), selected, align);
      if (!selected) {
        F(graph, place);
      }
    }

    // --------------------------------------------------------------------------
    template<typename L, typename ... A>
    column_list_row_drawer_t<L, A ...>::column_list_row_drawer_t ()
    {}

    template<typename L, typename ... A>
    column_list_row_drawer_t<L, A ...>::column_list_row_drawer_t (cell_drawer_t<A>... args)
      : super(std::make_tuple(args ...))
    {}

    template<typename L, typename ... A>
    void column_list_row_drawer_t<L, A ...>::operator= (const column_list_row_drawer_t& rhs) {
      super::operator= (rhs);
    }

    template<typename L, typename ... A>
    template<std::size_t I>
    void column_list_row_drawer_t<L, A ...>::draw_cell (const row& data,
                                                        const layout_type& l,
                                                        const draw::graphics& g,
                                                        const core::rectangle& place,
                                                        core::point::type x,
                                                        const draw::brush& background,
                                                        bool selected,
                                                        bool hilited) {
      if (x < place.x2()) {
        g.fill(draw::rectangle(core::point(x, place.y()), place.bottom_right()), background);
      }
    }

    template<typename L, typename ... A>
    template<std::size_t I, typename T, typename ... Args>
    void column_list_row_drawer_t<L, A ...>::draw_cell (const row& data,
                                                        const layout_type& l,
                                                        const draw::graphics& g,
                                                        const core::rectangle& r,
                                                        core::point::type x,
                                                        const draw::brush& background,
                                                        bool selected,
                                                        bool hilited) {
      core::size::type width = l.get_column_width(I);
      text_origin align = l.get_column_align(I);

      core::rectangle place(core::point(x, r.y()), core::point(x + width, r.y2()));
      std::get<I>(*this)(std::get<I>(data), g, place, background, selected, hilited, align);
      draw_cell<I + 1, Args ...>(data, l, g, r, x + width, background, selected, hilited);
    }

    template<typename L, typename ... A>
    void column_list_row_drawer_t<L, A ...>::operator() (const row& data,
                                                         const layout_type& l,
                                                         const draw::graphics& g,
                                                         const core::rectangle& place,
                                                         const draw::brush& background,
                                                         bool selected,
                                                         bool hilited) {
      draw_cell<0, A ...>(data, l, g, place, place.x(), background, selected, hilited);
    }

    // --------------------------------------------------------------------------
    template<typename L, typename ... A>
    column_list_t<L, A ...>::column_list_t (core::size::type item_size,
                                            os::color background,
                                            bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      this->get_column_layout().set_column_count(size);
    }

    template<typename L, typename ... A>
    void column_list_t<L, A ...>::set_drawer (data_drawer drawer) {
      this->drawer = drawer;
      this->list.set_drawer(core::bind_method(this, &column_list_t::draw_cells_t));
    }

    template<typename L, typename ... A>
    void column_list_t<L, A ...>::set_data (data_provider data, std::size_t count) {
      this->data = data;
      this->list.set_count(count);
    }

    template<typename L, typename ... A>
    void column_list_t<L, A ...>::draw_cells_t (std::size_t row_id,
                                                const draw::graphics& g,
                                                const core::rectangle& place,
                                                const draw::brush& back,
                                                bool selected,
                                                bool hilited) {
      drawer(data(row_id), this->get_column_layout(), g, place, back, selected, hilited);
    }

    // --------------------------------------------------------------------------
  } // win

} // gui


