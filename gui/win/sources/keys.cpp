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
* @brief     C++ API: key definitions
*
* @file
*/


// --------------------------------------------------------------------------
//
// Common includes
//
#include <sstream>
#include <ctype.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "keys.h"

namespace gui {

  namespace win {

    hot_key::hot_key (os::key_symbol k,
             os::key_state modifiers)
      : key(k)
      , modifiers(modifiers)
    {
#ifdef WIN32
      //if (islower(key)) {
      //  key = toupper(key);
      //}
#endif // WIN32
#ifdef X11
      if (isupper(key)) {
        key = tolower(key);
      }
#endif // X11
    }

    std::string key_symbol_to_string (os::key_symbol key) {
#ifdef WIN32
      UINT nScanCode = MapVirtualKey(key, MAPVK_VK_TO_VSC);
      switch (key) {
        // Keys which are "extended" (except for Return which is Numeric Enter as extended)
      case VK_INSERT:
      case VK_DELETE:
      case VK_HOME:
      case VK_END:
      case VK_NEXT:  // Page down
      case VK_PRIOR: // Page up
      case VK_LEFT:
      case VK_RIGHT:
      case VK_UP:
      case VK_DOWN:
        nScanCode |= 0x100; // Add extended bit
      }

      std::vector<char> buffer(32);
      int ret = GetKeyNameText(nScanCode << 16, buffer.data(), static_cast<int>(buffer.capacity()));
      std::string str;
      if (ret) {
        str = buffer.data();
      } else {
        switch (key) {
        case keys::system:
          str = "Win";
          break;
        default:
          str = (char)key;
        }
      }
      return str;
#endif // WIN32
#ifdef X11
      switch (key) {
        case XK_Control_L:
        case XK_Control_R:
        case XK_Alt_L:
        case XK_Alt_R:
        case XK_Shift_L:
        case XK_Shift_R:
        case XK_Super_L:
        case XK_Super_R:
        {
          std::string str = XKeysymToString(key);
          auto i = str.find_last_of('_');
          if (i != std::string::npos) {
            str = str.substr(0, i);
          }
          return str;
        }
        break;

        default:
          return XKeysymToString(key);
        break;
      }
#endif // X11
    }

    std::string key_state_to_string(os::key_state m) {
      return key_symbol_to_string(m);
    }

    // --------------------------------------------------------------------------
    bool hot_key::match (os::key_state m, os::key_symbol k) const {
      return (key && (key == k) && ((m & modifiers) == modifiers));
    }

    std::string hot_key::get_key_string () const {
      if (key && key_string.empty()) {
        std::ostringstream s;
        if (control_key_bit_mask::is_set(modifiers)) {
          s << key_state_to_string(keys::control) << "+";
        }
        if (alt_key_bit_mask::is_set(modifiers)) {
          s << key_state_to_string(keys::alt) << "+";
        }
        if (system_key_bit_mask::is_set(modifiers)) {
          s << key_state_to_string(keys::system) << "+";
        }
        if (shift_key_bit_mask::is_set(modifiers)) {
          s << key_state_to_string(keys::shift) << "+";
        }
        std::string str = key_symbol_to_string(key);
        if (str.length() == 1) {
          for (auto& c: str) {
            c = toupper(c);
          }
        }
        s << str;
        key_string = s.str();
      }
      return key_string;
    }

    // --------------------------------------------------------------------------
    bool hot_key::operator== (const hot_key& rhs) const {
      return (key == rhs.key) && (modifiers == rhs.modifiers);
    }

    bool hot_key::operator< (const hot_key& rhs) const {
      if (key == rhs.key) {
        os::key_state m = rhs.modifiers & ~(state::num_lock | state::scroll_lock | state::caps_lock);
        return (modifiers < m);
      }
      return key < rhs.key;
    }

  } // win

} // gui
