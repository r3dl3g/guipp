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
 * @brief     frames drawer
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>
#include <gui/draw/draw_fwd.h>
#include <gui/draw/gui++-draw-export.h>


namespace gui {

  namespace draw {

    namespace frame {

      typedef core::rectangle (drawer)(draw::graphics&, const core::rectangle&);

      core::rectangle no_frame (draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle black (draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle white (draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle dots (draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle lines (draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle vline (draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle hline (draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle vraise (draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle hraise (draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle vgroove (draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle hgroove (draw::graphics&, const core::rectangle&);

      GUIPP_DRAW_EXPORT core::rectangle raised_relief (draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle sunken_relief (draw::graphics&, const core::rectangle&);

      core::rectangle relief (draw::graphics& g, const core::rectangle& r, bool sunken);

      GUIPP_DRAW_EXPORT core::rectangle raised_deep_relief (draw::graphics&, const core::rectangle&);
      GUIPP_DRAW_EXPORT core::rectangle sunken_deep_relief (draw::graphics&, const core::rectangle&);

      core::rectangle deep_relief (draw::graphics& g, const core::rectangle& r, bool sunken);

    } // frame

  } // draw

} // gui

#include <gui/draw/frames.inl>
