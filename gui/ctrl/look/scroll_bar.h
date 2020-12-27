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
 * @brief     C++ API: scroll bars
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/core_fwd.h>
#include <gui/draw/draw_fwd.h>
#include <gui/ctrl/scrollbar_item.h>
#include <gui++-look-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace look {

    GUIPP_LOOK_EXPORT void scrollbar (const draw::graphics &g,
                                      ctrl::scrollbar_item select,
                                      ctrl::scrollbar_item hilite,
                                      bool is_enabled,
                                      bool horizontal,
                                      bool is_focused,
                                      const core::rectangle& up,
                                      const core::rectangle& down,
                                      const core::rectangle& thumb,
                                      const core::rectangle& page_up,
                                      const core::rectangle& page_down);

    GUIPP_LOOK_EXPORT void scrollbar_w95 (const draw::graphics &g,
                                          ctrl::scrollbar_item select,
                                          ctrl::scrollbar_item hilite,
                                          bool is_enabled,
                                          bool horizontal,
                                          bool is_focused,
                                          const core::rectangle& up,
                                          const core::rectangle& down,
                                          const core::rectangle& thumb,
                                          const core::rectangle& page_up,
                                          const core::rectangle& page_down);
  } // namespace look

} // namespace gui
