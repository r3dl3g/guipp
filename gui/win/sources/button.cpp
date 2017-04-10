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

#include "button.h"

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    window_class text_button::clazz(
#ifdef WIN32
      win::window_class::sub_class("MyButton",
                                   "BUTTON",
                                   BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT |
                                   WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
                                   WS_VISIBLE | WS_TABSTOP,
                                   WS_EX_NOPARENTNOTIFY)
#else // !WIN32
      window_class::custom_class("BUTTON",
                                 0,
                                 ButtonPressMask | ButtonReleaseMask | ExposureMask |
                                 PointerMotionMask | StructureNotifyMask | SubstructureRedirectMask |
                                 FocusChangeMask | EnterWindowMask | LeaveWindowMask,
                                 0, 0, 0,
                                 draw::color::buttonColor())
#endif // !WIN32
    );

    window_class radio_button::clazz(
#ifdef WIN32
      win::window_class::sub_class("MyRadioButton",
                                   "BUTTON",
                                   BS_AUTORADIOBUTTON | BS_TEXT |
                                   WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
                                   WS_VISIBLE | WS_TABSTOP,
                                   WS_EX_NOPARENTNOTIFY)
#else // !WIN32
      window_class::custom_class("RADIO_BUTTON",
                                 0,
                                 ButtonPressMask | ButtonReleaseMask | ExposureMask |
                                 PointerMotionMask | StructureNotifyMask | SubstructureRedirectMask |
                                 FocusChangeMask | EnterWindowMask | LeaveWindowMask,
                                 0, 0, 0,
                                 draw::color::buttonColor())
#endif // !WIN32
    );

    window_class check_box::clazz(
#ifdef WIN32
        win::window_class::sub_class("MyCheckBox",
                                     "BUTTON",
                                     BS_AUTOCHECKBOX | BS_TEXT |
                                     WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
                                     WS_VISIBLE | WS_TABSTOP,
                                     WS_EX_NOPARENTNOTIFY)
#else // !WIN32
        window_class::custom_class("CHECKBOX",
                                   0,
                                   ButtonPressMask | ButtonReleaseMask | ExposureMask |
                                   PointerMotionMask | StructureNotifyMask | SubstructureRedirectMask |
                                   FocusChangeMask | EnterWindowMask | LeaveWindowMask,
                                   0, 0, 0,
                                   draw::color::buttonColor())
#endif // !WIN32
    );

    // --------------------------------------------------------------------------
#ifdef X11
    namespace detail {
      Atom BN_CLICKED_MESSAGE = 0;
      Atom BN_PUSHED_MESSAGE = 0;
      Atom BN_UNPUSHED_MESSAGE = 0;
      Atom BN_STATE_MESSAGE = 0;
    }
// --------------------------------------------------------------------------
#endif // X11

// --------------------------------------------------------------------------
    button::button ()
#ifdef X11
      : checked(false), hilited(false)
