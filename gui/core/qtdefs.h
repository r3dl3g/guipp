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
#include <QtGui/QtEvents>
#include <QtWidgets/QWidget>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>


namespace gui {

  namespace win {

    class window;

  } // namespace win

  namespace os {

    typedef Qt::WindowType style;

    namespace qt {

      class Widget : public QWidget {
      public:
        Widget (Widget* parent, os::style s, win::window* w);
        ~Widget ();

        win::window* get_window () const;
        Widget* get_parent () const;
        QPainter *painter() const;


      protected:
        bool event (QEvent* e) override;

      private:
        win::window* win;
      };

    } // namespace qt

    typedef QApplication* instance;
    typedef qt::Widget* window;
    typedef QPaintDevice* drawable;
    typedef QPainter* graphics;

    typedef QRgb color;

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
