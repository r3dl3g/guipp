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
#include "slider.h"


namespace gui {

  namespace layout {

    typedef core::size::type column_size_type;

    struct column_info {
      column_size_type width;
      draw::text_origin align;
    };

    namespace detail {

      class column_list_layout : protected layout_base {
      public:
        typedef layout_base super;
        typedef win::detail::slider slider;

        typedef std::vector<slider*>(create_sliders)(size_t);

        column_list_layout (win::container* main)
          : super(main)
          , list(nullptr)
        {}

        void init_auto_layout () {
          super::init(core::bind_method(this, &column_list_layout::layout));
        }

        void layout(const core::size& new_size);
          
        void set_slider_creator(std::function<create_sliders> sc);

        inline std::size_t get_column_count() const {
          return widths.size();
        }

        void set_column_count(std::size_t i);

        inline draw::text_origin get_column_align(std::size_t i) const {
          return aligns[i];
        }

        inline column_size_type get_column_width(std::size_t i) const {
          return widths[i];
        }

        inline const slider* get_slider(std::size_t i) const {
          return sliders[i];
        }

        inline slider* get_slider(std::size_t i) {
          return sliders[i];
        }

        column_size_type get_column_left_pos(std::size_t i) const;
        column_size_type get_column_right_pos(std::size_t i) const;

        void set_column_align(std::size_t i, draw::text_origin a);

        void set_column_width(std::size_t i, column_size_type w, bool update = true);

        void set_slider(std::size_t i, win::detail::slider*);

        void set_column_info(std::size_t i, const column_info& info, bool update = true);

        void set_columns(std::initializer_list<column_info> infos, bool update = true);

        core::size::type get_available_width() const;

        core::size::type get_available_width(const core::size&) const;

        void set_list (win::list* l) {
          list = l;
        }

      protected:
        void update_views();
        void redraw_views();

        std::function<create_sliders> slider_creator;

        std::vector<column_size_type> widths;
        std::vector<draw::text_origin> aligns;
        std::vector<slider*> sliders;

        win::list* list;
      };

    }

    // --------------------------------------------------------------------------
    struct simple_column_info {
      column_size_type width;
      draw::text_origin align;
      column_size_type min_width;
    };

    // --------------------------------------------------------------------------
    class simple_column_list_layout : public detail::column_list_layout {
    public:
      typedef detail::column_list_layout super;

      simple_column_list_layout (win::container* main)
        : super(main)
      {}

      void set_column_width(std::size_t i, column_size_type w, bool update = true) {
        super::set_column_width(i, std::max(w, get_column_min_width(i)), update);
      }

      void set_column_count(std::size_t i) {
        min_widths.resize(i);
        super::set_column_count(i);
      }

      void set_columns(std::initializer_list<simple_column_info> infos, bool update = true);

      void set_column_min_width(std::size_t i, const column_size_type w) {
        min_widths[i] = w;
        if (get_column_width(i)< get_column_min_width(i)) {
          set_column_width(i, w);
        }
      }

      const column_size_type get_column_min_width(std::size_t i) const {
        return min_widths[i];
      }

    protected:
      std::vector<column_size_type> min_widths;

    };

    // --------------------------------------------------------------------------
    struct weight_column_info {
      column_size_type width;
      draw::text_origin align;
      column_size_type min_width;
      float weight;
    };

    // --------------------------------------------------------------------------
    class weight_column_list_layout : public simple_column_list_layout {
    public:
      typedef simple_column_list_layout super;

      weight_column_list_layout (win::container* main)
        : super(main)
      {}

      void init_auto_layout () {
        super::init(core::bind_method(this, &weight_column_list_layout::layout));
      }

      void layout(const core::size& new_size);

      void set_column_width(std::size_t i, column_size_type w, bool update = true);

      void set_column_count(std::size_t i) {
        weights.resize(i);
        super::set_column_count(i);
      }

      void set_columns(std::initializer_list<weight_column_info> infos, bool update = true);

      void set_column_weight(std::size_t i, float w) {
        weights[i] = w;
      }

      float get_column_weight(std::size_t i) const {
        return weights[i];
      }

    protected:
      std::vector<float> weights;

    };

    namespace detail {

      // --------------------------------------------------------------------------
      class base_column_list_layout : protected layout_base {
      public:
        typedef layout_base super;

