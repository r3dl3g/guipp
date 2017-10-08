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
#include "control.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
#ifdef WIN32
    using slider_event = event_handler<detail::SLIDER_MESSAGE, 0,
                                       params<int>::getter<get_param<0, int>>>;
#endif //WIN32
#ifdef X11
    using slider_event = event_handler<ClientMessage, 0,
                                       params<int>::getter<get_client_data<0, int>>,
                                       0, client_message_matcher<detail::SLIDER_MESSAGE>>;
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
      class slider_class : public no_erase_window_class {
      public:
        slider_class();
      };

      // --------------------------------------------------------------------------
      template<orientation O>
      class basic_slider : public slider_base {
      public:
        typedef slider_base super;

        basic_slider ();

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def);

      private:
        static slider_class<O> clazz;
      };

      // --------------------------------------------------------------------------
      template<orientation O>
      slider_class<O> basic_slider<O>::clazz;

      template<>
      basic_slider<orientation::vertical>::basic_slider ();

      template<>
      basic_slider<orientation::horizontal>::basic_slider ();

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
      inline auto slider_base::get_min () const -> type {
        return min;
      }

      inline auto slider_base::get_max () const -> type {
        return max;
      }

      template<orientation O>
      inline void basic_slider<O>::create (const container& parent,
                                       const core::rectangle& place) {
        super::create(clazz, parent, place);
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
      if (super::has_focus()) {
        draw::frame::black(g, place);
        place.shrink({1, 1});
      }
      g.fill(draw::rectangle(place), color::buttonColor());
      F(g, place);
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
