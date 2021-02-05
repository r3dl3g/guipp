/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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

#ifndef GUIPP_QT
# error Include guidefs.h instead guidefs_qt.h!
#endif // GUIPP_QT

// --------------------------------------------------------------------------
//
// Common includes
//
#include <QtGui/QtEvents>
#include <QtGui/QWindow>
#include <QtGui/QFontInfo>
#include <QtGui/QGuiApplication>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>


namespace gui {

  namespace win {

    class overlapped_window;

  } // namespace win

  namespace os {

    typedef Qt::WindowType style;

    namespace qt {

      class Widget : public QWindow {
      public:
        typedef QWindow super;

        Widget (Widget* parent, os::style s, win::overlapped_window* w);
        ~Widget ();

        void set_window (win::overlapped_window*);
        win::overlapped_window* get_window () const;

        Widget* get_parent () const;

      protected:
        bool event (QEvent* e) override;

      private:
        win::overlapped_window* win;
      };

    } // namespace qt

    typedef QGuiApplication* instance;
    typedef qt::Widget* window;
    typedef QPaintDevice* drawable;
    typedef QPainter* graphics;

    typedef QRgb color;

    typedef QBackingStore* backstore;
    typedef QPixmap* bitmap;
    typedef QIcon icon;
    typedef QCursor cursor;
    typedef QBrush brush;
    typedef QFont font;

    typedef int point_type;
    typedef int size_type;
    typedef QFontInfo font_type;
    typedef Qt::CursorShape cursor_type;

    typedef int key_state;
    typedef int key_symbol;

    typedef QPoint point;
    typedef QSize size;
    typedef QRect rectangle;

    inline point_type get_x (const QPoint& p) { return p.x(); }
    inline point_type get_y (const QPoint& p) { return p.y(); }

    inline size_type get_width (const QSize& s) { return s.width(); }
    inline size_type get_height (const QSize& s) { return s.height(); }

    inline point_type get_x (const QRect& r) { return r.x(); }
    inline point_type get_y (const QRect& r) { return r.y(); }
    inline size_type get_width (const QRect& r) { return r.width(); }
    inline size_type get_height (const QRect& r) { return r.height(); }

    typedef /*QEvent::Type*/int event_id;
    typedef /*QEvent::Type*/int message_type;
    typedef int event_result;

  } // os

} //gui
