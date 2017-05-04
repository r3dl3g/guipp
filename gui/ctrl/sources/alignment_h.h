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
    enum alignment_h : unsigned short {
      alignment_left   = static_cast<unsigned short>(gui::draw::text_origin::vcenter_left),
      alignment_center = static_cast<unsigned short>(gui::draw::text_origin::center),
      alignment_right  = static_cast<unsigned short>(gui::draw::text_origin::vcenter_right)
    };

  } // win

} // gui
