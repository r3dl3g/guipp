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
* @brief     C++ API: edit control
*
* @file
*/


// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef WIN32
#include <windowsx.h>
#endif // WIN32

// --------------------------------------------------------------------------
//
// Library includes
//
#include "edit.h"

//#define SHOW_TEXT_AREA

namespace gui {

  namespace win {

    namespace detail {

#ifdef WIN32
      edit_base::edit_base ()
      {}

      edit_base::~edit_base()
      {}

      window_class edit_base::create_edit_class (gui::win::alignment_h a) {
        return window_class::sub_class("MyEdit", "EDIT",
                                        a | ES_AUTOHSCROLL | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP,
                                        WS_EX_NOPARENTNOTIFY);
      }

	  void edit_base::set_selection (const edit_base::range& sel) {
        Edit_SetSel(get_id(), sel.first, sel.last);
      }

      edit_base::range edit_base::get_selection () const {
        range sel;
        SendMessage(get_id(), EM_GETSEL, (WPARAM)&sel.first, (LPARAM)&sel.last);
     		return sel;
      }

      edit_base::pos_t edit_base::get_cursor_pos () const {
        return (pos_t)SendMessage(get_id(), EM_GETTHUMB, 0, 0);
      }

      void edit_base::set_cursor_pos (pos_t pos, bool shift) {
        set_selection(range(pos, pos));
      }

      void edit_base::set_text_limit (pos_t max_chars) {
        SendMessage(get_id(), EM_LIMITTEXT, (WPARAM)max_chars, 0);
      }

      edit_base::pos_t edit_base::get_text_limit () const {
        return (pos_t)SendMessage(get_id(), EM_GETLIMITTEXT, 0, 0);
      }

      edit_base::pos_t edit_base::get_text_length () const {
        return (pos_t)Edit_GetTextLength(get_id());
      }

      void edit_base::replace_selection (const std::string &text) {
        Edit_ReplaceSel(get_id(), (LPARAM)text.c_str());
      }

      void edit_base::register_handler () {
      }
#endif // WIN32

#ifdef X11
      const std::string white_space = " (){}[],.;:'\"!@#$%^&/*-+";

      edit_base::edit_base ()
        : cursor_pos(0)
        , text_limit(std::numeric_limits<pos_t>::max())
        , scroll_pos(0)
        , im(0)
        , ic(0)
        , last_mouse_point(core::point::undefined)
    {}

      window_class edit_base::create_edit_class (gui::win::alignment_h a) {
        return gui::win::window_class::custom_class("EDIT",
                                                    0,
                                                    ButtonPressMask | ButtonReleaseMask | ExposureMask |
                                                    PointerMotionMask | KeyPressMask | FocusChangeMask |
                                                    StructureNotifyMask | SubstructureRedirectMask,
                                                    a, 0, 0,
                                                    gui::draw::color::white());
      }

      edit_base::~edit_base () {
        detail::s_window_ic_map.erase(get_id());
        XDestroyIC(ic);
        ic = 0;
        XCloseIM(im);
        im = 0;
      }

      void edit_base::prepare_input () {
        im = XOpenIM(core::global::get_instance(), NULL, NULL, NULL);
        XIMStyle app_supported_styles = XIMPreeditNone | XIMPreeditNothing | XIMPreeditArea |
                                        XIMStatusNone | XIMStatusNothing | XIMStatusArea;

        XIMStyles *im_supported_styles;
        /* figure out which styles the IM can support */
        XGetIMValues(im, XNQueryInputStyle, &im_supported_styles, NULL);
        XIMStyle best_style = 0x0F1F;
        for (int i = 0; i < im_supported_styles->count_styles; ++i) {
          XIMStyle style = im_supported_styles->supported_styles[i];
          if ((style & app_supported_styles) == style) {/* if we can handle it */
            best_style = std::min(style, best_style);
          }
        }
        XFree(im_supported_styles);

        ic = XCreateIC(im,
                       XNInputStyle, best_style,
                       XNClientWindow, get_id(),
                       NULL);

        detail::s_window_ic_map[get_id()] = ic;
      }

      void edit_base::set_selection (const edit_base::range& sel) {
        selection = sel;
        redraw_later();
      }

      edit_base::range edit_base::get_selection () const {
        return selection;
      }

