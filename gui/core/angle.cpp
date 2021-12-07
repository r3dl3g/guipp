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

    angle::angle (float degrees)
      : degrees(degrees)
    {}

    float angle::deg () const {
      return degrees;
    }

    float angle::rad () const {
      return static_cast<float>(M_PI * degrees / 180.0);
    }

    angle angle::deg (float d) {
      return {d};
    }

    angle angle::rad (float r) {
      return {static_cast<float>(r * 180.0 / M_PI)};
    }

    angle angle::os (int n) {
      return {static_cast<float>(n) / IF_WIN32_X11_QT_ELSE(1.0F, 64.0F, 16.0F, 1.0F)};
    }

    int angle::os () const {
      return static_cast<int>(degrees * IF_WIN32_X11_QT_ELSE(1.0F, 64.0F, 16.0F, 1.0F));
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

    angle angle::operator* (float f) const {
      return {degrees * f};
    }

    angle angle::operator/ (float f) const {
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

    void angle::operator*= (float f) {
      degrees *= f;
    }

    void angle::operator/= (float f) {
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
