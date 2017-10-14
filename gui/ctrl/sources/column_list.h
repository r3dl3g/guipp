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
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "list.h"
#include "container.h"


namespace gui {

  namespace layout {

    typedef core::size_type column_size_type;

    struct column_info {
      column_size_type width;
      text_origin align;
    };

    namespace detail {

      class column_list_layout : protected layout_base {
      public:
        typedef layout_base super;
        typedef win::basic_list<orientation::vertical> list_type;

        column_list_layout (win::container* main);
        column_list_layout (win::container* main, const column_list_layout& rhs);
        column_list_layout (win::container* main, column_list_layout&& rhs);

        void init_auto_layout ();

        void layout (const core::size& new_size);

        std::size_t get_column_count () const;
        void set_column_count (std::size_t i);

        text_origin get_column_align (std::size_t i) const;
        column_size_type get_column_width (std::size_t i) const;

        int split_idx_at (core::point_type pt, column_size_type delta) const;
        int index_at (core::point_type pt) const;

        column_size_type get_column_left_pos (std::size_t i) const;
        column_size_type get_column_right_pos (std::size_t i) const;

        void set_column_align (std::size_t i, text_origin a);
        void set_column_width (std::size_t i, column_size_type w, bool update = true);
        void set_column_info (std::size_t i, const column_info& info, bool update = true);
        void set_columns (std::initializer_list<column_info> infos, bool update = true);

        core::size::type get_available_width () const;
        core::size::type get_available_width (const core::size&) const;

        void set_list (list_type* l);

      protected:
        void redraw_views ();

        std::vector<column_size_type> widths;
        std::vector<text_origin> aligns;

        list_type* list;
      };

    }

    // --------------------------------------------------------------------------
    struct simple_column_info {
      column_size_type width;
      text_origin align;
      column_size_type min_width;
    };

    // --------------------------------------------------------------------------
    class simple_column_list_layout : public detail::column_list_layout {
    public:
      typedef detail::column_list_layout super;

      simple_column_list_layout (win::container* main);
      simple_column_list_layout (win::container* main, const simple_column_list_layout& rhs);
      simple_column_list_layout (win::container* main, simple_column_list_layout&& rhs);

      void set_column_width (std::size_t i, column_size_type w, bool update = true);
      void set_column_count (std::size_t i);

      void set_columns (std::initializer_list<simple_column_info> infos, bool update = true);
      void set_column_min_width (std::size_t i, const column_size_type w);
      const column_size_type get_column_min_width (std::size_t i) const;

    protected:
      std::vector<column_size_type> min_widths;

    };

    // --------------------------------------------------------------------------
    struct weight_column_info {
      column_size_type width;
      text_origin align;
      column_size_type min_width;
      float weight;
    };

    // --------------------------------------------------------------------------
    class weight_column_list_layout : public simple_column_list_layout {
    public:
      typedef simple_column_list_layout super;

      weight_column_list_layout (win::container* main);
      weight_column_list_layout (win::container* main, const weight_column_list_layout& rhs);
      weight_column_list_layout (win::container* main, weight_column_list_layout&& rhs);

      void layout (const core::size& new_size);

      void init_auto_layout ();

      void set_column_width (std::size_t i, column_size_type w, bool update = true);
      void set_column_count (std::size_t i);
      void set_columns (std::initializer_list<weight_column_info> infos, bool update = true);

      void set_column_weight (std::size_t i, float w);
      float get_column_weight (std::size_t i) const;

    protected:
      std::vector<float> weights;

    };

    namespace detail {

      // --------------------------------------------------------------------------
      class base_column_list_layout : protected layout_base {
      public:
        typedef layout_base super;
        typedef win::basic_list<orientation::vertical> list_type;

        base_column_list_layout (win::container* m);
        base_column_list_layout (win::container* m, const base_column_list_layout& rhs);
        base_column_list_layout (win::container* m, base_column_list_layout&& rhs);

        void layout (const core::size& sz);
        void set_header_and_list (win::container* header, list_type* list);

      protected:
        struct data {
          data ()
            : header(nullptr)
            , list(nullptr)
          {}

          win::container* header;
          list_type* list;
        } data;

      private:
        void init ();
      };

    } // detail

  } // layout

  namespace win {

