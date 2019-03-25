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
#include "logger.h"


/**
* Provides an API to stream into OutputDebugString.
*/
namespace basepp {

  namespace time {

    std::tm local_time (time_point const& tp) {
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
      time_point tp = std::chrono::system_clock::now();
      return local_time(tp);
    }

    // --------------------------------------------------------------------------
    std::string format_time (time_point const& tp,
                             const char* date_delem,
                             const char* sparator,
                             const char* time_delem,
                             bool add_millis) {
      std::ostringstream strm;
      format_time(strm, tp, date_delem, sparator, time_delem, add_millis);
      return strm.str();
    }

    BASEPP_EXPORT std::ostream& format_time (std::ostream& out,
                                             time_point const& tp,
                                             const char* date_delem,
                                             const char* sparator,
                                             const char* time_delem,
                                             bool add_millis) {
      std::time_t now = std::chrono::system_clock::to_time_t(tp);
      std::tm t;
#ifdef WIN32
      localtime_s(&t, &now);
#else
      localtime_r(&now, &t);
#endif

      ostream_resetter r(out);

      out << std::setfill('0')
           << (t.tm_year + 1900) << date_delem
           << std::setw(2) << (t.tm_mon + 1) << date_delem
           << std::setw(2) << t.tm_mday << sparator
           << std::setw(2) << t.tm_hour << time_delem
           << std::setw(2) << t.tm_min << time_delem
           << std::setw(2) << t.tm_sec;

      if (add_millis) {
        auto t0 = std::chrono::system_clock::from_time_t(now);
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(tp - t0);
        out << '.' << std::setw(3) << micros.count();
      }

      return out;
    }

    time_point parse_time (const std::string& s) {
      std::istringstream strm(s);
      return parse_time(strm);
    }

    time_point parse_time (std::istream& strm) {
      int year = 0, month = 1, day = 1, hour = 0, minute = 0, second = 0, millis = 0;
      try {
        if (strm.good()) {
          strm >> year;
          while (strm.good() && !isdigit(strm.peek())) {
            strm.ignore(1);
          }
          if (strm.good()) {
            strm >> month;
            while (strm.good() && !isdigit(strm.peek())) {
              strm.ignore(1);
            }
            if (strm.good()) {
              strm >> day;
              while (strm.good() && !isdigit(strm.peek())) {
                strm.ignore(1);
              }
              if (strm.good()) {
                strm >> hour;
                while (strm.good() && !isdigit(strm.peek())) {
                  strm.ignore(1);
                }
                if (strm.good()) {
                  strm >> minute;
                  while (strm.good() && !isdigit(strm.peek())) {
                    strm.ignore(1);
                  }
                  if (strm.good()) {
                    strm >> second;
                    if (strm.good() && (strm.peek() == '.')) {
                      while (strm.good() && !isdigit(strm.peek())) {
                        strm.ignore(1);
                      }
                      if (strm.good()) {
                        strm >> millis;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      } catch (std::exception& ex) {
        LogFatal << ex;
      }

      std::tm t{second, minute, hour, day, month - 1, year - 1900, 0};
      return std::chrono::system_clock::from_time_t(std::mktime(&t)) + std::chrono::milliseconds(millis);
    }

    std::ostream& operator<< (std::ostream& out, time_point const& tp) {
      return format_time(out, tp);
    }

    std::istream& operator>> (std::istream& in, time_point& tp) {
      tp = parse_time(in);
      return in;
    }

    // --------------------------------------------------------------------------
    std::ostream& format_duration_mt (std::ostream& out,
                                      duration const& d,
                                      int hours_per_mt,
                                      const char* sparator,
                                      const char* time_delem,
                                      bool add_millis) {
      ostream_resetter r(out);

      auto t0 = std::chrono::duration_cast<std::chrono::seconds>(d);
      auto t = t0.count();
      auto secs = t % 60;
      t = (t - secs) / 60;
      auto min = t % 60;
      t = (t - min) / 60;
      auto hours = t % hours_per_mt;
      auto days = (t - hours) / hours_per_mt;
      out << days << sparator << std::setfill('0')
          << std::setw(2) << hours << time_delem
          << std::setw(2) << min << time_delem
          << std::setw(2) << secs;
      if (add_millis) {
        auto tp = std::chrono::duration_cast<std::chrono::microseconds>(d);
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(tp - t0);
        out << '.' << std::setw(3) << micros.count();
      }
      return out;
    }

    // --------------------------------------------------------------------------
    std::ostream& format_duration_only_h (std::ostream& out,
                                          duration const& d,
                                          const char* time_delem,
                                          bool add_millis) {
      ostream_resetter r(out);

      auto t0 = std::chrono::duration_cast<std::chrono::seconds>(d);
      auto t = t0.count();
      auto secs = t % 60;
      t = (t - secs) / 60;
      auto min = t % 60;
      auto hours = (t - min) / 60;
      out << std::setfill('0')
          << std::setw(2) << hours << time_delem
          << std::setw(2) << min << time_delem
          << std::setw(2) << secs;
      if (add_millis) {
        auto tp = std::chrono::duration_cast<std::chrono::microseconds>(d);
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(tp - t0);
        out << '.' << std::setw(3) << micros.count();
      }
      return out;
    }
    // --------------------------------------------------------------------------
    std::ostream& format_duration (std::ostream& out,
                                   duration const& d,
                                   const char* sparator,
                                   const char* time_delem,
                                   bool add_millis) {
      return format_duration_mt(out, d, 24, sparator, time_delem, add_millis);
    }

    // --------------------------------------------------------------------------
    std::string format_duration (duration const& d,
                                 const char* sparator,
                                 const char* time_delem,
                                 bool add_millis) {
      std::ostringstream strm;
      format_duration(strm, d, sparator, time_delem, add_millis);
      return strm.str();
    }

    // --------------------------------------------------------------------------
    duration parse_duration (const std::string& s) {
      std::istringstream strm(s);
      return parse_duration(strm);
    }

    duration parse_duration (std::istream& in) {
      int day = 0, hour = 0, minute = 0, second = 0, millis = 0;
      try {
        if (in.good()) {
          in >> day;
          while (in.good() && !isdigit(in.peek())) {
            in.ignore(1);
          }
          if (in.good()) {
            in >> hour;
            while (in.good() && !isdigit(in.peek())) {
              in.ignore(1);
            }
            if (in.good()) {
              in >> minute;
              while (in.good() && !isdigit(in.peek())) {
                in.ignore(1);
              }
              if (in.good()) {
                in >> second;
                if (in.good() && (in.peek() == '.')) {
                  while (in.good() && !isdigit(in.peek())) {
                    in.ignore(1);
                  }
                  if (in.good()) {
                    in >> millis;
                  }
                }
              }
            }
          }
        }
      } catch (std::exception& ex) {
        LogFatal << ex;
      }

      return std::chrono::milliseconds(((((((day * 24) + hour) * 60) + minute) * 60) + second) * 1000 + millis);
    }

    std::ostream& operator<< (std::ostream& out, duration const& d) {
     return format_duration(out, d);
    }

    std::istream& operator>> (std::istream& in, duration& d) {
      d = parse_duration(in);
      return in;
    }

  } // namespace time

} // namespace basepp
