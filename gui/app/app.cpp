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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/app/app.h>
#include <gui/core/logger.h>
#include <gui/core/dbgstream.h>
#include <gui/core/guidefs.h>
#include <gui/core/string_util.h>


#ifdef WIN32
int APIENTRY WinMain (_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPTSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  gui::core::odebugstream dbgStrm;
  gui::log::core::instance().add_sink(&dbgStrm, gui::log::level::debug, gui::log::core::get_console_formatter());


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

#ifdef X11
#ifndef NDEBUG
//  gui::log::core::instance().removeSink(&std::cerr);
#endif // NDEBUG
#endif // X11

  gui::log::core::instance().finish();

  return ret;
}

