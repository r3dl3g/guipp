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
* @brief     C++ API: button
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
    typedef event_handlerT<WM_COMMAND, no_param_caller, 0,
                           command_matcher<BN_CLICKED>>
                           button_clicked_event;
    typedef event_handlerT<WM_COMMAND, no_param_caller, 0,
                           command_matcher<BN_PUSHED>>
                           button_pushed_event;
    typedef event_handlerT<WM_COMMAND, no_param_caller, 0,
                           command_matcher<BN_UNPUSHED>>
                           button_released_event;
    typedef event_handlerT<BM_SETCHECK,
                           one_param_caller<bool>>
                           button_state_event;
// --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
// --------------------------------------------------------------------------
    namespace detail {
      extern Atom BN_CLICKED_MESSAGE;
      extern Atom BN_PUSHED_MESSAGE;
      extern Atom BN_UNPUSHED_MESSAGE;
      extern Atom BN_STATE_MESSAGE;
    }

// --------------------------------------------------------------------------
    typedef event_handlerT<ClientMessage, no_param_caller, 0,
                           client_message_matcher<detail::BN_CLICKED_MESSAGE>>
                           button_clicked_event;
    typedef event_handlerT<ClientMessage, no_param_caller, 0,
                           client_message_matcher<detail::BN_PUSHED_MESSAGE>>
                           button_pushed_event;
    typedef event_handlerT<ClientMessage, no_param_caller, 0,
                           client_message_matcher<detail::BN_UNPUSHED_MESSAGE>>
                           button_released_event;
    typedef event_handlerT<ClientMessage,
                           one_param_caller<bool, get_client_data<bool, 0>>, 0,
                           client_message_matcher<detail::BN_STATE_MESSAGE>>
                           button_state_event;
    // --------------------------------------------------------------------------
#endif // X11
    // --------------------------------------------------------------------------
    class button : public gui::win::window_with_text {
    public:
      typedef gui::win::window_with_text super;

      typedef void(draw_button_item) (draw::graphics&, const button& btn);

      button ();

      bool is_checked () const;
      void set_checked (bool);

      bool is_hilited () const;
      void set_hilited (bool);

#ifdef X11
    private:
      bool checked;
      bool hilited;
#endif // X11
    };

    namespace paint {
      void push_button (draw::graphics& graph, const button& btn, const std::string& text);
      void radio_button (draw::graphics& graph, const button& btn, const std::string& text);
      void check_box (draw::graphics& graph, const button& btn, const std::string& text);
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

    private:
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

    private:
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

    private:
      static window_class clazz;
    };


  } // win

} // gui
