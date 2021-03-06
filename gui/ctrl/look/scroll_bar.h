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
 * @brief     scrollbar look
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/core_fwd.h>
#include <gui/draw/draw_fwd.h>
#include <gui/ctrl/scrollbar_item.h>
#include <gui/ctrl/look/look_and_feel.h>
#include <gui/ctrl/look/gui++-look-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace look {

    template<look_and_feel_t L = system_look_and_feel>
    void scrollbar (draw::graphics& g,
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

    template<>
    GUIPP_LOOK_EXPORT void scrollbar<look_and_feel_t::metal> (draw::graphics& , ctrl::scrollbar_item, ctrl::scrollbar_item,
                                                              bool, bool, bool, const core::rectangle&, const core::rectangle&,
                                                              const core::rectangle&, const core::rectangle&, const core::rectangle&);

    template<>
    GUIPP_LOOK_EXPORT void scrollbar<look_and_feel_t::w95> (draw::graphics& , ctrl::scrollbar_item, ctrl::scrollbar_item,
                                                            bool, bool, bool, const core::rectangle&, const core::rectangle&,
                                                            const core::rectangle&, const core::rectangle&, const core::rectangle&);

    template<>
    GUIPP_LOOK_EXPORT void scrollbar<look_and_feel_t::w10> (draw::graphics& , ctrl::scrollbar_item, ctrl::scrollbar_item,
                                                            bool, bool, bool, const core::rectangle&, const core::rectangle&,
                                                            const core::rectangle&, const core::rectangle&, const core::rectangle&);

    template<>
    GUIPP_LOOK_EXPORT void scrollbar<look_and_feel_t::osx> (draw::graphics& , ctrl::scrollbar_item, ctrl::scrollbar_item,
                                                            bool, bool, bool, const core::rectangle&, const core::rectangle&,
                                                            const core::rectangle&, const core::rectangle&, const core::rectangle&);

  } // namespace look

} // namespace gui
