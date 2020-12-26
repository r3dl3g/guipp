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
 * @brief     C++ API: slider control
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/control.h>
#include <gui/look/slider.h>


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
      class GUIPP_CTRL_EXPORT slider_base : public control {
      public:
        typedef control super;
        typedef core::point::type type;

        slider_base ();
        slider_base (const slider_base& rhs);
        slider_base (slider_base&& rhs) noexcept ;

        void set_min (type min);
        void set_max (type min);
        void set_min_max (type mi, type ma);

        inline type get_min () const;
        inline type get_max () const;

        void on_slide (std::function<void(int)>&& f);

      protected:
        type min;
        type max;

        core::point start_mouse_point;
        core::point start_window_point;

      private:
        void init ();
      };

      // --------------------------------------------------------------------------
      template<orientation_t O>
      struct slider_cursor {};

      template<>
      struct slider_cursor<orientation_t::horizontal> {
        static constexpr win::cursor_type value = win::cursor_type::size_v;
      };

      template<>
      struct slider_cursor<orientation_t::vertical> {
        static constexpr win::cursor_type value = win::cursor_type::size_h;
      };

      // --------------------------------------------------------------------------
      template<orientation_t O>
      class basic_slider : public slider_base {
      public:
        typedef slider_base super;
        typedef no_erase_window_class<basic_slider, slider_cursor<O>::value> clazz;

        basic_slider ();
        basic_slider (const basic_slider& rhs);
        basic_slider (basic_slider&& rhs) noexcept ;

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
    class basic_framed_slider : public detail::basic_slider<O> {
    public:
      typedef detail::basic_slider<O> super;

      basic_framed_slider ();
      basic_framed_slider (const basic_framed_slider& rhs);
      basic_framed_slider (basic_framed_slider&& rhs) noexcept ;

    private:
      void paint (const draw::graphics& g);

      void init ();
    };

    // --------------------------------------------------------------------------
    using vertical_slider = basic_framed_slider<orientation_t::vertical>;
    using horizontal_slider = basic_framed_slider<orientation_t::horizontal>;

  } // ctrl

} // gui

#include <gui/ctrl/slider.inl>
