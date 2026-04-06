/**
 * @copyright (c) 2026-2026 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     JS specific native system API calls
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#if defined(GUIPP_JS)

// --------------------------------------------------------------------------
//
// Common includes
//
#include <emscripten.h>
#include <map>
#include <iomanip>
#include <logging/logger.h>
#include <util/ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/native.h"
#include "gui/win/overlapped_window.h"

using namespace emscripten;

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace {
      std::map<const char*, class_info> window_class_info_map;
    }

    // --------------------------------------------------------------------------
    namespace native {

      namespace js {

        void send_to_main (const std::string& type, const std::string& message) {
            val self = val::global("self");

            // Wir erstellen ein leeres JS-Objekt
            val data = val::object();
            data.set("type", type);
            data.set("payload", message);
            data.set("timestamp", val::global("Date").call<val>("now"));

            // Nachricht an den Main-Thread senden
            self.call<void>("postMessage", data);
        }

        void send_to_main (const std::string& type, int id) {
            val self = val::global("self");

            // Wir erstellen ein leeres JS-Objekt
            val data = val::object();
            data.set("type", type);
            data.set("payload",  id);
            data.set("timestamp", val::global("Date").call<val>("now"));

          logging::trace() << "send to main: " << type << " id: " << id;

            // Nachricht an den Main-Thread senden
            self.call<void>("postMessage", data);
        }

      } // namespace js

      const class_info& get_window_class (const char* class_name) {
        return window_class_info_map[class_name];
      }

      void register_window_class (const class_info& type) {
        if (0 == window_class_info_map.count(type.get_class_name())) {
          window_class_info_map[type.get_class_name()] = type;
        }
      }

    void register_utf8_window (const window&) {}

    void unregister_utf8_window (const window&) {}

      typedef std::map<os::window, win::overlapped_window*> window_map;
      window_map global_window_map;

      overlapped_window* get_window (os::window id) {
        return global_window_map[id];
      }

      void set_os_window (overlapped_window* win, os::window id) {
        global_window_map[id] = win;
        if (win) {
          win->set_os_window(id);
        }
      }

      void unset_os_window (os::window id) {
        global_window_map.erase(id);
      }

      void move (os::window w, const core::point& pt) {
        // const auto npt = core::global::scale_to_native(pt);

        // val style = w["style"];
        // style.set("cssText", ostreamfmt("position:absolute; top:" 
        //     << npt.y() << "px;left:" << npt.x() << "px;"));
      }

      void resize (os::window w, const core::size& sz) {
        // w.set("width", sz.os_width());
        // w.set("height", sz.os_height());
      }

      void geometry (os::window w, const core::rectangle& r) {
        // const auto nr = core::global::scale_to_native(r);

        // w.set("width", nr.width());
        // w.set("height", nr.height());

        // val style = w["style"];
        // style.set("cssText", ostreamfmt("position:absolute; top:" 
        //     << nr.y() << "px;left:" << nr.x() << "px;" << "width:" 
        //     << nr.width() << "px;height:" << nr.height() << "px;"));
      }

      void notify_move (window& w, const core::point& pt, const core::point& ) {
        gui::os::event_result result = 0;
        gui::core::event e{{}, os::js::event_type::Move, pt};
        w.handle_event(e, result);
      }

      void notify_resize (window& w, const core::size& sz, const core::size& old) {
        gui::os::event_result result = 0;
        gui::core::event e{{}, os::js::event_type::Size, sz};
        w.handle_event(e, result);
      }

      core::native_point get_position (os::window) {
        return core::native_point::zero;
      }

      core::size client_size (os::window id, const core::size&) {
        return screen_size();
      }

      void prepare(overlapped_window&) {}

      os::window create (const class_info& type,
                         const core::rectangle& r,
                         os::window parent_id,
                         overlapped_window& data) {

        // const auto nr = core::global::scale_to_native(r);

        auto canvas = core::global::get_instance();//.call<val>("createElement", std::string("canvas"));

        auto self = emscripten::val::global("self");
        auto rect = self["rect"];
        int x = rect["x"].as<int>();
        int y = rect["y"].as<int>();
        int width = rect["width"].as<int>();
        int height = rect["height"].as<int>();

        data.geometry(core::rectangle(x, y, width, height), false, false);

        // auto ctx = canvas.call<val>("getContext", std::string("2d"));

        // val body = core::global::get_instance()["body"];
        // body.call<void>("appendChild", canvas);
        // canvas.set("width", nr.width());
        // canvas.set("height", nr.height());
        // val style = canvas["style"];

        // style.set("cssText", ostreamfmt("position:absolute; top:" 
        //     << nr.y() << "px;left:" << nr.x() << "px;" << "width:" 
        //     << nr.width() << "px;height:" << nr.height() << "px;"));

        set_os_window(&data, canvas);

        return canvas;
      }

      bool is_valid (const os::window& id) {
        return !(id.isUndefined() || id.isNull());
      }

      void destroy (os::window id) {
        //if (is_valid(id)) {
          // logging::debug() << "Call remove";
          // id.call<void>("remove");
          // logging::debug() << "id = val::undefined()";
          // id = val::undefined();
        //}
      }

      void notify_close (window& w) {
        w.notify_event(os::js::event_type::Close);
      }

      void prepare_win_for_event (const overlapped_window&) {}

      bool is_visible (os::window id) {
        return is_valid(id);
        // if (is_valid(id)) {
        //   auto visibility = id["visibility"].as<std::string>();
        //   return visibility != "hidden";
        // }
        // return false;
      }

      void set_visible (os::window id, bool s) {
        // if (is_valid(id)) {
        //   id.set("visibility", std::string(s ? "visible" : "hidden"));
        // }
      }

      void enable (overlapped_window&, bool) {
      }

      void to_front (os::window id) {
        // if (is_valid(id)) {
        //   id.set("z-index", 10);
        // }
      }

      void to_back (os::window id) {
        // if (is_valid(id)) {
        //   id.set("z-index", 0);
        // }
      }

      void take_focus (os::window) {
      }

      void set_cursor (os::window id, const os::cursor& c) {
        js::send_to_main("cursor", c);
      }

      void invalidate (os::window id, const core::native_rect&) {}

      void redraw (window&, os::window id, const core::native_rect&) {}

      void prepare_accept_focus (os::window, bool) {}

      void capture_pointer (os::window id) {}

      void uncapture_pointer (os::window id) {}

      os::window get_desktop_window () {
        return val::undefined();
      }

      core::size screen_size () {
        auto self = emscripten::val::global("self");
        auto rect = self["rect"];
        int width = rect["width"].as<int>();
        int height = rect["height"].as<int>();
        return core::global::scale_from_native(core::native_size(width, height));
      }

      core::rectangle screen_area () {
        return {core::point::zero, screen_size()};
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
        // if (is_valid(id)) {
        //   core::global::get_instance().set("title", title);
        // }
      }

      std::string get_title (os::window id) {
        // if (is_valid(id)) {
        //   return core::global::get_instance()["title"].as<std::string>();
        // }
        return {};
      }

      bool is_maximized (os::window id) {
        return false;
      }

      bool is_minimized (os::window id) {
        return false;
      }

      bool is_top_most (os::window id) {
        // if (is_valid(id)) {
        //   return id["z-index"].as<int>() > 0;
        // }
        return false;
      }

      bool is_fullscreen (os::window id) {
        return false;
      }

      void minimize (os::window id) {}

      void maximize (os::window id) {}

      void restore (os::window id) {}

      void set_top_most (os::window id, bool on) {
        if (on) {
          to_front(id);
        } else {
          to_back(id);
        }
      }

      void set_fullscreen (os::window id, bool on) {
      }

      void prepare_main_window (os::window) {
        logging::trace() << "prepare_main_window";
      }
      void prepare_popup_window (os::window) {}
      void prepare_tooltip_window (os::window) {}
      void prepare_dialog_window (os::window, os::window) {}
      void prepare_palette_window (os::window) {}

      void erase (os::drawable id, os::graphics gc, const core::native_rect& r, os::color c) {
        gc.set("fillStyle", ostreamfmt("#" << std::hex << std::setfill('0') << std::setw(6) << (c & 0xffffff)));
        gc.call<void>("fillRect", r.x(), r.y(), r.width(), r.height());
      }

      void frame (os::drawable id, os::graphics gc, const core::native_rect& r, os::color c) {
        gc.set("strokeStyle", ostreamfmt("#" << std::hex << std::setfill('0') << std::setw(6) << (c & 0xffffff)));
        gc.call<void>("strokeRect", r.x(), r.y(), r.width(), r.height());
      }

      os::backstore create_surface (const core::native_size&, os::window id) {
        return id;
      }

      void delete_surface (os::backstore id) {
      }

      void send_client_message (window* win, os::message_type message, const core::rectangle& r) {
        if (win && win->is_valid()) {
          gui::os::event_result result = 0;
          gui::core::event e{{}, message, r};
          win->handle_event(e, result);
        }
      }

      void send_client_message (window* win, os::message_type message, const float f) {
        if (win && win->is_valid()) {
          gui::os::event_result result = 0;
          gui::core::event e{{}, message, f};
          win->handle_event(e, result);
        }
      }

      void send_client_message (window* win, os::message_type message, int l1, int l2) {
        if (win && win->is_valid()) {
          gui::os::event_result result = 0;
          gui::core::event e{{}, message, l1, l2};
          win->handle_event(e, result);
        }
      }

      void send_client_message (window* win, os::message_type message, core::context& ctx, const core::native_rect& r) {
        if (win && win->is_valid()) {
          gui::os::event_result result = 0;
          gui::core::event e{{}, message, &ctx, &r};
          win->handle_event(e, result);
        }
      }

      void send_mouse_event (window* win, bool enter) {
        if (win && win->is_valid()) {
          gui::os::event_result result = 0;
          gui::core::event e{{}, enter ? os::js::event_type::MouseEnter : os::js::event_type::MouseLeave};
          win->handle_event(e, result);
        }
      }

      void send_notify_visibility (window* win, bool visible) {
        if (win && win->is_valid()) {
          gui::os::event_result result = 0;
          gui::core::event e{{}, visible ? os::js::event_type::Show : os::js::event_type::Hide};
          win->handle_event(e, result);
        }
      }

      core::native_point get_current_pointer_pos (gui::os::window id) {
        return {};
      }

    } // namespace native

  } // namespace win

} // namespace gui

#endif // GUIPP_JS
