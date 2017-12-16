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
* @brief     C++ API:logger
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <exception>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <functional>
#include <sstream>
#include <thread>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base++-export.h>
#include "blocking_queue.h"
#include "log_level.h"

#ifdef WIN32
#pragma warning (disable: 4251)
#endif

/**
* Provides an API for stream logging to multiple sinks.
*/
namespace basepp {

  class ostream_resetter {
  public:
    inline ostream_resetter (std::ostream& out)
      : m_out(out)
      , m_fill(out.fill())
      , m_width(out.width())
    {}

    inline ~ostream_resetter () {
      m_out.fill(m_fill);
      m_out.width(m_width);
    }

  private:
    std::ostream& m_out;
    const char m_fill;
    std::streamsize m_width;
  };

  namespace log {

    /// convenience stream operator to print log level to ostream.
    BASEPP_EXPORT std::ostream& operator << (std::ostream& out, level const& lvl);

    /// convenience stream operator to print system_clock to ostream.
    BASEPP_EXPORT std::ostream& operator << (std::ostream& out, std::chrono::system_clock::time_point const& tp);

    /// convenience stream operator to print exception infos to ostream.
    BASEPP_EXPORT std::ostream& operator << (std::ostream& out, const std::exception& ex);

    /**
    * Id for current logged line.
    */
    struct BASEPP_EXPORT line_id {
      line_id (unsigned int i);
      line_id ();

      unsigned int n;
    };

    /// convenience stream operator to print line id to ostream.
    BASEPP_EXPORT std::ostream& operator << (std::ostream& out, line_id const& id);

    /**
    * Logging record. Holds data for one record.
    */
    class BASEPP_EXPORT record {
    public:
      record (const std::chrono::system_clock::time_point& time_point,
              level lvl,
              const std::string& thread_name,
              const line_id& line,
              const std::string& message);

      record ();

      const line_id& line () const;
      const std::chrono::system_clock::time_point& time_point () const;
      const log::level& level () const;
      const std::string& thread_name () const;
      const std::string& message () const;

    private:
      std::chrono::system_clock::time_point m_time_point;
      log::level m_level;
      std::string m_thread_name;
      line_id m_line;
      std::string m_message;

    };

    /**
    * Formatter functor definition.
    */
    typedef std::function<void(std::ostream& out, const record& entry)> record_formatter;

    /**
    * Sink description with target ostream, level to log and log record formatter
    */
    struct BASEPP_EXPORT sink {
      sink (std::ostream* stream,
            level lvl,
            const record_formatter& formatter);

      std::ostream* m_stream;
      level m_level;
      record_formatter m_formatter;
    };

    /**
    * Logging core. Manage sinks and thread safe logging.
    */
    class BASEPP_EXPORT core {
    public:
      core ();
      ~core ();

      void start ();
      void finish ();

      void log (level lvl, const std::string& message);
      void log (level lvl, const std::chrono::system_clock::time_point& time_point, const std::string& message);

      void add_sink (std::ostream* stream, level lvl, const record_formatter& formatter);
      void remove_sink (std::ostream* stream);
      void remove_all_sinks ();

      static record_formatter get_standard_formatter ();
      static record_formatter get_console_formatter ();

      static core& instance ();

      static void rename_file_with_max_count (const std::string& name, int maxnum);

      static std::string build_temp_log_file_name (const std::string& name);
      
      /**
      * Thread name. Thread specific variable to hold the name of the thread.
      */
      static void set_thread_name (const char* name);

    protected:
      friend class record;

    private:
      static void logging_sink_call (core* core);

      void log_to_sinks (const record& entry);

      std::atomic_uint m_line_id;
      std::mutex m_mutex;

      typedef std::vector<sink> Sinks;
      Sinks m_sinks;

      typedef basepp::blocking_queue<record, 0xffff> Messages;
      Messages m_messages;

      std::thread m_sink_thread;
      volatile bool m_is_active;

      core (const core&) = delete;
      void operator= (const core&) = delete;
    };

    /**
    * Logging recorder. Capture data for one record.
    */
    class BASEPP_EXPORT recorder {
    public:
      recorder (level lvl);

      ~recorder ();

      template <typename T>
      recorder& operator<< (T const& value);

      recorder& operator<< (const char value);
      recorder& operator<< (const char* value);
      recorder& operator<< (const std::string& value);

    private:
      std::chrono::system_clock::time_point m_time_point;
      level m_level;
      std::ostringstream m_buffer;
    };

    class null_recoder {
    public:
      template <typename T>
      inline null_recoder& operator<< (T const&) {
        return *this;
      }
    };

  } // namespace log

} // namespace basepp

#include "logger.inl"

/// Log macro trace
#if defined(ENABLE_TRACE) || !defined(NDEBUG)
# define LogTrace basepp::log::recorder (basepp::log::level::trace)
#else
# define LogTrace basepp::log::null_recoder ()
#endif // ENABLE_TRACE

/// Log macro debug
#if defined(NDEBUG)
# define LogDebug basepp::log::null_recoder ()
#else
# define LogDebug basepp::log::recorder (basepp::log::level::debug)
#endif // NDEBUG

/// Log macro info
#define LogInfo  basepp::log::recorder (basepp::log::level::info)
/// Log macro warning
#define LogWarng basepp::log::recorder (basepp::log::level::warning)
/// Log macro error
#define LogError basepp::log::recorder (basepp::log::level::error)
/// Log macro fatal
#define LogFatal basepp::log::recorder (basepp::log::level::fatal)

/**
* Macro to declare the login core singleton.
*/
#define DECLARE_LOGGING_CORE(EXP) \
  namespace basepp { namespace log {\
    EXP core& get_logging_core();\
  }}

#if !defined(BASEPP_BUILT_AS_STATIC)
DECLARE_LOGGING_CORE(BASEPP_EXPORT)
#endif

/**
* Macro to define the login core singleton.
* Since statics are not shared over DLL boundaries, implement this somewhere
* where you can ensure, that it will ony be instantiated once.
*
* in C++11 statics are "magic statics" and their initialization _is_ thread safe.
* @see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2660.htm
*
* Unfortunately Visual Studio 2013 doesn't support them yet.
* @see https://msdn.microsoft.com/library/hh567368(v=vs.130).aspx
*/
#if (_MSC_VER >= 1900) || !defined(_MSC_VER)

#define DEFINE_LOGGING_CORE(EXP) \
  namespace basepp { namespace log {\
    EXP basepp::log::core& get_logging_core() {\
      static basepp::log::core s_logging_core; \
      return s_logging_core; \
    }\
  }}

#else // _MSC_VER < 1900

/**
* Macro to define the login core singleton with the "double-check, single-lock" pattern.
* No longer needed for modern C++11 compilers, except Visual Studio 2013.
*/
#define DEFINE_LOGGING_CORE(EXP) \
  namespace basepp { namespace log {\
    static std::atomic<core*> s_logging_core;\
    static std::mutex s_logging_core_mutex;\
    EXP core& get_logging_core() {\
      core* temp_core = s_logging_core.load();\
      if (nullptr == temp_core) {\
        std::lock_guard<std::mutex> lock(s_logging_core_mutex);\
        temp_core = s_logging_core.load();\
        if (nullptr == temp_core) {\
          temp_core = new core();\
          s_logging_core.store(temp_core);\
        }\
      }\
      return *temp_core;\
    }\
  }}

#endif
