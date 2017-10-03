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
      class slider : public window {
      public:
        typedef window super;
        typedef core::point::type type;

        slider ();

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
      class slider_t : public slider {
      public:
        typedef slider super;

        slider_t ();

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def);

      private:
        static slider_class<O> clazz;
      };

      // --------------------------------------------------------------------------
      inline auto slider::get_min () const -> type {
        return min;
      }

      inline auto slider::get_max () const -> type {
        return max;
      }

      // --------------------------------------------------------------------------
      template<orientation O>
      inline void slider_t<O>::create (const container& parent,
                                       const core::rectangle& place) {
        super::create(clazz, parent, place);
      }

      template<orientation O>
      slider_class<O> slider_t<O>::clazz;

      template<>
      slider_t<orientation::vertical>::slider_t ();

      template<>
      slider_t<orientation::horizontal>::slider_t ();

    }

    // --------------------------------------------------------------------------
    template<orientation O,
             draw::frame::drawer F = draw::frame::raised_relief>
    class framed_slider_t : public detail::slider_t<O> {
    public:
      typedef detail::slider_t<O> super;

      framed_slider_t ();

    };

    // --------------------------------------------------------------------------
    template<orientation O,
             draw::frame::drawer F>
    framed_slider_t<O, F>::framed_slider_t () {
      super::register_event_handler(REGISTER_FUNCTION, paint_event([&](const draw::graphics& g) {
        core::rectangle place = super::client_area();
        if (super::has_focus()) {
          draw::frame::black(g, place);
          place.shrink({1, 1});
        }
        g.fill(draw::rectangle(place), color::buttonColor());
        F(g, place);
      }));
    }

    // --------------------------------------------------------------------------
    using vslider = framed_slider_t<orientation::vertical>;
    using hslider = framed_slider_t<orientation::horizontal>;

    // --------------------------------------------------------------------------
  } // win

} // gui
