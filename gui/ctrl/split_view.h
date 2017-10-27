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
#include <gui/win/container.h>
#include <gui/ctrl/slider.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    struct split_view_data {
      split_view_data ();

      win::window* first;
      win::window* second;
      win::detail::slider_base* slider;
    };

    // --------------------------------------------------------------------------
    template<orientation O>
    class split_view : protected layout_base {
    public:
      typedef layout_base super;

      split_view (win::container* m);
      split_view (win::container* m, const split_view& rhs);
      split_view (win::container* m, split_view&& rhs);

      win::window* get_first () const;
      win::window* get_second () const;
      win::detail::slider_base* get_slider () const;

      void set_first (win::window* first);
      void set_second (win::window* second);
      void set_slider (win::detail::slider_base* slider);

      void set (win::window* first,
                win::window* second,
                win::detail::slider_base* slider);

      static core::rectangle get_first_place (const core::size&, double pos);
      static core::rectangle get_second_place (const core::size&, double pos);
      static core::rectangle get_slider_place (const core::size&, double pos);

      static core::size::type get_slider_width ();

      double get_split_pos (const core::size&) const;

      void layout (const core::size& sz);

    private:
      void init ();

      split_view_data data;
    };

    // --------------------------------------------------------------------------

  } // namespace layout

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      template<orientation O>
      class split_view : public layout_container<layout::split_view<O> > {
      public:
        typedef layout_container<layout::split_view<O> > super;
        typedef typename super::layout_type layout_type;
        typedef window_class<split_view, IF_WIN32_ELSE((os::color)(COLOR_WINDOW + 1), color::white)> clazz;

        typedef win::basic_framed_slider<O, draw::frame::raised_relief> slider_type;

        split_view ();
        split_view (const split_view& rhs);
        split_view (split_view&& rhs);

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def,
                     double split_pos = 0.5);

        double get_split_pos () const;
        void set_split_pos (double pos);

        slider_type slider;

      private:
        void init ();

      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation O, typename First, typename Second>
    class basic_split_view : public detail::split_view<O> {
    public:
      typedef detail::split_view<O> super;
      typedef typename super::layout_type layout_type;

      basic_split_view ();
      basic_split_view (const basic_split_view& rhs);
      basic_split_view (basic_split_view&& rhs);
      basic_split_view (First&& first, Second&& second);

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   double split_pos = 0.5);

      First first;
      Second second;
    };

    // --------------------------------------------------------------------------
    template<typename First, typename Second>
    using vertical_split_view = basic_split_view<orientation::vertical, First, Second>;

    template<typename First, typename Second>
    using horizontal_split_view = basic_split_view<orientation::horizontal, First, Second>;

  } // win

  // --------------------------------------------------------------------------
  // inlines
  namespace layout {

    // --------------------------------------------------------------------------
    inline split_view_data::split_view_data ()
      : first(nullptr)
      , second(nullptr)
      , slider(nullptr)
    {}

    // --------------------------------------------------------------------------
    template<orientation O>
    inline split_view<O>::split_view (win::container* m)
      : layout_base(m)
    {
      init();
    }

    template<orientation O>
    inline split_view<O>::split_view (win::container* m, const split_view& rhs)
      : layout_base(m, rhs.layout_base)
    {
      init();
    }

    template<orientation O>
    inline split_view<O>::split_view (win::container* m, split_view&& rhs)
      : layout_base(m, std::move(rhs.layout_base))
    {
      init();
    }

    template<orientation O>
    inline win::window* split_view<O>::get_first () const {
      return data.first;
    }

    template<orientation O>
    inline void split_view<O>::set_first (win::window* first) {
      data.first = first;
    }

    template<orientation O>
    inline win::window* split_view<O>::get_second () const {
      return data.second;
    }

    template<orientation O>
    inline void split_view<O>::set_second (win::window* second) {
      data.second = second;
    }

    template<orientation O>
    inline win::detail::slider_base* split_view<O>::get_slider () const {
      return data.slider;
    }

    template<orientation O>
    inline void split_view<O>::set_slider (win::detail::slider_base* slider) {
      data.slider = slider;
    }

    template<orientation O>
    inline void split_view<O>::set (win::window* first,
                                    win::window* second,
                                    win::detail::slider_base* slider) {
      data.first = first;
      data.second = second;
      data.slider = slider;
    }

    template<orientation O>
    void split_view<O>::layout (const core::size& sz) {
      double pos = get_split_pos(sz);
      if (data.first) {
        data.first->place(get_first_place(sz, pos));
      }
      if (data.second) {
        data.second->place(get_second_place(sz, pos));
      }
      if (data.slider) {
        data.slider->place(get_slider_place(sz, pos));
      }
    }

    template<orientation O>
    inline void split_view<O>::init () {
      super::init(core::bind_method(this, &split_view::layout));
    }

    // --------------------------------------------------------------------------
    template<>
    double split_view<orientation::vertical>::get_split_pos (const core::size&) const;

    template<>
    core::rectangle split_view<orientation::vertical>::get_first_place (const core::size&,
                                                                        double);

    template<>
    core::rectangle split_view<orientation::vertical>::get_second_place (const core::size&,
                                                                         double);

    template<>
    core::rectangle split_view<orientation::vertical>::get_slider_place (const core::size&, double);

    // --------------------------------------------------------------------------
    template<>
    double split_view<orientation::horizontal>::get_split_pos (const core::size&) const;

    template<>
    core::rectangle split_view<orientation::horizontal>::get_first_place (const core::size&,
                                                                          double);

    template<>
    core::rectangle split_view<orientation::horizontal>::get_second_place (const core::size&,
                                                                           double);

    template<>
    core::rectangle split_view<orientation::horizontal>::get_slider_place (const core::size&, double);
    // --------------------------------------------------------------------------

  } // namespace layout

  namespace win {

    namespace detail {

      template<orientation O>
      inline split_view<O>::split_view () {
        init();
      }

      template<orientation O>
      inline split_view<O>::split_view (const split_view& rhs)
        : super(rhs)
        , slider(rhs.slider)
      {
        init();
      }

      template<orientation O>
      inline split_view<O>::split_view (split_view&& rhs)
        : super(std::move(rhs))
      {
        std::swap(slider, rhs.slider);
        init();
      }

      template<orientation O>
      void split_view<O>::create (const container& parent,
                                  const core::rectangle& place,
                                  double split_pos) {
        super::create(clazz::get(), parent, place);
        slider.create(*this, layout_type::get_slider_place(place.size(), split_pos));
        slider.set_visible();
      }

      template<orientation O>
      inline double split_view<O>::get_split_pos () const {
        return super::get_layout().get_split_pos(super::size());
      }

      template<orientation O>
      inline void split_view<O>::set_split_pos (double pos) {
        slider.place(layout_type::get_slider_place(super::size(), pos));
        super::layout();
      }

      // --------------------------------------------------------------------------
      template<>
      void split_view<orientation::vertical>::init ();

      // --------------------------------------------------------------------------
      template<>
      void split_view<orientation::horizontal>::init ();

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation O, typename F, typename S>
    inline basic_split_view<O, F, S>::basic_split_view () {
      super::get_layout().set(&first, &second, &(super::slider));
    }

    template<orientation O, typename F, typename S>
    inline basic_split_view<O, F, S>::basic_split_view (const basic_split_view& rhs)
      : super(rhs)
      , first(rhs.first)
      , second(rhs.second)
    {
      super::get_layout().set(&first, &second, &(super::slider));
    }

    template<orientation O, typename F, typename S>
    inline basic_split_view<O, F, S>::basic_split_view (basic_split_view&& rhs)
      : super(std::move(rhs))
    {
      std::swap(first, rhs.first);
      std::swap(second, rhs.second);
      super::get_layout().set(&first, &second, &(super::slider));
    }

    template<orientation O, typename F, typename S>
    inline basic_split_view<O, F, S>::basic_split_view (F&& first, S&& second)
      : first(std::move(first))
      , second(std::move(second))
    {
      super::get_layout().set(&(this->first), &(this->second), &(super::slider));
    }

    template<orientation O, typename F, typename S>
    void basic_split_view<O, F, S>::create (const container& parent,
                                            const core::rectangle& place,
                                            double split_pos) {
      super::create(parent, place, split_pos);
      core::size sz = place.size();
      first.create(*this, layout_type::get_first_place(sz, split_pos));
      first.set_visible();
      second.create(*this, layout_type::get_second_place(sz, split_pos));
      second.set_visible();
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
