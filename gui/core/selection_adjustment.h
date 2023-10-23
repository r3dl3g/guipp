/**
 * @copyright (c) 2016-2023 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     orientation traits
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    enum class selection_adjustment : byte {
      start,
      center,
      end,
      next,
      center_always,
    };

    // --------------------------------------------------------------------------
    GUIPP_CORE_EXPORT float get_adjusted_scroll_position (float space,
                                                          float item_size,
                                                          float scroll_pos,
                                                          float sel_pos,
                                                          selection_adjustment adjust);

    // --------------------------------------------------------------------------
  } // namespace core

} // namespace gui
