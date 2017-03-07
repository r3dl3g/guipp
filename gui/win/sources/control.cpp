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


// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <map>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "control.h"
#include "window_event_handler.h"
#include "graphics.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------

    window_class label::clazz;

    label::label() {
      if (!clazz.is_valid()) {
#ifdef WIN32
        clazz = window_class::sub_class("MyStatic", "STATIC",
          SS_NOTIFY | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP,
          WS_EX_NOPARENTNOTIFY);
#endif // WIN32
#ifdef X11
        clazz = window_class::custom_class("STATIC",
          0,
          ButtonPressMask | ButtonReleaseMask | ExposureMask |
          PointerMotionMask | StructureNotifyMask | SubstructureRedirectMask |
          FocusChangeMask | EnterWindowMask | LeaveWindowMask,
          0, 0, 0,
          draw::color::buttonColor);
#endif // X11
      }
#ifdef X11
      register_event_handler(win::paint_event([&](draw::graphics& graph) {
        using namespace draw;
        core::rectangle area = client_area();
        graph.text(draw::text_box(text, area, vcenter_left), font::system(), color::black);
      }));
#endif // X11
    }

#ifdef X11
    Atom BN_CLICKED_MESSAGE = 0;
    Atom BN_PUSHED_MESSAGE = 0;
    Atom BN_UNPUSHED_MESSAGE = 0;
    Atom BN_STATE_MESSAGE = 0;

    bool bn_clicked_message_match::operator() (const core::event& e) {
      return (e.type == ClientMessage) && (e.xclient.message_type == BN_CLICKED_MESSAGE);
    }

    bool bn_pushed_message_match::operator() (const core::event& e) {
      return (e.type == ClientMessage) && (e.xclient.message_type == BN_CLICKED_MESSAGE);
    }

    bool bn_unpushed_message_match::operator() (const core::event& e) {
      return (e.type == ClientMessage) && (e.xclient.message_type == BN_CLICKED_MESSAGE);
    }

    bool bn_state_message_match::operator() (const core::event& e) {
      return (e.type == ClientMessage) && (e.xclient.message_type == BN_STATE_MESSAGE);
    }
    
#endif // X11

    button::button()
#ifdef X11
      : checked(false)
      , hilited(false)
#endif // X11
    {
#ifdef X11
      if (!BN_CLICKED_MESSAGE) {
        BN_CLICKED_MESSAGE = XInternAtom(core::global::get_instance(), "BN_CLICKED_MESSAGE", False);
      }
      if (!BN_PUSHED_MESSAGE) {
        BN_PUSHED_MESSAGE = XInternAtom(core::global::get_instance(), "BN_PUSHED_MESSAGE", False);
      }
      if (!BN_UNPUSHED_MESSAGE) {
        BN_UNPUSHED_MESSAGE = XInternAtom(core::global::get_instance(), "BN_UNPUSHED_MESSAGE", False);
      }
      if (!BN_STATE_MESSAGE) {
        BN_STATE_MESSAGE = XInternAtom(core::global::get_instance(), "BN_STATE_MESSAGE", False);
      }
      

      register_event_handler(win::left_btn_down_event([&](const core::point& p) {
        if (is_enabled()) {
          send_client_message(this, BN_PUSHED_MESSAGE);
          if (get_window_class()->get_ex_style()) {
            set_hilited(true);
          } else {
            set_checked(true);
          }
        }
      }));
      register_event_handler(win::left_btn_up_event([&](const core::point& p) {
        if (is_enabled()) {
          send_client_message(this, BN_UNPUSHED_MESSAGE);
          if (get_window_class()->get_ex_style()) {
            if (is_hilited()) {
              set_hilited(false);
              if (client_area().is_inside(p)) {
                set_checked(!is_checked());
                if (is_enabled()) {
                  send_client_message(this, BN_CLICKED_MESSAGE);
                }
              }
            }
          } else {
            set_checked(false);
            if (client_area().is_inside(p)) {
              send_client_message(this, BN_CLICKED_MESSAGE);
            }
          }
        }
      }));
#endif
    }

#ifdef WIN32
    bool button::is_checked() const {
      return SendMessage(get_id(), BM_GETCHECK, 0, 0) != BST_UNCHECKED;
    }

    void button::set_checked(bool f) {
      SendMessage(get_id(), BM_SETCHECK, (WPARAM)(f ? BST_CHECKED : BST_UNCHECKED), 0);
    }

    bool button::is_hilited () const {
      return SendMessage(get_id(), BM_GETSTATE, 0, 0 ) == BST_FOCUS;
    }

    void button::set_hilited (bool h) {
      SendMessage(get_id(), BM_SETSTATE, (WPARAM)(h ? BST_FOCUS : 0), 0 );
    }
#endif // WIN32

