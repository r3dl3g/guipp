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
* @brief     C++ API:log level
*
* @file
*/

#pragma once


/**
* Provides an API for stream logging to multiple sinks.
*/
namespace ibr {

  namespace log {

    /**
    * logging levels. A level always includes subsequent levels.
    */
    enum class level {
      undefined, trace, debug, info, warning, error, fatal
    };

  } // namespace log

} // namespace ibr
