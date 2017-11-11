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
    {}

    template<typename T>
    inline basic_point<T>::basic_point (type i)
      : data(i, i)
    {}

    template<typename T>
    inline basic_point<T>::basic_point (type x, type y)
      : data(x, y)
    {}

    template<typename T>
    inline auto basic_point<T>::x() const->type {
      return data.x;
    }

    template<typename T>
    inline void basic_point<T>::x (type v) {
      data.x = v;
    }

    template<typename T>
    inline auto basic_point<T>::y() const->type {
      return data.y;
    }

    template<typename T>
    inline void basic_point<T>::y (type v) {
      data.y = v;
    }

    template<typename T>
    inline os::point_type basic_point<T>::os_x () const {
      return os_dimension_cast<os::point_type>(data.x);
    }

    template<typename T>
    inline os::point_type basic_point<T>::os_y () const {
      return os_dimension_cast<os::point_type>(data.y);
    }

    template<typename T>
    inline basic_point<T>::basic_point (const os::point& rhs)
      : data(static_cast<type>(rhs.x),
             static_cast<type>(rhs.y))
    {}

#ifdef WIN32
    template<typename T>
    inline basic_point<T>::basic_point (const os::rectangle& rhs)
      : data(static_cast<type>(rhs.left),
             static_cast<type>(rhs.top))
    {}

#endif // WIN32
#ifdef X11
    template<typename T>
    inline basic_point<T>::basic_point (const os::rectangle& r)
      : data(static_cast<type>(r.x),
             static_cast<type>(r.y))
    {}

#endif // X11

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
    inline auto basic_point<T>::operator* (type f) const -> self {
      return {type(x() * f), type(y() * f)};
    }

    template<typename T>
    inline auto basic_point<T>::operator/ (type f) const -> self {
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
    inline auto basic_point<T>::operator*= (type f) -> self& {
      x(x() * f);
      y(y() * f);
      return *this;
    }

    template<typename T>
    inline auto basic_point<T>::operator/= (type f) -> self& {
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
    inline auto basic_point<T>::operator= (const self& rhs) -> self& {
      data = rhs.data;
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
    inline basic_point<T>::operator os::point() const {
      return {os_x(), os_y()};
    }

    template<typename T>
    inline os::point basic_point<T>::os () const {
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
    inline basic_point<T>::data::data (type x, type y)
      : x(x)
      , y(y)
    {}

    template<typename T>
    std::ostream& operator<< (std::ostream& out, const basic_point<T>& p) {
      out << p.x() << ", " << p.y();
      return out;
    }

  } // namespace core

} // namespace gui
