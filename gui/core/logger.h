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
#include <gui/core/blocking_queue.h>
#include <gui/core/log_level.h>


/**
* Provides an API for stream logging to multiple sinks.
*/
namespace gui {

  namespace log {

    /// convenience stream operator to print log level to ostream.
    std::ostream& operator << (std::ostream& out, level const& lvl);

    /// convenience stream operator to print system_clock to ostream.
    std::ostream& operator << (std::ostream& out, std::chrono::system_clock::time_point const& tp);

    /// convenience stream operator to print exception infos to ostream.
    std::ostream& operator << (std::ostream& out, const std::exception& ex);

    /**
    * Id for current logged line.
    */
    struct line_id {
      inline line_id (unsigned int i)
        : n(i)
      {}

      inline line_id ()
        : n(0)
      {}

      unsigned int n;
    };

    /// convenience stream operator to print line id to ostream.
    std::ostream& operator << (std::ostream& out, line_id const& id);

    /**
    * Logging record. Holds data for one record.
    */
    class record {
    public:
      record (const std::chrono::system_clock::time_point& time_point,
              log::level lvl,
              const std::string& thread_name,
              const line_id& line,
              const std::string& message);

      record ();

      inline const line_id& line () const {
        return m_line;
      }

      inline const std::chrono::system_clock::time_point& time_point () const {
        return m_time_point;
      }

      inline const log::level& level () const {
        return m_level;
      }

      inline const std::string& thread_name () const {
        return m_thread_name;
      }

      inline const std::string& message () const {
        return m_message;
      }

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
    struct sink {
      sink (std::ostream* stream,
            level lvl,
            const record_formatter& formatter)
        : m_stream(stream)
        , m_level(lvl)
        , m_formatter(formatter)
      {}

      std::ostream* m_stream;
      level m_level;
      record_formatter m_formatter;
    };

    /**
    * Logging core. Manage sinks and thread safe logging.
    */
    class core {
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

      typedef gui::core::blocking_queue<record, 0xffff> Messages;
      Messages m_messages;

      std::thread m_sink_thread;
      volatile bool m_is_active;
    };

    /**
    * Logging recorder. Capture data for one record.
    */
    class recorder {
    public:
      recorder (log::level lvl);

      ~recorder ();

      template <typename T>
      inline recorder& operator<< (T const& value) {
        m_buffer << value;
        return *this;
      }

      recorder& operator<< (const char value);
      recorder& operator<< (const char* value);

      inline recorder& operator<< (const std::string& value) {
        return operator<<(value.c_str());
      }

      inline std::ostream& ostream () {
        return m_buffer;
      }

    private:
      std::chrono::system_clock::time_point m_time_point;
      log::level m_level;
      std::ostringstream m_buffer;
    };


  } // namespace log

} // namespace gui

/// Log macro trace
#define LogTrace gui::log::recorder (gui::log::level::trace)
/// Log macro debug
#define LogDebug gui::log::recorder (gui::log::level::debug)
/// Log macro info
#define LogInfo  gui::log::recorder (gui::log::level::info)
/// Log macro warning
#define LogWarng gui::log::recorder (gui::log::level::warning)
/// Log macro error
#define LogError gui::log::recorder (gui::log::level::error)
/// Log macro fatal
#define LogFatal gui::log::recorder (gui::log::level::fatal)

/**
* Macro to declare the login core singleton.
*/
#define DECLARE_LOGGING_CORE(EXP) \
  namespace gui { namespace log {\
    EXP core& get_logging_core();\
  }}

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
  namespace gui { namespace log {\
    EXP gui::log::core& get_logging_core() {\
      static gui::log::core s_logging_core; \
      return s_logging_core; \
    }\
  }}

#else // _MSC_VER < 1900

/**
* Macro to define the login core singleton with the "double-check, single-lock" pattern.
* No longer needed for modern C++11 compilers, except Visual Studio 2013.
*/
#define DEFINE_LOGGING_CORE(EXP) \
  namespace gui { namespace log {\
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
