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

 #ifdef GUIPP_JS

#include <limits>
#include <emscripten.h>
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

    using clipboard_receiver = std::function<clipboard::text_callback>;

    static std::unordered_map<int, clipboard_receiver> s_clipboard_receiver;
    static int s_next_id = 0;

    clipboard::clipboard ()
    {}

    using namespace emscripten;

    void clipboard::set_text (window& win, const std::string& t) {
      native::js::send_to_main("copy", t);
    }

    void clipboard::get_text (window& win, std::function<clipboard::text_callback>&& cb) {
      int id = ++s_next_id;
      s_clipboard_receiver[id] = std::move(cb);
      native::js::send_to_main("paste", id);
    }

    void clipboard::handle_paste (const std::string& str, int id) {
      auto i = s_clipboard_receiver.find(id);
      if (i != s_clipboard_receiver.end()) {
        logging::trace() << "Call clipboard receiver " << id << " with text: '" << str << "'";
        i->second(str);
        s_clipboard_receiver.erase(i);
      }

    }

  } // win

} // gui

#endif // GUIPP_JS
