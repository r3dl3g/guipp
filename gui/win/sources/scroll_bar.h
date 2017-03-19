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
* @brief     C++ API: basic controls
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
#include "window.h"
#include "window_event_handler.h"


namespace gui {

  namespace win {

#ifdef WIN32
// --------------------------------------------------------------------------
    int get_scroll_pos(const core::event& e);

    struct scroll_matcher {
      bool operator() (const core::event& e);
    };

    typedef event_handlerT<WM_COMMAND,
                           one_param_caller<int, get_scroll_pos>,
                           0, scroll_matcher>                  scroll_event;
// --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
// --------------------------------------------------------------------------
    namespace detail {
      extern Atom SCROLLBAR_MESSAGE;
    }
// --------------------------------------------------------------------------
    typedef event_handlerT<ClientMessage,
                           one_param_caller<int, get_client_data<int, 0>>, 0,
                           client_message_matcher<detail::SCROLLBAR_MESSAGE>>
            scroll_event;
// --------------------------------------------------------------------------
#endif // X11

    class scroll_bar : public window {
    public:
      typedef window super;

      int get_min () const;
      int get_max () const;
      int get_step () const;
      int get_value () const;

      void set_min (int);
      void set_max (int);
      void set_min_max (int, int);
      void set_step (int);
      void set_value (int);

      static core::size::type get_scroll_bar_width ();

    protected:
      scroll_bar ();

      void create (const window_class& type,
                   const container& parent,
                   const core::rectangle& place = core::rectangle::default_rectangle);

      bool scroll_handle_event (const core::event& e, core::event_result& result);

    private:
#ifdef X11
      int min;
      int max;
      int step;
      int value;
#endif // X11

    };

    namespace detail {
      template<bool H>
      class scroll_barT : public scroll_bar {
      public:
        typedef scroll_bar super;

        scroll_barT ();

        ~scroll_barT ();

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::default_rectangle) {
          super::create(clazz, parent, place);
        }

      private:
        static window_class clazz;

#ifdef X11
        bool scroll_handle_eventT (const core::event& e,
                                   core::event_result& result);

        double get_scale () const {
          return get_scale(client_area());
        }

        double get_scale (const core::rectangle& place) const {
          return (double)(space_size(place) - thumb_size(place)) / (double)(get_max() - get_min());
        }

        core::size::type button_size (const core::rectangle& place) const;

        int space_size (const core::rectangle& place) const;

        core::point::type thumb_top (const core::rectangle& place) const {
          return core::point::type(button_size(place) + 1 + (get_value() - get_min()) * get_scale(place));
        }

        core::size::type thumb_size (const core::rectangle& place) const {
          return core::size::type(std::max(space_size(place) - (get_max() - get_min()),
                                           (int)button_size(place)));
        }

        core::rectangle up_button_place (const core::rectangle& place) const {
          core::size::type sz = button_size(place);
          return core::rectangle(place.x(), place.y(), sz, sz);
        }

        core::rectangle down_button_place (const core::rectangle& place) const;

        core::rectangle page_up_place (const core::rectangle& place) const;

        core::rectangle page_down_place (const core::rectangle& place) const;

        core::rectangle thumb_button_place (const core::rectangle& place) const;

        enum State {
          Nothing_pressed,
          Up_button_pressed,
          Down_button_pressed,
          Thumb_button_pressed,
          Page_up_pressed,
          Page_down_pressed
        };

        core::point last_mouse_point;
        State state;
        int last_position;
        core::graphics_id gc;
#endif // X11
      };

      template<bool H>
      window_class scroll_barT<H>::clazz;

      template<>
      scroll_barT<false>::scroll_barT ();

      template<>
      scroll_barT<true>::scroll_barT ();

      template<>
      scroll_barT<false>::~scroll_barT ();

      template<>
      scroll_barT<true>::~scroll_barT ();

#ifdef X11
      template<>
      bool scroll_barT<false>::scroll_handle_eventT (const core::event& e,
                                                     core::event_result& result);

      template<>
      bool scroll_barT<true>::scroll_handle_eventT (const core::event& e,
                                                    core::event_result& result);

      template<>
      core::size::type scroll_barT<false>::button_size (const core::rectangle& place) const;

      template<>
      core::size::type scroll_barT<true>::button_size (const core::rectangle& place) const;

      template<>
      int scroll_barT<false>::space_size (const core::rectangle& place) const;

      template<>
      int scroll_barT<true>::space_size (const core::rectangle& place) const;

      template<>
      core::rectangle scroll_barT<false>::down_button_place (const core::rectangle& place) const;

      template<>
      core::rectangle scroll_barT<true>::down_button_place (const core::rectangle& place) const;

      template<>
      core::rectangle scroll_barT<false>::page_up_place (const core::rectangle& place) const;

      template<>
      core::rectangle scroll_barT<true>::page_up_place (const core::rectangle& place) const;

      template<>
      core::rectangle scroll_barT<false>::page_down_place (const core::rectangle& place) const;

      template<>
      core::rectangle scroll_barT<true>::page_down_place (const core::rectangle& place) const;

      template<>
      core::rectangle scroll_barT<false>::thumb_button_place (const core::rectangle& place) const;

      template<>
      core::rectangle scroll_barT<true>::thumb_button_place (const core::rectangle& place) const;
#endif // X11
    }

    typedef detail::scroll_barT<false> vscroll_bar;
    typedef detail::scroll_barT<true> hscroll_bar;

  } // win

} // gui
