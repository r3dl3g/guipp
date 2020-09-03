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
 * @brief     C++ API: split views
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/layout/layout_container.h>
#include <gui/ctrl/slider.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT split_view_data {
      split_view_data ();

      win::window* first;
      win::window* second;
      ctrl::detail::slider_base* slider;
      double split_pos;
    };

    // --------------------------------------------------------------------------
    template<orientation_t O>
    class split_view {
    public:
      win::window* get_first () const;
      win::window* get_second () const;
      ctrl::detail::slider_base* get_slider () const;

      void set_first (win::window* first);
      void set_second (win::window* second);
      void set_slider (ctrl::detail::slider_base* slider);

      void set (win::window* first,
                win::window* second,
                ctrl::detail::slider_base* slider);

      static core::rectangle get_first_place (const core::rectangle&, double pos);
      static core::rectangle get_second_place (const core::rectangle&, double pos);
      static core::rectangle get_slider_place (const core::rectangle&, double pos);

      static core::size::type get_slider_width ();

      double get_split_pos (const core::rectangle&) const;
      void set_split_pos (double);

      void layout (const core::rectangle& sz) const;

    private:
      split_view_data data;
    };

    // --------------------------------------------------------------------------

  } // namespace layout

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      template<orientation_t O>
      class split_view : public win::layout_container<layout::split_view<O> > {
      public:
        typedef win::layout_container<layout::split_view<O> > super;
        typedef typename super::layout_type layout_type;
        typedef win::window_class<split_view, IF_WIN32_ELSE((os::color)(COLOR_WINDOW + 1), color::white)> clazz;

        typedef basic_framed_slider<O, draw::frame::raised_relief> slider_type;

        split_view ();
        split_view (split_view&& rhs);

        void create (const win::container& parent,
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
    template<orientation_t O, typename First, typename Second>
    class basic_split_view : public detail::split_view<O> {
    public:
      typedef detail::split_view<O> super;
      typedef typename super::layout_type layout_type;

      basic_split_view ();
      basic_split_view (basic_split_view&& rhs);
      basic_split_view (First&& first, Second&& second);

      void create (const win::container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   double split_pos = 0.5);

      First first;
      Second second;
    };

    // --------------------------------------------------------------------------
    template<typename First, typename Second>
    using vertical_split_view = basic_split_view<orientation_t::vertical, First, Second>;

    template<typename First, typename Second>
    using horizontal_split_view = basic_split_view<orientation_t::horizontal, First, Second>;

  } // ctrl

} // gui

#include <gui/ctrl/split_view.inl>
