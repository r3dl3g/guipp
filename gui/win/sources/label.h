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

namespace gui {

  namespace win {
// --------------------------------------------------------------------------
    enum Alignment {
      AlignmentLeft = IF_WIN32(SS_LEFT)IF_X11(gui::draw::vcenter_left),
      AlignmentCenter = IF_WIN32(SS_CENTER)IF_X11(gui::draw::center),
      AlignmentRight = IF_WIN32(SS_RIGHT)
      IF_X11(gui::draw::vcenter_right)
    };

// --------------------------------------------------------------------------
    namespace detail {
      class label_base : public gui::win::window_with_text {
      protected:
        static window_class register_label_class(Alignment);
        void register_handler();
      };
    }

    template<Alignment A>
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

    template<Alignment A> window_class labelT<A>::clazz;

// --------------------------------------------------------------------------
    typedef labelT<AlignmentLeft> label_left;
    typedef label_left label;
    typedef labelT<AlignmentRight> label_right;
    typedef labelT<AlignmentCenter> label_center;

  } // win

} // gui
