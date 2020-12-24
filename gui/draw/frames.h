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
#include <gui/draw/draw_fwd.h>
#include <gui++-draw-export.h>


namespace gui {

  namespace draw {

    namespace frame {

      typedef core::rectangle (drawer)(const draw::graphics&, const core::rectangle&);

      core::rectangle no_frame (const draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle black (const draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle white (const draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle dots (const draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle lines (const draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle vline (const draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle hline (const draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle vraise (const draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle hraise (const draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle vgroove (const draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle hgroove (const draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle raised_relief (const draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle sunken_relief (const draw::graphics&, const core::rectangle&);

      core::rectangle relief (const draw::graphics& g, const core::rectangle& r, bool sunken);

      GUIPP_DRAW_EXPORT core::rectangle raised_deep_relief (const draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle sunken_deep_relief (const draw::graphics&, const core::rectangle&);

      core::rectangle deep_relief (const draw::graphics& g, const core::rectangle& r, bool sunken);

    } // frame

  } // draw

} // gui

#include <gui/draw/frames.inl>
