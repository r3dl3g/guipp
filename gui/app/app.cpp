/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
#include <string.h>

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
#include <gui_version.h>
#include <gui++-export.h>


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
  if (QApplication::testAttribute(Qt::AA_EnableHighDpiScaling)) {
    clog::info() << "Qt HighDpiScaling is enabled";
  } else {
    clog::info() << "Qt enable HighDpiScaling";
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  }
  QApplication qapplication(argc, argv);
  gui::core::global::init(&qapplication);
#endif // GUIPP_QT

  int ret = 0;
  try {
    ret = gui_main(args);
  } catch (std::exception& e) {
    clog::fatal() << e;
    ret = 1;
  }

  logging::core::instance().finish();

  return ret;
}

