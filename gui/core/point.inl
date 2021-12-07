/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     2D point structure
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

namespace gui {

  namespace core {

    namespace convert {

      template<typename T, coordinate_system C>
      struct point {
        static gui::os::point_type to_os_x (const T&, const context&);
        static gui::os::point_type to_os_y (const T&, const context&);
        static T from_os_x (const gui::os::point_type&, const context&);
        static T from_os_y (const gui::os::point_type&, const context&);
      };

      template<typename T>
      struct point<T, coordinate_system::local> {
        static inline gui::os::point_type to_os_x (T v, const context& ctx) {
          return global::scale_to_native<gui::os::point_type>(v) + ctx.offset_x();
        }

        static inline gui::os::point_type to_os_y (T v, const context& ctx) {
          return global::scale_to_native<gui::os::point_type>(v) + ctx.offset_y();
        }

        static inline T from_os_x (const gui::os::point_type& v, const context& ctx) {
          return global::scale_from_native<T>(v - ctx.offset_x());
        }

        static inline T from_os_y (const gui::os::point_type& v, const context& ctx) {
          return global::scale_from_native<T>(v - ctx.offset_y());
        }

      };

      template<typename T>
      struct point<T, coordinate_system::surface> {
        static inline gui::os::point_type to_os_x (const T& v, const context&)    { return static_cast<gui::os::point_type>(v); }
        static inline gui::os::point_type to_os_y (const T& v, const context&)    { return static_cast<gui::os::point_type>(v); }
        static inline T from_os_x (const gui::os::point_type& v, const context&)  { return static_cast<T>(v); }
        static inline T from_os_y (const gui::os::point_type& v, const context&)  { return static_cast<T>(v); }
      };

    }

    template<typename T, coordinate_system C>
    const basic_point<T, C> basic_point<T, C>::zero;

    template<typename T, coordinate_system C>
    const basic_point<T, C> basic_point<T, C>::end = {std::numeric_limits<T>::max(), std::numeric_limits<T>::max()};

    template<typename T, coordinate_system C>
    const basic_point<T, C> basic_point<T, C>::undefined = {std::numeric_limits<T>::min(), std::numeric_limits<T>::min()};

    template<typename T, coordinate_system C>
    inline basic_point<T, C>::basic_point ()
      : x_(0)
      , y_(0)
    {}

    template<typename T, coordinate_system C>
    inline basic_point<T, C>::basic_point (type i)
      : x_(i)
      , y_(i)
    {}

    template<typename T, coordinate_system C>
    inline basic_point<T, C>::basic_point (type x, type y)
      : x_(x)
      , y_(y)
    {}

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::x () const->type {
      return x_;
    }

