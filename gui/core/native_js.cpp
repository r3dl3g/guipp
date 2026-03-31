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
 * @brief     JS specific native core functions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#ifdef GUIPP_JS

// --------------------------------------------------------------------------
//
// Common includes
//


// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/core/context.h"
#include "gui/core/keys.h"

using namespace emscripten;

namespace gui {

  namespace core {

    namespace native {

      void init_clipping (context& ctx) {
        logging::trace() << "Save clipping";
        ctx.graphics().call<void>("save");
      }

      // --------------------------------------------------------------------------
      void set_clip_rect (context& ctx, const gui::os::rectangle& r) {
        const auto x = gui::os::get_x(r);
        const auto y = gui::os::get_y(r);
        const auto w = gui::os::get_width(r);
        const auto h = gui::os::get_height(r);
        ctx.graphics().call<void>("beginPath");
        ctx.graphics().call<void>("rect", x, y, w, h);
        ctx.graphics().call<void>("clip");
        logging::trace() << "Set clip to (" << x << " ," << y << ", " << w << " ," << h << ")";
        // ctx.graphics().set("strokeStyle", "#FF00FF");
        // ctx.graphics().call<void>("strokeRect", x, y, w, h);
      }

      // --------------------------------------------------------------------------
      void clear_clipping (core::context& ctx) {
        logging::trace() << "Restore clipping";
        ctx.graphics().call<void>("restore");
      }

      // --------------------------------------------------------------------------
      gui::os::graphics create_graphics_context (gui::os::drawable id) {
        val options = val::object();
        options.set("willReadFrequently", true);
        return id.call<val>("getContext", val("2d"), options);
      }

      // --------------------------------------------------------------------------
      void delete_graphics_context (gui::os::graphics id) {
      }

      static std::unordered_map<gui::os::key_symbol, std::string> s_key_symbol_mapping = {
        {core::keys::left,        "Left"},
        {core::keys::right,       "Right"},
        {core::keys::up,          "Up"},
        {core::keys::down,        "Down"},
        {core::keys::page_up,     "PageUp"},
        {core::keys::page_down,   "PageDown"},
        {core::keys::home,        "Home"},
        {core::keys::end,         "End"},
        {core::keys::del,         "Delete"},
        {core::keys::insert,      "Insert"},
        {core::keys::escape,      "Escape"},
        {core::keys::enter,       "Enter"},
        {core::keys::space,       "Space"},
        {core::keys::clear,       "Clear"},
        {core::keys::back_space,  "<-"},
        {core::keys::tab,         "->|"},
        {core::keys::print,       "PrtSc"},
        {core::keys::f1,          "F1"},
        {core::keys::f2,          "F2"},
        {core::keys::f3,          "F3"},
        {core::keys::f4,          "F4"},
        {core::keys::f5,          "F5"},
        {core::keys::f6,          "F6"},
        {core::keys::f7,          "F7"},
        {core::keys::f8,          "F8"},
        {core::keys::f9,          "F9"},
        {core::keys::f10,         "F10"},
        {core::keys::f11,         "F11"},
        {core::keys::f12,         "F12"},
        {core::keys::f13,         "F13"},
        {core::keys::f14,         "F14"},
        {core::keys::f15,         "F15"},
        {core::keys::f16,         "F16"},
        {core::keys::f17,         "F17"},
        {core::keys::f18,         "F18"},
        {core::keys::f19,         "F19"},
        {core::keys::f20,         "F20"},
        {core::keys::f21,         "F21"},
        {core::keys::f22,         "F22"},
        {core::keys::f23,         "F23"},
        {core::keys::f24,         "F24"},
        {core::keys::control,     "Ctrl"},
        {core::keys::alt,         "Alt"},
        {core::keys::shift,       "Shift"},
        {core::keys::system,      "Meta"},
        {core::keys::num_lock,    "NumLock"},
        {core::keys::scroll_lock, "ScrollLock"},
        {core::keys::caps_lock,   "CapsLock"},
        {core::keys::a,           "A"},
        {core::keys::b,           "B"},
        {core::keys::c,           "C"},
        {core::keys::d,           "D"},
        {core::keys::e,           "E"},
        {core::keys::f,           "F"},
        {core::keys::g,           "G"},
        {core::keys::h,           "H"},
        {core::keys::i,           "I"},
        {core::keys::j,           "J"},
        {core::keys::k,           "K"},
        {core::keys::l,           "L"},
        {core::keys::m,           "M"},
        {core::keys::n,           "N"},
        {core::keys::o,           "O"},
        {core::keys::p,           "P"},
        {core::keys::q,           "Q"},
        {core::keys::r,           "R"},
        {core::keys::s,           "S"},
        {core::keys::t,           "T"},
        {core::keys::u,           "U"},
        {core::keys::v,           "V"},
        {core::keys::w,           "W"},
        {core::keys::x,           "X"},
        {core::keys::y,           "Y"},
        {core::keys::z,           "Z"},
        {core::keys::plus,        "+"},
        {core::keys::minus,       "-"},
        {core::keys::asterisk,    "*"},
        {core::keys::comma,       ","},
        {core::keys::period,      "-"},
        {core::keys::slash,       "/"},
        {core::keys::colon,       ":"},
        {core::keys::semicolon,   ";"},
        {core::keys::less,        "<"},
        {core::keys::equal,       "="},
        {core::keys::greater,     ">"},
        {core::keys::question,    "?"},
        {core::keys::at,          "@"}
      };

