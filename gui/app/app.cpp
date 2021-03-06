/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: app defines
 *
 * @file
 */
#include <fstream>
#include <cstring>
#include <signal.h>

#ifdef GUIPP_QT
#include <QtWidgets/qapplication.h>
#endif // GUIPP_QT

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
  clog::fatal() << boost::stacktrace::stacktrace();
  ::raise(SIGABRT);
}
#endif // ENABLE_STACKTRACE

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

#ifdef ENABLE_STACKTRACE
  ::signal(SIGSEGV, &fatal_error_handler);
  ::signal(SIGABRT, &fatal_error_handler);
  ::signal(SIGILL,  &fatal_error_handler);
#endif // ENABLE_STACKTRACE

#ifdef GUIPP_X11
  const char* display = NULL;

  for (int i = 0; i < argc; ++i) {
    if (strcmp(argv[i], "-d") == 0) {
      ++i;
      if (i < argc) {
        display = argv[i];
      } else {
        clog::fatal() << "Missing display parameter after '-d'";
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
    clog::info() << "Qt HighDpiScaling is enabled";
  } else {
    clog::info() << "Qt enable HighDpiScaling";
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  }
# endif // GUIPP_QT_HIDPI
  QGuiApplication qapplication(argc, argv);
  gui::core::global::init(&qapplication);
#endif // GUIPP_QT

  int ret = 0;
  try {
    ret = gui_main(args);
    clog::debug() << "gui_main finished width: " << ret;
  } catch (const std::exception& ex) {
    clog::fatal() << "Excception: " << ex.what();
    ret = 1;
  } catch (const std::string& s) {
    clog::fatal() << "Excception: " << s;
    ret = 1;
  } catch (...) {
    clog::fatal() << "Unknown Excception";
    ret = 1;
  }

  logging::core::instance().finish();

  return ret;
}

