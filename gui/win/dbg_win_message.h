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
 * @brief     C++ API: windows message debug helper
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iosfwd>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/event.h>


namespace gui {

  namespace win {

    bool is_frequent_event (const core::event& e);
    bool is_none_client_event (const core::event& e);

    struct EventId {
      inline EventId (const core::event& e)
        : id(e.type)
      {}

      os::event_id id;
    };

  } // namespace win

} // namespace gui

namespace std {

  std::ostream& operator<< (std::ostream& out, const gui::win::EventId& e);

} // namespace std