      // --------------------------------------------------------------------------
      std::string key_symbol_to_string (gui::os::key_symbol key) {
        auto it = s_key_symbol_mapping.find(key);
        if (it != s_key_symbol_mapping.end()) {
          return it->second;
        }
        std::ostringstream s;
        s << static_cast<char>(key);
        return s.str();
      }

      static std::unordered_map<gui::os::key_state, std::string> s_key_state_mapping = {
        {core::state::left_button,    "Mouse left"},
        {core::state::middle_button,  "Mouse middle"},
        {core::state::right_button,   "Mouse right"},
        {core::state::control,        "Ctrl"},
        {core::state::shift,          "Shift"},
        {core::state::alt,            "Alt"},
        {core::state::system,         "Meta"},
        {core::state::num_lock,       "NumLock"},
        {core::state::scroll_lock,    "ScrollLock"},
        {core::state::caps_lock,      "CapsLock"}
      };
      
      // --------------------------------------------------------------------------
      std::string key_state_to_string (gui::os::key_state state) {
        auto it = s_key_state_mapping.find(state);
        if (it != s_key_state_mapping.end()) {
          return it->second;
        }
        std::ostringstream s;
        s << static_cast<int>(state);
        return s.str();
      }

      // --------------------------------------------------------------------------
      namespace js {

        static std::unordered_map<std::string, gui::os::key_symbol> s_key_name_mapping = {
          {"ArrowLeft",   core::keys::left},
          {"ArrowRight",  core::keys::right},
          {"ArrowUp",     core::keys::up},
          {"ArrowDown",   core::keys::down},
          {"PageUp",      core::keys::page_up},
          {"PageDown",    core::keys::page_down},
          {"Home",        core::keys::home},
          {"End",         core::keys::end},
          {"Delete",      core::keys::del},
          {"Insert",      core::keys::insert},
          {"Escape",      core::keys::escape},
          {"Enter",       core::keys::enter},
          {"Space",       core::keys::space},
          {"NumpadClear", core::keys::clear},
          {"Backspace",   core::keys::back_space},
          {"Tab",         core::keys::tab},
          {"PrintScreen", core::keys::print},
          {"F1",          core::keys::f1},
          {"F2",          core::keys::f2},
          {"F3",          core::keys::f3},
          {"F4",          core::keys::f4},
          {"F5",          core::keys::f5},
          {"F6",          core::keys::f6},
          {"F7",          core::keys::f7},
          {"F8",          core::keys::f8},
          {"F9",          core::keys::f9},
          {"F10",         core::keys::f10},
          {"F11",         core::keys::f11},
          {"F12",         core::keys::f12},
          {"F13",         core::keys::f13},
          {"F14",         core::keys::f14},
          {"F15",         core::keys::f15},
          {"F16",         core::keys::f16},
          {"F17",         core::keys::f17},
          {"F18",         core::keys::f18},
          {"F19",         core::keys::f19},
          {"F20",         core::keys::f20},
          {"F21",         core::keys::f21},
          {"F22",         core::keys::f22},
          {"F23",         core::keys::f23},
          {"F24",         core::keys::f24},
          {"ControlLeft", core::keys::control},
          {"AltLeft",     core::keys::alt},
          {"ShiftLeft",   core::keys::shift},
          {"MetaLeft",    core::keys::system},
          {"ControlRight",core::keys::control},
          {"AltRight",    core::keys::alt},
          {"ShiftRight",  core::keys::shift},
          {"MetaRight",   core::keys::system},
          {"NumLock",     core::keys::num_lock},
          {"ScrollLock",  core::keys::scroll_lock},
          {"CapsLock",    core::keys::caps_lock},
          {"KeyA",        core::keys::a},
          {"KeyB",        core::keys::b},
          {"KeyC",        core::keys::c},
          {"KeyD",        core::keys::d},
          {"KeyE",        core::keys::e},
          {"KeyF",        core::keys::f},
          {"KeyG",        core::keys::g},
          {"KeyH",        core::keys::h},
          {"KeyI",        core::keys::i},
          {"KeyJ",        core::keys::j},
          {"KeyK",        core::keys::k},
          {"KeyL",        core::keys::l},
          {"KeyM",        core::keys::m},
          {"KeyN",        core::keys::n},
          {"KeyO",        core::keys::o},
          {"KeyP",        core::keys::p},
          {"KeyQ",        core::keys::q},
          {"KeyR",        core::keys::r},
          {"KeyS",        core::keys::s},
          {"KeyT",        core::keys::t},
          {"KeyU",        core::keys::u},
          {"KeyV",        core::keys::v},
          {"KeyW",        core::keys::w},
          {"KeyX",        core::keys::x},
          {"KeyY",        core::keys::y},
          {"KeyZ",        core::keys::z},
          {"Plus",        core::keys::plus},
          {"Minus",       core::keys::minus},
          {"Asterisk",    core::keys::asterisk},
          {"Comma",       core::keys::comma},
          {"Period",      core::keys::period},
          {"Slash",       core::keys::slash},
          {"Colon",       core::keys::colon},
          {"Semicolon",   core::keys::semicolon},
          {"Less",        core::keys::less},
          {"Equal",       core::keys::equal},
          {"Greater",     core::keys::greater},
          {"Question",    core::keys::question},
          {"At",          core::keys::at}
        };

        // --------------------------------------------------------------------
        gui::os::key_symbol key_name_to_symbol (const std::string& name) {
          auto it = s_key_name_mapping.find(name);
          if (it != s_key_name_mapping.end()) {
            return it->second;
          }
          return 0;
        }

      } // namespace js

    } // namespace native

  } // namespace core

} // namespace gui

#endif // GUIPP_JS
