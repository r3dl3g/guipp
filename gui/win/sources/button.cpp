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
    namespace detail {
#ifdef WIN32
      button_info::button_info (window& w)
      {}

      bool button_info::is_checked (const window& w) const {
        return SendMessage(w.get_id(), BM_GETCHECK, 0, 0) != BST_UNCHECKED;
      }

      void button_info::set_checked(window& w, bool f) {
        SendMessage(w.get_id(), BM_SETCHECK, (WPARAM)(f ? BST_CHECKED : BST_UNCHECKED), 0);
      }

      bool button_info::is_hilited (const window& w) const {
        return SendMessage(w.get_id(), BM_GETSTATE, 0, 0 ) == BST_FOCUS;
      }

      void button_info::set_hilited (window& w, bool h) {
        SendMessage(w.get_id(), BM_SETSTATE, (WPARAM)(h ? BST_FOCUS : 0), 0 );
      }
#endif // WIN32

#ifdef X11
      button_info::button_info (window& w)
        : checked(false)
        , hilited(false)
      {
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
        w.register_event_handler(set_focus_event([&](window*){
          w.redraw_later();
        }));
        w.register_event_handler(lost_focus_event([&](window*){
          w.redraw_later();
        }));
      }

      bool button_info::is_checked (const window&) const {
        return checked;
      }

      void button_info::set_checked (window& w, bool f) {
        if (checked != f) {
          checked = f;
          send_client_message(&w, detail::BN_STATE_MESSAGE, f ? 1 : 0);
          w.redraw_later();
        }
      }

      bool button_info::is_hilited (const window&) const {
        return hilited;
      }

      void button_info::set_hilited (window& w, bool h) {
        hilited = h;
        w.redraw_later();
      }
#endif // X11
    }

    namespace paint {
      // --------------------------------------------------------------------------
      void push_button (draw::graphics& graph, const win::window& btn, bool is_checked, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();
        core::rectangle area = btn.client_area();

        if (focus) {
          graph.frame(draw::rectangle(area), color::black());
          area.shrink({1, 1});
        }
        draw::frame::deep_relief(graph, area, is_checked);
        graph.text(draw::text_box(text, area, center), font::system(), btn.is_enabled() ? color::black() : color::gray());
        if (focus) {
          area.shrink({3, 3});
          graph.frame(draw::rectangle(area), draw::pen(color::black(), draw::pen::dot));
        }
      }

      // --------------------------------------------------------------------------
      void radio_button (draw::graphics& graph, const win::window& btn, bool is_checked, bool is_hilited, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();

        color col = btn.is_enabled() ? color::black() : color::gray();
        core::rectangle area = btn.client_area();
        core::point_type y = area.position().y() + area.size().height() / 2;
        core::point pt(area.position().x() + 6, y);
        graph.draw(arc(pt, 5, 0, 360),
                   is_hilited ? color::veryLightGray()
                              : color::buttonColor(),
                   col);
        if (is_checked) {
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
      void check_box (draw::graphics& graph, const win::window& btn, bool is_checked, bool is_hilited, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();

        core::rectangle area = btn.client_area();
        core::point_type y = area.y() + area.height() / 2;

        color col = btn.is_enabled() ? color::black() : color::gray();

        core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
        graph.draw(rectangle(r),
                   is_hilited ? color::veryLightGray()
                              : color::buttonColor(),
                   col);
        if (is_checked) {
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
    namespace detail {
      void register_push_button_handler(window& win, button_info& bi) {
#ifdef X11
        win.register_event_handler(left_btn_down_event([&](const core::point&){
          if (win.is_enabled()) {
            win.take_focus();
            send_client_message(&win, detail::BN_PUSHED_MESSAGE);
            bi.set_checked(win, true);
          }
        }));
        win.register_event_handler(left_btn_up_event([&](const core::point& pos){
          if (win.is_enabled()) {
            send_client_message(&win, detail::BN_UNPUSHED_MESSAGE);
            bi.set_checked(win, false);
            if (win.client_area().is_inside(pos)) {
              send_client_message(&win, detail::BN_CLICKED_MESSAGE);
            }
          }
        }));
#endif
      }

      // --------------------------------------------------------------------------
      void register_toggle_button_handler (window& w, button_info& b) {
#ifdef X11
        w.register_event_handler(left_btn_down_event([&](const core::point&){
          if (w.is_enabled()) {
            w.take_focus();
            send_client_message(&w, detail::BN_PUSHED_MESSAGE);
            b.set_hilited(w, true);
          }
        }));
        w.register_event_handler(left_btn_up_event([&](const core::point& pos){
          if (w.is_enabled()) {
            send_client_message(&w, detail::BN_UNPUSHED_MESSAGE);
            if (b.is_hilited(w)) {
              b.set_hilited(w, false);
              if (w.client_area().is_inside(pos)) {
                b.set_checked(w, !b.is_checked(w));
                if (w.is_enabled()) {
                  send_client_message(&w, detail::BN_CLICKED_MESSAGE);
                }
              }
            }
          }
        }));
#endif
      }
    }

    // --------------------------------------------------------------------------
    text_button::text_button () {
#ifdef X11
      register_event_handler(paint_event([&] (draw::graphics& graph) {
        paint::push_button(graph, *this, is_checked(), get_text());
      }));
#endif
    }

    // --------------------------------------------------------------------------
    radio_button::radio_button () {
#ifdef X11
      register_event_handler(win::paint_event([&] (draw::graphics& graph) {
        paint::radio_button(graph, *this, is_checked(), is_hilited(), get_text());
      }));
#endif
    }

    // --------------------------------------------------------------------------
    check_box::check_box () {
#ifdef X11
      register_event_handler(win::paint_event([&] (draw::graphics& graph) {
        paint::check_box(graph, *this, is_checked(), is_hilited(), get_text());
      }));
#endif
    }

    // --------------------------------------------------------------------------
    owner_draw_button::owner_draw_button () {
      register_event_handler(this, &owner_draw_button::handle_event);
      register_event_handler(size_event([&](const core::size& sz) {
        set_item_size(sz);
      }));
    }

    bool owner_draw_button::handle_event (const core::event& e,
                                        os::event_result& result) {
      switch (e.type) {
#ifdef WIN32
        case WM_DRAWITEM: {
          PDRAWITEMSTRUCT pdis = (PDRAWITEMSTRUCT)e.param_2;
          switch (pdis->itemAction) {
            case ODA_SELECT:
            case ODA_DRAWENTIRE: {
              draw::graphics g(get_id(), pdis->hDC);
              draw_item(g);
              g.flush();
            }
          }
          return true;
        }
#endif // WIN32
#ifdef X11
        case Expose: {
          draw::graphics g(e.xexpose.window, get_graphics(e));
          draw_item(g);
          g.flush();
          return true;
        }
      }
#endif // X11
      return false;
    }

    // --------------------------------------------------------------------------
    window_class custom_push_button::clazz(
    #ifdef WIN32
      win::window_class::sub_class("CUSTOM_BUTTON",
                                   "BUTTON",
                                   BS_PUSHBUTTON | BS_OWNERDRAW |
                                   WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP,
                                   WS_EX_NOPARENTNOTIFY)
    #else // !WIN32
      window_class::custom_class("CUSTOM_BUTTON",
                                 0,
                                 ButtonPressMask | ButtonReleaseMask | ExposureMask |
                                 PointerMotionMask | StructureNotifyMask | SubstructureRedirectMask |
                                 FocusChangeMask | EnterWindowMask | LeaveWindowMask,
                                 0, 0, 0,
                                 draw::color::buttonColor())
    #endif // !WIN32
    );


  } // win

} // gui
