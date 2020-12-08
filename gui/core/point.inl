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

namespace gui {

  namespace core {

    template<typename T>
    const basic_point<T> basic_point<T>::zero;

    template<typename T>
    const basic_point<T> basic_point<T>::end = {std::numeric_limits<T>::max(), std::numeric_limits<T>::max()};

    template<typename T>
    const basic_point<T> basic_point<T>::undefined = {std::numeric_limits<T>::min(), std::numeric_limits<T>::min()};

    template<typename T>
    inline basic_point<T>::basic_point ()
      : x_(0)
      , y_(0)
    {}

    template<typename T>
    inline basic_point<T>::basic_point (type i)
      : x_(i)
      , y_(i)
    {}

    template<typename T>
    inline basic_point<T>::basic_point (type x, type y)
      : x_(x)
      , y_(y)
    {}

    template<typename T>
    inline auto basic_point<T>::x () const->type {
      return x_;
    }

    template<typename T>
    inline void basic_point<T>::x (type v) {
      x_ = v;
    }

    template<typename T>
    inline auto basic_point<T>::y () const->type {
      return y_;
    }

    template<typename T>
    inline void basic_point<T>::y (type v) {
      y_ = v;
    }

    template<typename T>
    inline gui::os::point_type basic_point<T>::os_x () const {
      return global::scale<gui::os::point_type>(x_);
    }

    template<typename T>
    inline gui::os::point_type basic_point<T>::os_y () const {
      return global::scale<gui::os::point_type>(y_);
    }

    template<typename T>
    inline basic_point<T>::basic_point (const gui::os::point& rhs)
      : x_(global::scale<T>(IF_QT_ELSE(rhs.x(), rhs.x)))
      , y_(global::scale<T>(IF_QT_ELSE(rhs.y(), rhs.y)))
    {}

    template<typename T>
    inline basic_point<T>::basic_point (const gui::os::rectangle& r)
#ifdef GUIPP_WIN
      : x_(global::scale<T>(r.left))
      , y_(global::scale<T>(r.top))
#elif GUIPP_X11
      : x_(global::scale<T>(r.x))
      , y_(global::scale<T>(r.y))
#elif GUIPP_QT
      : x_(global::scale<T>(r.x()))
      , y_(global::scale<T>(r.y()))
#else
#error Unknown target system: basic_point<T>::basic_point (const gui::os::rectangle& r)
#endif // GUIPP_QT
    {}

    template<typename T>
    inline auto basic_point<T>::operator+ (const size_t& s) const -> self {
      return {type(x() + s.width()), type(y() + s.height())};
    }

    template<typename T>
    inline auto basic_point<T>::operator- (const size_t& s) const -> self {
      return {type(x() - s.width()), type(y() - s.height())};
    }

    template<typename T>
    inline auto basic_point<T>::operator+= (const size_t& s) -> self& {
      operator= (*this + s);
      return *this;
    }

    template<typename T>
    inline auto basic_point<T>::operator-= (const size_t& s) -> self& {
      operator= (*this - s);
      return *this;
    }

    template<typename T>
    inline auto basic_point<T>::operator- (const self& pt) const -> size_t {
      return {type(x() - pt.x()), type(y() - pt.y())};
    }

    template<typename T>
    inline auto basic_point<T>::operator+ (const self& pt) const -> self {
      return {type(x() + pt.x()), type(y() + pt.y())};
    }

    template<typename T>
    inline auto basic_point<T>::operator* (double f) const -> self {
      return {type(x() * f), type(y() * f)};
    }

    template<typename T>
    inline auto basic_point<T>::operator/ (double f) const -> self {
      return {type(x() / f), type(y() / f)};
    }

    template<typename T>
    inline auto basic_point<T>::operator+= (const self& pt) -> self& {
      x(x() + pt.x());
      y(y() + pt.y());
      return *this;
    }

    template<typename T>
    inline auto basic_point<T>::operator-= (const self& pt) -> self& {
      x(x() - pt.x());
      y(y() - pt.y());
      return *this;
    }

    template<typename T>
    inline auto basic_point<T>::operator*= (double f) -> self& {
      x(x() * f);
      y(y() * f);
      return *this;
    }

    template<typename T>
    inline auto basic_point<T>::operator/= (double f) -> self& {
      x(x() / f);
      y(y() / f);
      return *this;
    }

    template<typename T>
    inline auto basic_point<T>::operator- () const -> self {
      return {-x(), -y()};
    }

    template<typename T>
    inline void basic_point<T>::move (const self& pt) {
      x(x() + pt.x());
      y(y() + pt.y());
    }

    template<typename T>
    inline void basic_point<T>::move_x (type dx) {
      x(x() + dx);
    }

    template<typename T>
    inline void basic_point<T>::move_y (type dy) {
      y(y() + dy);
    }

    template<typename T>
    inline auto basic_point<T>::dx (type dx) const -> self {
      return {type(x() + dx), y()};
    }

    template<typename T>
    inline auto  basic_point<T>::dy (type dy) const -> self {
      return {x(), type(y() + dy)};
    }

    template<typename T>
    inline auto  basic_point<T>::dxy (type dx, type dy) const -> self {
      return {type(x() + dx), type(y() + dy)};
    }

    template<typename T>
    inline auto basic_point<T>::operator= (const self& rhs) -> self& {
      x_ = rhs.x_;
      y_ = rhs.y_;
      return *this;
    }

    template<typename T>
    inline bool basic_point<T>::operator== (const self& rhs) const {
      return (x() == rhs.x()) && (y() == rhs.y());
    }

    template<typename T>
    inline bool basic_point<T>::operator!= (const self& rhs) const {
      return !operator== (rhs);
    }

    template<typename T>
    inline bool basic_point<T>::operator< (const self& rhs) const {
      return (x() < rhs.x()) && (y() < rhs.y());
    }

    template<typename T>
    inline bool basic_point<T>::operator<= (const self& rhs) const {
      return (x() <= rhs.x()) && (y() <= rhs.y());
    }

    template<typename T>
    inline bool basic_point<T>::operator> (const self& rhs) const {
      return (x() > rhs.x()) && (y() > rhs.y());
    }

    template<typename T>
    inline bool basic_point<T>::operator>= (const self& rhs) const {
      return (x() >= rhs.x()) && (y() >= rhs.y());
    }

    template<typename T>
    inline basic_point<T>::operator gui::os::point() const {
      return {os_x(), os_y()};
    }

    template<typename T>
    inline gui::os::point basic_point<T>::os () const {
      return {os_x(), os_y()};
    }

    template<typename T>
    inline bool basic_point<T>::is_valid () const {
      return operator!=(undefined);
    }

    template<typename T>
    inline void basic_point<T>::clear () {
      operator=(undefined);
    }

    template<typename T>
    std::ostream& operator<< (std::ostream& out, const basic_point<T>& p) {
      out << p.x() << ", " << p.y();
      return out;
    }

    // --------------------------------------------------------------------------
    namespace global {

      inline point scale (const native_point& v) {
        return point(scale<point::type>(v.x()), scale<point::type>(v.y()));
      }

      inline native_point scale (const point& v) {
        return native_point(scale<native_point::type>(v.x()), scale<native_point::type>(v.y()));
      }

    } // namespace global

  } // namespace core

} // namespace gui
