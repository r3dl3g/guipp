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
                          core::windows_style class_style_ = 0,
                          core::windows_style style_ = 0,
                          core::windows_style ex_style_ = 0,
                          core::instance_id instance_ = 0,
                          core::icon_id icon_ = NULL,
                          core::cursor_id cursor_ = NULL,
                          core::brush_id brush_ = NULL)
        : class_name(cls_name)
        , class_style(class_style_)
        , style(style_)
        , ex_style(ex_style_)
        , instance(instance_)
        , icon(icon_)
        , cursor(cursor_)
        , brush(brush_) {
        register_class();
      }

      inline ~window_class() {
        unregister_class();
      }

      std::string class_name;
      core::windows_style class_style;
      core::windows_style style;
      core::windows_style ex_style;
      core::instance_id instance;
      core::icon_id icon;
      core::cursor_id cursor;
      core::brush_id brush;

      void register_class();
      void unregister_class();
    };

  } // win

} // gui
