/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     system clipboard helper
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#ifdef GUIPP_QT
#include <QtGui/QClipboard>
#include <QtGui/QGuiApplication>
#endif // GUIPP_QT
// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>
#include <gui/win/clipboard.h>
#include <gui/win/overlapped_window.h>
#include <limits>

namespace gui {

  namespace win {

    clipboard& clipboard::get () {
      static clipboard c;
      return c;
    }

#ifdef GUIPP_WIN

    clipboard::clipboard ()
    {}

    void clipboard::set_text (window& win, const std::string& t) {
      text = t;
      auto id = win.get_overlapped_window().get_os_window();
      if (OpenClipboard(id)) {
        const std::size_t len = text.size() + 1;
        HGLOBAL hmem = GlobalAlloc(GMEM_DDESHARE, len);
        if (hmem) {
          EmptyClipboard();
          char* data = static_cast<char*>(GlobalLock(hmem));
          memcpy(data, text.c_str(), len);
          GlobalUnlock(hmem);
          SetClipboardData(CF_TEXT, hmem);
        }
        CloseClipboard();
      }
    }

    void clipboard::get_text (window& win, std::function<clipboard::text_callback>&& cb) {
      auto id = win.get_overlapped_window().get_os_window();
      if (OpenClipboard(id)) {
        HANDLE hmem = GetClipboardData(CF_UNICODETEXT);
        if (hmem) {
          const wchar_t* data = static_cast<wchar_t*>(GlobalLock(hmem));
          cb(util::string::utf16_to_utf8(std::wstring(data)));
          GlobalUnlock(hmem);
        } else {
          hmem = GetClipboardData(CF_TEXT);
          if (hmem) {
            const char* data = static_cast<char*>(GlobalLock(hmem));
            cb(std::string(data));
            GlobalUnlock(hmem);
          }
        }
        CloseClipboard();
      }
    }

#endif // GUIPP_WIN

#ifdef GUIPP_X11

    namespace detail {

      Atom CLIPBOARD = 0;
      Atom UTF8_STRING = 0;
      Atom XSEL_DATA = 0;
      Atom TARGETS = 0;
      Atom TEXT = 0;

      void init_clipboard_atoms () {
        core::x11::init_atom(CLIPBOARD, "CLIPBOARD");
        core::x11::init_atom(UTF8_STRING, "UTF8_STRING");
        core::x11::init_atom(XSEL_DATA, "XSEL_DATA");
        core::x11::init_atom(TARGETS, "TARGETS");
        core::x11::init_atom(TEXT, "TEXT");
      }

    } // namespace detail

    clipboard::clipboard ()
      : filter_id(0)
    {
      detail::init_clipboard_atoms();
      global::register_message_filter([&](const core::event & e)->bool {
        if ((e.type == SelectionRequest) &&
            (e.xselectionrequest.selection == detail::CLIPBOARD)) {

          XSelectionEvent se = {
            SelectionNotify,
            0, // serial
            0, // send_event
            e.xselectionrequest.display,
            e.xselectionrequest.requestor,
            e.xselectionrequest.selection,
            e.xselectionrequest.target,
            e.xselectionrequest.property,
            e.xselectionrequest.time
          };

          int result = 0;
          if (se.target == detail::TARGETS) {
            result = XChangeProperty(se.display, se.requestor, se.property, XA_ATOM, 32,
                                     PropModeReplace, (unsigned char*)&detail::UTF8_STRING, 1);
          } else if ((se.target == XA_STRING) || (se.target == detail::TEXT)) {
            result = XChangeProperty(se.display, se.requestor, se.property, XA_STRING, 8, PropModeReplace,
                                     reinterpret_cast<const unsigned char*>(text.c_str()), text.size());
          } else if (se.target == detail::UTF8_STRING) {
            result = XChangeProperty(se.display, se.requestor, se.property, se.target, 8, PropModeReplace,
                                     reinterpret_cast<const unsigned char*>(text.c_str()), text.size());
          } else {
            se.property = None;
          }
          if ((result & 2) == 0) {
            XSendEvent(se.display, se.requestor, 0, 0, (XEvent*)&se);
          }

          return true;
        }
        return false;
      });
    }

    void clipboard::set_text (window& win, const std::string& t) {
      text = t;
      XSetSelectionOwner(core::global::get_instance(), detail::CLIPBOARD, win.get_overlapped_window().get_os_window(), CurrentTime);
    }

    void clipboard::get_text (window& win, std::function<clipboard::text_callback>&& cb) {
      win.add_event_mask(PropertyChangeMask);
      if (filter_id) {
        global::unregister_message_filter(filter_id);
        filter_id = 0;
      }
      filter_id = global::register_message_filter([&, cb](const core::event & e)->bool {
        if ((e.type != SelectionNotify) || (e.xselection.selection != detail::CLIPBOARD)) {
          return false;
        }
        int resbits;
        unsigned long ressize, restail;
        char *result;
        Atom format;
        XGetWindowProperty(e.xselection.display, e.xselection.requestor, detail::XSEL_DATA, 0, std::numeric_limits<long>::max() / 4, True, AnyPropertyType,
                           &format, &resbits, &ressize, &restail, (unsigned char**)&result);
        if ((format == detail::UTF8_STRING) || (format == XA_STRING)) {
          cb(std::string(result, ressize));
        }
        XDeleteProperty(e.xselection.display, e.xselection.requestor, e.xselection.property);
        global::unregister_message_filter(filter_id);
        filter_id = 0;

        return true;
      });
      XConvertSelection(core::global::get_instance(),
                        detail::CLIPBOARD,
                        detail::UTF8_STRING,
                        detail::XSEL_DATA,
                        win.get_overlapped_window().get_os_window(), CurrentTime);
    }

#endif // GUIPP_X11

#ifdef GUIPP_QT

    clipboard::clipboard ()
    {}

    void clipboard::set_text (window& win, const std::string& t) {
      QGuiApplication::clipboard()->setText(QString::fromStdString(t));
    }

    void clipboard::get_text (window& win, std::function<clipboard::text_callback>&& cb) {
      cb(QGuiApplication::clipboard()->text().toStdString());
    }

#endif // GUIPP_QT

  } // win

} // gui
