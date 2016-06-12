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
#include <ostream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "pen.h"


namespace gui {

	namespace draw {

    const pen pen::default((core::pen_id)GetStockObject(BLACK_PEN));

    pen::pen(core::pen_id id)
      :id(id)
    {
      GetObject(id, sizeof(core::pen_type), &type);
    }

    pen::pen(const draw::color& color, Style style, size_type width)
      : id(CreatePen(style, width, color))
    {
      GetObject(id, sizeof(core::pen_type), &type);
    }

    pen::pen(const pen& rhs) 
      : id(CreatePenIndirect(&rhs.type))
      , type(rhs.type)
    {}

    pen::~pen() {
      if (id) {
        DeleteObject(id);
        id = 0;
      }
    }

    draw::color pen::color() const {
      return type.lopnColor;
    }

    pen::size_type pen::size() const {
      return type.lopnWidth.x;
    }

    pen::Style pen::style() const {
      return (pen::Style)type.lopnStyle;
    }

    pen pen::with_size(size_type sz) const {
      core::pen_type newType = type;
      newType.lopnWidth = { sz, sz };
      return pen(CreatePenIndirect(&newType));
    }

    pen pen::with_style(Style s) const {
      core::pen_type newType = type;
      newType.lopnStyle = s;
      return pen(CreatePenIndirect(&newType));
    }

    pen pen::with_color(const draw::color& c) const {
      core::pen_type newType = type;
      newType.lopnColor = c;
      return pen(CreatePenIndirect(&newType));
    }

    bool pen::operator== (const pen& rhs) const {
      return ((type.lopnColor == rhs.type.lopnColor) &&
              (type.lopnStyle == rhs.type.lopnStyle) &&
              (type.lopnWidth.x == rhs.type.lopnWidth.x) &&
              (type.lopnWidth.y == rhs.type.lopnWidth.y));
    }

    void pen::swap(pen& rhs) {
      core::pen_id id_ = id;
      core::pen_type type_ = type;
      id = rhs.id;
      type = rhs.type;
      rhs.id = id_;
      rhs.type = type_;
    }

  }

}