#ifdef X11
    bool button::is_checked() const {
      return checked;
    }

    void button::set_checked(bool f) {
      if (checked != f) {
        checked = f;
        send_client_message(this, BN_STATE_MESSAGE, f ? 1 : 0);
        redraw_later();
      }
    }

    bool button::is_hilited () const {
      return hilited;
    }

    void button::set_hilited (bool h) {
      hilited = h;
      redraw_later();
    }
#endif // X11

    window_class push_button::clazz;

    push_button::push_button() {
      if (!clazz.is_valid()) {
#ifdef WIN32
        clazz = win::window_class::sub_class("MyButton",
                                             "BUTTON",
                                             BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT |
                                             WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
                                             WS_VISIBLE | WS_TABSTOP,
                                             WS_EX_NOPARENTNOTIFY);
#else // !WIN32
        clazz = window_class::custom_class("BUTTON",
                                           1,
                                           ButtonPressMask | ButtonReleaseMask | ExposureMask |
                                           PointerMotionMask | StructureNotifyMask | SubstructureRedirectMask |
                                           FocusChangeMask | EnterWindowMask | LeaveWindowMask,
                                           0, 0, 0,
                                           draw::color::buttonColor);
#endif // !WIN32
      }
#ifdef X11
      register_event_handler(win::paint_event([&](draw::graphics& graph) {
        core::size sz = size() - core::size(1, 1);

        using namespace draw;

        graph.drawLines({ core::point(0, sz.height), core::point(), core::point(sz.width, 0) },
          is_checked() ? color::darkGray : color::veryLightGray);
        graph.drawLines({ { 0, sz.height },{ sz.width, sz.height },{ sz.width, 0 } },
          is_checked() ? color::veryLightGray : color::darkGray);
        core::rectangle area = client_area();
        graph.text(draw::text_box(text, area, center), font::system(), is_enabled() ? color::black : color::gray);
      }));
#endif
    }

    window_class radio_button::clazz;

    radio_button::radio_button() {
      if (!clazz.is_valid()) {
#ifdef WIN32
        clazz = win::window_class::sub_class("MyRadioButton",
          "BUTTON",
          BS_AUTORADIOBUTTON | BS_TEXT |
          WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
          WS_VISIBLE | WS_TABSTOP,
          WS_EX_NOPARENTNOTIFY);
#else // !WIN32
        clazz = window_class::custom_class("RADIO_BUTTON",
          0,
          ButtonPressMask | ButtonReleaseMask | ExposureMask |
          PointerMotionMask | StructureNotifyMask | SubstructureRedirectMask |
          FocusChangeMask | EnterWindowMask | LeaveWindowMask,
          1, 0, 0,
          draw::color::buttonColor);
#endif // !WIN32
      }
#ifdef X11
      register_event_handler(win::paint_event([&](draw::graphics& graph) {
        using namespace draw;

        color col = is_enabled() ? color::black : color::gray;
        core::rectangle area = client_area();
        int y = area.position().y + area.size().height / 2;
        core::point pt(area.position().x + 6, y);
        graph.draw(arc(pt, 5, 0, 360),
                   is_hilited() ? color::veryLightGray
                                : color::buttonColor,
                   col);
        if (is_checked()) {
          graph.fill(arc(pt, 3, 0, 360), col);
        }
        area.topleft.x += 20;
        graph.text(draw::text_box(text, area, vcenter_left), font::system(), col);
      }));
#endif
    }

    window_class check_box::clazz;

    check_box::check_box() {
      if (!clazz.is_valid()) {
#ifdef WIN32
        clazz = win::window_class::sub_class("MyCheckBox",
          "BUTTON",
          BS_AUTOCHECKBOX | BS_TEXT |
          WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
          WS_VISIBLE | WS_TABSTOP,
          WS_EX_NOPARENTNOTIFY);
#else // !WIN32
        clazz = window_class::custom_class("CHECKBOX",
          0,
          ButtonPressMask | ButtonReleaseMask | ExposureMask |
          PointerMotionMask | StructureNotifyMask | SubstructureRedirectMask |
          FocusChangeMask | EnterWindowMask | LeaveWindowMask,
          1, 0, 0,
          draw::color::buttonColor);
#endif // !WIN32
      }
#ifdef X11
      register_event_handler(win::paint_event([&](draw::graphics& graph) {
        using namespace draw;

        core::rectangle area = client_area();
        int y = area.position().y + area.size().height / 2;

        color col = is_enabled() ? color::black : color::gray;

        core::rectangle r(core::point(area.position().x + 1, y - 5), core::size(10, 10));
        graph.draw(rectangle(r),
                   is_hilited() ? color::veryLightGray
                                : color::buttonColor,
                   col);
        if (is_checked()) {
          r.topleft += core::size(2, 2);
          r.bottomright -= core::size(1, 1);
          graph.fill(rectangle(r), col);
        }
        area.topleft.x += 20;
        graph.text(draw::text_box(text, area, vcenter_left), font::system(), col);
      }));
#endif
    }

  } // win

} // gui