    void default_header_cell_drawer (std::size_t i,
                                     const draw::graphics& g,
                                     const core::rectangle& r,
                                     const draw::brush& background);

    // --------------------------------------------------------------------------
    template<typename Layout, os::color background = color::very_very_light_gray>
    class column_list_header : public layout_container<Layout> {
    public:
      typedef Layout layout_type;
      typedef layout_container<Layout> super;
      typedef window_class<column_list_header, background> clazz;

      typedef void (cell_draw)(std::size_t,            // idx
                               const draw::graphics&,  // gc
                               const core::rectangle&, // place
                               const draw::brush&);    // background

      column_list_header ();

      void paint (const draw::graphics& g);

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void set_cell_drawer (std::function<cell_draw> cd);

      void handle_left_btn_down (os::key_state, const core::point& pt);
      void handle_left_btn_up (os::key_state keys, const core::point& pt);
      void handle_mouse_move (os::key_state keys, const core::point& pt);

    private:
      std::function<cell_draw> cell_drawer;
      core::point last_mouse_point;
      int down_idx;

      void operator= (column_list_header&) = delete;
    };

    namespace detail {

      // --------------------------------------------------------------------------
      template<typename Layout>
      class base_column_list : public layout_container<layout::detail::base_column_list_layout> {
      public:
        typedef Layout layout_type;
        typedef layout_container<layout::detail::base_column_list_layout> super;
        typedef column_list_header<layout_type> header_type;
        typedef win::basic_list<orientation::vertical> list_type;
        typedef no_erase_window_class<base_column_list> clazz;

        base_column_list (core::size_type item_size = 20,
                          os::color background = color::white,
                          bool grab_focus = true);
        base_column_list (const base_column_list& rhs);
        base_column_list (base_column_list&& rhs);

        layout_type& get_column_layout ();
        const layout_type& get_column_layout () const;

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def);

        header_type header;
        list_type list;

      private:
        void init ();

      };

    }

    // --------------------------------------------------------------------------
    // static data for simple_column_list.
    // --------------------------------------------------------------------------
    template<typename T,
             draw::frame::drawer F = draw::frame::no_frame>
    struct simple_column_list_data : public std::vector<std::vector<T> > {
      typedef std::vector<T> row;
      typedef std::vector<row> super;

      typedef typename super::iterator iterator;

      simple_column_list_data ();
      simple_column_list_data (std::initializer_list<row> args);
      simple_column_list_data (iterator b, iterator e);

      template<size_t N>
      simple_column_list_data (const row(&t)[N]);

      template<typename L>
      void update_list (L& l);

      void operator() (std::size_t row_id,
                       std::size_t col_id,
                       const draw::graphics& graph,
                       const core::rectangle& place,
                       const draw::brush& background,
                       bool selected,
                       bool,
                       text_origin align);

    };

    // --------------------------------------------------------------------------
    template<typename Layout>
    class simple_column_list : public detail::base_column_list<Layout> {
    public:
      typedef Layout layout_type;
      typedef detail::base_column_list<layout_type> super;

      typedef void (cell_draw)(std::size_t,            // row_id
                               std::size_t,            // col_id
                               const draw::graphics&,  // g
                               const core::rectangle&, // place
                               const draw::brush&,     // background
                               bool,                   // selected
                               bool,                   // hilited
                               text_origin);           // align

      simple_column_list (core::size_type item_size = 20,
                          os::color background = color::white,
                          bool grab_focus = true);

      void set_drawer (std::function<cell_draw> drawer);

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      template<typename T, draw::frame::drawer F = draw::frame::no_frame>
      void set_data (simple_column_list_data<T, F> data);

    protected:
      void draw_cells (std::size_t idx,
                       const draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       bool selected,
                       bool hilited);

      std::function<cell_draw> drawer;
    };

    // --------------------------------------------------------------------------
    template<typename ... Arguments>
    using column_list_row_t = std::tuple<Arguments ...>;

    // --------------------------------------------------------------------------
    template<typename ... Arguments>
    struct static_column_list_data_t : public std::vector<column_list_row_t<Arguments ...> > {
      typedef column_list_row_t<Arguments ...> row;
      typedef std::vector<row> super;

      typedef typename super::iterator iterator;

      static_column_list_data_t ();
      static_column_list_data_t (std::initializer_list<row> args);
      static_column_list_data_t (iterator b, iterator e);

