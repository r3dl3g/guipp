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
* @brief     C++ API: separator view
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
    namespace detail {
      class separator_base : public window {
      public:
        typedef window super;

        separator_base ();

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def) {
          super::create(clazz, parent, place);
        }

      protected:
        static no_erase_window_class clazz;
      };

    }

    // --------------------------------------------------------------------------
    template<orientation Horizontal, bool Sunken = true, os::color B = color::light_gray>
    class separator_t : public detail::separator_base {
    public:
      separator_t ();
    };

    // --------------------------------------------------------------------------
    template<os::color B>
    class separator_t<orientation::vertical, false, B> : public detail::separator_base {
    public:
      separator_t () {
        register_event_handler(REGISTER_FUNCTION, paint_event([&] (const draw::graphics& graph) {
          core::rectangle r = client_area() - core::size(1, 0);
          graph.frame(draw::line(r.top_left(), r.bottom_left()), color::lighter(B));
          graph.frame(draw::line(r.top_right(), r.bottom_right()), color::darker(B));
        }));
      }
    };

    // --------------------------------------------------------------------------
    template<os::color B>
    class separator_t<orientation::vertical, true, B> : public detail::separator_base {
    public:
      separator_t () {
        register_event_handler(REGISTER_FUNCTION, paint_event([&] (const draw::graphics& graph) {
          core::rectangle r = client_area() - core::size(1, 0);
          graph.frame(draw::line(r.top_left(), r.bottom_left()), color::darker(B));
          graph.frame(draw::line(r.top_right(), r.bottom_right()), color::lighter(B));
        }));
      }
    };

    // --------------------------------------------------------------------------
    template<os::color B>
    class separator_t<orientation::horizontal, false, B> : public detail::separator_base {
    public:
      separator_t () {
        register_event_handler(REGISTER_FUNCTION, paint_event([&] (const draw::graphics& graph) {
          core::rectangle r = client_area() - core::size(0, 1);
          graph.frame(draw::line(r.top_left(), r.top_right()), color::lighter(B));
          graph.frame(draw::line(r.bottom_right(), r.bottom_left()), color::darker(B));
        }));
      }
    };

    // --------------------------------------------------------------------------
    template<os::color B>
    class separator_t<orientation::horizontal, true, B> : public detail::separator_base {
    public:
      separator_t () {
        register_event_handler(REGISTER_FUNCTION, paint_event([&] (const draw::graphics& graph) {
          core::rectangle r = client_area() - core::size(0, 1);
          graph.frame(draw::line(r.top_left(), r.top_right()), color::darker(B));
          graph.frame(draw::line(r.bottom_right(), r.bottom_left()), color::lighter(B));
        }));
      }
    };

    // --------------------------------------------------------------------------
    using hseparator = separator_t<orientation::horizontal>;
    using vseparator = separator_t<orientation::vertical>;

  } // win

} // gui
