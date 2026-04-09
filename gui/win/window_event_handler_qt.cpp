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
 * @brief     generic window events
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_QT

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/window_event_handler.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    os::key_state get_key_state (const core::event& e) {
      return e.cast<QKeyEvent>().modifiers();
    }

    os::key_symbol get_key_symbol (const core::event& e) {
      return e.cast<QKeyEvent>().key();
    }

    std::string get_key_chars (const core::event& e) {
      return e.cast<QKeyEvent>().text().toStdString();
    }

    // --------------------------------------------------------------------------
    QClientEvent::QClientEvent (Type type, const core::rectangle& rect)
      : QEvent(type)
      , m_rect(rect)
      , m_l1(0)
      , m_l2(0)
    {}

    QClientEvent::QClientEvent (Type type, long l1, long l2)
      : QEvent(type)
      , m_l1(l1)
      , m_l2(l2)
    {}

    QClientEvent::QClientEvent (Type type)
      : QEvent(type)
      , m_l1(0)
      , m_l2(0)
    {}

    QClientEvent::~QClientEvent ()
    {}

    const core::rectangle &QClientEvent::rect() const {
      return m_rect;
    }

    long QClientEvent::l1 () const {
      return m_l1;
    }

    long QClientEvent::l2 () const {
      return m_l2;
    }

    // --------------------------------------------------------------------------
    template<>
    long get_param<0> (const core::event& e) {
      return e.cast<QClientEvent>().l1();
    }

    template<>
    long get_param<1> (const core::event& e) {
      return e.cast<QClientEvent>().l2();
    }

    core::native_point::type get_wheel_delta_x (const core::event& e) {
      auto& we = e.cast<QWheelEvent>();
      os::point numPixels = we.pixelDelta();
      if (numPixels.isNull()) {
        return we.angleDelta().x() / 15;
      }
      return numPixels.x();
    }

    core::native_point::type get_wheel_delta_y (const core::event& e) {
      auto& we = e.cast<QWheelEvent>();
      os::point numPixels = we.pixelDelta();
      if (numPixels.isNull()) {
        return we.angleDelta().y() / 15;
      }
      return numPixels.y();
    }

    core::point get_move_point (const core::event& e) {
      const auto p = e.cast<QMoveEvent>().pos();
      return core::global::scale_from_native(core::native_point(p.x(), p.y()));
    }

    core::native_point get_wheel_point (const core::event& e) {
#if QT_VERSION > QT_VERSION_CHECK(5, 13, 0)
      const auto pt = e.cast<QWheelEvent>().position();
      return core::native_point(pt.x(), pt.y());
#else
      return core::native_point(e.cast<QWheelEvent>().x(), e.cast<QWheelEvent>().y());
#endif
    }

    // --------------------------------------------------------------------------
    core::rectangle get_client_data_rect (const core::event& e) {
      return e.cast<QClientEvent>().rect();
    }

    core::context* get_context (const core::event& e) {
      return (core::context*)e.cast<QClientEvent>().l1();
    }

    const core::native_rect* get_paint_rect (const core::event& e) {
      return (const core::native_rect*)(e.cast<QClientEvent>().l2());
    }

    bool is_mouse_event (const core::event& e) {
      switch (e.type()) {
        case QEvent::MouseMove:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::DragEnter:
        case QEvent::DragMove:
        case QEvent::DragLeave:
//        case QEvent::Enter:
//        case QEvent::Leave:
        case QEvent::MouseTrackingChange:
        case QEvent::Wheel:
          return true;
        default:
          return false;
      }
    }

    core::native_point get_mouse_event_point (const core::event& e) {
      return mouse_move_event::Caller::get_param<1>(e);
    }

    bool is_key_event (const core::event& e) {
      switch (e.type()) {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
          return true;
        default:
          return false;
      }
    }

    bool is_hotkey_event (const core::event& e) {
      return (e.type() == QEvent::KeyPress);
    }

  } // win

} // gui

#endif // GUIPP_QT
