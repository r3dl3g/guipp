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

#ifdef WIN32
#define WINVER 0x0602
#include <windowsx.h>
#endif // WIN32

#define NOMINMAX
// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <map>
#ifdef X11
# include <X11/Xlib.h>
#endif // X11

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

    // --------------------------------------------------------------------------
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
      register_event_handler(this, &button::button_handle_event);
#endif // X11
    }

#ifdef X11
    bool button::button_handle_event (const core::event& e, core::event_result& result) {
        if ((e.type == ButtonPress) && (e.xbutton.button == Button1)) {
            if (is_enabled()) {
                take_focus();
                send_client_message(this, BN_PUSHED_MESSAGE);
                if (get_window_class()->get_ex_style()) {
                    set_hilited(true);
                } else {
                    set_checked(true);
                }
            }
            return true;
        } else if ((e.type == ButtonRelease) &&
                   (e.xbutton.button == Button1) &&
                   ((e.xbutton.state & Button1Mask) == Button1Mask)) {
            if (is_enabled()) {
                send_client_message(this, BN_UNPUSHED_MESSAGE);
                core::point p = get_param<core::point, XButtonEvent>(e);
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
            return true;
        } else if ((e.type == FocusIn) || (e.type == FocusOut)) {
            redraw_later();
        }
        return false;
    }
#endif // X11

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

    // --------------------------------------------------------------------------
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
                                           0,
                                           ButtonPressMask | ButtonReleaseMask | ExposureMask |
                                           PointerMotionMask | StructureNotifyMask | SubstructureRedirectMask |
                                           FocusChangeMask | EnterWindowMask | LeaveWindowMask,
                                           0, 0, 0,
                                           draw::color::buttonColor);
#endif // !WIN32
      }
#ifdef X11
      register_event_handler(win::paint_event([&](draw::graphics& graph) {
        using namespace draw;

        const bool focus = has_focus();
        core::rectangle area = client_area();
        area.bottom_right -= core::size(1, 1);

        if (focus) {
          graph.frame(draw::rectangle(area), color::black);
          area.grow({-1, -1});
        }
        const core::point& tl = area.top_left;
        const core::point& br = area.bottom_right;

        graph.drawLines({ { tl.x, br.y }, { tl.x, tl.y }, { br.x, tl.y } },
                        is_checked() ? color::darkGray : color::veryLightGray);
        graph.drawLines({ { tl.x, br.y }, { br.x, br.y }, { br.x, tl.y } },
                        is_checked() ? color::veryLightGray : color::darkGray);
        graph.text(draw::text_box(text, area, center), font::system(), is_enabled() ? color::black : color::gray);
        if (focus) {
          area.grow({-3, -3});
          graph.frame(draw::rectangle(area), draw::pen(color::black, draw::pen::dot));
        }
      }));
#endif
    }

    // --------------------------------------------------------------------------
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

        const bool focus = has_focus();

        color col = is_enabled() ? color::black : color::gray;
        core::rectangle area = client_area() - core::size(1, 1);
        int y = area.position().y + area.size().height / 2;
        core::point pt(area.position().x + 6, y);
        graph.draw(arc(pt, 5, 0, 360),
                   is_hilited() ? color::veryLightGray
                                : color::buttonColor,
                   col);
        if (is_checked()) {
          graph.fill(arc(pt, 3, 0, 360), col);
        }
        area.top_left.x += 20;
        graph.text(draw::text_box(text, area, vcenter_left), font::system(), col);
        if (focus) {
          graph.text(draw::bounding_box(text, area, vcenter_left), font::system(), color::black);
          area.grow({ 3, 3 });
          graph.frame(draw::rectangle(area), draw::pen(color::black, draw::pen::dot));
        }
      }));
#endif
    }

    // --------------------------------------------------------------------------
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

        const bool focus = has_focus();

        core::rectangle area = client_area();
        int y = area.position().y + area.size().height / 2;

        color col = is_enabled() ? color::black : color::gray;

        core::rectangle r(core::point(area.position().x + 1, y - 5), core::size(10, 10));
        graph.draw(rectangle(r),
                   is_hilited() ? color::veryLightGray
                                : color::buttonColor,
                   col);
        if (is_checked()) {
          r.top_left += core::size(2, 2);
          r.bottom_right -= core::size(1, 1);
          graph.fill(rectangle(r), col);
        }
        area.top_left.x += 20;
        graph.text(draw::text_box(text, area, vcenter_left), font::system(), col);
        if (focus) {
          graph.text(draw::bounding_box(text, area, vcenter_left), font::system(), color::black);
          area.grow({ 3, 3 });
          graph.frame(draw::rectangle(area), draw::pen(color::black, draw::pen::dot));
        }
      }));
