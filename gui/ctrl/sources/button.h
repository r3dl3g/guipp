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
    class button : public window {
    public:
      typedef window super;

      button ();

      bool is_checked () const;
      void set_checked (bool b);

      bool is_hilited () const;
      void set_hilited (bool b);

    private:
      bool checked;
      bool hilited;
    };

    // --------------------------------------------------------------------------
    namespace paint {
      void push_button (const draw::graphics& graph, const win::window& btn, bool is_checked, bool is_hilited, const std::string& text);
      void flat_button (const draw::graphics& graph, const win::window& btn, bool is_checked, bool is_hilited, const std::string& text);
      void radio_button (const draw::graphics& graph, const win::window& btn, bool is_checked, bool is_hilited, const std::string& text);
      void check_box (const draw::graphics& graph, const win::window& btn, bool is_checked, bool is_hilited, const std::string& text);
    }

    // --------------------------------------------------------------------------
    class push_button : public button {
    public:
      typedef button super;

      push_button () {
        register_event_handler(left_btn_down_event([&](const core::point&) {
          if (is_enabled()) {
            take_focus();
            send_client_message(this, detail::BN_PUSHED_MESSAGE);
            set_checked(true);
          }
        }));
        register_event_handler(left_btn_up_event([&](const core::point& pos) {
          if (is_enabled()) {
            send_client_message(this, detail::BN_UNPUSHED_MESSAGE);
            set_checked(false);
            if (client_area().is_inside(pos)) {
              send_client_message(this, detail::BN_CLICKED_MESSAGE);
            }
          }
        }));
      }
    };

    // --------------------------------------------------------------------------
    class toggle_button : public button {
    public:
      typedef button super;

      toggle_button () {
        register_event_handler(left_btn_down_event([&](const core::point&) {
          if (is_enabled()) {
            take_focus();
            send_client_message(this, detail::BN_PUSHED_MESSAGE);
            set_hilited(true);
          }
        }));
        register_event_handler(left_btn_up_event([&](const core::point& pos) {
          if (is_enabled()) {
            send_client_message(this, detail::BN_UNPUSHED_MESSAGE);
            if (is_hilited()) {
              set_hilited(false);
              if (client_area().is_inside(pos)) {
                set_checked(!is_checked());
                if (is_enabled()) {
                  send_client_message(this, detail::BN_CLICKED_MESSAGE);
                }
              }
            }
          }
        }));
      }
    };

    // --------------------------------------------------------------------------
    class text_button : public push_button {
    public:
      typedef push_button super;

      text_button ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
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

      static window_class clazz;
    };

    // --------------------------------------------------------------------------
    class radio_button : public toggle_button {
    public:
      typedef toggle_button super;

      radio_button ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
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

      static window_class clazz;
    };

    // --------------------------------------------------------------------------
    class check_box : public toggle_button {
    public:
      typedef toggle_button super;

      check_box ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def,
                   const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
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

      static window_class clazz;
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
        super::create(clazz, parent, place);
      }

      void set_drawer (std::function<button_drawer> d) {
        drawer = d;
      }

    private:
      std::function<button_drawer> drawer;

      static window_class clazz;
    };

    template<class super>
    window_class custom_button<super>::clazz(
    #ifdef WIN32
      window_class::custom_class("CUSTOM_BUTTON++", 0,
                                 WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP,
                                 WS_EX_NOPARENTNOTIFY)
    #else // !WIN32
      window_class::custom_class("CUSTOM_BUTTON++",
                                 draw::color::buttonColor())
    #endif // !WIN32
    );


    typedef custom_button<push_button> custom_push_button;
    typedef custom_button<toggle_button> custom_toggle_button;

  } // win

} // gui
