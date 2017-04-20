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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "control.h"
#include "alignment_h.h"

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace detail {
      class label_base : public gui::win::window_with_text {
      protected:
        static no_erase_window_class clazz;
      };

    }

    namespace paint {

      void label (const draw::graphics& graph,
                  const win::window& win,
                  const std::string& text,
                  os::color foreground,
                  os::color background,
                  draw::text_origin origin);

    }

    template<alignment_h A,
             void(F)(const draw::graphics&, const core::rectangle&) = draw::frame::no_frame,
             os::color FC = color::black,
             os::color BC = color::very_light_gray>
    class labelT : public detail::label_base {
    public:
      typedef detail::label_base super;

      labelT () {
        register_event_handler(paint_event([&] (const draw::graphics& graph) {
          gui::core::rectangle place = client_area();
          paint::label(graph, *this, get_text(), FC, BC, (draw::text_origin)A);
          F(graph, place);
        }));
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
        set_text(txt);
      }
    };

    // --------------------------------------------------------------------------
    using label_left = labelT<alignment_left, draw::frame::no_frame, color::black, color::very_light_gray>;

    using label = label_left;

    using label_right = labelT<alignment_right, draw::frame::no_frame, color::black, color::very_light_gray>;

    using label_center = labelT<alignment_center, draw::frame::no_frame, color::black, color::very_light_gray>;

  } // win

} // gui
