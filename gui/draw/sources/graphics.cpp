/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
* @brief     C++ API: basic window types
*
* @file
*/

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "graphics.h"


namespace gui {

	namespace draw {

    void graphics::drawPixel(const core::position &pt, const draw::color &color) {
      SetPixel(id, pt.x, pt.y, color);
    }
    
    draw::color graphics::getPixel(const core::position &pt) const {
      return GetPixel(id, pt.x, pt.y);
    }

    void graphics::drawLine(const core::position &from, const core::position &to, const draw::pen& pen) {
      HGDIOBJ old = SelectObject(id, pen);
      MoveToEx(id, from.x, from.y, NULL);
      LineTo(id, to.x, to.y);
      SelectObject(id, old);
    }
    
    void graphics::drawLines(std::vector<core::position> &points, const draw::pen& pen) {
      HGDIOBJ old = SelectObject(id, pen);
      bool first = true;
      for (core::position pt : points) {
        if (first) {
          first = false;
          MoveToEx(id, pt.x, pt.y, NULL);
        } else {
          LineTo(id, pt.x, pt.y);
        }
      }
      SelectObject(id, old);
    }

  }

}
