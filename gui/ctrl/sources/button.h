/**
* @copyright (c) 2016-2017 Ing. Buero Rothfuss
*                          Riedlinger Str. 8
*                          70327 Stuttgart
*                          Germany
*                          http://wwrothfuss-web.de
*
* @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
*
* Project    standard lib
*
* Customer   -
*
* @brief     C++ API: button
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "control.h"
#include "window_event_handler.h"

namespace gui {

  namespace win {

#ifdef WIN32
    // --------------------------------------------------------------------------
    typedef event_handler<detail::BN_CLICKED_MESSAGE, 0,
                          Params<>::caller<>>         button_clicked_event;
    typedef event_handler<detail::BN_PUSHED_MESSAGE, 0,
                          Params<>::caller<>>         button_pushed_event;
    typedef event_handler<detail::BN_UNPUSHED_MESSAGE, 0,
                          Params<>::caller<>>         button_released_event;
    typedef event_handler<detail::BN_STATE_MESSAGE, 0,
                          Params<bool>::
                          caller<get_param<0, bool>>> button_state_event;
// --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    typedef event_handler<ClientMessage, 0,
                          Params<>::caller<>, 0,
                          client_message_matcher<detail::BN_CLICKED_MESSAGE>>
                          button_clicked_event;
    typedef event_handler<ClientMessage, 0,
                          Params<>::caller<>, 0,
                          client_message_matcher<detail::BN_PUSHED_MESSAGE>>
                          button_pushed_event;
    typedef event_handler<ClientMessage, 0,
                          Params<>::caller<>, 0,
                          client_message_matcher<detail::BN_UNPUSHED_MESSAGE>>
                          button_released_event;
    typedef event_handler<ClientMessage, 0,
                           Params<bool>::caller<get_client_data<bool, 0>>, 0,
                           client_message_matcher<detail::BN_STATE_MESSAGE>>
                           button_state_event;
    // --------------------------------------------------------------------------
#endif // X11
    // --------------------------------------------------------------------------
    class button : public client_window {
    public:
      typedef window super;

      button ();

      inline bool is_hilited () const {
        return hilited;
      }

      inline bool is_pushed () const {
        return pushed;
      }

      inline bool is_checked () const {
        return checked;
      }

      void set_hilited (bool b);
      void set_pushed (bool b);
      void set_checked (bool b);

    private:
      bool hilited;
      bool pushed;
      bool checked;
    };

    // --------------------------------------------------------------------------
    namespace paint {
      void push_button (const draw::graphics& graph, const win::button& btn, const std::string& text);
      void flat_button (const draw::graphics& graph, const win::button& btn, const std::string& text);
      void radio_button (const draw::graphics& graph, const win::button& btn, const std::string& text);
      void check_box (const draw::graphics& graph, const win::button& btn, const std::string& text);
    }

    // --------------------------------------------------------------------------
    class push_button : public button {
    public:
      typedef button super;

      push_button ();

    };

    // --------------------------------------------------------------------------
    class toggle_button : public button {
    public:
      typedef button super;

      toggle_button ();

    };

    // --------------------------------------------------------------------------
    class text_push_button : public push_button {
    public:
      typedef push_button super;

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   const std::string& txt = std::string()) {
        super::create(parent, place);
        set_text(txt);
      }

      void set_text (const std::string& t) {
        text = t;
        redraw_later();
      }

      const std::string& get_text () const {
        return text;
      }

    private:
      std::string text;
    };

    // --------------------------------------------------------------------------
    class text_button : public text_push_button {
    public:
      typedef text_push_button super;

      text_button ();

    };

    // --------------------------------------------------------------------------
    class flat_button : public text_push_button {
    public:
      typedef text_push_button super;

      flat_button ();

    };

    // --------------------------------------------------------------------------
    class text_toggle_button : public toggle_button {
    public:
      typedef toggle_button super;

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   const std::string& txt = std::string()) {
        super::create(parent, place);
        set_text(txt);
      }

      void set_text (const std::string& t) {
        text = t;
        redraw_later();
      }

      const std::string& get_text () const {
        return text;
      }

    private:
      std::string text;
    };

    // --------------------------------------------------------------------------
    class radio_button : public text_toggle_button {
    public:
      typedef text_toggle_button super;

      radio_button ();

    };

    // --------------------------------------------------------------------------
    class check_box : public text_toggle_button {
    public:
      typedef text_toggle_button super;

      check_box ();

    };

    // --------------------------------------------------------------------------
    template<class super>
    class custom_button : public super {
    public:
      typedef void (button_drawer)(const draw::graphics&, const custom_button&);

      custom_button () {
        super::register_event_handler(win::paint_event([&] (const draw::graphics& graph) {
          if (drawer) {
            drawer(graph, *this);
          }
        }));
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
      }

      void set_drawer (std::function<button_drawer> d) {
        drawer = d;
      }

    private:
      std::function<button_drawer> drawer;
    };

    typedef custom_button<push_button> custom_push_button;
    typedef custom_button<toggle_button> custom_toggle_button;

  } // win

} // gui
