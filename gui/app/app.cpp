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


#ifdef WIN32
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
#endif // WIN32

#ifdef X11
int main (int argc, char* argv[]) {

  const char* display = NULL;

  std::vector<std::string> args(argv, argv + argc);
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
#endif // X11

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

