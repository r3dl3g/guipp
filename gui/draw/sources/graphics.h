/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
// Common includes
//
#include <vector>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui_types.h"
#include "color.h"
#include "pen.h"
#include "font.h"


namespace gui {

  namespace draw {

    class graphics {
    public:
      graphics(core::graphics_id id)
        :id(id)
      {}

      void drawPixel(const core::position &pt, const draw::color &color);
      draw::color getPixel(const core::position &) const;

      void drawLine(const core::position &from, const core::position &to, const draw::pen& pen);
      void drawLines(std::vector<core::position> &points, const draw::pen& pen);

    protected:

    private:
      core::graphics_id id;

    };

  }

}
