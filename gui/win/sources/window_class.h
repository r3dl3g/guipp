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
#include "brush.h"


namespace gui {

  namespace win {

    class window;

    class window_class {
    public:
      static window_class custom_class(const std::string& cls_name,
                                       os::style class_style = 0, // X11: Border width
                                       os::style style = IF_WIN32(0) IF_X11(ButtonPressMask |
                                                                            ButtonReleaseMask |
                                                                            ExposureMask |
                                                                            PointerMotionMask |
                                                                            StructureNotifyMask |
                                                                            SubstructureRedirectMask |
                                                                            FocusChangeMask |
                                                                            EnterWindowMask |
                                                                            LeaveWindowMask),
                                       os::style ex_style = 0,
                                       os::icon icon = 0,
                                       os::cursor_type cursor = 0,
                                       const draw::brush& background = draw::brush(draw::color::windowColor()),
                                       draw::color foreground = draw::color::black());

      static window_class custom_class(const std::string& cls_name, const draw::brush& background, os::style class_style = 0);

      static window_class sub_class (window_class& cls, const std::string& base_cls);

      static window_class sub_class (const std::string& cls,
                                     const std::string& base_cls,
                                     os::style style = 0,
                                     os::style ex_style = 0,
                                     draw::color foreground = draw::color::black());

      window_class ();
      window_class (const window_class&);

      window_class (const std::string& cls_name,
                    os::style class_style = 0, // X11: Border width
                    os::style style = 0,
                    os::style ex_style = 0,
                    os::icon icon = 0,
                    os::cursor_type cursor = 0,
                    const draw::brush& background = draw::brush(draw::color::windowColor()),
                    draw::color foreground = draw::color::black(),
                    os::event_callback callback = nullptr);

      void prepare (window*) const;

      const std::string& get_class_name () const;
      const os::style get_class_style () const;
      const os::style get_style () const;
      const os::style get_ex_style () const;
      const os::icon get_icon () const;
      const os::cursor_type get_cursor () const;
      const draw::brush& get_background () const;
      const draw::color get_foreground () const;
      const os::event_callback get_callback () const;

      bool is_valid () const;

      inline ~window_class () {
        unregister_class();
      }

      os::style class_style;
      os::style style;
      os::style ex_style;
      os::icon icon;
      os::cursor_type cursor;
      draw::brush background;
      draw::color foreground;

    private:
      void register_class () const;
      void unregister_class ();

      std::string class_name;
      os::event_callback callback;
      mutable bool is_initialized;
    };

  } // win

} // gui
