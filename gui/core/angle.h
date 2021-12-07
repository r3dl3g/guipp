/**
 * @copyright (c) 2020-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     angle wrapper
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui++-core-export.h>


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

      angle operator+ (const angle&) const;
      angle operator- (const angle&) const;
      angle operator* (int) const;
      angle operator/ (int) const;
      angle operator* (float) const;
      angle operator/ (float) const;

      void operator+= (const angle&);
      void operator-= (const angle&);
      void operator*= (int);
      void operator/= (int);
      void operator*= (float);
      void operator/= (float);

      bool operator< (const angle&) const;
      bool operator<= (const angle&) const;
      bool operator== (const angle&) const;
      bool operator!= (const angle&) const;
      bool operator> (const angle&) const;
      bool operator>= (const angle&) const;

    private:
      float degrees;
    };

// --------------------------------------------------------------------------
  } // namespace core

} // namespace gui
