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
#include <gui/ctrl/control.h>


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
#ifdef WIN32
    using slider_event = core::event_handler<detail::SLIDER_MESSAGE, 0,
                                       core::params<int>::getter<get_param<0, int> > >;
#endif //WIN32
#ifdef X11
    using slider_event = core::event_handler<ClientMessage, 0,
                                       core::params<int>::getter<get_client_data<0, int> >,
                                       0, client_message_matcher<detail::SLIDER_MESSAGE> >;
#endif // X11
       // --------------------------------------------------------------------------

    namespace detail {

      // --------------------------------------------------------------------------
      class slider_base : public window {
      public:
        typedef window super;
        typedef core::point::type type;

        slider_base ();

        void set_min (type min);
        void set_max (type min);
        void set_min_max (type mi, type ma);

        inline type get_min () const;
        inline type get_max () const;

      protected:
        type min;
        type max;

        core::point start_mouse_point;
        core::point start_window_point;
      };

      // --------------------------------------------------------------------------
      template<orientation O>
      struct slider_cursor {};

      template<>
      struct slider_cursor<orientation::horizontal> {
        static constexpr cursor_type value = cursor_type::size_v;
      };

      template<>
      struct slider_cursor<orientation::vertical> {
        static constexpr cursor_type value = cursor_type::size_h;
      };

      // --------------------------------------------------------------------------
      template<orientation O>
      class basic_slider : public slider_base {
      public:
        typedef slider_base super;
        typedef no_erase_window_class<basic_slider, slider_cursor<O>::value> clazz;

        basic_slider ();

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def);

        void set_value (core::point::type v);
        core::point::type get_value () const;
      };

      // --------------------------------------------------------------------------
      template<>
      basic_slider<orientation::vertical>::basic_slider ();

      template<>
      void basic_slider<orientation::vertical>::set_value (core::point::type v);

      template<>
      core::point::type basic_slider<orientation::vertical>::get_value () const;

      // --------------------------------------------------------------------------
      template<>
      basic_slider<orientation::horizontal>::basic_slider ();

      template<>
      void basic_slider<orientation::horizontal>::set_value (core::point::type v);

      template<>
      core::point::type basic_slider<orientation::horizontal>::get_value () const;

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation O,
             draw::frame::drawer F = draw::frame::raised_relief>
    class basic_framed_slider : public detail::basic_slider<O> {
    public:
      typedef detail::basic_slider<O> super;

      basic_framed_slider ();

    private:
      void paint (const draw::graphics& g);

    };

    // --------------------------------------------------------------------------
    using vertical_slider = basic_framed_slider<orientation::vertical>;
    using horizontal_slider = basic_framed_slider<orientation::horizontal>;

    // --------------------------------------------------------------------------
    // inlines
    namespace detail {

      // --------------------------------------------------------------------------
      inline auto slider_base::get_min() const->type {
        return min;
      }

      inline auto slider_base::get_max() const->type {
        return max;
      }

      template<orientation O>
      inline void basic_slider<O>::create (const container& parent,
                                           const core::rectangle& place) {
        super::create(clazz::get(), parent, place);
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation O,
             draw::frame::drawer F>
    inline basic_framed_slider<O, F>::basic_framed_slider () {
      super::register_event_handler(REGISTER_FUNCTION, paint_event(this, &basic_framed_slider::paint));
    }

    template<orientation O,
             draw::frame::drawer F>
    void basic_framed_slider<O, F>::paint (const draw::graphics& g) {
      core::rectangle place = super::client_area();
      g.fill(draw::rectangle(place), color::buttonColor());
      F(g, place);
      if (super::has_focus()) {
        draw::frame::dots(g, place);
      }
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
