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
 * @brief     C++ API: basic structures
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef GUIPP_WIN
# include <windowsx.h>
#endif
#ifdef GUIPP_X11
//# ifdef XLIB
# include <X11/Xlib.h>
//# endif
#endif // GUIPP_X11

namespace gui {

  namespace core {

    namespace convert {

      template<typename T, coordinate_system C>
      struct size {
        static gui::os::size_type to_os (const T&);
        static T from_os (const gui::os::size_type&);
      };

      template<typename T>
      struct size<T, coordinate_system::local> {
        static inline gui::os::size_type to_os (const T& v) {
          return global::scale_to_native<gui::os::size_type>(v);
        }

        static inline T from_os (const gui::os::size_type& v) {
          return global::scale_from_native<T>(v);
        }
      };

      template<typename T>
      struct size<T, coordinate_system::surface> {
        static inline gui::os::size_type to_os (const T& v) {
          return static_cast<gui::os::size_type>(v);
        }

        static inline T from_os (const gui::os::size_type& v) {
          return static_cast<T>(v);
        }
      };

    }

    template<typename T, coordinate_system C>
    const basic_size<T, C> basic_size<T, C>::zero;

    template<typename T, coordinate_system C>
    const basic_size<T, C> basic_size<T, C>::one(T(1));

    template<typename T, coordinate_system C>
    const basic_size<T, C> basic_size<T, C>::two(T(2));

    // --------------------------------------------------------------------------
    template<typename T, coordinate_system C>
    inline basic_size<T, C>::basic_size ()
      : w(0)
      , h(0)
    {}

    template<typename T, coordinate_system C>
    inline basic_size<T, C>::basic_size (type i)
      : w(i)
      , h(i)
    {}

