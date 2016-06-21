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

    class window_class {
    public:
      inline window_class(const std::string& cls_name,
                          core::windows_style class_style,
                          core::windows_style style,
                          core::windows_style ex_style = 0,
                          core::icon_id icon = 0,
                          core::cursor_id cursor = 0,
                          core::brush_id background = 0)
        : is_initialized(false)
        , class_name(cls_name)
        , class_style(class_style)
        , style(style)
        , ex_style(ex_style)
        , icon(icon)
        , cursor(cursor)
        , background(background) {
      }

      inline ~window_class() {
        unregister_class();
      }

      const std::string& get_class_name() const;
      const core::windows_style get_class_style() const;
      const core::windows_style get_style() const;
      const core::windows_style get_ex_style() const;
      const core::icon_id get_icon() const;
      const core::cursor_id get_cursor() const;
      const core::brush_id get_background() const;


    private:
      void register_class() const;
      void unregister_class();

      mutable bool is_initialized;

      const std::string class_name;
      const core::windows_style class_style;
      const core::windows_style style;
      const core::windows_style ex_style;
      const core::icon_id icon;
      const core::cursor_id cursor;
      const core::brush_id background;
    };

  } // win

} // gui
