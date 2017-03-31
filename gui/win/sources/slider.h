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
#include "window.h"
#include "window_event_handler.h"


namespace gui {

  namespace win {

    class slider : public window {
    public:
      typedef window super;

      slider ();

      void set_min (int min);
      void set_max (int min);
      void set_min_max (int mi, int ma);

      inline int get_min () const {
        return min;
      }

      inline int get_max () const {
        return max;
      }

    protected:
      int min;
      int max;

      core::point last_mouse_point;
    };

    namespace detail {

      template<bool H>
      class sliderT : public slider {
      public:
        typedef slider super;

        sliderT ();

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def) {
          super::create(clazz, parent, place);
        }

      private:
        static window_class clazz;
      };

      template<bool H>
      window_class sliderT<H>::clazz;

      template<>
      sliderT<false>::sliderT ();

      template<>
      sliderT<true>::sliderT ();
    }

    typedef detail::sliderT<false> vslider;
    typedef detail::sliderT<true> hslider;

  } // win

} // gui