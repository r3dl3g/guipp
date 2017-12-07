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

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iomanip>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "time_util.h"
#include "ostreamfmt.h"


/**
* Provides an API to stream into OutputDebugString.
*/
namespace basepp {

  namespace time {

    std::string format_time (std::chrono::system_clock::time_point const& tp,
                             const char* date_delem,
                             const char* sparator,
                             const char* time_delem) {
      std::time_t now = std::chrono::system_clock::to_time_t(tp);
      std::tm t;
#ifdef WIN32
      localtime_s(&t, &now);
#else
      localtime_r(&now, &t);
#endif

      std::ostringstream strm;
      strm << std::setfill('0')
           << (t.tm_year + 1900) << date_delem
           << std::setw(2) << (t.tm_mon + 1) << date_delem
           << std::setw(2) << t.tm_mday << sparator
           << std::setw(2) << t.tm_hour << time_delem
           << std::setw(2) << t.tm_min << time_delem
           << std::setw(2) << t.tm_sec;
      return strm.str();
    }

    std::tm local_time (std::chrono::system_clock::time_point const& tp) {
      std::time_t now = std::chrono::system_clock::to_time_t(tp);
      std::tm t;
#ifdef WIN32
      localtime_s(&t, &now);
#else
      localtime_r(&now, &t);
#endif
      return t;
    }

    std::tm local_time_now () {
      std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
      return local_time(tp);
    }

    std::ostream& operator<< (std::ostream& out, std::chrono::system_clock::time_point const& tp) {
      out << format_time(tp);
      return out;
    }

  } // namespace time

} // namespace basepp
