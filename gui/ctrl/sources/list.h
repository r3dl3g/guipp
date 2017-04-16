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
#include "scroll_bar.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
#ifdef WIN32
    typedef event_handler<detail::SELECTION_CHANGE_MESSAGE, 0,
                          Params<>::caller<>>
            selection_changed_event;
    // --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
    typedef event_handler<ClientMessage, 0,
                          Params<>::caller<>, 0,
                          client_message_matcher<detail::SELECTION_CHANGE_MESSAGE>>
            selection_changed_event;
#endif // X11

    namespace detail {

      // --------------------------------------------------------------------------
      class list : public window {
      public:
        typedef window super;

        // --------------------------------------------------------------------------
        list ();

        ~list ();

        size_t get_count () const;
        int get_selection () const;

        inline const gui::core::size& get_item_size () const {
          return item_size;
        }

        inline gui::core::size::type get_item_width () const {
          return get_item_size().width();
        }

        inline gui::core::size::type get_item_height () const {
          return get_item_size().height();
        }

        typedef void(draw_list_item) (int idx,
                                      const draw::graphics&,
                                      const core::rectangle& place,
                                      const draw::brush& background,
                                      bool selected);

        void set_drawer (std::function<draw_list_item> drawer,
                         const core::size& sz = {20, 20});

      protected:
        void draw_item (int idx,
                        const draw::graphics&,
                        const core::rectangle& place,
                        bool selected) const;

        size_t item_count;
        gui::core::size item_size;

        int selection;
        bool moved;
        core::point last_mouse_point;

      private:
        std::function<draw_list_item> drawer;

      };
    }

    template<typename T,
             draw::text_origin O = draw::vcenter_left,
             void(F)(const draw::graphics&, const core::rectangle&) = draw::frame::no_frame>
    void list_item_drawer (const T& t,
                           const draw::graphics& g,
                           const core::rectangle& place,
                           const draw::brush& background,
                           bool selected) {
      paint::text_item(convert_to_string<T>(t), g, place, background, selected, O);
      if (!selected) {
        F(g, place);
      }
    }

    // static data for list.
    // --------------------------------------------------------------------------
    template<typename T,
             void(F)(const T&, const draw::graphics&, const core::rectangle&, const draw::brush&, bool) = list_item_drawer<T>>
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
                       const draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       bool selected) {
        F(super::at(idx), g, place, background, selected);
      }

    };

    // --------------------------------------------------------------------------
    template<bool V>
    class listT : public detail::list {
    public:
      listT ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      template<typename T,
               void(F)(const T&, const draw::graphics&, const core::rectangle&, const draw::brush&, bool) = list_item_drawer<T>>
      void create (const container& parent,
                   const core::rectangle& place,
                   simple_list_data<T, F> data,
                   int item_height = 20) {
        set_data(data, item_height);
        create(parent, place);
      }

      template<typename T,
               void(F)(const T&, const draw::graphics&, const core::rectangle&, const draw::brush&, bool) = list_item_drawer<T>>
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

      void paint (const draw::graphics& graph);

    private:
      core::rectangle get_scroll_bar_area ();
      void adjust_scroll_bar ();

      detail::scroll_barT<!V> scrollbar;

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

    template<>
    void listT<false>::paint (const draw::graphics& graph);

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

    template<>
    void listT<true>::paint (const draw::graphics& graph);

    // --------------------------------------------------------------------------
    template<>
    core::rectangle listT<false>::get_scroll_bar_area ();

    template<>
    core::rectangle listT<true>::get_scroll_bar_area ();

    template<>
    void listT<false>::adjust_scroll_bar ();

    template<>
    void listT<true>::adjust_scroll_bar ();

    // --------------------------------------------------------------------------
    typedef listT<false> hlist;
    typedef listT<true> vlist;
    typedef vlist list;

  } // win

} // gui
