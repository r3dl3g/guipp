/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     split window control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/layout/layout_container.h>
#include <gui/layout/split_layout.h>
#include <gui/ctrl/splitter.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    template<orientation_t O>
    struct split_view_traits {
      static core::rectangle get_first_geometry (const core::rectangle&, double pos);
      static core::rectangle get_second_geometry (const core::rectangle&, double pos);
      static core::rectangle get_splitter_geometry (const core::rectangle&, double pos);
      static core::size::type get_splitter_width ();
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
      ctrl::detail::splitter_base* get_splitter () const;

      void set_first (First* first);
      void set_second (Second* second);
      void set_splitter (ctrl::detail::splitter_base* splitter);

      void set (First* first,
                Second* second,
                ctrl::detail::splitter_base* splitter);

      double get_split_pos (const core::point&, const core::size&) const;
      void set_split_pos (double);

      void layout (const core::rectangle& r) const;

    private:
      First* first;
      Second* second;
      ctrl::detail::splitter_base* splitter;
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
    class split_view : public win::layout_container<layout::split_view<O, First, Second> > {
    public:
      typedef win::layout_container<layout::split_view<O, First, Second>> super;
      typedef typename super::layout_type layout_type;
      typedef win::window_class<split_view, IF_WIN32_ELSE((os::color)(COLOR_WINDOW + 1), color::white)> clazz;

      typedef basic_framed_splitter<O, draw::frame::raised_relief> splitter_type;

      split_view ();
      split_view (split_view&& rhs) noexcept;
      split_view (First&& first, Second&& second);

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   double split_pos = 0.5);

      double get_split_pos () const;
      void set_split_pos (double pos);

      splitter_type splitter;
      First first;
      Second second;

    private:
      void init ();

    };

    // --------------------------------------------------------------------------
    template<typename First, typename Second>
    using vertical_split_view = split_view<orientation_t::vertical, First, Second>;

    template<typename First, typename Second>
    using horizontal_split_view = split_view<orientation_t::horizontal, First, Second>;

    // --------------------------------------------------------------------------
    template<typename Header, typename Body, int S = 25, alignment_t A = alignment_t::top>
    class fix_split_view : public win::layout_container<layout::split_layout<A, S> > {
    public:
      typedef win::layout_container<layout::split_layout<A, S>> super;
      typedef typename super::layout_type layout_type;
      typedef win::window_class<fix_split_view, IF_WIN32_ELSE((os::color)(COLOR_WINDOW + 1), color::white)> clazz;

      fix_split_view ();
      fix_split_view (fix_split_view&& rhs) noexcept;
      fix_split_view (Header&& header, Body&& body);

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      Header header;
      Body body;
    };

    // --------------------------------------------------------------------------

  } // ctrl

} // gui

#include <gui/ctrl/split_view.inl>
