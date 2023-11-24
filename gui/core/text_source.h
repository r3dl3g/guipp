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
 * @brief     text source
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>
#include <functional>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    // --------------------------------------------------------------------------
    typedef std::string (text_source_fn)();
    typedef std::function<text_source_fn> text_source;

    // --------------------------------------------------------------------------
    struct const_text {
      const_text () = default;

      const_text (const std::string& text)
        : text(text)
      {}

      const_text (std::string&& text)
        : text(std::move(text))
      {}

      const_text (const char* text)
        : text(text)
      {}

      const std::string& operator() () const {
        return text;
      }

    private:
      std::string text;
    };

    // --------------------------------------------------------------------------

  } // namespace core

} // namespace gui