      void edit_base::set_cursor_pos (pos_t pos, bool shift) {
        pos_t new_pos = std::min(pos, get_text_length());
        if (shift) {
          if (cursor_pos == selection.last) {
            selection.last = new_pos;
          } else if (cursor_pos == selection.first) {
            selection.first = new_pos;
          } else {
            selection = range(new_pos);
          }
          selection.sort();
        } else {
          selection = range(new_pos);
        }
        cursor_pos = new_pos;

        // make cursor pos visible
        if (cursor_pos < scroll_pos) {
          scroll_pos = 0;
        }

        core::size sz = draw::font::system().get_text_size(text.substr(scroll_pos, cursor_pos - scroll_pos));
        core::size max_sz = client_size();
        max_sz -= {6, 4};
        while ((sz.width() > max_sz.width()) && (scroll_pos < cursor_pos)) {
          ++scroll_pos;
          sz = draw::font::system().get_text_size(text.substr(scroll_pos, cursor_pos - scroll_pos));
        }

        redraw_later();
      }

      edit_base::pos_t edit_base::get_cursor_pos () const {
        return cursor_pos;
      }

      void edit_base::set_text_limit (pos_t max_chars) {
        text_limit = max_chars;
      }

      edit_base::pos_t edit_base::get_text_limit () const {
        return text_limit;
      }

      edit_base::pos_t edit_base::get_text_length () const {
        return (pos_t)text.size();
      }

      void edit_base::replace_selection (const std::string &new_text) {
        range sel = get_selection();
        text.replace(sel.first, sel.last - sel.first, new_text);
        set_selection(range(sel.first + new_text.size()));
        redraw_later();
      }

      namespace utf8 {
        bool is_continuation_char (char ch) {
          return ((ch & 0b11000000) == 0b10000000);
        }
      }

      edit_base::pos_t edit_base::get_char_at_point (const core::point& pt) {
          pos_t max_chars = text.size();
          for (pos_t i = scroll_pos + 1; i < max_chars; ++i) {
            core::size sz = draw::font::system().get_text_size(text.substr(scroll_pos, i - scroll_pos));
            if (sz.width() >= pt.x()) {
              return i - 1;
            }
          }
          return max_chars;
      }

