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
        typedef no_erase_window_class<separator_base> clazz;

        separator_base ();

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def);

      };

      // --------------------------------------------------------------------------
      template<orientation Horizontal>
      struct line_traits {
        static draw::line first (const core::rectangle&);
        static draw::line second (const core::rectangle&);
        static core::rectangle area (const core::rectangle&);
      };

      // --------------------------------------------------------------------------
      template<bool Sunken>
      struct color_traits {
        static os::color first (os::color);
        static os::color second (os::color);
      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation O, bool Sunken = true, os::color background = color::light_gray>
    class basic_separator : public detail::separator_base {
    public:
      typedef detail::line_traits<O> lt;
      typedef detail::color_traits<Sunken> ct;

      basic_separator ();

    private:
      void paint (const draw::graphics& graph);

    };

    // --------------------------------------------------------------------------
    using horizontal_separator = basic_separator<orientation::horizontal>;
    using vertical_separator = basic_separator<orientation::vertical>;

    // --------------------------------------------------------------------------
    // inlines
    namespace detail {

      // --------------------------------------------------------------------------
      template<>
      inline draw::line line_traits<orientation::vertical>::first (const core::rectangle& r) {
        return draw::line(r.top_left(), r.bottom_left());
      }

      template<>
      inline draw::line line_traits<orientation::vertical>::second (const core::rectangle& r) {
        return draw::line(r.top_right(), r.bottom_right());
      }

      template<>
      inline core::rectangle line_traits<orientation::vertical>::area (const core::rectangle& r) {
        return r - core::size(1, 0);
      }

      // --------------------------------------------------------------------------
      template<>
      inline draw::line line_traits<orientation::horizontal>::first (const core::rectangle& r) {
        return draw::line(r.top_left(), r.top_right());
      }

      template<>
      inline draw::line line_traits<orientation::horizontal>::second (const core::rectangle& r) {
        return draw::line(r.bottom_right(), r.bottom_left());
      }

      template<>
      inline core::rectangle line_traits<orientation::horizontal>::area (const core::rectangle& r) {
        return r - core::size(0, 1);
      }

      // --------------------------------------------------------------------------
      template<>
      inline os::color color_traits<false>::first (os::color c) {
        return color::lighter(c);
      }

      template<>
      inline os::color color_traits<false>::second (os::color c) {
        return color::darker(c);
      }

      // --------------------------------------------------------------------------
      template<>
      inline os::color color_traits<true>::first (os::color c) {
        return color::darker(c);
      }

      template<>
      inline os::color color_traits<true>::second (os::color c) {
        return color::lighter(c);
      }

    } // namespace detail

    template<orientation O, bool S, os::color B>
    inline basic_separator<O, S, B>::basic_separator () {
      register_event_handler(REGISTER_FUNCTION, paint_event(this, &basic_separator::paint));
    }

    template<orientation O, bool S, os::color B>
    inline void basic_separator<O, S, B>::paint (const draw::graphics& graph) {
      core::rectangle r = lt::area(client_area());
      graph.frame(lt::first(r), ct::first(B));
      graph.frame(lt::second(r), ct::second(B));
    }

  } // win

} // gui
