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

    namespace detail {

      // --------------------------------------------------------------------------
      class list : public owner_draw_list {
      public:
        // --------------------------------------------------------------------------
        list ();

        ~list ();

        template<typename T>
        void create (const container& parent,
                     const core::rectangle& place,
                     data<T> data,
                     int item_height = 20) {
          create(parent, place);
          set_data(data, item_height);
        }

        void set_count (size_t count);
        size_t get_count () const;

        void set_selection (int count);
        int get_selection () const;

      private:
        bool list_handle_event (const core::event& e,
                                core::event_result& result);

#ifdef X11
        core::rectangle get_scroll_bar_area ();

        scroll_barT<H> scrollbar;

        size_t item_count;
        int selection;
        bool moved;
        core::point last_mouse_point;
#endif // X11

      };
    }

    // --------------------------------------------------------------------------
    template<bool V>
    class listT : public detail::list {
    public:
      listT ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(clazz, parent, place);
      }

      template<typename T>
      void set_data (data<T> data,
                     core::size::type item_height = 20) {
        set_drawer(data, calc_item_size(item_height));
        set_count(data.size());
      }

      int get_scroll_pos () const;
      void set_scroll_pos (int pos);

      void enable_scroll_bar (bool enable);
      bool is_scroll_bar_enabled () const;
      bool is_scroll_bar_visible () const;

      core::size client_size () const;

    private:
      core::size calc_item_size (core::size::type item_height) const;

      static window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<bool V>
    window_class listT<V>::clazz;

    // --------------------------------------------------------------------------
    template<>
    listT<false>::listT ();

    template<>
    int listT<false>::get_scroll_pos () const;

    template<>
    void listT<false>::set_scroll_pos(int pos);

    template<>
    void listT<false>::enable_scroll_bar(bool enable);

    template<>
    bool listT<false>::is_scroll_bar_enabled() const;

    template<>
    bool listT<false>::is_scroll_bar_visible() const;

    template<>
    core::size listT<false>::client_size() const;

    template<>
    core::size listT<false>::calc_item_size(core::size::type item_height) const;

    // --------------------------------------------------------------------------
    template<>
    listT<true>::listT();

    template<>
    int listT<true>::get_scroll_pos() const;

    template<>
    void listT<true>::set_scroll_pos(int pos);

    template<>
    void listT<true>::enable_scroll_bar(bool enable);

    template<>
    bool listT<true>::is_scroll_bar_enabled() const;

    template<>
    bool listT<true>::is_scroll_bar_visible() const;

    template<>
    core::size listT<true>::client_size() const;

    template<>
    core::size listT<true>::calc_item_size(core::size::type item_height) const;

    // --------------------------------------------------------------------------
    typedef listT<false> hlist;
    typedef listT<true> vlist;
    typedef vlist list;

  } // win

} // gui
