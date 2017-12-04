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
#include <gui/app/app.h>
#include <gui/core/logger.h>
#include <gui/core/dbgstream.h>
#include <gui/core/guidefs.h>
#include <gui/core/string_util.h>
#include <gui_version.h>
#include <gui++-export.h>


//#ifdef GUIPP_BUILT_AS_STATIC
//#define NO_EXPORT
//
//DEFINE_LOGGING_CORE(NO_EXPORT)
//#endif // GUIPP_EXPORT


#ifdef WIN32
int APIENTRY WinMain (_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPTSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  gui::core::odebugstream dbgStrm;
  gui::log::core::instance().add_sink(&dbgStrm, gui::log::level::debug, gui::log::core::get_console_formatter());

#ifndef NDEBUG
  std::ofstream log_file("gui++.log");
  gui::log::core::instance().add_sink(&log_file, gui::log::level::trace, gui::log::core::get_standard_formatter());
#endif // NDEBUG

  std::vector<std::string> args = gui::string::split<' '>(lpCmdLine);
  gui::core::global::init(hInstance);
#endif // WIN32

#ifdef X11
int main (int argc, char* argv[]) {

#ifndef NDEBUG
//  gui::log::core::instance().add_sink(&std::cerr,
//                                     gui::log::level::debug,
//                                     gui::log::core::get_standard_formatter());
#endif // NDEBUG

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
#ifdef X11
//  gui::log::core::instance().remove_sink(&std::cerr);
#endif // X11
#ifdef WIN32
  gui::log::core::instance().remove_sink(&log_file);
#endif // WIN32
#endif // NDEBUG

  gui::log::core::instance().finish();

  return ret;
}

