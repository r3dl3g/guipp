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
#include "gui/core/gui++-core-export.h"


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    struct GUIPP_CORE_EXPORT angle {

      angle (double degrees = 0);
      angle (const angle&) = default;
      angle (angle&&) = default;

      static angle deg (double);
      static angle rad (double);
      static angle os (int);

      double deg () const;
      double rad () const;

      int os () const;

      angle operator+ (const angle&) const;
      angle operator- (const angle&) const;
      angle operator* (int) const;
      angle operator/ (int) const;
      angle operator* (double) const;
      angle operator/ (double) const;

      void operator+= (const angle&);
      void operator-= (const angle&);
      void operator*= (int);
      void operator/= (int);
      void operator*= (double);
      void operator/= (double);

      bool operator< (const angle&) const;
      bool operator<= (const angle&) const;
      bool operator== (const angle&) const;
      bool operator!= (const angle&) const;
      bool operator> (const angle&) const;
      bool operator>= (const angle&) const;

    private:
      double degrees;
    };

// --------------------------------------------------------------------------
  } // namespace core

} // namespace gui
