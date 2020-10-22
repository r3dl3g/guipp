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

      inline text_origin_t column_list_layout::get_column_align (std::size_t i) const {
        return aligns[i];
      }

      inline column_size_type column_list_layout::get_column_width (std::size_t i) const {
        return widths[i];
      }

      inline void column_list_layout::set_list (list_type* l) {
        list = l;
      }

      // --------------------------------------------------------------------------
      inline void base_column_list_layout::layout (const core::rectangle& sz) const {
        clog::trace() << "base_column_list_layout::layout(" << sz << ")";
        data.header->resize(core::size(sz.width(), 20), false);
        data.list->resize(sz.size() - core::size(0, 20), false);
      }

      inline void base_column_list_layout::set_header_and_list (win::window* header, list_type* list) {
        data.list = list;
        data.header = header;
      }

      inline base_column_list_layout::data::data ()
        : header(nullptr)
        , list(nullptr)
      {}


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

    inline column_size_type simple_column_list_layout::get_column_min_width (std::size_t i) const {
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
      main->on_layout(util::bind_method(this, &weight_column_list_layout::layout));
      main->on_show([&] () {
        layout(list->content_area(list->client_size()));
      });
    }

    inline void weight_column_list_layout::set_column_count (std::size_t i) {
      weights.resize(i, 1.0F / i);
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

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<typename Layout, os::color background>
    column_list_header<Layout, background>::column_list_header ()
      : down_idx(-1)
      , layouter(this)
    {
      super::set_accept_focus(false);
      set_cell_drawer(default_header_cell_drawer);
      layouter.init_auto_layout();
      init();
    }

    template<typename Layout, os::color background>
    column_list_header<Layout, background>::column_list_header (const column_list_header& rhs)
      : super(rhs)
      , cell_drawer(rhs.cell_drawer)
      , last_mouse_point(rhs.last_mouse_point)
      , down_idx(rhs.down_idx)
      , layouter(rhs.layouter)
    {
      init();
    }

    template<typename Layout, os::color background>
    column_list_header<Layout, background>::column_list_header (column_list_header&& rhs)
      : super(std::move(rhs))
      , cell_drawer(std::move(rhs.cell_drawer))
      , last_mouse_point(std::move(rhs.last_mouse_point))
      , down_idx(std::move(rhs.down_idx))
      , layouter(std::move(rhs.layouter))
    {
      init();
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::init () {
      using namespace win;
      super::register_event_handler(event_handler_function([&] (const core::event& e, gui::os::event_result& r) {
        if (!mouse_move_event::if_match_call(e, this, &column_list_header::handle_mouse_move)) {
          if (!left_btn_down_event::if_match_call(e, this, &column_list_header::handle_left_btn_down)) {
            left_btn_up_event::if_match_call(e, this, &column_list_header::handle_left_btn_up);
          }
        }
        return false;
      }), static_cast<os::event_id>(mouse_move_event::mask | left_btn_down_event::mask | left_btn_up_event::mask));
      super::on_paint(draw::buffered_paint(this, &column_list_header::paint));
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
        g.fill(rectangle(r.top_left(), area.x2y2()), back_brush);
      }
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::create (win::container& parent,
                                                         const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::set_cell_drawer (std::function<cell_draw> cd) {
      cell_drawer = cd;
    }

    template<typename L, os::color B>
    void column_list_header<L, B>::set_labels (std::initializer_list<std::string> args) {
      std::vector<std::string> labels(args);
      set_cell_drawer([labels] (std::size_t i,
                                const draw::graphics& g,
                                const core::rectangle& r,
                                const draw::brush& background) {
        using namespace draw;
        g.fill(rectangle(r), background);
        frame::raised_relief(g, r);
        g.text(text_box(labels[i], r, text_origin_t::center), font::system(), color::windowTextColor());
      });
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::handle_left_btn_down (os::key_state, const core::point& pt) {
      last_mouse_point = pt;
      down_idx = layouter.split_idx_at(pt.x(), 2.0F);
      super::set_cursor(down_idx > -1 ? win::cursor::size_h() : win::cursor::arrow());
      super::capture_pointer();
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::handle_left_btn_up (os::key_state keys, const core::point& pt) {
      last_mouse_point = core::point::undefined;
      down_idx = -1;
      super::set_cursor(win::cursor::arrow());
      super::uncapture_pointer();
    }

    template<typename Layout, os::color background>
    void column_list_header<Layout, background>::handle_mouse_move (os::key_state keys, const core::point& pt) {
      if (win::left_button_bit_mask::is_set(keys)) {
        if (last_mouse_point != core::point::undefined) {
          auto delta = pt.x() - last_mouse_point.x();
          if (down_idx > -1) {
            layouter.set_column_width(down_idx, layouter.get_column_width(down_idx) + delta, true);
            invalidate();
          }
        }
        last_mouse_point = pt;
      } else {
        const int idx = layouter.split_idx_at(pt.x(), 2.0F);
        super::set_cursor(idx > -1 ? win::cursor::size_h() : win::cursor::arrow());
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
    inline void column_list_header<Layout, background>::layout (const core::rectangle& sz) {
      clog::trace() << "column_list_header::layout(" << sz << ")";
      layouter.layout(sz);
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
      void base_column_list<Layout>::create (win::container& parent,
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
        super::on_layout([&] (const core::rectangle& sz) {
          header.layout(sz);
        });
      }

      // --------------------------------------------------------------------------
    } // detail

    // --------------------------------------------------------------------------
    template<typename T, draw::frame::drawer F>
    void cell_drawer (const T& t,
                      const draw::graphics& graph,
                      const core::rectangle& place,
                      const draw::brush& background,
                      item_state state,
                      text_origin_t align) {
      paint::text_item(graph, place, background, util::string::convert::from(t), state, align);
      if (item_state::selected != state) {
        F(graph, place);
      }
    }

    // --------------------------------------------------------------------------
    template<typename ... A>
    template<std::size_t I>
    void column_list_row_drawer_t<A ...>::draw_cell (const row_type&,
                                                     const drawer_type&,
                                                     const layout_type&,
                                                     const draw::graphics& g,
                                                     const core::rectangle& place,
                                                     core::point::type x,
                                                     const draw::brush& background,
                                                     item_state) {
      if (x < place.x2()) {
        g.fill(draw::rectangle(core::point(x, place.y()), place.x2y2()), background);
      }
    }

    template<typename ... A>
    template<std::size_t I, typename T, typename ... Args>
    void column_list_row_drawer_t<A ...>::draw_cell (const row_type& row,
                                                     const drawer_type& drawer,
                                                     const layout_type& layout,
                                                     const draw::graphics& g,
                                                     const core::rectangle& r,
                                                     core::point::type x,
                                                     const draw::brush& background,
                                                     item_state state) {
      core::size::type width = layout.get_column_width(I);
      text_origin_t align = layout.get_column_align(I);

      core::rectangle place(core::point(x, r.y()), core::point(x + width, r.y2()));
      auto dat = std::get<I>(row);
      auto drw = std::get<I>(static_cast<const std::tuple<cell_drawer_t<A>...>&>(drawer));
      drw(dat, g, place, background, state, align);
      draw_cell<I + 1, Args ...>(row, drawer, layout, g, r, x + width, background, state);
    }

    template<typename ... A>
    void column_list_row_drawer_t<A ...>::draw_row (const row_type& row,
                                                    const drawer_type& drawer,
                                                    const layout_type& layout,
                                                    const draw::graphics& g,
                                                    const core::rectangle& place,
                                                    const draw::brush& background,
                                                    item_state state) {
      draw_cell<0, A ...>(row, drawer, layout, g, place, place.x(), background, state);
    }

    // --------------------------------------------------------------------------
    template<typename L, typename ... A>
    column_list_t<L, A ...>::column_list_t (core::size::type item_size,
                                            os::color background,
                                            bool grab_focus)
      : super(item_size, background, grab_focus)
    {}

    template<typename L, typename ... A>
    column_list_t<L, A ...>::column_list_t (column_list_t&& rhs)
      : super(std::move(rhs))
    {}

    template<typename L, typename ... A>
    void column_list_t<L, A ...>::set_data (std::function<column_list_data_provider> data) {
      data().set_layout(&super::get_column_layout());
      super::list.set_data(data);
    }

    // --------------------------------------------------------------------------
  } // ctrl

} // gui


