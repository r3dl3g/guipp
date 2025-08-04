/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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

// --------------------------------------------------------------------------
//
// Common includes
//
# include <cmath>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>
#include <gui/core/angle.h>


namespace gui {

  namespace core {

    angle::angle (double degrees)
      : degrees(degrees)
    {}

    double angle::deg () const {
      return degrees;
    }

    double angle::rad () const {
      return static_cast<double>(M_PI * degrees / 180.0);
    }

    angle angle::deg (double d) {
      return {d};
    }

    angle angle::rad (double r) {
      return {static_cast<double>(r * 180.0 / M_PI)};
    }

    angle angle::os (int n) {
      return {static_cast<double>(n) / IF_WIN32_X11_QT_ELSE(1.0, 64.0, 16.0, 1.0)};
    }

    int angle::os () const {
      return static_cast<int>(degrees * IF_WIN32_X11_QT_ELSE(1.0, 64.0, 16.0, 1.0));
    }

    angle angle::operator+ (const angle& rhs) const {
      return {degrees + rhs.degrees};
    }

    angle angle::operator- (const angle& rhs) const {
      return {degrees - rhs.degrees};
    }

    angle angle::operator* (int i) const {
      return {degrees * i};
    }

    angle angle::operator/ (int i) const {
      return {degrees / i};
    }

    angle angle::operator* (double f) const {
      return {degrees * f};
    }

    angle angle::operator/ (double f) const {
      return {degrees / f};
    }

    void angle::operator+= (const angle& rhs) {
      degrees += rhs.degrees;
    }

    void angle::operator-= (const angle& rhs) {
      degrees -= rhs.degrees;
    }

    void angle::operator*= (int i) {
      degrees *= i;
    }

    void angle::operator/= (int i) {
      degrees /= i;
    }

    void angle::operator*= (double f) {
      degrees *= f;
    }

    void angle::operator/= (double f) {
      degrees /= f;
    }

    bool angle::operator< (const angle& rhs) const {
      return degrees < rhs.degrees;
    }

    bool angle::operator<= (const angle& rhs) const {
      return degrees <= rhs.degrees;
    }

    bool angle::operator== (const angle& rhs) const {
      return degrees == rhs.degrees;
    }

    bool angle::operator!= (const angle& rhs) const {
      return degrees != rhs.degrees;
    }

    bool angle::operator> (const angle& rhs) const {
      return degrees > rhs.degrees;
    }

    bool angle::operator>= (const angle& rhs) const {
      return degrees >= rhs.degrees;
    }

  } // namespace core

} // namespace gui
