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
                          core::windows_style class_style_,
                          core::windows_style style_,
                          core::windows_style ex_style_ = 0,
                          core::icon_id icon_ = NULL,
                          core::cursor_id cursor_ = NULL,
                          core::brush_id brush_ = NULL)
        : is_initialized(false)
        , class_name(cls_name)
        , class_style(class_style_)
        , style(style_)
        , ex_style(ex_style_)
        , icon(icon_)
        , cursor(cursor_)
        , brush(brush_) {
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
      const core::brush_id get_brush() const;


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
      const core::brush_id brush;
    };

  } // win

} // gui