      template<size_t N>
      static_column_list_data_t (const row(&t)[N]);

      template<typename L>
      void update_list (L& l);

      row operator() (std::size_t row_id) const;

    };

    // --------------------------------------------------------------------------
    template<typename T,
             draw::frame::drawer F = draw::frame::no_frame>
    void cell_drawer (const T& t,
                      const draw::graphics& graph,
                      const core::rectangle& place,
                      const draw::brush& background,
                      bool selected,
                      bool,
                      text_origin align);

    // --------------------------------------------------------------------------
    template<typename T>
    using cell_drawer_t = void (*)(const T& t,
                                   const draw::graphics& g,
                                   const core::rectangle& place,
                                   const draw::brush& background,
                                   bool selected,
                                   bool hilited,
                                   text_origin align);

    // --------------------------------------------------------------------------
    template<typename ... Arguments>
#if _MSC_VER < 1910

    struct row_cell_drawer_t : public std::tuple<cell_drawer_t<Arguments>...> {
      typedef std::tuple<cell_drawer_t<Arguments>...> super;

      row_cell_drawer_t ()
      {}

      row_cell_drawer_t (super rhs)
        : super(rhs)
      {}

    };
#else
    using row_cell_drawer_t = std::tuple<cell_drawer_t<Arguments>...>;
#endif // _MSC_VER < 1900

    // --------------------------------------------------------------------------
    template<typename Layout, typename ... Arguments>
    struct column_list_row_drawer_t : public row_cell_drawer_t<Arguments ...> {
      typedef Layout layout_type;
      typedef row_cell_drawer_t<Arguments ...> super;
      typedef column_list_row_t<Arguments ...> row;

      column_list_row_drawer_t ();

      column_list_row_drawer_t (cell_drawer_t<Arguments>... args);

      void operator= (const column_list_row_drawer_t& rhs);

      template<std::size_t I>
      void draw_cell (const row& data,
                      const layout_type& l,
                      const draw::graphics& g,
                      const core::rectangle& place,
                      core::point_type x,
                      const draw::brush& background,
                      bool selected,
                      bool hilited);

      template<std::size_t I, typename T, typename ... Args>
      void draw_cell (const row& data,
                      const layout_type& l,
                      const draw::graphics& g,
                      const core::rectangle& r,
                      core::point_type x,
                      const draw::brush& background,
                      bool selected,
                      bool hilited);

      void operator() (const row& data,
                       const layout_type& l,
                       const draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       bool selected,
                       bool hilited);
    };

    // --------------------------------------------------------------------------
    template<typename Layout, typename ... Arguments>
    class column_list_t : public detail::base_column_list<Layout> {
    public:
      static const std::size_t size = sizeof ... (Arguments);

      typedef Layout layout_type;
      typedef detail::base_column_list<layout_type> super;

      typedef column_list_row_t<Arguments ...> row;

      typedef static_column_list_data_t<Arguments ...> standard_data;

      typedef column_list_row_drawer_t<layout_type, Arguments ...> row_drawer;

      typedef row (get_row_data_t)(std::size_t idy);

      typedef void (draw_row_data_t)(const row&,             // r
                                     const layout_type&,     // l
                                     const draw::graphics&,  // g
                                     const core::rectangle&, // place
                                     const draw::brush&,     // background
                                     bool,                   // selected
                                     bool);                  // hilited

      typedef std::function<get_row_data_t> data_provider;
      typedef std::function<draw_row_data_t> data_drawer;

      column_list_t (core::size_type item_size = 20,
                     os::color background = color::white,
                     bool grab_focus = true);

      void set_drawer (data_drawer drawer);

      void set_data (data_provider data, std::size_t count);

    protected:
      void draw_cells_t (std::size_t row_id,
                         const draw::graphics& g,
                         const core::rectangle& place,
                         const draw::brush& back,
                         bool selected,
                         bool hilited);

