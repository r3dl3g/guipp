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
 * @brief     C++ API: basic structures
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef WIN32
# include <windowsx.h>
#endif
#ifdef X11
//# ifdef XLIB
# include <X11/Xlib.h>
//# endif
#endif // X11

namespace gui {

  namespace core {

    template<typename T>
    const basic_size<T> basic_size<T>::zero;

    template<typename T>
    const basic_size<T> basic_size<T>::one(T(1));

    template<typename T>
    const basic_size<T> basic_size<T>::two(T(2));

    // --------------------------------------------------------------------------
    template<typename T>
    inline basic_size<T>::basic_size ()
      : w(0)
      , h(0)
    {}

    template<typename T>
    inline basic_size<T>::basic_size (type i)
      : w(i)
      , h(i)
    {}

    template<typename T>
    inline basic_size<T>::basic_size (type width, type height)
      : w(width)
      , h(height)
    {}

    template<typename T>
    inline auto basic_size<T>::width() const -> type {
      return w;
    }

    template<typename T>
    inline auto basic_size<T>::height() const -> type {
      return h;
    }

    template<typename T>
    inline os::size_type basic_size<T>::os_width () const {
      return global::scale<os::size_type, T>(w);
    }

    template<typename T>
    inline os::size_type basic_size<T>::os_height () const {
      return global::scale<os::size_type, T>(h);
    }

    template<typename T>
    inline void basic_size<T>::width (type new_w) {
      w = new_w;
    }

    template<typename T>
    inline void basic_size<T>::height (type new_h) {
      h = new_h;
    }

    template<typename T>
    inline basic_size<T>::basic_size (const os::size& s)
      : w(global::scale<T>(s.cx))
      , h(global::scale<T>(s.cy))
    {}

    template<typename T>
    inline basic_size<T>::basic_size (const os::point& pt)
      : w(global::scale<T>(pt.x))
      , h(global::scale<T>(pt.y))
    {}

#ifdef WIN32
    template<typename T>
    inline basic_size<T>::basic_size (const os::rectangle& r)
      : w(global::scale<T>(r.right - r.left))
      , h(global::scale<T>(r.bottom - r.top))
    {}

#endif // WIN32
#ifdef X11
    template<typename T>
    inline basic_size<T>::basic_size (const os::rectangle& r)
      : w(global::scale<T>(r.width))
      , h(global::scale<T>(r.height))
    {}

#endif // X11

    template<typename T>
    inline void basic_size<T>::clear (type v) {
      w = h = v;
    }

    template<typename T>
    inline bool basic_size<T>::empty () const {
      return (w <= type(0)) || (h <= type(0));
    }

    template<typename T>
    inline bool basic_size<T>::operator== (const self& rhs) const {
      return (w == rhs.w) && (h == rhs.h);
    }

    template<typename T>
    inline auto basic_size<T>::operator= (const self& rhs) -> self& {
      w = rhs.w;
      h = rhs.h;
      return *this;
    }

    template<typename T>
    inline bool basic_size<T>::operator!= (const self& rhs) const {
      return !operator== (rhs);
    }

    template<typename T>
    inline bool basic_size<T>::operator< (const self& rhs) const {
      return (w < rhs.w) || (h < rhs.h);
    }

    template<typename T>
    inline bool basic_size<T>::operator<= (const self& rhs) const {
      return (w <= rhs.w) || (h <= rhs.h);
    }

    template<typename T>
    inline bool basic_size<T>::operator> (const self& rhs) const {
      return (w > rhs.w) && (h > rhs.h);
    }

    template<typename T>
    inline bool basic_size<T>::operator>= (const self& rhs) const {
      return (w >= rhs.w) && (h >= rhs.h);
    }

    template<typename T>
    inline auto basic_size<T>::operator+ (const self& rhs) const -> self {
      return {type(width() + rhs.width()), type(height() + rhs.height())};
    }

    template<typename T>
    inline auto basic_size<T>::operator- (const self& rhs) const -> self {
      return {type(width() - rhs.width()), type(height() - rhs.height())};
    }

    template<typename T>
    inline auto basic_size<T>::operator* (type f) const -> self {
      return {type(width() * f), type(height() * f)};
    }

    template<typename T>
    inline auto basic_size<T>::operator/ (type f) const -> self {
      return {type(width() / f), type(height() / f)};
    }

    template<typename T>
    inline auto basic_size<T>::operator+= (const self& s) -> self& {
      w += s.w;
      h += s.h;
      return *this;
    }

    template<typename T>
    inline auto basic_size<T>::operator-= (const self& s) -> self& {
      w -= s.w;
      h -= s.h;
      return *this;
    }

    template<typename T>
    inline auto basic_size<T>::operator*= (type f) -> self& {
      w *= f;
      h *= f;
      return *this;
    }

    template<typename T>
    inline auto basic_size<T>::operator/= (type f) -> self& {
      w /= f;
      h /= f;
      return *this;
    }

    template<typename T>
    inline auto basic_size<T>::operator- () const -> self {
      return {-width(), -height()};
    }

    template<typename T>
    inline basic_size<T>::operator os::size() const {
      return os();
    }

    template<typename T>
    inline os::size basic_size<T>::os () const {
      return {os_width(), os_height()};
    }

    template<typename T>
    inline basic_size<T>::operator os::point() const {
      return {static_cast<os::point_type>(os_width()), static_cast<os::point_type>(os_height())};
    }

    template<typename T>
    std::ostream& operator<< (std::ostream& out, const basic_size<T>& sz) {
      out << sz.width() << ", " << sz.height();
      return out;
    }

    // --------------------------------------------------------------------------
    namespace global {

      inline size scale (const native_size& v) {
        return size(scale<size::type>(v.width()), scale<size::type>(v.height()));
      }

      inline native_size scale (const size& v) {
        return native_size(scale<native_size::type, size::type>(v.width()), scale<native_size::type, size::type>(v.height()));
      }

    } // namespace global

  } // namespace core

} // namespace gui
