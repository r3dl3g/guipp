/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     app starting point
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#include <fstream>
#include <cstring>
#include <signal.h>

#ifdef GUIPP_QT
#include <QtWidgets/qapplication.h>
#endif // GUIPP_QT

#ifdef ANDROID
#include <android/log.h>
#include <logging/redirect_stream.h>
#endif // ANDROID

 // --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/dbgstream.h>
#include <logging/file_logger.h>
#include <logging/logger.h>
#include <logging/core.h>
#include <util/string_util.h>
#include <gui/app/app.h>
#include <gui/core/guidefs.h>

#ifdef USE_BOOST
#define ENABLE_STACKTRACE
#endif

#ifdef ENABLE_STACKTRACE
#include <boost/stacktrace.hpp>

void fatal_error_handler(int signum) {
  ::signal(signum, SIG_DFL);
//  boost::stacktrace::safe_dump_to("./backtrace.dump");
  logging::fatal() << boost::stacktrace::stacktrace();
  ::raise(SIGABRT);
}
#endif // ENABLE_STACKTRACE

#ifdef ANDROID
struct android_log {

  android_log (const std::string& app_name)
    : app_name(app_name)
  {}

  void operator ()(const std::string& t) {
    __android_log_write(ANDROID_LOG_INFO, app_name.c_str(), t.c_str());
  }

private:
  std::string app_name;
};

struct android_log_stream : public logging::basic_redirect_stream<android_log, char> {
  typedef logging::basic_redirect_stream<android_log, char>super;

  android_log_stream (const std::string& app_name, logging::level lvl, const logging::record_formatter& fmt)
    : super(app_name) {
    logging::core::instance().add_sink(this, lvl, fmt);
  }

  ~android_log_stream () override {
    logging::core::instance().remove_sink(this);
  }

};

#endif // ANDROID


#ifdef GUIPP_WIN
int APIENTRY WinMain (_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPTSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  UNREFERENCED_PARAMETER(nCmdShow);

  logging::odebugstream dbgStrm(logging::level::debug, logging::core::get_console_formatter());

#ifndef NDEBUG
  logging::file_logger log_file("gui++.log", logging::level::trace, logging::core::get_standard_formatter());
#endif // NDEBUG

  std::vector<std::string> args = util::string::split<' '>(lpCmdLine);
  gui::core::global::init(hInstance);
#endif // GUIPP_WIN

#if defined(GUIPP_X11) || defined(GUIPP_QT)
int main (int argc, char* argv[]) {
  std::vector<std::string> args(argv, argv + argc);
#endif // GUIPP_X11 || GUIPP_QT

#ifndef NDEBUG
  logging::core::instance().set_log_level(logging::level::trace);
#endif

#ifdef ENABLE_STACKTRACE
  ::signal(SIGSEGV, &fatal_error_handler);
  ::signal(SIGABRT, &fatal_error_handler);
  ::signal(SIGILL,  &fatal_error_handler);
#endif // ENABLE_STACKTRACE

#ifdef ANDROID
  std::string appname = [&] () {
    auto end = args[0].find('-');
    auto begin = args[0].find_last_of('/', end);
    if (begin == std::string::npos) {
      begin = 0;
    } else {
      ++begin;
    }
    return args[0].substr(begin, end - begin);
  }();
  android_log_stream android_logger(appname, logging::level::debug, logging::core::get_no_time_formatter());
  logging::debug() << "Found app name: '" << appname << "'";
#endif // ANDROID

#ifdef GUIPP_X11
  const char* display = NULL;

  for (int i = 0; i < argc; ++i) {
    if (strcmp(argv[i], "-d") == 0) {
      ++i;
      if (i < argc) {
        display = argv[i];
      } else {
        logging::fatal() << "Missing display parameter after '-d'";
        return 1;
      }
    }
  }

  gui::core::global::init(XOpenDisplay(display));
#endif // GUIPP_X11
#ifdef GUIPP_QT
  QGuiApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
# ifdef GUIPP_QT_HIDPI
  if (QGuiApplication::testAttribute(Qt::AA_EnableHighDpiScaling)) {
    logging::debug() << "Qt HighDpiScaling is enabled";
  } else {
    logging::debug() << "Qt enable HighDpiScaling";
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  }
# endif // GUIPP_QT_HIDPI
  QGuiApplication qapplication(argc, argv);
  gui::core::global::init(&qapplication);
  logging::debug() << "Qt app name: '" << qapplication.applicationName().toStdString() << "'";
  logging::debug() << "Qt app display name: '" << qapplication.applicationDisplayName().toStdString() << "'";
  logging::debug() << "Qt app desktop file name: '" << qapplication.desktopFileName().toStdString() << "'";
  logging::debug() << "Qt app platform name: '" << qapplication.platformName().toStdString() << "'";
  logging::debug() << "Qt version: '" QT_VERSION_STR << "', runtime: '" << qVersion() << "'";
#endif // GUIPP_QT

  int ret = 0;
  try {
    ret = gui_main(args);
    logging::debug() << "gui_main finished width: " << ret;
  } catch (const std::exception& ex) {
    logging::fatal() << "Excception: " << ex.what();
    ret = 1;
  } catch (const std::string& s) {
    logging::fatal() << "Excception: " << s;
    ret = 1;
  } catch (...) {
    logging::fatal() << "Unknown Excception";
    ret = 1;
  }

  logging::core::instance().finish();

  return ret;
}

