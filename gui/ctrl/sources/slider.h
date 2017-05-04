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

#ifdef X11
    // --------------------------------------------------------------------------
    typedef event_handler<ClientMessage, 0,
                           Params<int>::caller<get_client_data<0, int>>, 0,
                           client_message_matcher<detail::SLIDER_MESSAGE>>
            slider_event;
    // --------------------------------------------------------------------------
#endif // X11
#ifdef WIN32
    typedef event_handler<detail::SLIDER_MESSAGE, 0,
                          Params<int>::
                          caller<get_param<0, int>>>
            slider_event;
    // --------------------------------------------------------------------------
#endif //WIN32

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

        inline type get_min () const {
          return min;
        }

        inline type get_max () const {
          return max;
        }

      protected:
        type min;
        type max;

        core::point last_mouse_point;
      };

      // --------------------------------------------------------------------------
      template<orientation H>
      class slider_class : public no_erase_window_class {
      public:
        slider_class();
      };

      // --------------------------------------------------------------------------
      template<orientation H>
      class slider_t : public slider {
      public:
        typedef slider super;

        slider_t ();

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def) {
          super::create(clazz, parent, place);
        }

      private:
        static slider_class<H> clazz;
      };

      template<orientation H>
      slider_class<H> slider_t<H>::clazz;

      template<>
      slider_t<orientation::vertical>::slider_t ();

      template<>
      slider_t<orientation::horizontal>::slider_t ();

    }

    // --------------------------------------------------------------------------
    template<orientation H,
             void(F)(const draw::graphics&, const core::rectangle&)>
    class framed_slider_t : public detail::slider_t<H> {
    public:
      typedef detail::slider_t<H> super;

      framed_slider_t () {
        super::register_event_handler(win::paint_event([&](const draw::graphics& g) {
          core::rectangle place = super::client_area();
          g.fill(draw::rectangle(place), color::buttonColor());
          F(g, place);
        }));
      }
    };

    // --------------------------------------------------------------------------
    typedef framed_slider_t<orientation::vertical, draw::frame::raised_relief> vslider;
    typedef framed_slider_t<orientation::horizontal, draw::frame::raised_relief> hslider;

  } // win

} // gui
