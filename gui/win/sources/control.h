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

    enum Alignment {
      AlignmentLeft   = IF_WIN32(SS_LEFT) IF_X11(draw::vcenter_left),
      AlignmentCenter = IF_WIN32(SS_CENTER) IF_X11(draw::center),
      AlignmentRight  = IF_WIN32(SS_RIGHT) IF_X11(draw::vcenter_right)
    };

    template<Alignment A>
    class labelT : public window_with_text {
    public:
      typedef window_with_text super;

      labelT() {
        if (!clazz.is_valid()) {
#ifdef WIN32
          clazz = window_class::sub_class("MyStatic", "STATIC",
                                          A | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP,
                                          WS_EX_NOPARENTNOTIFY);
#endif // WIN32
#ifdef X11
          clazz = window_class::custom_class("STATIC",
                                             0,
                                             ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                             StructureNotifyMask | SubstructureRedirectMask | FocusChangeMask |
                                             EnterWindowMask | LeaveWindowMask,
                                             A, 0, 0,
                                             draw::color::buttonColor);
#endif // X11
        }
#ifdef X11
        register_event_handler(win::paint_event([&] (draw::graphics& graph) {
          using namespace draw;
          core::rectangle area = client_area();
          text_origin origin = (text_origin)get_window_class()->ex_style;
          graph.text(draw::text_box(text, area, origin),
                     font::system(),
                     color::black);
          graph.text(draw::bounding_box(text, area, origin), font::system(), color::black);
          graph.frame(draw::rectangle(area), draw::pen(color::black, draw::pen::dot));
        }));
#endif // X11
      }

      void create (const window& parent,
                   const core::rectangle& place = core::rectangle::default_rectangle,
                   const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
        set_text(txt);
      }

    private:
      static window_class clazz;
    };

    template<Alignment A> window_class labelT<A>::clazz;

    typedef labelT<AlignmentLeft> label_left;
    typedef label_left label;
    typedef labelT<AlignmentRight> label_right;
    typedef labelT<AlignmentCenter> label_center;


    class button : public window_with_text {
    public:
      typedef window_with_text super;

      button ();

      bool is_checked () const;

      void set_checked (bool);

      bool is_hilited () const;

      void set_hilited (bool);

#ifdef X11
    private:
      bool button_handle_event (const core::event& e,
                                core::event_result& result);

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

      push_button ();

      void create (const window& parent,
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

      radio_button ();

      void create (const window& parent,
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

      check_box ();

      void create (const window& parent,
                   const core::rectangle& place = core::rectangle::default_rectangle,
                   const std::string& txt = std::string()) {
        super::create(clazz, parent, place);
        set_text(txt);
      }

    private:
      static window_class clazz;
    };

    class list : public window {
    public:
      typedef window super;
      typedef void(item_draw)(draw::graphics&, int idx, const core::rectangle& place, bool selected);

      list ();
      ~list ();

      void create (const window& parent,
                   const core::rectangle& place = core::rectangle::default_rectangle) {
        super::create(clazz, parent, place);
      }

      void set_drawer (std::function<item_draw> drawer) {
        this->drawer = drawer;
      }

      void set_count (int count);
      int get_count () const;

    private:
      bool list_handle_event (const core::event& e,
                              core::event_result& result);

      void draw_item (draw::graphics&, int idx, const core::rectangle& place, bool selected);

      std::function<item_draw> drawer;
#ifdef X11
      int item_count;
      core::graphics_id gc;
#endif
      static window_class clazz;
    };

  } // win

} // gui
