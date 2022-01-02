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
 * @brief     movable separator view control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/control.h>
#include <gui/ctrl/look/splitter.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
#ifdef GUIPP_WIN
    using slider_event = core::event_handler<detail::SLIDER_MESSAGE, 0,
                                       core::params<int>::getter<win::get_param<0, int> > >;
#elif GUIPP_X11
    using slider_event = core::event_handler<ClientMessage, 0,
                                       core::params<int>::getter<win::get_client_data<0, int> >,
                                       0, win::event::functor<win::client_message_matcher<detail::SLIDER_MESSAGE>>>;
#elif GUIPP_QT
    using slider_event = core::event_handler<detail::SLIDER_MESSAGE, 0,
                                       core::params<long>::getter<win::get_param<0>>>;
#else
# error Undefined system: slider_event
#endif // GUIPP_QT
       // --------------------------------------------------------------------------

    namespace detail {

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT splitter_base : public control {
      public:
        typedef control super;
        typedef core::point::type type;

        splitter_base ();
        splitter_base (const splitter_base& rhs);
        splitter_base (splitter_base&& rhs) noexcept ;

        void set_min (type min);
        void set_max (type min);
        void set_min_max (type mi, type ma);

        inline type get_min () const;
        inline type get_max () const;

        void on_slide (std::function<void(int)>&& f);

      protected:
        type min;
        type max;

        core::native_point start_mouse_point;
        core::point start_window_point;

      private:
        void init ();
      };

      // --------------------------------------------------------------------------
      template<orientation_t O>
      struct splitter_cursor {};

      template<>
      struct splitter_cursor<orientation_t::horizontal> {
        static constexpr win::cursor_type value = win::cursor_type::size_v;
      };

      template<>
      struct splitter_cursor<orientation_t::vertical> {
        static constexpr win::cursor_type value = win::cursor_type::size_h;
      };

      // --------------------------------------------------------------------------
      template<orientation_t O>
      class basic_splitter : public splitter_base {
      public:
        typedef splitter_base super;
        typedef win::no_erase_window_class<basic_splitter, splitter_cursor<O>::value> clazz;

        basic_splitter ();
        basic_splitter (const basic_splitter& rhs);
        basic_splitter (basic_splitter&& rhs) noexcept ;

        void create (win::container& parent,
                     const core::rectangle& place = core::rectangle::def);

        void set_value (core::point::type v);
        core::point::type get_value () const;

      private:
        void init ();
      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation_t O,
             draw::frame::drawer F = draw::frame::raised_relief>
    class basic_framed_splitter : public detail::basic_splitter<O> {
    public:
      typedef detail::basic_splitter<O> super;

      basic_framed_splitter ();
      basic_framed_splitter (const basic_framed_splitter& rhs);
      basic_framed_splitter (basic_framed_splitter&& rhs) noexcept ;

    private:
      void paint (draw::graphics& g);

      void init ();
    };

    // --------------------------------------------------------------------------
    using vertical_splitter = basic_framed_splitter<orientation_t::vertical>;
    using horizontal_splitter = basic_framed_splitter<orientation_t::horizontal>;

  } // ctrl

} // gui

#include <gui/ctrl/splitter.inl>
