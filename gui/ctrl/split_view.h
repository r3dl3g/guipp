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
#include <gui/core/orientation_traits.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    template<orientation_t O>
    struct split_view_traits {
      typedef core::orientation_traits<O> traits;

      static core::rectangle get_window_geometry (const core::rectangle&, double pos1, double pos2);
      static core::rectangle get_first_geometry (const core::rectangle&, double pos);
      static core::rectangle get_last_geometry (const core::rectangle&, double pos);

      static core::rectangle get_splitter_geometry (const core::rectangle&, double pos);

      static core::size::type get_splitter_width ();

      static core::point::type get_position (const core::point& p, const core::size& s, double pos, core::size::type offset);

      static double get_split_pos (const core::point&, const core::size&);
    };

    // --------------------------------------------------------------------------
    template<orientation_t O, std::size_t T>
    class split_view {
    public:
      typedef split_view_traits<O> traits;
      typedef win::window splitter_t;

      split_view ();

      template<std::size_t N>
      win::window* get () const;

      template<std::size_t N>
      void set (win::window* first);

      template<std::size_t N>
      splitter_t* get_splitter () const;

      template<std::size_t N>
      void set_splitter (splitter_t* splitter);

      void set_all (win::window* first,
                    win::window* second,
                    splitter_t* splitter);

      double get_split_pos (const core::point&, const core::size&) const;

      template<std::size_t N>
      void set_split_pos (double);

      void layout (const core::rectangle& r) const;

      void add (const std::vector<std::reference_wrapper<win::window>>&);

    private:
      std::array<win::window*, T> views;
      std::array<splitter_t*, T-1> splitter;
      std::array<double, T-1> split_pos;
    };

    template<orientation_t O, std::size_t T>
    struct is_layout<split_view<O, T>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------

  } // namespace layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<orientation_t O, typename... Ts>
    class split_view : public win::layout_container<layout::split_view<O, sizeof...(Ts)> > {
    public:
      static constexpr std::size_t N = sizeof...(Ts);
      typedef win::layout_container<layout::split_view<O, N>> super;
      typedef std::tuple<Ts...> tuple_t;
      typedef typename super::layout_type layout_type;
      typedef win::window_class<split_view, IF_WIN32_ELSE((os::color)(COLOR_WINDOW + 1), color::white)> clazz;

      typedef basic_framed_splitter<O, draw::frame::raised_relief> splitter_type;

      split_view ();
      split_view (split_view&& rhs) noexcept;
      split_view (Ts&&... views);

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   double split_pos = 0.5);

      double get_split_pos () const;
      void set_split_pos (double pos);

      template<size_t I, typename T = std::tuple_element_t<I, tuple_t>>
      constexpr T& get() {
        return std::get<I>(views); 
      }

      tuple_t views;

    protected:
      std::array<splitter_type, N-1> splitter;

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
