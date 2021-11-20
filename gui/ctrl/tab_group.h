/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: separator view
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/toggle_group.h>
#include <gui/ctrl/button.h>
#include <gui/layout/adaption_layout.h>


namespace gui {

  namespace ctrl {

    template<origin_t o = origin_t::start,
             uint32_t min = 60,
             uint32_t max = std::numeric_limits<uint32_t>::max(),
             uint32_t border = 0,
             uint32_t gap = 0>
    using top_tab_group = htoggle_group<tab_button<alignment_t::top, true>,
                                        layout::horizontal_adaption<border, gap, 0, min, max, o>>;

    template<origin_t o = origin_t::start,
             uint32_t min = 60,
             uint32_t max = std::numeric_limits<uint32_t>::max(),
             uint32_t border = 0,
             uint32_t gap = 0>
    using bottom_tab_group = htoggle_group<tab_button<alignment_t::bottom, true>,
                                           layout::horizontal_adaption<border, gap, 0, min, max, o>>;


    template<origin_t o = origin_t::start,
             uint32_t min = 20,
             uint32_t max = std::numeric_limits<uint32_t>::max(),
             uint32_t border = 0,
             uint32_t gap = 0>
    using left_tab_group = vtoggle_group<tab_button<alignment_t::left, true>,
                                         layout::vertical_adaption<border, gap, 0, min, max, o>>;

    template<origin_t o = origin_t::start,
             uint32_t min = 20,
             uint32_t max = std::numeric_limits<uint32_t>::max(),
             uint32_t border = 0,
             uint32_t gap = 0>
    using right_tab_group = vtoggle_group<tab_button<alignment_t::right, true>,
                                          layout::vertical_adaption<border, gap, 0, min, max, o>>;

  } // ctrl

} // gui
