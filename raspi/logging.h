/**
* @copyright (c) 2015-2020 Ing. Buero Rothfuss
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
* @brief     C++ API:ostreamfmt
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef LOGGING

# include <logging/logger.h>

#else

/// Log macro trace
# define LogTrace std::cerr << "Trace:"
/// Log macro debug
# define LogDebug std::cerr << "Debug:"
/// Log macro info
# define LogInfo  std::cerr << "Info:"
/// Log macro warning
# define LogWarng std::cerr << "Warning:"
/// Log macro error
# define LogError std::cerr << "Error:"
/// Log macro fatal
# define LogFatal std::cerr << "Fatal:"

/// convenience stream operator to print exception infos to ostream.
namespace std {
  inline ostream& operator<< (ostream& out, const exception& ex) {
    out << ex.what();
    return out;
  }
}

#endif

