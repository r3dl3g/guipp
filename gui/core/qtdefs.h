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
 * @brief     C++ API: x11 typedefs
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <QtWidgets/qwidget.h>
#include <QtCore/qcoreevent.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>


namespace gui {

  namespace os {

    typedef QApplication* instance;
    typedef QWidget* window;
    typedef QPaintDevice* drawable;
    typedef QPainter& graphics;

    typedef QRgb color;
    typedef Qt::WindowType style;

    typedef QPixmap bitmap;
    typedef QIcon icon;
    typedef QCursor cursor;
    typedef QBrush brush;
    typedef QFont font;
    typedef int menu;

    typedef int point_type;
    typedef int size_type;
    typedef QFontInfo font_type;
    typedef Qt::CursorShape cursor_type;

    typedef int key_state;
    typedef int key_symbol;

    typedef QPoint point;
    typedef QSize size;
    typedef QRect rectangle;

    typedef /*QEvent::Type*/int event_id;
    typedef /*QEvent::Type*/int message_type;
    typedef int event_result;

  } // os

} //gui

#define FC_WEIGHT_THIN       QFont::Weight::Thin
#define FC_WEIGHT_ULTRALIGHT QFont::Weight::ExtraLight
#define FC_WEIGHT_LIGHT      QFont::Weight::Light
#define FC_WEIGHT_REGULAR    QFont::Weight::Normal
#define FC_WEIGHT_MEDIUM     QFont::Weight::Medium
#define FC_WEIGHT_SEMIBOLD   QFont::Weight::DemiBold
#define FC_WEIGHT_BOLD       QFont::Weight::Bold
#define FC_WEIGHT_ULTRABOLD  QFont::Weight::ExtraBold
#define FC_WEIGHT_HEAVY      QFont::Weight::Black
