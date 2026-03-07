/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     layout to place two windows with a defined split position
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
#include "gui/layout/layout.h"


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    class split_layout_base {
    public:
      void set_header_and_body (layout_function header, layout_function body) {
        this->header = header;
        this->body = body;
      }

      void set_header (layout_function header) {
        this->header = header;
      }

      void set_body (layout_function body) {
        this->body = body;
      }

      void add (const std::vector<std::reference_wrapper<win::window>>& list) {
        if (list.size() > 0) {
          header = lay(list[0].get());
          if (list.size() > 1) {
            body = lay(list[1].get());
          }
        }
      }

    protected:
      layout_function header;
      layout_function body;
    };

    // --------------------------------------------------------------------------
    template<alignment_t A = alignment_t::top, int S = 25>
    class split_layout : public split_layout_base {};

    // --------------------------------------------------------------------------
    template<int S>
    class split_layout<alignment_t::top, S> : public split_layout_base {
    public:
      typedef core::size::type type;

      type size = S;

      gui::core::rectangle header_size (const gui::core::rectangle& r) {
        return r.with_height(size);
      }

      gui::core::rectangle body_size (const gui::core::rectangle& r) {
        return r.with_vertical(r.y() + size, r.height() - size);
      }

      void layout (const gui::core::rectangle& r) {
        if (header) {
          header(header_size(r));
        }
        if (body) {
          body(body_size(r));
        }
      }
    };

    // --------------------------------------------------------------------------
    template<int S>
    class split_layout<alignment_t::bottom, S> : public split_layout_base {
    public:
      typedef core::size::type type;

      type size = S;

      gui::core::rectangle header_size (const gui::core::rectangle& r) {
        return r.with_vertical(r.y2() - size, size);
      }

      gui::core::rectangle body_size (const gui::core::rectangle& r) {
        return r.with_height(r.height() - size);
      }

      void layout (const gui::core::rectangle& r) {
        if (header) {
          header(header_size(r));
        }
        if (body) {
          body(body_size(r));
        }
      }
    };

    // --------------------------------------------------------------------------
    template<int S>
    class split_layout<alignment_t::left, S> : public split_layout_base {
    public:
      typedef core::size::type type;

      type size = S;

      gui::core::rectangle header_size (const gui::core::rectangle& r) {
        return r.with_width(size);
      }

      gui::core::rectangle body_size (const gui::core::rectangle& r) {
        return r.with_horizontal(r.x() + size, r.width() - size);
      }

      void layout (const gui::core::rectangle& r) {
        if (header) {
          header(header_size(r));
        }
        if (body) {
          body(body_size(r));
        }
      }
    };

    // --------------------------------------------------------------------------
    template<int S>
    class split_layout<alignment_t::right, S> : public split_layout_base {
    public:
      typedef core::size::type type;

      type size = S;

      gui::core::rectangle header_size (const gui::core::rectangle& r) {
        return r.with_horizontal(r.x2() - size, size);
      }

      gui::core::rectangle body_size (const gui::core::rectangle& r) {
        return r.with_width(r.width() - size);
      }

      void layout (const gui::core::rectangle& r) {
        if (header) {
          header(header_size(r));
        }
        if (body) {
          body(body_size(r));
        }
      }
    };

    // --------------------------------------------------------------------------
    template<alignment_t A, int S>
    struct is_layout<split_layout<A, S>> {
      enum {
        value = true
      };
    };

    typedef split_layout<alignment_t::top, 25> header_layout;
    typedef split_layout<alignment_t::bottom, 25> footer_layout;

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

