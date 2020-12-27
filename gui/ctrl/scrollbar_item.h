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
 * @brief     C++ API: scroll bars
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    enum class scrollbar_item {
      nothing,
      up_button,
      page_up,
      thumb_button,
      page_down,
      down_button
    };

  } // ctrl

} // gui