    template<typename T, coordinate_system C>
    inline void basic_point<T, C>::x (type v) {
      x_ = v;
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::y () const->type {
      return y_;
    }

    template<typename T, coordinate_system C>
    inline void basic_point<T, C>::y (type v) {
      y_ = v;
    }

    template<typename T, coordinate_system C>
    inline gui::os::point_type basic_point<T, C>::os_x (const context& ctx) const {
      return convert::point<T, C>::to_os_x(x_, ctx);
    }

    template<typename T, coordinate_system C>
    inline gui::os::point_type basic_point<T, C>::os_y (const context& ctx) const {
      return convert::point<T, C>::to_os_y(y_, ctx);
    }

    template<typename T, coordinate_system C>
    inline basic_point<T, C>::basic_point (const gui::os::point& rhs, const context& ctx)
      : x_(convert::point<T, C>::from_os_x(IF_QT_ELSE(rhs.x(), rhs.x), ctx))
      , y_(convert::point<T, C>::from_os_y(IF_QT_ELSE(rhs.y(), rhs.y), ctx))
    {}

    template<typename T, coordinate_system C>
    inline basic_point<T, C>::basic_point (const gui::os::rectangle& r, const context& ctx)
#ifdef GUIPP_WIN
      : x_(convert::point<T, C>::from_os_x(r.left, ctx))
      , y_(convert::point<T, C>::from_os_y(r.top, ctx))
#elif GUIPP_X11
      : x_(convert::point<T, C>::from_os_x(r.x, ctx))
      , y_(convert::point<T, C>::from_os_y(r.y, ctx))
#elif GUIPP_QT
      : x_(convert::point<T, C>::from_os_x(r.x(), ctx))
      , y_(convert::point<T, C>::from_os_y(r.y(), ctx))
#else
#error Unknown target system: basic_point<T, C>::basic_point (const gui::os::rectangle& r)
#endif // GUIPP_QT
    {}

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator+ (const size_t& s) const -> self {
      return {type(x() + s.width()), type(y() + s.height())};
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator- (const size_t& s) const -> self {
      return {type(x() - s.width()), type(y() - s.height())};
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator+= (const size_t& s) -> self& {
      operator= (*this + s);
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator-= (const size_t& s) -> self& {
      operator= (*this - s);
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator- (const self& pt) const -> self {
      return {type(x() - pt.x()), type(y() - pt.y())};
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::diff (const self& pt) const -> size_t {
      return {std::abs(x() - pt.x()), std::abs(y() - pt.y())};
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator+ (const self& pt) const -> self {
      return {type(x() + pt.x()), type(y() + pt.y())};
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator* (double f) const -> self {
      return {type(x() * f), type(y() * f)};
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator/ (double f) const -> self {
      return {type(x() / f), type(y() / f)};
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator+= (const self& pt) -> self& {
      x(x() + pt.x());
      y(y() + pt.y());
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator-= (const self& pt) -> self& {
      x(x() - pt.x());
      y(y() - pt.y());
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator*= (double f) -> self& {
      x(x() * f);
      y(y() * f);
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator/= (double f) -> self& {
      x(x() / f);
      y(y() / f);
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator- () const -> self {
      return {-x(), -y()};
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::move (const self& pt) -> self& {
      x(x() + pt.x());
      y(y() + pt.y());
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::move_x (type dx) -> self& {
      x(x() + dx);
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::move_y (type dy) -> self& {
      y(y() + dy);
      return *this;
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::dx (type dx) const -> self {
      return {type(x() + dx), y()};
    }

    template<typename T, coordinate_system C>
    inline auto  basic_point<T, C>::dy (type dy) const -> self {
      return {x(), type(y() + dy)};
    }

    template<typename T, coordinate_system C>
    inline auto  basic_point<T, C>::dxy (type dx, type dy) const -> self {
      return {type(x() + dx), type(y() + dy)};
    }

    template<typename T, coordinate_system C>
    inline auto basic_point<T, C>::operator= (const self& rhs) -> self& {
      x_ = rhs.x_;
      y_ = rhs.y_;
      return *this;
    }

    template<typename T, coordinate_system C>
    inline bool basic_point<T, C>::operator== (const self& rhs) const {
      return (x() == rhs.x()) && (y() == rhs.y());
    }

    template<typename T, coordinate_system C>
    inline bool basic_point<T, C>::operator!= (const self& rhs) const {
      return !operator== (rhs);
    }

    template<typename T, coordinate_system C>
    inline bool basic_point<T, C>::operator< (const self& rhs) const {
      return (x() < rhs.x()) && (y() < rhs.y());
    }

    template<typename T, coordinate_system C>
    inline bool basic_point<T, C>::operator<= (const self& rhs) const {
      return (x() <= rhs.x()) && (y() <= rhs.y());
    }

    template<typename T, coordinate_system C>
    inline bool basic_point<T, C>::operator> (const self& rhs) const {
      return (x() > rhs.x()) && (y() > rhs.y());
    }

    template<typename T, coordinate_system C>
    inline bool basic_point<T, C>::operator>= (const self& rhs) const {
      return (x() >= rhs.x()) && (y() >= rhs.y());
    }

    template<typename T, coordinate_system C>
    inline gui::os::point basic_point<T, C>::os (const context& ctx) const {
      return {os_x(ctx), os_y(ctx)};
    }

    template<typename T, coordinate_system C>
    inline bool basic_point<T, C>::is_valid () const {
      return operator!=(undefined);
    }

    template<typename T, coordinate_system C>
    inline bool basic_point<T, C>::isnan () const {
      return std::isnan(x_) || std::isnan(y_);
    }

    template<typename T, coordinate_system C>
    inline void basic_point<T, C>::clear () {
      operator=(undefined);
    }

    template<typename T, coordinate_system C>
    std::ostream& operator<< (std::ostream& out, const basic_point<T, C>& p) {
      out << p.x() << ", " << p.y();
      return out;
    }

    // --------------------------------------------------------------------------
    namespace global {

      inline point scale_from_native (const native_point& v) {
        return point(scale_from_native<point::type>(v.x()), scale_from_native<point::type>(v.y()));
      }

      inline native_point scale_to_native (const point& v) {
        return native_point(scale_to_native<native_point::type>(v.x()), scale_to_native<native_point::type>(v.y()));
      }

    } // namespace global

  } // namespace core

} // namespace gui
