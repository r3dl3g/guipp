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
    template<bool H>
    class split_view {
    public:
      split_view ()
        : first(nullptr)
        , second(nullptr)
        , slider(nullptr)
      {}

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

      void layout (const core::size& sz, win::container* main) {
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
    double split_view<false>::get_split_pos (const core::size&) const;

    template<>
    core::rectangle split_view<false>::get_first_place (const core::size&,
                                                        double);

    template<>
    core::rectangle split_view<false>::get_second_place (const core::size&,
                                                         double);

    template<>
    core::rectangle split_view<false>::get_slider_place (const core::size&, double);

    // --------------------------------------------------------------------------
    template<>
    double split_view<true>::get_split_pos (const core::size&) const;

    template<>
    core::rectangle split_view<true>::get_first_place (const core::size&,
                                                       double);

    template<>
    core::rectangle split_view<true>::get_second_place (const core::size&,
                                                        double);

    template<>
    core::rectangle split_view<true>::get_slider_place (const core::size&, double);
    // --------------------------------------------------------------------------
  }

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      template<bool H>
      class split_view : public layout_container<layout::split_view<H>> {
      public:
        typedef layout_container<layout::split_view<H>> super;
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
          super::do_layout();
        }

        win::framed_slider_t<H, draw::frame::raised_relief> slider;

      private:
        static window_class clazz;
      };

      template<>
      window_class split_view<false>::clazz;

      template<>
      window_class split_view<true>::clazz;

      // --------------------------------------------------------------------------
      template<>
      split_view<false>::split_view ();

      // --------------------------------------------------------------------------
      template<>
      split_view<true>::split_view ();

    }

    // --------------------------------------------------------------------------
    template<bool H, typename First, typename Second>
    class split_view_t : public detail::split_view<H> {
    public:
      typedef detail::split_view<H> super;
      typedef typename super::layout_type layout_type;

      split_view_t () {
        super::get_layout().set_first(&first);
        super::get_layout().set_second(&second);
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

  } // win

} // gui
