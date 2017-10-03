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
#include "slider.h"
#include "container.h"


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    struct split_view_data {
      split_view_data ()
        : first(nullptr)
        , second(nullptr)
        , slider(nullptr)
      {}

      win::window* first;
      win::window* second;
      win::detail::slider* slider;
    };

    // --------------------------------------------------------------------------
    template<orientation O>
    class split_view : protected layout_base {
    public:
      typedef layout_base super;

      split_view (win::container* m)
        : layout_base(m)
      {
        init();
      }

      split_view (win::container* m, const split_view& rhs)
        : layout_base(m, rhs.layout_base)
      {
        init();
      }

      split_view (win::container* m, split_view&& rhs)
        : layout_base(m, std::move(rhs.layout_base))
      {
        init();
      }

      win::window* get_first () const {
        return data.first;
      }

      void set_first (win::window* first) {
        data.first = first;
      }

      win::window* get_second () const {
        return data.second;
      }

      void set_second (win::window* second) {
        data.second = second;
      }

      win::detail::slider* get_slider () const {
        return data.slider;
      }

      void set_slider (win::detail::slider* slider) {
        data.slider = slider;
      }

      void set (win::window* first,
                win::window* second,
                win::detail::slider* slider) {
        data.first = first;
        data.second = second;
        data.slider = slider;
      }

      static core::rectangle get_first_place (const core::size&, double pos);
      static core::rectangle get_second_place (const core::size&, double pos);
      static core::rectangle get_slider_place (const core::size&, double pos);

      static core::size::type get_slider_width ();

      double get_split_pos (const core::size&) const;

      void layout (const core::size& sz) {
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

    private:
      void init () {
        super::init(core::bind_method(this, &split_view::layout));
      }

      split_view_data data;
    };
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
  }

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      template<orientation O>
      class split_view : public layout_container<layout::split_view<O>> {
      public:
        typedef layout_container<layout::split_view<O>> super;
        typedef typename super::layout_type layout_type;

        typedef win::framed_slider_t<O, draw::frame::raised_relief> slider_type;

        split_view () {
          init();
        }

        split_view (const split_view& rhs)
          : super(rhs)
          , slider(rhs.slider)
        {
          init();
        }

        split_view (split_view&& rhs)
          : super(std::move(rhs))
        {
          std::swap(slider, rhs.slider);
          init();
        }

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def,
                     double split_pos = 0.5) {
          super::create(clazz, parent, place);
          slider.create(*this, layout_type::get_slider_place(place.size(), split_pos));
          slider.set_visible();
        }

        double get_split_pos () const {
          return super::get_layout().get_split_pos(super::size());
        }

        void set_split_pos (double pos) {
          slider.place(layout_type::get_slider_place(super::size(), pos));
          super::layout();
        }

        slider_type slider;

      private:
        void init ();

        static window_class clazz;
      };

      // --------------------------------------------------------------------------
      template<>
      void split_view<orientation::vertical>::init ();

      // --------------------------------------------------------------------------
      template<>
      void split_view<orientation::horizontal>::init ();

    }

    // --------------------------------------------------------------------------
    template<orientation O, typename First, typename Second>
    class split_view_t : public detail::split_view<O> {
    public:
      typedef detail::split_view<O> super;
      typedef typename super::layout_type layout_type;

      split_view_t () {
        super::get_layout().set(&first, &second, &(super::slider));
      }

      split_view_t (const split_view_t& rhs)
        : super(rhs)
        , first(rhs.first)
        , second(rhs.second)
      {
        super::get_layout().set(&first, &second, &(super::slider));
      }

      split_view_t (split_view_t&& rhs)
        : super(std::move(rhs))
      {
        std::swap(first, rhs.first);
        std::swap(second, rhs.second);
        super::get_layout().set(&first, &second, &(super::slider));
      }

      split_view_t (First&& first, Second&& second)
        : first(std::move(first))
        , second(std::move(second))
      {
        super::get_layout().set(&(this->first), &(this->second), &(super::slider));
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   double split_pos = 0.5) {
        super::create(parent, place, split_pos);
        core::size sz = place.size();
        first.create(*this, layout_type::get_first_place(sz, split_pos));
        first.set_visible();
        second.create(*this, layout_type::get_second_place(sz, split_pos));
        second.set_visible();
      }

      First first;
      Second second;
    };

    template<typename First, typename Second>
    using vsplit_view = split_view_t<orientation::vertical, First, Second>;

    template<typename First, typename Second>
    using hsplit_view = split_view_t<orientation::horizontal, First, Second>;

  } // win

} // gui
