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
    namespace detail {
      extern Atom SLIDER_MESSAGE;
    }

    // --------------------------------------------------------------------------
    typedef event_handler<ClientMessage,
                           Params<int>::caller<get_client_data<int, 0>>, 0,
                           client_message_matcher<detail::SLIDER_MESSAGE>>
            slider_event;
    // --------------------------------------------------------------------------
#endif // X11
#ifdef WIN32
    namespace detail {
      const os::event_id SLIDER_MESSAGE = WM_USER + 1;
    }

    typedef event_handler<detail::SLIDER_MESSAGE,
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
      template<bool H>
      class slider_class : public window_class {
      public:
        slider_class();

        virtual void prepare (window*) const;
      };

      // --------------------------------------------------------------------------
      template<bool H>
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

      template<bool H>
      slider_class<H> slider_t<H>::clazz;

      template<>
      slider_t<false>::slider_t ();

      template<>
      slider_t<true>::slider_t ();

    }

    // --------------------------------------------------------------------------
    template<bool H,
             void(F)(draw::graphics&, const core::rectangle&)>
    class framed_slider_t : public detail::slider_t<H> {
    public:
      typedef detail::slider_t<H> super;

      framed_slider_t () {
        super::register_event_handler(win::paint_event([&](draw::graphics& g) {
          core::rectangle place = super::client_area();
#ifdef X11
          g.fill(draw::rectangle(place), draw::color::buttonColor());
#endif // X11
          F(g, place);
        }));
      }
    };

    // --------------------------------------------------------------------------
    typedef framed_slider_t<false, draw::frame::raised_relief> vslider;
    typedef framed_slider_t<true, draw::frame::raised_relief> hslider;

  } // win

} // gui