#endif
    }

    // --------------------------------------------------------------------------
    std::map<int, core::size> owner_draw::measure_item_size;
    int owner_draw::next_owner_draw_id = 0;
    
    void owner_draw::set_item_size(const core::size& sz) {
      item_size = sz;
      measure_item_size[owner_draw_id] = sz;
    }

    const core::size& owner_draw::get_item_size() const {
      return item_size;
    }

    const core::size& owner_draw::get_item_size(int id) {
      std::map<int, core::size>::iterator i = measure_item_size.find(id);
      if (i != measure_item_size.end()) {
        return i->second;
      }
      return core::size::zero;
    }

    // --------------------------------------------------------------------------
#ifdef X11
    Atom SELECTION_CHANGE_MESSAGE = 0;

    bool selection_changed_message_match::operator() (const core::event& e) {
      return (e.type == ClientMessage) && (e.xclient.message_type == SELECTION_CHANGE_MESSAGE);
    }
#endif // X11

    // --------------------------------------------------------------------------
    window_class list::clazz;

    list::list ()
#ifdef X11
      : item_count(0)
      , selection(-1)
      , gc(0)
      , offset(0)
#endif // X11
    {
#ifdef X11
      if (!SELECTION_CHANGE_MESSAGE) {
        SELECTION_CHANGE_MESSAGE = XInternAtom(core::global::get_instance(), "SELECTION_CHANGE_MESSAGE", False);
      }
#endif // X11
      if (!clazz.is_valid()) {
#ifdef WIN32
        clazz = win::window_class::sub_class("MyListBox",
            "LISTBOX",
            LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_NODATA | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
            WS_EX_NOPARENTNOTIFY);
#else // !WIN32
        clazz = window_class::custom_class("LISTBOX",
                                           1,
                                           ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                                           FocusChangeMask | KeyPressMask,
                                           0, 0, 0,
                                           draw::color::white);
#endif // !WIN32
      }
      register_event_handler(this, &list::list_handle_event);
    }

    void list::set_count (size_t count) {
#ifdef WIN32
      SendMessage(get_id(), LB_SETCOUNT, count, 0);
#endif // WIN32
#ifdef X11
      item_count = count;
      redraw_later();
#endif // X11
    }

    size_t list::get_count () const {
#ifdef WIN32
      return (size_t)ListBox_GetCount(get_id());
#endif // WIN32
#ifdef X11
      return item_count;
#endif // X11
    }

    void list::set_selection (int sel) {
#ifdef WIN32
      ListBox_SetCurSel(get_id(), sel);
      SendMessage(GetParent(get_id()), WM_COMMAND, MAKEWPARAM(get_owner_draw_id(), LBN_SELCHANGE), (LPARAM)get_id());
#endif // WIN32
#ifdef X11
      selection = std::min(std::max(0, sel), get_count() - 1);
      // Make selection visible
      const int sel_pos = item_size.height * selection;
      const core::size sz = size();
      if (offset + sel_pos < 0) {
        offset = -sel_pos;
      } else if (offset + sel_pos + item_size.height > sz.height) {
        offset = sz.height - (sel_pos + item_size.height);
      }
      send_client_message(this, SELECTION_CHANGE_MESSAGE);
      redraw_later();
#endif // X11
    }

    int list::get_selection () const {
#ifdef WIN32
      return ListBox_GetCurSel(get_id());
#endif // WIN32
#ifdef X11
      return selection;
#endif // X11
    }

    void list::set_drawer(std::function<item_draw> drawer, int item_height) {
      this->drawer = drawer;
      set_item_size( { 0, item_height } );
    }

    void list::draw_item (draw::graphics& g, int idx, const core::rectangle& place, bool selected) {
      if (drawer) {
        drawer(g, idx, place, selected);
      }
    }

    bool list::list_handle_event (const core::event& e,
                                  core::event_result& result) {
#ifdef WIN32
      if (e.type == WM_DRAWITEM) {
        PDRAWITEMSTRUCT pdis = (PDRAWITEMSTRUCT)e.param_2;
        // If there are no list box items, skip this message. 
        if (pdis->itemID == -1) {
          return false;
        }
        switch (pdis->itemAction) {
          case ODA_SELECT:
          case ODA_DRAWENTIRE: {
            draw::graphics g(get_id(), pdis->hDC);
            core::rectangle place(pdis->rcItem);
            bool selected = (pdis->itemState & ODS_SELECTED);
            draw_item(g, pdis->itemID, place, selected);
          }
        }
        return true;
      }
#endif // WIN32
#ifdef X11
      switch (e.type) {
        case Expose: {
          if (!gc) {
            gc = XCreateGC(e.xexpose.display, e.xexpose.window, 0, 0);
          }
          draw::graphics g(e.xexpose.window, gc);
          core::rectangle place = client_area();
          const int max_y = place.bottom_right.y;
          const int max_idx = get_count();
          const int first = -offset / item_size.height;

          place.top_left.y = offset + (item_size.height * first);
          place.set_height(item_size.height);

          for(int idx = first; (idx < max_idx) && (place.top_left.y < max_y); ++idx, place.move(item_size)) {
            draw_item(g, idx, place, selection == idx);
          }
          XFlushGC(e.xexpose.display, gc);
          return true;
        }
        case ButtonPress:
          if (e.xbutton.button == Button1) {
            last_mouse_point = { e.xbutton.x, e.xbutton.y };
            moved = false;
            return true;
          }
          break;
        case ButtonRelease:
          switch (e.xbutton.button) {
            case Button1:
              if (!moved) {
                const int new_selection = (e.xbutton.y - offset) / item_size.height;
                selection = new_selection == selection ? -1 : new_selection;
                send_client_message(this, SELECTION_CHANGE_MESSAGE);
                redraw_later();
                return true;
              }
              break;
            case Button4: { // Y-Wheel
              const int max_delta = std::max(0, (item_size.height * item_count) - size().height);
              offset = std::max(std::min(0, offset + item_size.height), -max_delta);
              moved = true;
              redraw_later();
              return true;
            }
            case Button5: { // Y-Wheel
              const int max_delta = std::max(0, (item_size.height * item_count) - size().height);
              offset = std::max(std::min(0, offset - item_size.height), -max_delta);
              moved = true;
              redraw_later();
              return true;
            }
          }
          break;
        case MotionNotify:
          if ((e.xmotion.state & Button1Mask) == Button1Mask) {
            int dy = e.xmotion.y - last_mouse_point.y;
            const int max_delta = std::max(0, (item_size.height * item_count) - size().height);
            offset = std::max(std::min(0, offset + dy), -max_delta);
            last_mouse_point = { e.xmotion.x, e.xmotion.y };
            moved = true;
            redraw_later();
            return true;
          }
          break;
        case KeyPress: {
          KeySym key;
          char text[8] = {0};
          XLookupString(const_cast<XKeyEvent*>(&e.xkey), text, 8, &key, 0);
          switch (key) {
            case XK_Up:
            case XK_KP_Up:
              set_selection(get_selection() - 1);
              return true;
            case XK_Down:
            case XK_KP_Down:
              set_selection(get_selection() + 1);
              return true;
            case XK_Page_Up:
            case XK_KP_Page_Up:
              set_selection(get_selection() - (size().height / item_size.height));
              return true;
            case XK_Page_Down:
            case XK_KP_Page_Down:
              set_selection(get_selection() + (size().height / item_size.height));
              return true;
            case XK_Home:
            case XK_KP_Home:
              set_selection(0);
              return true;
            case XK_End:
            case XK_KP_End:
              set_selection(get_count() - 1);
              return true;
          }
          break;
        }
      }
#endif // X11
      return false;
    }

    list::~list () {
#ifdef X11
      if (gc) {
        if (core::global::get_instance()) {
          XFreeGC(core::global::get_instance(), gc);
        }
        gc = 0;
      }
#endif // X11
    }

    void list::draw_text_item (draw::graphics& g,
                               const std::string& text,
                               const core::rectangle& place,
                               bool selected) {
      using namespace draw;
      g.fill(rectangle(place), selected ? color::highLightColor : color::white);
      g.text(text_box(text, place, vcenter_left), font::system(),
             selected ? color::highLightTextColor : color::windowTextColor);
    }

  } // win

} // gui
