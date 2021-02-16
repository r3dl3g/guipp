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
 * @brief     C++ API: basic structures
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
#include <gui/core/guidefs.h>
#include <gui/core/core_fwd.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    struct context;

    // --------------------------------------------------------------------------
    struct GUIPP_CORE_EXPORT clipping_stack {
      typedef std::vector<gui::os::rectangle> stack_t;

      void push (core::context&, const gui::os::rectangle&);
      void pop (core::context&);

      void set (core::context&, const gui::os::rectangle&);
      void restore (core::context&);
      void clear (core::context&);

      bool empty () const;
      const gui::os::rectangle& back () const;

    private:
      stack_t stack;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_CORE_EXPORT context {

      context (gui::os::drawable, gui::os::graphics);
      context (gui::os::drawable);
      context (const context&) = delete;

      ~context ();

      void operator= (const context&) = delete;

      operator gui::os::graphics () const;
      operator gui::os::drawable () const;

      gui::os::graphics graphics () const;
      gui::os::drawable drawable () const;

      void push_clipping (const gui::os::rectangle&);
      void pop_clipping ();
      void restore_clipping ();

      const clipping_stack& clipping () const;

      int offset_x () const;
      int offset_y () const;
      void set_offset (int x, int y);

    private:
      gui::os::drawable id;
      gui::os::graphics g;
      int offs_x;
      int offs_y;
      bool own_gc;

      mutable clipping_stack clippings;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_CORE_EXPORT clip {
      clip (context& g, const native_rect& r);
      ~clip ();
    private:
      context& ctx;
    };
    // --------------------------------------------------------------------------

  } // namespace core

} // namespace gui

#include <gui/core/context.inl>
