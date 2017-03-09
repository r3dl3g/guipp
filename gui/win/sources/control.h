/**
* @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
* @brief     C++ API: basic window
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

    class label : public window_with_text {
    public:
      typedef window_with_text super;

      label();

      void create(const window& parent,
        const core::rectangle& place = core::rectangle::default_rectangle,
        const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
        set_text(txt);
    }

    private:
      static window_class clazz;
    };

    class button : public window_with_text {
    public:
      typedef window_with_text super;

      button();

      bool is_checked() const;
      void set_checked (bool);

      bool is_hilited () const;
      void set_hilited (bool);

#ifdef X11
    private:
      bool button_handle_event (const core::event& e, core::event_result& result);

      bool checked;
      bool hilited;
#endif // X11
    };

#ifdef WIN32
    typedef event_handlerT<WM_COMMAND, no_param_caller, 0,
                           command_matcher<BN_CLICKED>>         button_clicked_event;
    typedef event_handlerT<WM_COMMAND, no_param_caller, 0,
                           command_matcher<BN_PUSHED>>          button_pushed_event;
    typedef event_handlerT<WM_COMMAND, no_param_caller, 0,
                           command_matcher<BN_UNPUSHED>>        button_released_event;
    typedef event_handlerT<BM_SETCHECK, one_param_caller<bool>> button_state_event;

#endif //WIN32

#ifdef X11
    extern Atom BN_CLICKED_MESSAGE;
    extern Atom BN_PUSHED_MESSAGE;
    extern Atom BN_UNPUSHED_MESSAGE;
    extern Atom BN_STATE_MESSAGE;

    struct bn_clicked_message_match {
      bool operator() (const core::event& e);
    };
    struct bn_pushed_message_match {
      bool operator() (const core::event& e);
    };
    struct bn_unpushed_message_match {
      bool operator() (const core::event& e);
    };
    struct bn_state_message_match {
      bool operator() (const core::event& e);
    };

    typedef event_handlerT<ClientMessage, no_param_caller, 0,
                           bn_clicked_message_match>          button_clicked_event;
    typedef event_handlerT<ClientMessage, no_param_caller, 0,
                           bn_pushed_message_match>           button_pushed_event;
    typedef event_handlerT<ClientMessage, no_param_caller, 0,
                           bn_unpushed_message_match>         button_released_event;
    typedef event_handlerT<ClientMessage,
                           one_param_caller<bool, get_client_data<bool, 0>>,
                           0, bn_state_message_match>         button_state_event;
#endif // X11

    class push_button : public button {
    public:
      typedef button super;

      push_button();

      void create(const window& parent,
                  const core::rectangle& place = core::rectangle::default_rectangle,
                  const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
        set_text(txt);
      }

    private:
      static window_class clazz;
    };

    class radio_button : public button {
    public:
      typedef button super;

      radio_button();

      void create(const window& parent,
                  const core::rectangle& place = core::rectangle::default_rectangle,
                  const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
        set_text(txt);
      }
    private:
      static window_class clazz;
    };

    class check_box : public button {
    public:
      typedef button super;

      check_box();

      void create(const window& parent,
                  const core::rectangle& place = core::rectangle::default_rectangle,
                  const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
        set_text(txt);
      }
    private:
      static window_class clazz;
    };

  } // win

} // gui
