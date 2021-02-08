/**
 * @copyright (c) 2020-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: angle
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui++-core-export.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    struct GUIPP_CORE_EXPORT angle {

      angle (float degrees = 0);
      angle (const angle&) = default;
      angle (angle&&) = default;

      static angle deg (float);
      static angle rad (float);
      static angle os (int);

      float deg () const;
      float rad () const;

      int os () const;
      operator int () const;

      angle operator+ (const angle&) const;
      angle operator- (const angle&) const;

      void operator+= (const angle&);
      void operator-= (const angle&);

      bool operator< (const angle&) const;
      bool operator== (const angle&) const;

    private:
      float degrees;
    };

// --------------------------------------------------------------------------
  } // namespace core

} // namespace gui
