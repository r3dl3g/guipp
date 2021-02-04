/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic window
 *
 * @file
 */

#ifdef GUIPP_QT

// --------------------------------------------------------------------------
//
// Common includes
//
#include <map>

#include <QtWidgets/QApplication>
#include <QtGui/QScreen>
#include <QtGui/QPainter>
#include <QtGui/QBitmap>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <gui/win/native.h>
#include <gui/win/container.h>


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace {
      std::map<const char*, class_info> window_class_info_map;
    }

    // --------------------------------------------------------------------------
    namespace native {

      const class_info& get_window_class (const char* class_name) {
        return window_class_info_map[class_name];
      }

      void move (os::window w, const core::point& pt) {
        w->move(pt.os_x(), pt.os_y());
      }

      void resize (os::window w, const core::size& sz) {
        w->resize(sz.os_width(), sz.os_height());
      }

      void place (os::window w, const core::rectangle& r) {
        w->setGeometry(r.os());
      }

      core::rectangle get_geometry (os::window w) {
        return core::rectangle(w->pos(), w->frameSize());
      }

      void prepare(overlapped_window&) {}

      os::window create (const class_info& type,
                         const core::rectangle& r,
                         os::window parent_id,
                         overlapped_window& data) {
        os::window id = new os::qt::Widget(parent_id, type.get_style(), &data);
        Qt::WindowFlags style = id->windowFlags();
        //clog::debug() << "Expected style: " << std::hex << type.get_style() << ", current style: " << std::hex << style;

        id->setGeometry(r.os());
        id->setCursor(type.get_cursor());

        QPalette pal = id->palette();
        pal.setColor(QPalette::Window, QColor(type.get_background()));
        id->setAutoFillBackground(true);
        id->setPalette(pal);
        id->setFocusPolicy(data.can_accept_focus() ? Qt::WheelFocus : Qt::NoFocus);

        if (0 == window_class_info_map.count(type.get_class_name())) {
          window_class_info_map[type.get_class_name()] = type;
        }

        return id;
      }

      void notify_created(window* w) {
        w->notify_event(core::qt::WM_CREATE_WINDOW);
      }

      void destroy (os::window id) {
        if (id) {
          // notify_event(core::qt::WM_DESTROY_WINDOW);
          id->set_window(nullptr);
          id->deleteLater();
        }
      }

      void close (os::window id) {
        if (id) {
          id->close();
        }
      }

      void notify_close (window& w) {
        w.notify_event(QEvent::Close);
      }

      bool is_visible (os::window id) {
        return id && id->isVisible();
      }

      void set_visible (os::window id, bool s) {
        if (id) {
          id->setVisible(s);
        }
      }

      void enable (overlapped_window& w, bool s) {
        if (w.is_valid()) {
          detail::get_os_window(w)->setEnabled(s);
        }
      }

      void to_front (os::window id) {
        if (id) {
          id->stackUnder(id->parentWidget());
        }
      }

      void to_back (os::window id) {
        if (id) {
          id->lower();
        }
      }

      void take_focus (os::window id) {
        if (id) {
          id->setFocus();
        }
      }

      void set_cursor (os::window id, const os::cursor& c) {
        if (id) {
          id->setCursor(c);
        }
      }

      void invalidate (os::window id, const core::rectangle& r) {
        if (id) {
          id->update(r.os());
        }
      }

      void redraw (const window&, os::window id, const core::rectangle& r) {
        if (id) {
          id->repaint(r.os());
        }
      }

      void prepare_accept_focus (os::window id, bool a) {
        if (id) {
          id->setFocusPolicy(a ? Qt::WheelFocus : Qt::NoFocus);
        }
      }

      void prepare_capture_pointer () {}

      void unprepare_capture_pointer () {}

      void capture_pointer (os::window id) {
        if (id) {
          id->grabMouse();
        }
      }

      void uncapture_pointer (os::window id) {
        if (id) {
          id->releaseMouse();
        }
      }

      os::window get_desktop_window () {
        return NULL;
      }

      core::size screen_size () {
        return core::size(core::global::get_instance()->primaryScreen()->availableSize());
      }

      core::rectangle screen_area () {
        return core::rectangle(core::global::get_instance()->primaryScreen()->availableGeometry());
      }

      core::rectangle adjust_overlapped_area (const core::rectangle& r, const class_info&) {
        return r;
      }

      void prepare_overlapped (os::window id, os::window pid) {
      }

      os::window get_overlapped_parent (os::window pid) {
        return pid;
      }

      void set_title (os::window id, const std::string& title) {
        if (id) {
          id->setWindowTitle(QString::fromStdString(title));
        }
      }

      std::string get_title (os::window id) {
        if (id) {
          return id->windowTitle().toStdString();
        }
        return {};
      }

      bool is_maximized (os::window id) {
        return id && id->isMaximized();
      }

      bool is_minimized (os::window id) {
        return id && id->isMinimized();
      }

      bool is_top_most (os::window id) {
        return id && ((id->windowFlags() & Qt::WindowStaysOnTopHint) == Qt::WindowStaysOnTopHint);
      }

      void minimize (os::window id) {
        if (id) {
          id->showMinimized();
        }
      }

      void maximize (os::window id) {
        if (id) {
          id->showMaximized();
        }
      }

      void restore (os::window id) {
        if (id) {
          id->showNormal();
        }
      }

      void set_top_most (os::window id, bool on) {
        if (id) {
          id->setWindowFlag(Qt::WindowStaysOnTopHint, on);
        }
      }

      void prepare_main_window (os::window) {}
      void prepare_popup_window (os::window) {}
      void prepare_dialog_window (os::window, os::window) {}

      void erase (os::bitmap id, os::graphics gc, const core::native_rect& r, os::color c) {
        gc->fillRect(r.x(), r.y(), r.width(), r.height(), c);
      }

      os::bitmap create_surface (const core::native_size& size, os::window id) {
        return new QPixmap(size.width(), size.height());
      }

      void delete_surface (os::bitmap id) {
        delete id;
      }

      os::graphics create_graphics_context (os::bitmap id) {
        return new QPainter(id);
      }

      void delete_graphics_context (os::graphics id) {
        delete id;
      }

      void copy_surface (os::bitmap src, os::drawable target, os::graphics,
                         const core::native_point& from, const core::native_point& to,
                         const core::native_size& size) {
        QPainter p(target);
        p.drawPixmap(to.x(), to.y(), *src, from.x(), from.y(), size.width(), size.height());
      }

    } // namespace native

  } // namespace win

  namespace os {

    namespace qt {

      Widget::Widget (Widget* parent, os::style s, win::overlapped_window* w)
        : QWidget(parent, s)
        , win(w)
      {}

      Widget::~Widget () {
        if (win) {
          win::detail::set_os_window(win, nullptr);
        }
      }

      win::overlapped_window* Widget::get_window () const {
        return win;
      }

      void Widget::set_window (win::overlapped_window* w) {
        win = w;
      }

      Widget* Widget::get_parent () const {
        return static_cast<Widget*>(parentWidget());
      }

      bool Widget::event (QEvent* e) {
        gui::os::event_result result;
        gui::core::event ev = {this, e};
        clog::trace() << "Widget received event: " << ev;
        if (win && win->handle_event(ev, result)) {
          return true;
        }
        return QWidget::event(e);
      }

    } // namespace qt

  } // namespace os

} // namespace gui

#endif // GUIPP_QT