        base_column_list_layout(win::container* m)
          : super(m)
          , header(nullptr)
          , list(nullptr)
        {
          super::init(core::bind_method(this, &base_column_list_layout::layout));
        }

        void layout (const core::size& sz) {
          header->resize(core::size(sz.width(), 20));
          list->resize(sz - core::size(0, 20));
        }

        void set_header_and_list(win::container* h, win::list* l) {
          list = l;
          header = h;
        }

      protected:
        win::container* header;
        win::list* list;
      };

    } // detail

  }

  namespace win {

    inline void default_cell_drawer (int i,
                                     const draw::graphics& g,
                                     const core::rectangle& r,
                                     const draw::brush& background) {
      using namespace draw;
      frame::raised_relief(g, r);
      g.text(text_box(ostreamfmt((i + 1) << '.'), r, center), font::system(), color::windowTextColor());
    }

    // --------------------------------------------------------------------------
    template<typename Layout>
    class column_list_header : public group_window<Layout> {
    public:
      typedef Layout layout_type;
      typedef group_window<layout_type> super;

      typedef framed_slider_t<false, draw::frame::sunken_relief> slider_type;

      typedef void(cell_draw)(int idx,
                              const draw::graphics&,
                              const core::rectangle& place,
                              const draw::brush& background);

      column_list_header() {
        set_cell_drawer(default_cell_drawer);
        this->get_layout().init_auto_layout();
        this->get_layout().set_slider_creator([&](std::size_t i) {
          return create_slider(i);
        });
        this->register_event_handler(win::paint_event([&](const draw::graphics& g) {
          using namespace draw;

          core::rectangle r = this->client_area();
          draw::brush background(color::buttonColor());
          g.fill(rectangle(r), background);

          std::size_t count = this->get_layout().get_column_count();
          for (int i = 0; i < count; ++i) {
            layout::column_size_type w = this->get_layout().get_column_width(i);
            r.width(w - 1);
            if (cell_drawer) {
              cell_drawer(i, g, r, background);
            }
            r.move_x(w);
          }

        }));
      }

      std::vector<detail::slider*> create_slider (std::size_t count) {
        core::rectangle r(0, 0, 2, this->size().height());
        sliders.resize(count);
        std::vector<detail::slider*> v;
        for (int i = 0; i < count; ++i) {
          slider_type& s = sliders[i];
          v.push_back(&s);
          r.move_x(this->get_layout().get_column_width(i) - 2);
          if (!s.is_valid()) {
            s.create(*this, r);
            s.set_visible();
            s.register_event_handler(win::slider_event([=](int dx) {
              this->get_layout().set_column_width(i, this->get_layout().get_column_width(i) + dx);
            }));
          }
          r.move_x(2);
        }
        return v;
      }

      void set_cell_drawer(std::function<cell_draw> cd) {
        cell_drawer = cd;
      }

    private:
      std::vector<slider_type> sliders;
      std::function<cell_draw> cell_drawer;

      void operator= (column_list_header&) = delete;
    };

    namespace detail {

      extern window_class base_column_list_clazz;

      // --------------------------------------------------------------------------
      template<typename Layout>
      class base_column_list : public layout_container<layout::detail::base_column_list_layout> {
      public:
        typedef Layout layout_type;
        typedef layout_container<layout::detail::base_column_list_layout> super;

        base_column_list () {
          get_layout().set_header_and_list(&header, &list);
          get_column_layout().set_list(&list);
        }

        ~base_column_list ()
        {}

        layout_type& get_column_layout() {
          return header.get_layout();
        }

        const layout_type& get_column_layout() const {
          return header.get_layout();
        }

        void create(const container& parent,
                     const core::rectangle& place = core::rectangle::def) {
          super::create(base_column_list_clazz, parent, place);
          header.create(*reinterpret_cast<container*>(this), core::rectangle(0, 0, place.width(), 20));
          header.set_visible();
          list.create(*reinterpret_cast<container*>(this), core::rectangle(0, 20, place.width(), place.height() - 20));
          list.set_visible();
          header.layout();
        }

        column_list_header<layout_type> header;
        win::list list;
      };

    }

    // --------------------------------------------------------------------------
    // static data for simple_column_list.
    // --------------------------------------------------------------------------
    template<typename T,
             void(F)(const draw::graphics&, const core::rectangle&) = draw::frame::no_frame>
    struct simple_column_list_data : public std::vector<std::vector<T>> {
      typedef std::vector<T> row;
      typedef std::vector<row> super;