      void edit_base::register_handler () {
        register_event_handler(paint_event([&] (const gui::draw::graphics& graph) {
          using namespace gui::draw;
          gui::core::rectangle area = client_area();
          draw::frame::sunken_relief(graph, area);
          area.shrink({3, 2});
          draw::clip clp(graph, area);
          text_origin origin = (text_origin)get_window_class()->get_ex_style();
          int y1 = area.y() + 2;
          int y2 = area.y2() - 2;
          if (selection.first < selection.last) {
            gui::core::rectangle a1 = area;
            if (selection.first > scroll_pos) {
              graph.text(draw::bounding_box(text.substr(scroll_pos, selection.first - scroll_pos), a1, origin),
                         font::system(), color::black());
            } else {
              a1.x2(a1.x());
            }
            gui::core::rectangle a2 = area;
            graph.text(draw::bounding_box(text.substr(scroll_pos, selection.last - scroll_pos), a2, origin),
                       font::system(), color::black());
            graph.fill(draw::rectangle(core::point(a1.x2(), y1),
                                       core::point(a2.x2(), y2)),
                       color::lightBlue());
          }
          if (cursor_pos < std::numeric_limits<pos_t>::max()) {
            gui::core::rectangle cursor_area = area;
            graph.text(draw::bounding_box(text.substr(scroll_pos, cursor_pos - scroll_pos), cursor_area, origin),
                       font::system(), color::black());
            graph.frame(line(core::point(cursor_area.x2(), y1),
                             core::point(cursor_area.x2(), y2)),
                             color::black());
          }
          graph.text(draw::text_box(text.substr(scroll_pos), area, origin), font::system(), color::black());
#ifdef SHOW_TEXT_AREA
          graph.text(draw::bounding_box(text, area, origin), font::system(), color::black());
          graph.frame(draw::rectangle(area), draw::pen(color::black(), draw::pen::dot));
#endif // SHOW_TEXT_AREA
        }));
        register_event_handler(key_down_event([&] (unsigned int keystate,
                                                   KeySym keycode,
                                                   const std::string& chars) {
          bool shift = (keystate & ShiftMask) != 0;
          bool ctrl = (keystate & ControlMask) != 0;

          switch (keycode) {
            case XK_Left:
            case XK_KP_Left:
              if (ctrl) {
                // next word begin
                if (cursor_pos > 1) {
                  std::string::size_type pos = text.find_last_not_of(white_space, cursor_pos - 1);
                  if (pos != std::string::npos) {
                    std::string::size_type pos2 = text.find_last_of(white_space, pos);
                    if (pos2 != std::string::npos) {
                      set_cursor_pos(pos2 + 1, shift);
                      return true;
                    }
                  }
                }
              } else if (cursor_pos > 0) {
                int cp = cursor_pos - 1;
                while ((cp > 0) && utf8::is_continuation_char(text.at(cp))) {
                  --cp;
                }
                set_cursor_pos(cp, shift);
                return true;
              }
              set_cursor_pos(0, shift);
              return true;
            case XK_Right:
            case XK_KP_Right:
              if (ctrl) {
                // previous word begin
                std::string::size_type pos = text.find_first_of(white_space, cursor_pos + 1);
                if (pos != std::string::npos) {
                  std::string::size_type pos2 = text.find_first_not_of(text[pos], pos);
                  if (pos2 != std::string::npos) {
                    set_cursor_pos(pos2, shift);
                  } else {
                    set_cursor_pos(pos, shift);
                  }
                } else {
                  set_cursor_pos(get_text_length(), shift);
                }
              } else if (cursor_pos < get_text_length ()) {
                int cp = cursor_pos + 1;
                while ((cp < get_text_length ()) && utf8::is_continuation_char(text.at(cp))) {
                  ++cp;
                }
                set_cursor_pos(cp, shift);
              }
              return true;
            case XK_Home:
            case XK_KP_Home:
              set_cursor_pos(0, shift);
              return true;
            case XK_End:
            case XK_KP_End:
              set_cursor_pos(get_text_length(), shift);
              return true;
            case XK_Delete:
            case XK_KP_Delete:
              if (selection.empty()) {
                int cp = cursor_pos + 1;
                while ((cp < get_text_length ()) && utf8::is_continuation_char(text.at(cp))) {
                  ++cp;
                }
                text.replace(cursor_pos, cp - cursor_pos, std::string());
                redraw_later();
              } else {
                replace_selection(std::string());
                set_cursor_pos(selection.first, false);
              }
              return true;
            case XK_BackSpace:
              if (selection.empty()) {
                if (cursor_pos > 0) {
                  int cp = cursor_pos - 1;
                  while ((cp > 0) && utf8::is_continuation_char(text.at(cp))) {
                    --cp;
                  }
                  text.replace(cp, cursor_pos - cp, std::string());
                  set_cursor_pos(cp, false);
                }
              } else {
                replace_selection(std::string());
                set_cursor_pos(selection.first, false);
              }
              return true;
            case XK_Escape:
              set_selection(range());
              return true;
            case XK_Clear:
              set_selection(range(0, text.size()));
              replace_selection(std::string());
              set_cursor_pos(0, false);
              return true;
            case XK_Tab:
            case XK_Return:
            case XK_Linefeed:
            case XK_Pause:
              return false;
            // TBD: cut, copy, paste
            case XK_a:
              if (ctrl) {
                // select all
                set_selection(range(0, text.size()));
                return true;
              }
              // fall through!
            default: {
              if (ctrl) {
                LogDebug << "Key Ctrl + 0x" << std::hex << keycode;
              } else {
                if (chars.size()) {
                  replace_selection(chars);
                  set_cursor_pos(selection.last, false);
                }
              }
            }
          }
        }));
        register_event_handler(left_btn_down_event([&](const core::point& pt) {
          last_mouse_point = pt;
          set_cursor_pos(get_char_at_point(pt));
        }));
        register_event_handler(left_btn_up_event([&](const core::point& pt) {
          last_mouse_point = core::point::undefined;
        }));
        register_event_handler(mouse_move_event([&](unsigned int keys, const core::point& pt) {
          if ((last_mouse_point != core::point::undefined) && left_button_bit_mask::is_set(keys)) {
            set_cursor_pos(get_char_at_point(pt), true);
          }
        }));
      }

#endif // X11

    } // detail

  } // win

} // gui
