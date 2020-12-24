/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui lib
 *
 * Customer   -
 *
 * @brief     C++ API: draw forward declarations
 *
 * @file
 */

#pragma once

namespace gui {

  enum class text_origin_t : unsigned short;
  enum class placement_t : unsigned short;
  enum class line_handling_t : unsigned short;

  namespace draw {

    struct brush;
    struct pen;
    struct font;

    class graphics;

    class basic_map;
    class bitmap;
    class pixmap;
    class masked_bitmap;

    class basic_datamap;

  } // namespace draw

} // namespace gui
