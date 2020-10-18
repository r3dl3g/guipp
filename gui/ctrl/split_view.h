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
    template<orientation_t O>
    struct split_view_traits {
      static core::rectangle get_first_place (const core::rectangle&, double pos);
      static core::rectangle get_second_place (const core::rectangle&, double pos);
      static core::rectangle get_slider_place (const core::rectangle&, double pos);
      static core::size::type get_slider_width ();
      static double get_split_pos (const core::point&, const core::size&);
    };

    // --------------------------------------------------------------------------
    template<orientation_t O, typename First, typename Second>
    class split_view {
    public:
      typedef split_view_traits<O> traits;

      split_view ();

      First* get_first () const;
      Second* get_second () const;
      ctrl::detail::slider_base* get_slider () const;

      void set_first (First* first);
      void set_second (Second* second);
      void set_slider (ctrl::detail::slider_base* slider);

      void set (First* first,
                Second* second,
                ctrl::detail::slider_base* slider);

      double get_split_pos (const core::point&, const core::size&) const;
      void set_split_pos (double);

      void layout (const core::rectangle& sz) const;

    private:
      First* first;
      Second* second;
      ctrl::detail::slider_base* slider;
      double split_pos;
    };

    template<orientation_t O, typename First, typename Second>
    struct is_layout<split_view<O, First, Second>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------

  } // namespace layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<orientation_t O, typename First, typename Second>
    class split_view : public control {
    public:
      typedef control super;
      typedef layout::split_view<O, First, Second> layout_type;
      typedef win::window_class<split_view, IF_WIN32_ELSE((os::color)(COLOR_WINDOW + 1), color::white)> clazz;

      typedef basic_framed_slider<O, draw::frame::raised_relief> slider_type;

      split_view ();
      split_view (split_view&& rhs);
      split_view (First&& first, Second&& second);

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   double split_pos = 0.5);

      double get_split_pos () const;
      void set_split_pos (double pos);

      void layout () const;
      void layout (const core::rectangle& sz) const;

      slider_type slider;
      First first;
      Second second;

    private:
      layout_type layouter;
      void init ();

    };

    // --------------------------------------------------------------------------
    template<typename First, typename Second>
    using vertical_split_view = split_view<orientation_t::vertical, First, Second>;

    template<typename First, typename Second>
    using horizontal_split_view = split_view<orientation_t::horizontal, First, Second>;

  } // ctrl

} // gui

#include <gui/ctrl/split_view.inl>
