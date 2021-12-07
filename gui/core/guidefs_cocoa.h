/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     cocoa specific gui defines and typedefs
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <CoreGraphics/CoreGraphics.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>


namespace gui {

  namespace os {

    typedef struct CF_BRIDGED_TYPE(id) CGInstance *instance;
    typedef struct CF_BRIDGED_TYPE(id) CGWindow *window;
    typedef struct CF_BRIDGED_TYPE(id) CGDrawable *drawable;
    typedef CGContextRef graphics;

    typedef unsigned long color;
    typedef CGWindowLevel style;

    typedef CGImageRef bitmap;
    typedef CGImageRef icon;
    typedef struct CF_BRIDGED_TYPE(id) CGCursor *cursor;
    typedef CGColorRef brush;
    typedef CGFontRef font;

    typedef CGFloat point_type;
    typedef CGFloat size_type;
    typedef CGFontIndex font_type;
    typedef unsigned int cursor_type;

    typedef unsigned int key_state;
    typedef CGKeyCode key_symbol;

    typedef CGPoint point;
    typedef CGSize size;
    typedef CGRect rectangle;

    typedef CGEventType event_id;
    typedef OSStatus event_result;

  } // os

  namespace core {

  } // core

} //gui
