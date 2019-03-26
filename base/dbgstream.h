/**
* @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
* @brief     C++ API:dbgstream
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <sstream>
#ifdef WIN32
#include <windows.h>
#endif // Win32
#ifdef UNIX
#include <iostream>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//
#include "redirect_stream.h"


/**
* Provides an API to stream into OutputDebugString.
*/
namespace basepp {

  struct debug_log {
    debug_log ()
    {}

    void operator ()(const std::string& t) {
#ifdef WIN32
      ::OutputDebugString(t.c_str());
#endif
#ifdef UNIX
      std::cerr << t << std::endl;
#endif // UNIX
    }
  };

  typedef oredirect_stream<debug_log> odebugstream;

} // namespace basepp
