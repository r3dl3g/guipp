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

#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtGui/QPainter>
#include <QtGui/QBitmap>
#include <QtGui/QBackingStore>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <gui/win/native.h>
#include <gui/win/overlapped_window.h>


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

      void register_window_class (const class_info& type) {
        if (0 == window_class_info_map.count(type.get_class_name())) {
          window_class_info_map[type.get_class_name()] = type;
        }
      }

      void move (os::window w, const core::point& pt) {
        const auto npt = core::global::scale_to_native(pt);
        w->setPosition(npt.x(), npt.y());
      }

      void resize (os::window w, const core::size& sz) {
        w->resize(sz.os_width(), sz.os_height());
      }

      void geometry (os::window w, const core::rectangle& r) {
        const auto nr = core::global::scale_to_native(r);
        w->setGeometry(nr.x(), nr.y(), nr.width(), nr.height());
      }

      void notify_move (window& w, const core::point& pt, const core::point& old) {
        const auto npt = core::global::scale_to_native(pt);
        const auto nold = core::global::scale_to_native(old);
        QMoveEvent qe({npt.x(), npt.y()}, {nold.x(), nold.y()});
        gui::os::event_result result = 0;
        gui::core::event e{0, &qe};
        w.handle_event(e, result);
      }

      void notify_resize (window& w, const core::size& sz, const core::size& old) {
        QResizeEvent qe(sz.os(), old.os());
        gui::os::event_result result = 0;
        gui::core::event e{0, &qe};
        w.handle_event(e, result);
      }

      core::native_point get_position (os::window w) {
        const auto pt = w->position();
        return {pt.x(), pt.y()};
      }

      core::size client_size (os::window id, const core::size&) {
        return core::global::scale_from_native(core::native_size(id->width(), id->height()));
      }

      void prepare(overlapped_window&) {}

      os::window create (const class_info& type,
                         const core::rectangle& r,
                         os::window parent_id,
                         overlapped_window& data) {
        os::window id = new os::qt::window(parent_id, type.get_style(), &data);
        Qt::WindowFlags style = id->flags();
        const auto nr = core::global::scale_to_native(r);
        //clog::debug() << "Expected style: " << std::hex << type.get_style() << ", current style: " << std::hex << style;

        id->setGeometry(nr.x(), nr.y(), nr.width(), nr.height());
        id->setCursor(type.get_cursor());

        return id;
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

      void prepare_win_for_event (const overlapped_window&) {}

      bool is_visible (os::window id) {
        return id && id->isVisible();
      }

      void set_visible (os::window id, bool s) {
        if (id) {
          id->setVisible(s);
        }
      }

      void enable (overlapped_window&, bool) {
      }

      void to_front (os::window id) {
        if (id) {
          id->raise();
        }
      }

      void to_back (os::window id) {
        if (id) {
          id->lower();
        }
      }

      void take_focus (os::window) {
      }

      void set_cursor (os::window id, const os::cursor& c) {
        if (id) {
          id->setCursor(c);
        }
      }

      void invalidate (os::window id, const core::native_rect&) {
        if (id) {
          id->requestUpdate();
        }
      }

      void redraw (window&, os::window id, const core::native_rect&) {
        if (id) {
          id->requestUpdate();
        }
      }

      void prepare_accept_focus (os::window, bool) {}

#define NO_CAPTUREx

      void capture_pointer (os::window id) {
#ifndef NO_CAPTURE
        if (id) {
          id->setMouseGrabEnabled(true);
        }
#endif // NO_CAPTURE
      }

      void uncapture_pointer (os::window id) {
#ifndef NO_CAPTURE
        if (id) {
          id->setMouseGrabEnabled(false);
        }
#endif // NO_CAPTURE
      }

      os::window get_desktop_window () {
        return NULL;
      }

      core::size screen_size () {
        return core::size(core::global::get_instance()->primaryScreen()->availableSize());
      }

      core::rectangle screen_area () {
        const auto nr = core::global::get_instance()->primaryScreen()->availableGeometry();
        return core::global::scale_from_native(core::native_rect(nr.x(), nr.y(), nr.width(), nr.height()));
      }

      core::rectangle adjust_overlapped_area (const core::rectangle& r, const class_info&) {
        return r;
      }

      core::native_point surface_to_screen (os::window id, const core::native_point& pt) {
        return pt + get_position(id);
      }

      core::native_point screen_to_surface (os::window id, const core::native_point& pt) {
        return pt - get_position(id);
      }

      void prepare_overlapped (os::window id, os::window pid) {
      }

      os::window get_overlapped_parent (os::window pid) {
        return get_desktop_window();
      }

      void set_title (os::window id, const std::string& title) {
        if (id) {
          id->setTitle(QString::fromStdString(title));
        }
      }

      std::string get_title (os::window id) {
        if (id) {
          return id->title().toStdString();
        }
        return {};
      }

      bool is_maximized (os::window id) {
        return id && (id->visibility() == QWindow::Maximized);
      }

      bool is_minimized (os::window id) {
        return id && (id->visibility() == QWindow::Minimized);
      }

      bool is_top_most (os::window id) {
        return id && id->isTopLevel();
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
          id->setFlag(Qt::WindowStaysOnTopHint, on);
        }
      }

      void prepare_main_window (os::window) {}
      void prepare_popup_window (os::window) {}
      void prepare_dialog_window (os::window, os::window) {}

      void erase (os::drawable id, os::graphics gc, const core::native_rect& r, os::color c) {
        const int sc = static_cast<int>(core::global::get_scale_factor());
        gc->fillRect(r.x(), r.y(), r.width() - sc, r.height() - sc, c);
      }

      void frame (os::drawable id, os::graphics gc, const core::native_rect& r, os::color c) {
        gc->setPen(QPen(c));
        gc->setBrush(QBrush(Qt::BrushStyle::NoBrush));
        gc->drawRect(r.x(), r.y(), r.width(), r.height());
      }

      os::backstore create_surface (const core::native_size& size, os::window id) {
        auto bs = new QBackingStore(id);
        bs->resize(QSize(size.width(), size.height()));
        return bs;
      }

      void delete_surface (os::backstore id) {
        delete id;
      }

      void copy_surface (os::bitmap src, os::drawable target, os::graphics,
                         const core::native_point& from, const core::native_point& to,
                         const core::native_size& size) {
        QPainter p(target);
        p.drawPixmap(to.x(), to.y(), *src, from.x(), from.y(), size.width(), size.height());
      }

      void send_client_message (window* win, os::message_type message, const core::rectangle& r) {
        if (win && win->is_valid()) {
          gui::os::event_result result;
          QClientEvent e(static_cast<QEvent::Type>(message), r);
          win->handle_event(gui::core::event{nullptr, &e}, result);
        }
      }

      void send_client_message (window* win, os::message_type message, const core::size& sz) {
        send_client_message(win, message, core::rectangle(sz));
      }

      void send_client_message (window* win, os::message_type message, long l1, long l2) {
        if (win && win->is_valid()) {
          gui::os::event_result result;
          QClientEvent e(static_cast<QEvent::Type>(message), l1, l2);
          win->handle_event(gui::core::event{nullptr, &e}, result);
        }
      }

      void send_client_message (window* win, os::message_type message, void* v1, void* v2) {
        send_client_message(win, message, reinterpret_cast<std::uintptr_t>(v1), reinterpret_cast<std::uintptr_t>(v2));
      }

      void send_mouse_event (window* win, bool enter) {
        if (win && win->is_valid()) {
          gui::os::event_result result;
          QClientEvent e(enter ? QEvent::Enter : QEvent::Leave);
          win->handle_event(gui::core::event{nullptr, &e}, result);
        }
      }

      void send_notify_visibility (window* win, bool visible) {
        if (win && win->is_valid()) {
          gui::os::event_result result;
          if (visible) {
            QShowEvent e;
            win->handle_event(gui::core::event{nullptr, &e}, result);
          } else {
            QHideEvent e;
            win->handle_event(gui::core::event{nullptr, &e}, result);
          }
        }
      }

    } // namespace native

  } // namespace win

  namespace os {

    namespace qt {

      window::window (window* parent, os::style s, win::overlapped_window* w)
        : super(parent)
        , win(w)
      {
        setFlags(s);
      }

      window::~window () {
        if (win) {
          win::detail::set_os_window(win, nullptr);
        }
      }

      win::overlapped_window* window::get_window () const {
        return win;
      }

      void window::set_window (win::overlapped_window* w) {
        win = w;
      }

      window* window::get_parent () const {
        return static_cast<window*>(parent());
      }

      bool window::event (QEvent* e) {
        gui::os::event_result result;
        gui::core::event ev = {this, e};
//        clog::info() << "window received event: " << ev;
        if (win && win->handle_event(ev, result)) {
          return true;
        }
        return super::event(e);
      }

    } // namespace qt

  } // namespace os

} // namespace gui

#endif // GUIPP_QT
