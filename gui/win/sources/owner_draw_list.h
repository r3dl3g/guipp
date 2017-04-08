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
#include<ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "owner_draw.h"
#include "scroll_bar.h"


namespace gui {

  namespace win {

    template<typename T>
    inline std::string convert_to_string(const T& t) {
      return ostreamfmt(t);
    }

    template<>
    inline std::string convert_to_string<std::string>(const std::string& t) {
      return t;
    }

    template<typename T,
             draw::text_origin O = draw::vcenter_left,
             void(F)(draw::graphics&, const core::rectangle&) = draw::frame::no_frame>
    void list_item_drawer (const T& t,
                           draw::graphics& g,
                           const core::rectangle& place,
                           const draw::brush& background,
                           bool selected) {
      owner_draw::draw_text_item(convert_to_string<T>(t), g, place, background, selected, O);
      if (!selected) {
        F(g, place);
      }
    }

    // static data for list.
    // --------------------------------------------------------------------------
    template<typename T,
             void(F)(const T&, draw::graphics&, const core::rectangle&, const draw::brush&, bool) = list_item_drawer<T>>
    struct simple_list_data : public std::vector<T> {
      typedef std::vector<T> super;

      typedef typename super::iterator iterator;

      simple_list_data ()
      {}

      simple_list_data (std::initializer_list<T> args)
        : super(args)
      {}

      simple_list_data (iterator b, iterator e)
        : super(b, e)
      {}

      template<size_t N>
      simple_list_data (const T (& t)[N])
        : super(t, t + N)
      {}

      template<typename L>
      void update_list (L& l) {
        l.set_count(super::size());
      }

      void operator() (int idx,
                       draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       bool selected) {
        F(super::at(idx), g, place, background, selected);
      }

    };


#ifdef X11
    namespace detail {
      extern Atom SELECTION_CHANGE_MESSAGE;
    }

    typedef event_handlerT<ClientMessage, no_param_caller, 0,
                           client_message_matcher<detail::SELECTION_CHANGE_MESSAGE>>
            selection_changed_event;
#endif // X11


    class owner_draw_list : public owner_draw {
    public:
      typedef owner_draw super;

      typedef void(item_draw) (int idx,
                               draw::graphics&,
                               const core::rectangle& place,
                               const draw::brush& background,
                               bool selected);

      owner_draw_list ();
      ~owner_draw_list ();

      void set_drawer (std::function<item_draw> drawer,
                       const core::size& sz = {20, 20});

    protected:
      void draw_item (int idx,
                      draw::graphics&,
                      const core::rectangle& place,
                      bool selected);

#ifdef X11
      os::graphics get_graphics (const core::event& e);
#endif // X11

    private:
      std::function<item_draw> drawer;

#ifdef X11
      os::graphics gc;
#endif // X11
    };

  }
}

