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
    typedef event_handler<WM_COMMAND,
                          Params<>::caller<>, 0,
                          command_matcher<LBN_SELCHANGE>>      selection_changed_event;
#endif //WIN32

    namespace detail {

      // --------------------------------------------------------------------------
      class list : public owner_draw_list {
      public:
        // --------------------------------------------------------------------------
        list ();

        ~list ();

        size_t get_count () const;
        int get_selection () const;

      private:
        bool list_handle_event (const core::event& e,
                                os::event_result& result);

#ifdef X11
      protected:
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
                   const core::rectangle& place = core::rectangle::def);

      template<typename T,
               void(F)(const T&, draw::graphics&, const core::rectangle&, const draw::brush&, bool) = list_item_drawer<T>>
      void create (const container& parent,
                   const core::rectangle& place,
                   simple_list_data<T, F> data,
                   int item_height = 20) {
        set_data(data, item_height);
        create(parent, place);
      }

      template<typename T,
               void(F)(const T&, draw::graphics&, const core::rectangle&, const draw::brush&, bool) = list_item_drawer<T>>
      void set_data (simple_list_data<T, F> data,
                     core::size::type item_height = 20) {
        set_drawer(data, calc_item_size(item_height));
        set_count(data.size());
      }

      void set_count (size_t count);
      void set_selection (int count);

      core::point::type get_scroll_pos() const;
      void set_scroll_pos(core::point::type pos);

      void enable_scroll_bar (bool enable);
      bool is_scroll_bar_enabled () const;
      bool is_scroll_bar_visible () const;

      core::size client_size () const;

    private:
#ifdef X11
      bool listT_handle_event (const core::event& e,
                               os::event_result& result);

      core::rectangle get_scroll_bar_area ();
      void adjust_scroll_bar ();

      detail::scroll_barT<!V> scrollbar;
#endif // X11

      core::size calc_item_size (core::size::type item_height) const;

      static window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<>
    listT<false>::listT ();

    template<>
    void listT<false>::create (const container& parent,
                               const core::rectangle& place);

    template<>
    void listT<false>::set_count (size_t count);

    template<>
    void listT<false>::set_selection (int count);

    template<>
    core::point::type listT<false>::get_scroll_pos() const;

    template<>
    void listT<false>::set_scroll_pos(core::point::type pos);

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
    void listT<true>::create (const container& parent,
                              const core::rectangle& place);

    template<>
    void listT<true>::set_count (size_t count);

    template<>
    void listT<true>::set_selection (int count);

    template<>
    core::point::type listT<true>::get_scroll_pos() const;

    template<>
    void listT<true>::set_scroll_pos(core::point::type pos);

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

#ifdef X11
    template<>
    core::rectangle listT<false>::get_scroll_bar_area ();

    template<>
    core::rectangle listT<true>::get_scroll_bar_area ();

    template<>
    void listT<false>::adjust_scroll_bar ();

    template<>
    void listT<true>::adjust_scroll_bar ();

    template<>
    bool listT<false>::listT_handle_event (const core::event& e,
                                           os::event_result& result);

    template<>
    bool listT<true>::listT_handle_event (const core::event& e,
                                          os::event_result& result);
#endif // X11

    // --------------------------------------------------------------------------
    typedef listT<false> hlist;
    typedef listT<true> vlist;
    typedef vlist list;

  } // win

} // gui
