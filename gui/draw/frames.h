/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: color definition
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>


namespace gui {

  namespace draw {

    class graphics;

    namespace frame {

      typedef void (drawer)(const draw::graphics&, const core::rectangle&);

      void no_frame (const draw::graphics&, const core::rectangle&);

      GUIPP_EXPORT void black (const draw::graphics&, const core::rectangle&);
      GUIPP_EXPORT void white (const draw::graphics&, const core::rectangle&);

      GUIPP_EXPORT void dots (const draw::graphics&, const core::rectangle&);

      GUIPP_EXPORT void lines (const draw::graphics&, const core::rectangle&);
      GUIPP_EXPORT void vline (const draw::graphics&, const core::rectangle&);
      GUIPP_EXPORT void hline (const draw::graphics&, const core::rectangle&);

      GUIPP_EXPORT void vraise (const draw::graphics&, const core::rectangle&);
      GUIPP_EXPORT void hraise (const draw::graphics&, const core::rectangle&);

      GUIPP_EXPORT void vgroove (const draw::graphics&, const core::rectangle&);
      GUIPP_EXPORT void hgroove (const draw::graphics&, const core::rectangle&);

      GUIPP_EXPORT void raised_relief (const draw::graphics&, const core::rectangle&);
      GUIPP_EXPORT void sunken_relief (const draw::graphics&, const core::rectangle&);

      void relief (const draw::graphics& g, const core::rectangle& r, bool sunken);

      GUIPP_EXPORT void raised_deep_relief (const draw::graphics&, const core::rectangle&);
      GUIPP_EXPORT void sunken_deep_relief (const draw::graphics&, const core::rectangle&);

      void deep_relief (const draw::graphics& g, const core::rectangle& r, bool sunken);

    } // frame

  } // draw

} // gui

#include <gui/draw/frames.inl>
