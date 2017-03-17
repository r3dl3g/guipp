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
* @brief     C++ API: list
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
#include "owner_draw.h"
#include "scroll_bar.h"


namespace gui {

  namespace win {

#ifdef X11
    namespace detail {
      extern Atom SELECTION_CHANGE_MESSAGE;

      struct selection_changed_message_match {
        bool operator() (const core::event& e);
      };
    }

    typedef event_handlerT<ClientMessage, no_param_caller, 0,
      detail::selection_changed_message_match>     selection_changed_event;
#endif // X11


    class owner_draw_list : public owner_draw {
    public:
      typedef owner_draw super;

      typedef void(item_draw) (draw::graphics&,
                               int idx,
                               const core::rectangle& place,
                               bool selected);

      // --------------------------------------------------------------------------
      // static data for list.
      // --------------------------------------------------------------------------
      template<typename T>
      struct data : public std::vector<T> {
        typedef std::vector<T> super;

        typedef typename super::iterator iterator;

        data () {}

        data (std::initializer_list<T> args)
          : super(args) {}

        data (iterator b,
              iterator e)
          : super(b, e) {}

        template<size_t N>
        data (const T (& t)[N])
          : super(t, t + N) {}

        template<typename L>
        void update_list (L& l) {
          l.set_count(super::size());
        }

        void operator() (draw::graphics& g,
                         int idx,
                         const core::rectangle& place,
                         bool selected) {
          draw_text_item(g, ostreamfmt(super::at(idx)), place, selected);
        }

      };

      owner_draw_list ();
      ~owner_draw_list ();

      void set_drawer (std::function<item_draw> drawer,
                       int item_height = 20);

      void enable_vscroll_bar (bool enable);

      bool is_vscroll_bar_enabled () const;

    protected:
      void draw_item (draw::graphics&,
                      int idx,
                      const core::rectangle& place,
                      bool selected);

#ifdef X11
      vscroll_bar scrollbar;

      core::graphics_id get_graphics (const core::event& e);
#endif // X11

    private:
      std::function<item_draw> drawer;

#ifdef X11
      core::graphics_id gc;
#endif // X11
    };

  }
}