      data_drawer drawer;
      data_provider data;
    };
  } // win

  // --------------------------------------------------------------------------
  // inlines
  namespace layout {

    namespace detail {

      // --------------------------------------------------------------------------
      inline column_list_layout::column_list_layout (win::container* main)
        : super(main)
        , list(nullptr)
      {}

      inline column_list_layout::column_list_layout (win::container* main, const column_list_layout& rhs)
        : super(main, rhs)
        , widths(rhs.widths)
        , aligns(rhs.aligns)
        , list(nullptr)
      {}

      inline column_list_layout::column_list_layout (win::container* main, column_list_layout&& rhs)
        : super(main, std::move(rhs))
        , widths(std::move(rhs.widths))
        , aligns(std::move(rhs.aligns))
        , list(std::move(rhs.list))
      {}

      inline void column_list_layout::init_auto_layout () {
        super::init(core::bind_method(this, &column_list_layout::layout));
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
        data.header->resize(core::size(sz.width(), 20));
        data.list->resize(sz - core::size(0, 20));
      }

      inline void base_column_list_layout::set_header_and_list (win::container* header, list_type* list) {
        data.list = list;
        data.header = header;
      }

      inline void base_column_list_layout::init () {
        super::init(core::bind_method(this, &base_column_list_layout::layout));
      }

      // --------------------------------------------------------------------------
    } // detail

    // --------------------------------------------------------------------------
    inline simple_column_list_layout::simple_column_list_layout (win::container* main)
      : super(main)
    {}

    inline simple_column_list_layout::simple_column_list_layout (win::container* main, const simple_column_list_layout& rhs)
      : super(main, rhs)
      , min_widths(rhs.min_widths)
    {}

    inline simple_column_list_layout::simple_column_list_layout (win::container* main, simple_column_list_layout&& rhs)
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
    inline weight_column_list_layout::weight_column_list_layout (win::container* main)
      : super(main)
    {}

    inline weight_column_list_layout::weight_column_list_layout (win::container* main, const weight_column_list_layout& rhs)
      : super(main, rhs)
      , weights(rhs.weights)
    {}

    inline weight_column_list_layout::weight_column_list_layout (win::container* main, weight_column_list_layout&& rhs)
      : super(main, std::move(rhs))
      , weights(std::move(rhs.weights))
    {}

    inline void weight_column_list_layout::init_auto_layout () {
      super::init(core::bind_method(this, &weight_column_list_layout::layout));
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
    {
      super::set_accept_focus(false);
      set_cell_drawer(default_header_cell_drawer);
      this->get_layout().init_auto_layout();
      this->register_event_handler(REGISTER_FUNCTION, paint_event(this, &column_list_header::paint));
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

      auto count = this->get_layout().get_column_count();
      for (decltype(count) i = 0;i < count;++i) {
        layout::column_size_type w = this->get_layout().get_column_width(i);
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
      down_idx = this->get_layout().split_idx_at(pt.x(), 2.0F);
      super::set_cursor(down_idx > -1 ? cursor::size_h() : cursor::move());
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
            this->get_layout().set_column_width(down_idx, this->get_layout().get_column_width(down_idx) + delta);
          }
        }
        last_mouse_point = pt;
      } else {
        const int idx = this->get_layout().split_idx_at(pt.x(), 2.0F);
        super::set_cursor(idx > -1 ? cursor::size_h() : cursor::arrow());
      }
    }

    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      template<typename Layout>
      base_column_list<Layout>::base_column_list (core::size_type item_size,
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
      auto base_column_list<Layout>::get_column_layout()->layout_type & {
        return header.get_layout();
      }

      template<typename Layout>
      auto base_column_list<Layout>::get_column_layout() const->const layout_type &{
        return header.get_layout();
      }

      template<typename Layout>
      void base_column_list<Layout>::create (const container& parent,
                                             const core::rectangle& place) {
        super::create(clazz::get(), parent, place);
        header.create(*reinterpret_cast<container*>(this), core::rectangle(0, 0, place.width(), 20));
        header.set_visible();
        list.create(*reinterpret_cast<container*>(this), core::rectangle(0, 20, place.width(), place.height() - 20));
        list.set_visible();
        header.layout();
      }

      template<typename Layout>
      void base_column_list<Layout>::init () {
        super::get_layout().set_header_and_list(&header, &list);
        get_column_layout().set_list(&list);
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
    simple_column_list<L>::simple_column_list (core::size_type item_size,
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
                                                        core::point_type x,
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
                                                        core::point_type x,
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
    column_list_t<L, A ...>::column_list_t (core::size_type item_size,
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


