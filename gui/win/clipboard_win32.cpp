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

 #ifdef GUIPP_WIN

#include <limits>
#include <util/string_util.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/clipboard.h"
#include "gui/win/overlapped_window.h"
#include "gui/win/native.h"

namespace gui {

  namespace win {

    clipboard& clipboard::get () {
      static clipboard c;
      return c;
    }

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

  } // win

} // gui

#endif // GUIPP_WIN