      typedef typename super::iterator iterator;

      simple_column_list_data()
      {}

      simple_column_list_data (std::initializer_list<row> args)
        : super(args)
      {}

      simple_column_list_data (iterator b, iterator e)
        : super(b, e)
      {}

      template<size_t N>
      simple_column_list_data (const row(&t) [N])
        : super(t, t + N)
      {}

      template<typename L>
      void update_list (L& l) {
        l.set_count(super::size());
      }

      void operator() (int row_id, int col_id,
                       const draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       bool selected,
                       draw::text_origin align) {
        paint::text_item(convert_to_string(super::at(row_id).at(col_id)), g, place, background, selected, align);
        if (!selected) {
          F(g, place);
        }
      }

    };

    // --------------------------------------------------------------------------
    template<typename Layout>
    class simple_column_list : public detail::base_column_list<Layout> {
    public:
      typedef Layout layout_type;
      typedef detail::base_column_list<layout_type> super;

      typedef void(cell_draw)(int row_id, int col_id,
                              const draw::graphics&,
                              const core::rectangle& place,
                              const draw::brush& background,
                              bool selected,
                              draw::text_origin align);


      void set_drawer (std::function<cell_draw> drawer,
                       core::size::type item_height = 20) {
        this->drawer = drawer;
        this->list.set_drawer(core::bind_method(this, &simple_column_list::draw_cells), { item_height, item_height });
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
      }

      template<typename T,
               void(F)(const draw::graphics&, const core::rectangle&) = draw::frame::no_frame>
      void set_data (simple_column_list_data<T, F> data,
                     core::size::type item_height = 20) {
        set_drawer(data, item_height);
        this->list.set_count(data.size());
      }

    protected:
      void draw_cells (int idx,
                       const draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       bool selected) {
        if (drawer) {
          core::rectangle r = place;
          std::size_t count = this->get_column_layout().get_column_count();
          for (int i = 0; i < count; ++i) {
            core::size::type w = this->get_column_layout().get_column_width(i);
            r.width(w - 1);
            drawer(idx, i, g, r, background, selected, this->get_column_layout().get_column_align(i));
            r.move_x(w);
          }
        }
      }

      std::function<cell_draw> drawer;
    };

    // --------------------------------------------------------------------------
    template<typename... Arguments>
    using column_list_row_t = std::tuple<Arguments...>;

    // --------------------------------------------------------------------------
    template<typename... Arguments>
    struct static_column_list_data_t : public std::vector<column_list_row_t<Arguments...>> {
      typedef column_list_row_t<Arguments...> row;
      typedef std::vector<row> super;

      typedef typename super::iterator iterator;

      static_column_list_data_t ()
      {}

      static_column_list_data_t (std::initializer_list<row> args)
        : super(args)
      {}

      static_column_list_data_t (iterator b, iterator e)
        : super(b, e)
      {}

      template<size_t N>
      static_column_list_data_t (const row(&t) [N])
        : super(t, t + N)
      {}

      template<typename L>
      void update_list (L& l) {
        l.set_count(super::size());
      }

      row operator() (int row_id) const {
        return super::at(row_id);
      }

    };

    // --------------------------------------------------------------------------
    template<typename T,
             void(F)(const draw::graphics&, const core::rectangle&) = draw::frame::no_frame>
    void cell_drawer (const T& t,
                      const draw::graphics& g,
                      const core::rectangle& place,
                      const draw::brush& background,
                      bool selected,
                      draw::text_origin align) {
      paint::text_item(convert_to_string(t), g, place, background, selected, align);
      if (!selected) {
        F(g, place);
      }
    }

    // --------------------------------------------------------------------------
    template<typename T>
    using cell_drawer_t = void (*)(const T& t,
                                   const draw::graphics& g,
                                   const core::rectangle& place,
                                   const draw::brush& background,
                                   bool selected,
                                   draw::text_origin align);

    // --------------------------------------------------------------------------
    template<typename... Arguments>
#ifdef _MSC_VER

    struct row_cell_drawer_t : public std::tuple<cell_drawer_t<Arguments>...> {
      typedef std::tuple<cell_drawer_t<Arguments>...> super;

      row_cell_drawer_t () 
      {}

      row_cell_drawer_t(super rhs)
        : super(rhs)
      {}
    };
#else
    using row_cell_drawer_t = std::tuple<cell_drawer_t<Arguments>...>;
#endif // _MSC_VER < 1900

