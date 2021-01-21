/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: layout
 *
 * @file
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
#include <gui/layout/layout.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    template<int H = 0>
    class header_layout {
    public:
      static constexpr int height = H;

      void set_header_body (layout_function header, layout_function body) {
        this->header = header;
        this->body = body;
      }

      void set_header (layout_function header) {
        this->header = header;
      }

      void set_body (layout_function body) {
        this->body = body;
      }

      void layout (const gui::core::rectangle& r) {
        if (header) {
          header(r.with_height(height));
        }
        if (body) {
          body(r.with_vertical(r.y() + height, r.height() - height));
        }
      }

    protected:
      layout_function header;
      layout_function body;
    };

    template<int H>
    struct is_layout<header_layout<H>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

