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
 * @brief     system specific window event handler function
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_QT


// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <set>
#include <thread>

#include <QtGui/QGuiApplication>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>

#include <logging/logger.h>
#include <util/robbery.h>
#include <util/blocking_queue.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/win/overlapped_window.h"
#include "gui/win/window_event_proc.h"
#include "gui/win/dbg_win_message.h"
#include "gui/win/native.h"
#include "gui/win/clipboard.h"


namespace util {

  namespace robbery {

#ifdef _LIBCPP_THREAD
    using thread_id = jugger<std::thread::native_handle_type, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::__id_>;
#elif linux
    using thread_id = jugger<std::thread::native_handle_type, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::_M_thread>;
#elif USE_MINGW
# if __MINGW_GCC_VERSION > 100000
    using thread_id = jugger<std::thread::native_handle_type, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::_M_thread>;
# else
    using thread_id = jugger<DWORD, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::mId>;
# endif
#elif WIN32
    using thread_id = jugger<_Thrd_id_t, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::_Id>;
#endif // GUIPP_WIN

    thread_id::type get_native_thread_id (std::thread::id& t) {
      return t.*rob(thread_id());
    }

  } // namespace robbery

} // namespace util

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace detail {

      typedef std::pair<os::window, core::hot_key::call> hot_key_entry;
      typedef std::map<core::hot_key, hot_key_entry> hot_key_map;
      hot_key_map hot_keys;

#if MINGW
      static std::thread::_M_id main_thread_id;
#else
      static std::thread::id main_thread_id;
#endif

      class ShortcutFilter : public QObject {
      public:
        typedef QObject super;

        bool eventFilter (QObject *watched, QEvent *event) override;
      };

      bool ShortcutFilter::eventFilter (QObject *watched, QEvent *event) {
        core::event ev{dynamic_cast<os::qt::window*>(watched), event};
        if (detail::check_message_filter(ev)) {
          return true;
        }
        if (check_hot_key(ev)) {
          return true;
        }
        return false;
      }

      void install_message_filter () {
        static ShortcutFilter* shortcut_filter = nullptr;

        if (!shortcut_filter) {
          shortcut_filter = new ShortcutFilter();
          core::global::get_instance()->installEventFilter(shortcut_filter);
        }
      }

      // --------------------------------------------------------------------------
      bool check_hot_key (const core::event& e) {
        if (e.type() == QEvent::KeyPress) {
          core::hot_key hk(get_key_symbol(e), get_key_state(e));
#if KEY_DEBUG
          logging::debug() << "Check hot key '" << hk.get_key_string() << "'";
#endif
          auto i = hot_keys.find(hk);
          if (i != hot_keys.end()) {
            i->second.second();
            return true;
          }
        }

        return false;
      }

    }

    namespace global {

      // --------------------------------------------------------------------------
      void register_hot_key (const core::hot_key& hk, const core::hot_key::call& fn, window* win) {
        os::window root = {};
#if KEY_DEBUG
        logging::debug() << "Register hot key '" << hk.get_key_string() << "'";
#endif
        if (win && win->is_valid()) {
          root = win->get_overlapped_window().get_os_window();
        } else {
          auto top_windows = QGuiApplication::topLevelWindows();
          if (!top_windows.isEmpty()) {
            root = (os::window)top_windows.first();
          }
        }
        detail::install_message_filter();
        if (root) {
          detail::hot_keys.emplace(hk, std::make_pair(root, fn));
        } else {
          logging::warn() << "Root Windows is not yet created. Hotkey could not be registered!";
        }
      }

      void unregister_hot_key (const core::hot_key& hk) {
        auto i = detail::hot_keys.find(hk);
        if (i == detail::hot_keys.end()) {
          return;
        }

        detail::hot_keys.erase(i);
      }

      window* get_current_focus_window () {
        return native::get_window(static_cast<os::window>(QGuiApplication::focusWindow()));
      }

      overlapped_window& get_application_main_window() {
        auto list = QGuiApplication::topLevelWindows();
        if (list.size() > 0) {
          auto* win = native::get_window(static_cast<os::window>(list.first()));
          if (win) {
            return win->get_overlapped_window();
          }
        }
        throw std::runtime_error("application_main_window could not be found!");
      }

      std::thread::id get_current_thread_id () {
        return std::this_thread::get_id();
      }

    } // global


    bool is_button_event_outside (const window& w, const core::event& e) {
      switch (e.type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease: {
          const QMouseEvent& me = e.cast<QMouseEvent>();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
          const auto pt = me.globalPosition();
#else
          const auto pt = me.globalPos();
#endif
          return !w.absolute_geometry().is_inside(core::global::scale_from_native(core::native_point(pt.x(), pt.y())));
        }
        default: break;
      }
      return false;
    }

    // --------------------------------------------------------------------------
    int run_loop (volatile bool& running, const detail::filter_call& filter) {

      running = true;

      QEventLoop event_loop;
      return event_loop.exec(QEventLoop::AllEvents);
    }

    namespace {
      bool main_loop_is_running = false;
    }

    int run_main_loop () {
      main_loop_is_running = true;
      detail::main_thread_id = global::get_current_thread_id();
      return gui::core::global::get_instance()->exec();
    }

    void quit_main_loop () {
      logging::trace() << "Received quit_main_loop()";
      main_loop_is_running = false;
      core::global::fini();
      gui::core::global::get_instance()->exit();
    }

    void run_on_main (const window& w, const std::function<void()>& action) {
      QTimer* timer = new QTimer();
      timer->moveToThread(qApp->thread());
      timer->setSingleShot(true);
      QObject::connect(timer, &QTimer::timeout, [=]()
      {
        // main thread
        action();
        timer->deleteLater();
      });
      QMetaObject::invokeMethod(timer, "start", Qt::QueuedConnection, Q_ARG(int, 0));
    }
  }   // win
} // gui

#endif // GUIPP_QT