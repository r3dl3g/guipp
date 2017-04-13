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
                                       os::style class_style = 0, // X11: Border width
                                       os::style style = 0,
                                       os::style ex_style = 0,
                                       os::icon icon = 0,
                                       os::cursor_type cursor = 0,
                                       os::color background = os::white,
                                       os::color foreground = os::black);

      static window_class custom_class(const std::string& cls_name,
                                       os::color background,
                                       os::style class_style = 0);

      static window_class sub_class (window_class& cls,
                                     const std::string& base_cls);

      static window_class sub_class (const std::string& sub_cls,
                                     const std::string& cls,
                                     os::style style = 0,
                                     os::style ex_style = 0,
                                     os::color foreground = os::black);

      window_class ();
      window_class (const window_class&);

      window_class (const std::string& cls_name,
                    os::style class_style = 0, // X11: Border width
                    os::style style = 0,
                    os::style ex_style = 0,
                    os::icon icon = 0,
                    os::cursor_type cursor = 0,
                    os::color background = os::white,
                    os::color foreground = os::black,
                    os::event_callback callback = nullptr);

      window_class (const std::string& cls_name,
                    os::style class_style = 0, // X11: Border width
                    os::style style = 0,
                    os::style ex_style = 0,
                    os::icon icon = 0,
                    os::cursor cursor = 0,
                    os::color background = os::white,
                    os::color foreground = os::black,
                    os::event_callback callback = nullptr);

      virtual void prepare (window*) const;

      const std::string& get_class_name () const;
      const os::style get_class_style () const;
      const os::style get_style () const;
      const os::style get_ex_style () const;
      const os::icon get_icon () const;
      const os::cursor get_cursor () const;
      const os::color get_background () const;
      const os::color get_foreground () const;
      const os::event_callback get_callback () const;

      bool is_valid () const;

      inline ~window_class () {
        unregister_class();
      }

    private:
      void register_class () const;
      void unregister_class ();

      window_class (const std::string& cls,
                    const std::string& sub_cls,
                    os::style style = 0,
                    os::style ex_style = 0,
                    os::color foreground = os::black);

      std::string class_name;
      std::string sub_class_name;
      mutable os::style class_style;
      os::style style;
      os::style ex_style;
      mutable os::icon icon;
      mutable os::cursor cursor;
      os::cursor_type cursor_type;
      mutable os::color background;
      os::color foreground;
      mutable os::event_callback callback;
      mutable bool is_initialized;
      bool is_sub_class;
    };

  } // win

} // gui
