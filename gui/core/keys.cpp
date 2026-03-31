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
 * @brief     key definitions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */


// --------------------------------------------------------------------------
//
// Common includes
//
#include <sstream>
#include <cctype>
#include <logging/logger.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/core/keys.h"


namespace gui {

  namespace core {

    hot_key::hot_key (gui::os::key_symbol k,
                      gui::os::key_state modifiers)
      : key(k)
      , modifiers(modifiers)
    {
#ifdef GUIPP_WIN
      //if (islower(key)) {
      //  key = toupper(key);
      //}
#endif // GUIPP_WIN
#ifdef GUIPP_X11
      try {
        if (((int)key < 0xFF) && isupper(key)) {
          key = tolower(key);
        }
      } catch (std::exception& ex) {
        logging::fatal() << "Exception in isupper(0x" << std::hex << (int)key << "):" << ex.what();
      } catch (...) {
        logging::fatal() << "Unknown exception in isupper(0x" << std::hex << (int)key << "):";
      }
#endif // GUIPP_X11
    }

    // --------------------------------------------------------------------------
    std::string hot_key::get_key_string () const {
      if (key && key_string.empty()) {
        std::ostringstream s;
        if (control_key_bit_mask::is_set(modifiers)) {
          s << core::native::key_state_to_string(state::control) IF_QT_ELSE(, << "+");
        }
        if (alt_key_bit_mask::is_set(modifiers)) {
          s << core::native::key_state_to_string(state::alt) IF_QT_ELSE(, << "+");
        }
        if (system_key_bit_mask::is_set(modifiers)) {
          s << core::native::key_state_to_string(state::system) IF_QT_ELSE(, << "+");
        }
        if (shift_key_bit_mask::is_set(modifiers)) {
          s << core::native::key_state_to_string(state::shift) IF_QT_ELSE(, << "+");
        }
        std::string str = core::native::key_symbol_to_string(key);
        if (str.length() == 1) {
          for (auto& c : str) {
            c = toupper(c);
          }
        }
        s << str;
        key_string = s.str();
      }
      return key_string;
    }

    // --------------------------------------------------------------------------
    bool hot_key::operator< (const hot_key& rhs) const {
      if (key == rhs.key) {
        gui::os::key_state m = rhs.modifiers & ~(state::num_lock | state::scroll_lock | state::caps_lock);
        return (modifiers < m);
      }
      return key < rhs.key;
    }

  } // win

} // gui
