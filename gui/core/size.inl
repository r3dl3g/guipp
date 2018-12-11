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
    typename std::enable_if<std::is_unsigned<T>::value, T>::type os_dimension_cast (size::type v) {
      return global::scale(static_cast<T>(std::max<size::type>(v, 0)));
    }

    template<typename T>
    typename std::enable_if<std::is_signed<T>::value, T>::type os_dimension_cast (size::type v) {
      return global::scale(static_cast<T>(v));
    }

    template<typename T>
    const basic_size<T> basic_size<T>::zero;

    template<typename T>
    const basic_size<T> basic_size<T>::one(T(1));

    template<typename T>
    const basic_size<T> basic_size<T>::two(T(2));

    // --------------------------------------------------------------------------
    template<typename T>
    inline basic_size<T>::basic_size ()
    {}

    template<typename T>
    inline basic_size<T>::basic_size (type i)
      : data(i, i)
    {}

    template<typename T>
    inline basic_size<T>::basic_size (type width, type height)
      : data(width, height)
    {}

    template<typename T>
    inline auto basic_size<T>::width() const -> type {
      return data.w;
    }

    template<typename T>
    inline auto basic_size<T>::height() const -> type {
      return data.h;
    }

    template<typename T>
    inline os::size_type basic_size<T>::os_width () const {
      return os_dimension_cast<os::size_type>(data.w);
    }

    template<typename T>
    inline os::size_type basic_size<T>::os_height () const {
      return os_dimension_cast<os::size_type>(data.h);
    }

    template<typename T>
    inline void basic_size<T>::width (type w) {
      data.w = w;
    }

    template<typename T>
    inline void basic_size<T>::height (type h) {
      data.h = h;
    }

    template<typename T>
    inline basic_size<T>::basic_size (const os::size& s)
      : data(global::unscale<type>(s.cx),
             global::unscale<type>(s.cy))
    {}

    template<typename T>
    inline basic_size<T>::basic_size (const os::point& pt)
      : data(global::unscale<type>(pt.x),
             global::unscale<type>(pt.y))
    {}

#ifdef WIN32
    template<typename T>
    inline basic_size<T>::basic_size (const os::rectangle& r)
      : data(global::unscale<type>(r.right - r.left),
             global::unscale<type>(r.bottom - r.top))
    {}

#endif // WIN32
#ifdef X11
    template<typename T>
    inline basic_size<T>::basic_size (const os::rectangle& r)
      : data(global::unscale<type>(r.width),
             global::unscale<type>(r.height))
    {}

#endif // X11

    template<typename T>
    inline void basic_size<T>::clear (type v) {
      data.w = data.h = v;
    }

    template<typename T>
    inline bool basic_size<T>::empty () const {
      return (data.w <= type(0)) || (data.h <= type(0));
    }

    template<typename T>
    inline bool basic_size<T>::operator== (const self& rhs) const {
      return (data.w == rhs.data.w) && (data.h == rhs.data.h);
    }

    template<typename T>
    inline auto basic_size<T>::operator= (const self& rhs) -> self& {
      data = rhs.data;
      return *this;
    }

    template<typename T>
    inline bool basic_size<T>::operator!= (const self& rhs) const {
      return !operator== (rhs);
    }

    template<typename T>
    inline bool basic_size<T>::operator< (const self& rhs) const {
      return (data.w < rhs.data.w) || (data.h < rhs.data.h);
    }

    template<typename T>
    inline bool basic_size<T>::operator<= (const self& rhs) const {
      return (data.w <= rhs.data.w) || (data.h <= rhs.data.h);
    }

    template<typename T>
    inline bool basic_size<T>::operator> (const self& rhs) const {
      return (data.w > rhs.data.w) && (data.h > rhs.data.h);
    }

    template<typename T>
    inline bool basic_size<T>::operator>= (const self& rhs) const {
      return (data.w >= rhs.data.w) && (data.h >= rhs.data.h);
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
      data.w += s.data.w;
      data.h += s.data.h;
      return *this;
    }

    template<typename T>
    inline auto basic_size<T>::operator-= (const self& s) -> self& {
      data.w -= s.data.w;
      data.h -= s.data.h;
      return *this;
    }

    template<typename T>
    inline auto basic_size<T>::operator*= (type f) -> self& {
      data.w *= f;
      data.h *= f;
      return *this;
    }

    template<typename T>
    inline auto basic_size<T>::operator/= (type f) -> self& {
      data.w /= f;
      data.h /= f;
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
    inline basic_size<T>::data::data (type w, type h)
      : w(w)
      , h(h)
    {}

    template<typename T>
    std::ostream& operator<< (std::ostream& out, const basic_size<T>& sz) {
      out << sz.width() << ", " << sz.height();
      return out;
    }

  } // namespace core

} // namespace gui
