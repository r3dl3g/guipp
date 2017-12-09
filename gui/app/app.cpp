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
#include <base/dbgstream.h>
#include <base/logger.h>
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

  basepp::odebugstream dbgStrm;
  basepp::log::core::instance().add_sink(&dbgStrm, basepp::log::level::debug, basepp::log::core::get_console_formatter());

#ifndef NDEBUG
  std::ofstream log_file("gui++.log");
  basepp::log::core::instance().add_sink(&log_file, basepp::log::level::trace, basepp::log::core::get_standard_formatter());
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

#ifndef NDEBUG
#ifdef WIN32
  basepp::log::core::instance().remove_sink(&log_file);
#endif // WIN32
#endif // NDEBUG

  basepp::log::core::instance().finish();

  return ret;
}