    // --------------------------------------------------------------------------
    template<typename Layout, typename... Arguments>
    struct column_list_row_drawer_t : public row_cell_drawer_t<Arguments...> {
      typedef Layout layout_type;
      typedef row_cell_drawer_t<Arguments...> super;
      typedef column_list_row_t<Arguments...> row;

      column_list_row_drawer_t ()
      {}

#ifdef _MSC_VER
      column_list_row_drawer_t (const super::super& rhs)
        : super(rhs)
      {}
#endif // _MSC_VER

      column_list_row_drawer_t (cell_drawer_t<Arguments>...args)
        : super(std::make_tuple(args...))
      {}

      void operator= (const column_list_row_drawer_t& rhs) {
        super::operator=(rhs);
      }

      template<int I>
      void draw_cell (const row& data,
                      const layout_type& l,
                      const draw::graphics& g,
                      core::rectangle place,
                      const draw::brush& background,
                      bool selected) {
      }

      template<int I, typename T, typename... Args>
      void draw_cell (const row& data,
                      const layout_type& l,
                      const draw::graphics& g,
                      core::rectangle place,
                      const draw::brush& background, 
                      bool selected) {
        core::size::type width = l.get_column_width(I);
        draw::text_origin align = l.get_column_align(I);

        place.width(width - 1);
        std::get<I>(*this)(std::get<I>(data), g, place, background, selected, align);
        place.move_x(width);
        draw_cell<I + 1, Args...>(data, l, g, place, background, selected);
      }

      void operator() (const row& data,
                       const layout_type& l,
                       const draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       bool selected) {
        draw_cell<0, Arguments...>(data, l, g, place, background, selected);
      }
    };

    namespace detail {

      // --------------------------------------------------------------------------
//#if defined(_MSC_VER) && (_MSC_VER < 1900)
//      template<typename Layout, typename... Arguments>
//      struct simple_column_list_drawer_t : public column_list_row_drawer_t<Layout, cell_drawer<Arguments, draw::frame::no_frame>...> {
//        typedef win::column_list_row_drawer_t<Layout, win::cell_drawer<Arguments, draw::frame::no_frame>...> super;
//
//        simple_column_list_drawer_t ()
//        {}
//        
//        simple_column_list_drawer_t (super rhs)
//          : super(rhs) 
//        {}
//      };
//#else
//      template<typename Layout, typename...Arguments>
//      using row_cell_drawer_t = column_list_row_drawer_t<Layout, cell_drawer<Arguments, draw::frame::no_frame>...>;
//#endif // _MSC_VER < 1900

    } // detail

    // --------------------------------------------------------------------------
    template<typename Layout, typename... Arguments>
    class column_list_t : public detail::base_column_list<Layout> {
    public:
      static const std::size_t size = sizeof...(Arguments);

      typedef Layout layout_type;
      typedef detail::base_column_list<layout_type> super;

      typedef column_list_row_t<Arguments...> row;

      typedef static_column_list_data_t<Arguments...> standard_data;

      //typedef column_list_row_drawer_t<layout_type, cell_drawer<Arguments>...> standard_drawer;
      typedef column_list_row_drawer_t<layout_type, Arguments...> row_drawer;

      typedef row (get_row_data_t)(int idy);

      typedef void (draw_row_data_t)(const row&,
                                     const layout_type& l,
                                     const draw::graphics& g,
                                     const core::rectangle& place,
                                     const draw::brush& background,
                                     bool selected);

      typedef std::function<get_row_data_t> data_provider;
      typedef std::function<draw_row_data_t> data_drawer;

      column_list_t () {
        this->get_column_layout().set_column_count(size);
      }

      void set_drawer (data_drawer drawer, core::size::type item_height = 20) {
        this->drawer = drawer;
        this->list.set_drawer(core::bind_method(this, &column_list_t::draw_cells_t), { item_height, item_height });
      }

      void set_data (data_provider data, std::size_t count) {
        this->data = data;
        this->list.set_count(count);
      }

    protected:
      void draw_cells_t (int row_id,
                         const draw::graphics& g,
                         const core::rectangle& place,
                         const draw::brush& background,
                         bool selected) {
        drawer(data(row_id), this->get_column_layout(), g, place, background, selected);
      }

      data_drawer drawer;
      data_provider data;
    };

  } // win

} // gui


