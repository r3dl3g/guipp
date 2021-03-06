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
 * @brief     helper to debug windows messages
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include <gui/win/gui++-win-export.h>


namespace gui {

  namespace win {

    GUIPP_WIN_EXPORT bool is_frequent_event (const core::event& e);
    GUIPP_WIN_EXPORT bool is_none_client_event (const core::event& e);

  } // namespace win

} // namespace gui
