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
* @brief     C++ API: time utilities
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>
#include <chrono>
#include <iosfwd>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui++-export.h>


/**
* Provides an API to stream into OutputDebugString.
*/
namespace gui {

  namespace time {

    GUIPP_EXPORT std::string format_time (std::chrono::system_clock::time_point const& tp,
                                          const char* year_delem = "-",
                                          const char* sparator = " ",
                                          const char* time_delem = ":");

    GUIPP_EXPORT std::tm local_time (std::chrono::system_clock::time_point const& tp);
    GUIPP_EXPORT std::tm local_time_now ();

    GUIPP_EXPORT std::ostream& operator<< (std::ostream&, std::chrono::system_clock::time_point const&);

  } // namespace time

} // namespace gui
