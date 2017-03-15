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
#include "window_event_handler.h"
#include "owner_draw.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
#ifdef WIN32
    typedef event_handlerT<WM_COMMAND, no_param_caller, 0,
                           command_matcher<LBN_SELCHANGE>>      selection_changed_event;
#endif //WIN32
#ifdef X11
    extern Atom SELECTION_CHANGE_MESSAGE;

    struct selection_changed_message_match {
      bool operator() (const core::event& e);
    };

    typedef event_handlerT<ClientMessage, no_param_caller, 0,
      selection_changed_message_match> selection_changed_event;
#endif // X11

    // --------------------------------------------------------------------------
    class list : public owner_draw {
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

        void update_list (list& l) {
          l.set_count(super::size());
        }

        void operator() (draw::graphics& g,
                         int idx,
                         const core::rectangle& place,
                         bool selected) {
          draw_text_item(g, ostreamfmt(super::at(idx)), place, selected);
        }

      };
      // --------------------------------------------------------------------------

      list ();

      ~list ();

      void create (const window& parent,
                   const core::rectangle& place = core::rectangle::default_rectangle) {
        super::create(clazz, parent, place);
      }

      template<typename T>
      void create (const window& parent,
                   const core::rectangle& place,
                   data<T> data,
                   int item_height = 20) {
        create(parent, place);
        set_data(data, item_height);
      }

      void set_drawer (std::function<item_draw> drawer,
                       int item_height = 20);

      template<typename T>
      void set_data (data<T> data,
                     int item_height = 20) {
        set_drawer(data, item_height);
        set_count(data.size());
      }

      void set_count (size_t count);

      size_t get_count () const;

      void set_selection (int count);

      int get_selection () const;

      void set_scroll_pos (int pos);

      int get_scroll_pos () const;

      static void draw_text_item (draw::graphics&,
                                  const std::string& text,
                                  const core::rectangle& place,
                                  bool selected);

    private:
      bool list_handle_event (const core::event& e,
                              core::event_result& result);

      void draw_item (draw::graphics&,
                      int idx,
                      const core::rectangle& place,
                      bool selected);

      std::function<item_draw> drawer;

#ifdef X11
      size_t item_count;
      int selection;
      int offset;
      bool moved;
      core::point last_mouse_point;
      core::graphics_id gc;
#endif // X11

      static window_class clazz;
    };

    template<>
    inline void list::data<std::string>::operator() (draw::graphics& g,
                                                     int idx,
                                                     const core::rectangle& place,
                                                     bool selected) {
      draw_text_item(g, at(idx), place, selected);
    }

  } // win

} // gui
