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
#include "owner_draw_list.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
#ifdef WIN32
    typedef event_handlerT<WM_COMMAND, no_param_caller, 0,
                           command_matcher<LBN_SELCHANGE>>      selection_changed_event;
#endif //WIN32

    // --------------------------------------------------------------------------
    class list : public owner_draw_list {
    public:
      // --------------------------------------------------------------------------
      list ();

      ~list ();

      void create (const window& parent,
                   const core::rectangle& place = core::rectangle::default_rectangle) {
        super::create(clazz, parent, place);
#ifdef X11
        scrollbar.create(*this, get_scroll_area());
#endif // X11
      }

      template<typename T>
      void create (const window& parent,
                   const core::rectangle& place,
                   data<T> data,
                   int item_height = 20) {
        create(parent, place);
        set_data(data, item_height);
      }

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

      void enable_vscroll_bar(bool enable);
      bool is_vscroll_bar_enabled() const;

    private:
      bool list_handle_event (const core::event& e,
                              core::event_result& result);

#ifdef X11
      core::rectangle get_scroll_area();

      vscroll_bar scrollbar;

      size_t item_count;
      int selection;
      bool moved;
      core::point last_mouse_point;
#endif // X11

      static window_class clazz;
    };

  } // win

} // gui
