/**
* @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
* @brief     C++ API: alignment
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
#ifdef X11
#include "graphics.h"
#endif // X11

namespace gui {

  namespace win {
// --------------------------------------------------------------------------
    enum alignment_h {
      alignment_left   = IF_WIN32(0x0) IF_X11(gui::draw::vcenter_left),
      alignment_center = IF_WIN32(0x1) IF_X11(gui::draw::center),
      alignment_right  = IF_WIN32(0x2) IF_X11(gui::draw::vcenter_right)
    };

  } // win

} // gui
