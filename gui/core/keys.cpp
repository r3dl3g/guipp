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
 * @brief     C++ API: key definitions
 *
 * @file
 */


// --------------------------------------------------------------------------
//
// Common includes
//
#include <sstream>
#include <cctype>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <gui/core/keys.h>


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

    std::string key_symbol_to_string (gui::os::key_symbol key) {
#ifdef GUIPP_WIN
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
        case core::keys::system:
          str = "Win";
          break;
        default:
          str = (char)key;
        }
      }
      return str;
#elif GUIPP_X11
      switch (key) {
      case XK_Control_L:
      case XK_Control_R:
      case XK_Alt_L:
      case XK_Alt_R:
      case XK_Shift_L:
      case XK_Shift_R:
      case XK_Super_L:
      case XK_Super_R: {
        std::string str = XKeysymToString(key);
        auto i = str.find_last_of('_');
        if (i != std::string::npos) {
          str = str.substr(0, i);
        }
        return str;
      }
      break;
      case ControlMask: return key_symbol_to_string(XK_Control_L);
      case ShiftMask:   return key_symbol_to_string(XK_Shift_L);
      case Mod1Mask:    return key_symbol_to_string(XK_Alt_L);
      case Mod4Mask:    return key_symbol_to_string(XK_Super_L);

      default:
        return XKeysymToString(key);
        break;
      }
#elif GUIPP_QT
      return QKeySequence(key).toString().toStdString();
#else
# error Undefined system: std::string key_symbol_to_string (gui::os::key_symbol key)
#endif // GUIPP_X11
    }

    std::string key_state_to_string (gui::os::key_state m) {
      return key_symbol_to_string(m);
    }

    // --------------------------------------------------------------------------
    std::string hot_key::get_key_string () const {
      if (key && key_string.empty()) {
        std::ostringstream s;
        if (control_key_bit_mask::is_set(modifiers)) {
          s << key_state_to_string(state::control) IF_QT_ELSE(, << "+");
        }
        if (alt_key_bit_mask::is_set(modifiers)) {
          s << key_state_to_string(state::alt) IF_QT_ELSE(, << "+");
        }
        if (system_key_bit_mask::is_set(modifiers)) {
          s << key_state_to_string(state::system) IF_QT_ELSE(, << "+");
        }
        if (shift_key_bit_mask::is_set(modifiers)) {
          s << key_state_to_string(state::shift) IF_QT_ELSE(, << "+");
        }
        std::string str = key_symbol_to_string(key);
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
