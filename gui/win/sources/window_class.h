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

  class window_class {
  public:
    inline window_class(const std::string& cls_name,
                        windows_style class_style_ = 0,
                        windows_style style_ = 0,
                        windows_style ex_style_ = 0,
                        instance_id instance_ = 0,
                        icon_id icon_ = NULL,
                        cursor_id cursor_ = NULL,
                        brush_id brush_ = NULL)
      : class_name(cls_name)
      , class_style(class_style_)
      , style(style_)
      , ex_style(ex_style_)
      , instance(instance_)
      , icon(icon_)
      , cursor(cursor_)
      , brush(brush_)
    {
      register_class();
    }

    inline ~window_class() {
      unregister_class();
    }

    std::string class_name;
    windows_style class_style;
    windows_style style;
    windows_style ex_style;
    instance_id instance;
    icon_id icon;
    cursor_id cursor;
    brush_id brush;

    void register_class();
    void unregister_class();
  };

}


