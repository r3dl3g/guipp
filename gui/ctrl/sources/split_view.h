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


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    template<orientation O>
    class split_view : protected layout_base {
    public:
      split_view (win::container* m)
        : layout_base(m)
        , first(nullptr)
        , second(nullptr)
        , slider(nullptr)
      {
        init(core::bind_method(this, &split_view::layout));
      }

      win::window* get_first () const {
        return first;
      }

      void set_first (win::window* left) {
        split_view::first = left;
      }

      win::window* get_second () const {
        return second;
      }

      void set_second (win::window* right) {
        split_view::second = right;
      }

      win::detail::slider* get_slider () const {
        return slider;
      }

      void set_slider (win::detail::slider* slider) {
        split_view::slider = slider;
      }

      static core::rectangle get_first_place (const core::size&, double pos);
      static core::rectangle get_second_place (const core::size&, double pos);
      static core::rectangle get_slider_place (const core::size&, double pos);

      static core::size::type get_slider_width ();

      double get_split_pos (const core::size&) const;

      void layout (const core::size& sz) {
        double pos = get_split_pos(sz);
        if (first) {
          first->place(get_first_place(sz, pos));
        }
        if (second) {
          second->place(get_second_place(sz, pos));
        }
        if (slider) {
          slider->place(get_slider_place(sz, pos));
        }
      }

    private:
      win::window* first;
      win::window* second;
      win::detail::slider* slider;
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

        split_view ();

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

        win::framed_slider_t<O, draw::frame::raised_relief> slider;

      private:
        static window_class clazz;
      };

      // --------------------------------------------------------------------------
      template<>
      split_view<orientation::vertical>::split_view ();

      // --------------------------------------------------------------------------
      template<>
      split_view<orientation::horizontal>::split_view ();

    }

    // --------------------------------------------------------------------------
    template<orientation O, typename First, typename Second>
    class split_view_t : public detail::split_view<O> {
    public:
      typedef detail::split_view<O> super;
      typedef typename super::layout_type layout_type;

      split_view_t () {
        super::get_layout().set_first(&first);
        super::get_layout().set_second(&second);
      }

      split_view_t (First&& first, Second&& second)
        : first(std::move(first))
        , second(std::move(second))
      {
        super::get_layout().set_first(&this->first);
        super::get_layout().set_second(&this->second);
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
