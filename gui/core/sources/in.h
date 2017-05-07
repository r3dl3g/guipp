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
* @brief     C++ API: in helper class
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <utility>

// --------------------------------------------------------------------------
//
// Library includes
//

namespace gui {

  template<typename T>
  class in {
  public:
    in (const T& t)
      : value(t)
      , rval(false)
    {}

    in (T&& t)
      : value(t)
      , rval(true)
    {}

    bool rvalue () const        { return rval;  }
    bool lvalue () const        { return !rval; }

    operator const T& () const  { return value; }
    const T& get () const       { return value; }
    T&& rget () const           { return std::move(const_cast<T&>(value)); }

  private:
    const T& value;
    bool rval;
  };

} //gui
