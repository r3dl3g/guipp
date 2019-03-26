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
#include <base++-export.h>


/**
* Provides an API to stream into OutputDebugString.
*/
namespace basepp {

  namespace time {

    // --------------------------------------------------------------------------
    using time_point = std::chrono::system_clock::time_point;
    using duration = std::chrono::system_clock::duration;

    // --------------------------------------------------------------------------
    BASEPP_EXPORT std::tm local_time (time_point const& tp);
    BASEPP_EXPORT std::tm local_time_now ();

    // --------------------------------------------------------------------------
    BASEPP_EXPORT std::ostream& format_time (std::ostream&,
                                             time_point const& tp,
                                             const char* year_delem = "-",
                                             const char* sparator = " ",
                                             const char* time_delem = ":",
                                             bool add_millis = false);

    BASEPP_EXPORT std::string format_time (time_point const& tp,
                                           const char* year_delem = "-",
                                           const char* sparator = " ",
                                           const char* time_delem = ":",
                                           bool add_millis = false);

    BASEPP_EXPORT time_point parse_time (const std::string& s);
    BASEPP_EXPORT time_point parse_time (std::istream& in);

    BASEPP_EXPORT std::ostream& operator<< (std::ostream&, time_point const&);
    BASEPP_EXPORT std::istream& operator>> (std::istream&, time_point&);

    // --------------------------------------------------------------------------
    BASEPP_EXPORT std::ostream& format_duration (std::ostream&,
                                                 duration const& d,
                                                 const char* sparator = " ",
                                                 const char* time_delem = ":",
                                                 bool add_millis = false);

    BASEPP_EXPORT std::ostream& format_duration_mt (std::ostream& out,
                                                    duration const& d,
                                                    int hours_per_mt = 8,
                                                    const char* sparator = " ",
                                                    const char* time_delem = ":",
                                                    bool add_millis = false);

    BASEPP_EXPORT std::ostream& format_duration_only_h (std::ostream& out,
                                                        duration const& d,
                                                        const char* time_delem = ":",
                                                        bool add_millis = false);

    BASEPP_EXPORT std::string format_duration (duration const& d,
                                               const char* sparator = " ",
                                               const char* time_delem = ":",
                                               bool add_millis = false);

    BASEPP_EXPORT duration parse_duration (const std::string& s);
    BASEPP_EXPORT duration parse_duration (std::istream& in);

    BASEPP_EXPORT std::ostream& operator<< (std::ostream&, duration const&);
    BASEPP_EXPORT std::istream& operator>> (std::istream&, duration&);

  } // namespace time

} // namespace basepp
