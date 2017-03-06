/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
* @brief     C++ API: window class definition
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"


namespace gui {

  namespace win {

    class window;

    class window_class {
    public:
      static window_class custom_class(const std::string& cls_name,
                                       core::windows_style class_style = IF_WIN32(0) IF_X11(1), // X11: Border width
                                       core::windows_style style = IF_WIN32(0) IF_X11(ButtonPressMask |
                                                                                      ButtonReleaseMask |
                                                                                      ExposureMask |
                                                                                      PointerMotionMask |
                                                                                      StructureNotifyMask |
                                                                                      SubstructureRedirectMask |
                                                                                      FocusChangeMask |
                                                                                      EnterWindowMask |
                                                                                      LeaveWindowMask),
                                       core::windows_style ex_style = 0,
                                       core::icon_id icon = 0,
                                       core::cursor_id cursor = 0,
                                       core::brush_id background = IF_WIN32(0) IF_X11(0xFFFFFF),
                                       core::color_type foreground = IF_WIN32(0) IF_X11(CopyFromParent));

      static window_class sub_class(window_class& cls, const std::string& base_cls);

      static window_class sub_class(const std::string& cls,
                                    const std::string& base_cls,
                                    core::windows_style style = IF_WIN32(0) IF_X11(1),
                                    core::windows_style ex_style = 0,
                                    core::color_type foreground = IF_WIN32(0) IF_X11(CopyFromParent));

      window_class();
      window_class(const window_class&);

      window_class(const std::string& cls_name,
                   core::windows_style class_style = IF_WIN32(0) IF_X11(1), // X11: Border width
                   core::windows_style style = 0,
                   core::windows_style ex_style = 0,
                   core::icon_id icon = 0,
                   core::cursor_id cursor = 0,
                   core::brush_id background = IF_WIN32(0) IF_X11(0xFFFFFF),
                   core::color_type foreground = IF_WIN32(0) IF_X11(CopyFromParent),
                   core::event_callback callback = nullptr);

      void operator=(const window_class&);

      void prepare(window*) const;

      const std::string& get_class_name() const;
      const core::windows_style get_class_style() const;
      const core::windows_style get_style() const;
      const core::windows_style get_ex_style() const;
      const core::icon_id get_icon() const;
      const core::cursor_id get_cursor() const;
      const core::brush_id get_background() const;
      const core::color_type get_foreground() const;
      const core::event_callback get_callback() const;

      bool is_valid () const;

      inline ~window_class() {
        unregister_class();
      }

    private:
      void register_class() const;
      void unregister_class();

      std::string class_name;
      core::windows_style class_style;
      core::windows_style style;
      core::windows_style ex_style;
      core::icon_id icon;
      core::cursor_id cursor;
      core::brush_id background;
      core::color_type foreground;
      core::event_callback callback;

      mutable bool is_initialized;
    };

  } // win

} // gui
