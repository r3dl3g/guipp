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
 * @brief     system specific window event handler function
 *
 * @license   MIT license. See accompanying file LICENSE.
 */


// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <set>
#include <thread>

#include <logging/logger.h>
#include <util/robbery.h>
#include <util/blocking_queue.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/win/overlapped_window.h"
#include "gui/win/window_event_proc.h"
#include "gui/win/dbg_win_message.h"
#include "gui/win/native.h"
#include "gui/win/clipboard.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace detail {

      static int g_next_filter_id = 1;

      typedef std::pair<int, filter_call> filter_call_entry;
      typedef std::vector<filter_call_entry> filter_list;
      filter_list message_filters;

      // --------------------------------------------------------------------------
      bool check_message_filter (const core::event& ev) {
        for (auto& i : message_filters) {
          detail::filter_call& f = i.second;
          if (f(ev)) {
            return true;
          }
        }
        return false;
      }
    }

    namespace global {

      int register_message_filter (const detail::filter_call& filter) {
        logging::trace() << "Register nessage filter";
        detail::install_message_filter();
        detail::message_filters.emplace_back(std::make_pair(detail::g_next_filter_id, filter));
        return detail::g_next_filter_id++;
      }

      void unregister_message_filter (int id) {
        logging::trace() << "Try unregister nessage filter";
        auto e = detail::message_filters.end();
        auto b = detail::message_filters.begin();
        auto i = std::find_if(b, e, [id](const detail::filter_call_entry & e)->bool {
                                return e.first == id;
                              });
        if (i != e) {
          logging::trace() << "Unregister nessage filter";
          detail::message_filters.erase(i);
        }
      }

    } // global  
  }   // win
} // gui