#endif // X11
    {
#ifdef X11
      if (!detail::BN_CLICKED_MESSAGE) {
        detail::BN_CLICKED_MESSAGE = XInternAtom(core::global::get_instance(), "BN_CLICKED_MESSAGE", False);
      }
      if (!detail::BN_PUSHED_MESSAGE) {
        detail::BN_PUSHED_MESSAGE = XInternAtom(core::global::get_instance(), "BN_PUSHED_MESSAGE", False);
      }
      if (!detail::BN_UNPUSHED_MESSAGE) {
        detail::BN_UNPUSHED_MESSAGE = XInternAtom(core::global::get_instance(), "BN_UNPUSHED_MESSAGE", False);
      }
      if (!detail::BN_STATE_MESSAGE) {
        detail::BN_STATE_MESSAGE = XInternAtom(core::global::get_instance(), "BN_STATE_MESSAGE", False);
      }
      register_event_handler(set_focus_event([&](window*){
        redraw_later();
      }));
      register_event_handler(lost_focus_event([&](window*){
        redraw_later();
      }));
#endif // X11
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
    bool button::is_checked () const {
      return checked;
    }

    void button::set_checked (bool f) {
      if (checked != f) {
        checked = f;
        send_client_message(this, detail::BN_STATE_MESSAGE, f ? 1 : 0);
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

    namespace paint {
      // --------------------------------------------------------------------------
      void push_button (draw::graphics& graph, const button& btn, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();
        core::rectangle area = btn.client_area();

        if (focus) {
          graph.frame(draw::rectangle(area), color::black());
          area.shrink({1, 1});
        }
        draw::frame::deep_relief(graph, area, btn.is_checked());
        graph.text(draw::text_box(text, area, center), font::system(), btn.is_enabled() ? color::black() : color::gray());
        if (focus) {
          area.shrink({3, 3});
          graph.frame(draw::rectangle(area), draw::pen(color::black(), draw::pen::dot));
        }
      }

      // --------------------------------------------------------------------------
      void radio_button (draw::graphics& graph, const button& btn, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();

        color col = btn.is_enabled() ? color::black() : color::gray();
        core::rectangle area = btn.client_area();
        int y = area.position().y() + area.size().height() / 2;
        core::point pt(area.position().x() + 6, y);
        graph.draw(arc(pt, 5, 0, 360),
                   btn.is_hilited() ? color::veryLightGray()
                                    : color::buttonColor(),
                   col);
        if (btn.is_checked()) {
          graph.fill(arc(pt, 3, 0, 360), col);
        }
        area.x(20);
        graph.text(draw::text_box(text, area, vcenter_left), font::system(), col);
        if (focus) {
          graph.text(draw::bounding_box(text, area, vcenter_left), font::system(), color::black());
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), draw::pen(color::black(), draw::pen::dot));
        }
      }

      // --------------------------------------------------------------------------
      void check_box (draw::graphics& graph, const button& btn, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();

        core::rectangle area = btn.client_area();
        int y = area.y() + area.height() / 2;

        color col = btn.is_enabled() ? color::black() : color::gray();

        core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
        graph.draw(rectangle(r),
                   btn.is_hilited() ? color::veryLightGray()
                                    : color::buttonColor(),
                   col);
        if (btn.is_checked()) {
          core::point center = r.center();
          /*
          graph.draw_lines({ core::point(r.x() + 1, center.y()),
                             core::point(center.x() - 1, r.y2() - 2),
                             core::point(r.x2() - 1, r.y() + 1 ) }, pen(col, pen::solid, 2));
          */
          r.shrink(core::size(2, 2));
          graph.fill(rectangle(r), col);
        }
        area += core::point(20, 0);
        graph.text(draw::text_box(text, area, vcenter_left), font::system(), col);
        if (focus) {
          graph.text(draw::bounding_box(text, area, vcenter_left), font::system(), color::black());
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), draw::pen(color::black(), draw::pen::dot));
        }
      }
    }

    // --------------------------------------------------------------------------
    push_button::push_button () {
#ifdef X11
      register_event_handler(left_btn_down_event([&](const core::point&){
        if (is_enabled()) {
          take_focus();
          send_client_message(this, detail::BN_PUSHED_MESSAGE);
          set_checked(true);
        }
      }));
      register_event_handler(left_btn_up_event([&](const core::point& pos){
        if (is_enabled()) {
          send_client_message(this, detail::BN_UNPUSHED_MESSAGE);
          set_checked(false);
          if (client_area().is_inside(pos)) {
            send_client_message(this, detail::BN_CLICKED_MESSAGE);
          }
        }
      }));
#endif
    }

    // --------------------------------------------------------------------------
    toggle_button::toggle_button () {
#ifdef X11
      register_event_handler(left_btn_down_event([&](const core::point&){
        if (is_enabled()) {
          take_focus();
          send_client_message(this, detail::BN_PUSHED_MESSAGE);
          set_hilited(true);
        }
      }));
      register_event_handler(left_btn_up_event([&](const core::point& pos){
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
#endif
    }

    // --------------------------------------------------------------------------
    text_button::text_button () {
#ifdef X11
      register_event_handler(paint_event([&] (draw::graphics& graph) {
        paint::push_button(graph, *this, get_text());
      }));
#endif
    }

    // --------------------------------------------------------------------------
    radio_button::radio_button () {
#ifdef X11
      register_event_handler(win::paint_event([&] (draw::graphics& graph) {
        paint::radio_button(graph, *this, get_text());
      }));
#endif
    }

    // --------------------------------------------------------------------------
    check_box::check_box () {
#ifdef X11
      register_event_handler(win::paint_event([&] (draw::graphics& graph) {
        paint::check_box(graph, *this, get_text());
      }));
#endif
    }

  } // win

} // gui