    template<typename T, coordinate_system C>
    inline basic_size<T, C>::basic_size (type width, type height)
      : w(width)
      , h(height)
    {}

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::width() const -> type {
      return w;
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::height() const -> type {
      return h;
    }

    template<typename T, coordinate_system C>
    inline gui::os::size_type basic_size<T, C>::os_width () const {
      return convert::size<T, C>::to_os(w);
    }

    template<typename T, coordinate_system C>
    inline gui::os::size_type basic_size<T, C>::os_height () const {
      return convert::size<T, C>::to_os(h);
    }

    template<typename T, coordinate_system C>
    inline void basic_size<T, C>::width (type new_w) {
      w = new_w;
    }

    template<typename T, coordinate_system C>
    inline void basic_size<T, C>::height (type new_h) {
      h = new_h;
    }

    template<typename T, coordinate_system C>
    inline basic_size<T, C>::basic_size (const gui::os::size& s)
      : w(convert::size<T, C>::from_os(IF_QT_ELSE(s.width(), s.cx)))
      , h(convert::size<T, C>::from_os(IF_QT_ELSE(s.height(), s.cy)))
    {}

    template<typename T, coordinate_system C>
    inline basic_size<T, C>::basic_size (const gui::os::point& pt)
      : w(convert::size<T, C>::from_os(IF_QT_ELSE(pt.x(), pt.x)))
      , h(convert::size<T, C>::from_os(IF_QT_ELSE(pt.y(), pt.y)))
    {}

    template<typename T, coordinate_system C>
    inline basic_size<T, C>::basic_size (const gui::os::rectangle& r)
#ifdef GUIPP_WIN
      : w(convert::size<T, C>::from_os(r.right - r.left))
      , h(convert::size<T, C>::from_os(r.bottom - r.top))
#elif GUIPP_X11
      : w(convert::size<T, C>::from_os(r.width))
      , h(convert::size<T, C>::from_os(r.height))
#elif GUIPP_QT
      : w(convert::size<T, C>::from_os(r.width()))
      , h(convert::size<T, C>::from_os(r.height()))
#else
# error Unknown target system: basic_size<T, C>::basic_size (const gui::os::rectangle& r)
#endif // GUIPP_QT
    {}

    template<typename T, coordinate_system C>
    inline void basic_size<T, C>::clear (type v) {
      w = h = v;
    }

    template<typename T, coordinate_system C>
    inline bool basic_size<T, C>::empty () const {
      return (w <= type(0)) || (h <= type(0));
    }

    template<typename T, coordinate_system C>
    inline bool basic_size<T, C>::isnan () const {
      return std::isnan(w) || std::isnan(w);
    }

    template<typename T, coordinate_system C>
    inline bool basic_size<T, C>::operator== (const self& rhs) const {
      return (w == rhs.w) && (h == rhs.h);
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::operator= (const self& rhs) -> self& {
      w = rhs.w;
      h = rhs.h;
      return *this;
    }

    template<typename T, coordinate_system C>
    inline bool basic_size<T, C>::operator!= (const self& rhs) const {
      return !operator== (rhs);
    }

    template<typename T, coordinate_system C>
    inline bool basic_size<T, C>::operator< (const self& rhs) const {
      return (w < rhs.w) || (h < rhs.h);
    }

    template<typename T, coordinate_system C>
    inline bool basic_size<T, C>::operator<= (const self& rhs) const {
      return (w <= rhs.w) || (h <= rhs.h);
    }

    template<typename T, coordinate_system C>
    inline bool basic_size<T, C>::operator> (const self& rhs) const {
      return (w > rhs.w) && (h > rhs.h);
    }

    template<typename T, coordinate_system C>
    inline bool basic_size<T, C>::operator>= (const self& rhs) const {
      return (w >= rhs.w) && (h >= rhs.h);
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::operator+ (const self& rhs) const -> self {
      return {type(width() + rhs.width()), type(height() + rhs.height())};
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::operator- (const self& rhs) const -> self {
      return {type(width() - rhs.width()), type(height() - rhs.height())};
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::operator* (double f) const -> self {
      return {type(width() * f), type(height() * f)};
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::operator/ (double f) const -> self {
      return {type(width() / f), type(height() / f)};
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::operator+= (const self& s) -> self& {
      w += s.w;
      h += s.h;
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::operator-= (const self& s) -> self& {
      w -= s.w;
      h -= s.h;
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::operator*= (double f) -> self& {
      w = type(w * f);
      h = type(h * f);
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::operator/= (double f) -> self& {
      w = type(w / f);
      h = type(h / f);
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::operator- () const -> self {
      return {-width(), -height()};
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::min (const self& rhs) const -> self {
      return { std::min(width(), rhs.width()), std::min(height(), rhs.height()) };
    }

    template<typename T, coordinate_system C>
    inline auto basic_size<T, C>::max (const self& rhs) const -> self {
      return { max(width(), rhs.width()), max(height(), rhs.height()) };
    }

    template<typename T, coordinate_system C>
    inline gui::os::size basic_size<T, C>::os () const {
      return {os_width(), os_height()};
    }

//    template<typename T, coordinate_system C>
//    inline basic_size<T, C>::operator gui::os::size() const {
//      return os();
//    }

    template<typename T, coordinate_system C>
    inline gui::os::point basic_size<T, C>::os_point () const {
      return {static_cast<gui::os::point_type>(os_width()), static_cast<gui::os::point_type>(os_height())};
    }

    template<typename T, coordinate_system C>
    std::ostream& operator<< (std::ostream& out, const basic_size<T, C>& sz) {
      out << sz.width() << ", " << sz.height();
      return out;
    }

    // --------------------------------------------------------------------------
    namespace global {

      inline size scale_from_native (const native_size& v) {
        return size(scale_from_native<size::type>(v.width()), scale_from_native<size::type>(v.height()));
      }

      inline native_size scale_to_native (const size& v) {
        return native_size(scale_to_native<native_size::type, size::type>(v.width()), scale_to_native<native_size::type, size::type>(v.height()));
      }

    } // namespace global

  } // namespace core

} // namespace gui
