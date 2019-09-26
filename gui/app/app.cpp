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

 // --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/dbgstream.h>
#include <logging/logger.h>
#include <base/string_util.h>
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

  logging::odebugstream dbgStrm;
  logging::core::instance().add_sink(&dbgStrm, logging::level::debug, logging::core::get_console_formatter());

#ifndef NDEBUG
  std::ofstream log_file("gui++.log");
  logging::core::instance().add_sink(&log_file, logging::level::trace, logging::core::get_standard_formatter());
#endif // NDEBUG

  std::vector<std::string> args = basepp::string::split<' '>(lpCmdLine);
  gui::core::global::init(hInstance);
#endif // WIN32

#ifdef X11
int main (int argc, char* argv[]) {

  std::vector<std::string> args;
  for (int i = 0; i < argc; ++i) {
    args.push_back(argv[i]);
  }

  gui::core::global::init(XOpenDisplay(0));
#endif // X11

  int ret = 0;
  try {
    ret = gui_main(args);
  } catch (std::exception& e) {
    LogFatal << e;
    ret = 1;
  }

#ifdef WIN32
#ifndef NDEBUG
  logging::core::instance().remove_sink(&log_file);
#endif // NDEBUG
  logging::core::instance().remove_sink(&dbgStrm);
#endif // WIN32

  logging::core::instance().finish();

  return ret;
}

