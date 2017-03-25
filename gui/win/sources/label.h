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
* @brief     C++ API: label
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include<ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window.h"
#include "window_event_handler.h"
#include "alignment_h.h"

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace detail {
      class label_base : public gui::win::window_with_text {
      protected:
        static window_class register_label_class(gui::win::alignment_h);
        void register_handler();
      };
    }

    template<alignment_h A>
    class labelT : public detail::label_base {
    public:
      typedef detail::label_base super;

      labelT () {
        if (!clazz.is_valid()) {
          clazz = register_label_class(A);
        }
        register_handler();
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
        set_text(txt);
      }

    private:
      static window_class clazz;
    };

    template<alignment_h A> window_class labelT<A>::clazz;

    // --------------------------------------------------------------------------
    typedef labelT<alignment_left> label_left;
    typedef label_left label;
    typedef labelT<alignment_right> label_right;
    typedef labelT<alignment_center> label_center;

  } // win

} // gui
