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
 * @brief     C++ API: basic window types
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <ostream>
#include <algorithm>

#ifdef WIN32
# include <windowsx.h>
#endif
#ifdef X11
//# ifdef XLIB
# include <X11/Xlib.h>
//# endif
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>


namespace gui {

  namespace core {

#ifdef WIN32
    const rectangle rectangle::def = rectangle(point(-1, -1), point(-1, -1));
#endif // WIN32
#ifdef X11
    const rectangle rectangle::def = rectangle(0, 0, 1, 1);
#endif // X11

    // --------------------------------------------------------------------------
    const size size::zero;
    const size size::one(1);
    const size size::two(2);

    const point point::zero;
    const point point::undefined(std::numeric_limits<point::type>::min(), std::numeric_limits<point::type>::min());

    const rectangle rectangle::zero;

    size::size (const os::size& s)
      : data(static_cast<type>(s.cx),
             static_cast<type>(s.cy))
    {}

    size::size (const os::point& pt)
      : data(static_cast<type>(pt.x),
             static_cast<type>(pt.y))
    {}

#ifdef WIN32
    size::size (const os::rectangle& r)
      : data(static_cast<type>(r.right - r.left),
             static_cast<type>(r.bottom - r.top))
    {}

#endif // WIN32
#ifdef X11
    size::size (const os::rectangle& r)
      : data(static_cast<type>(r.width),
             static_cast<type>(r.height))
    {}

#endif // X11

    void size::clear (type v) {
      data.w = data.h = v;
    }

    bool size::empty () const {
      return (data.w <= type(0)) || (data.h <= type(0));
    }

    bool size::operator== (const size& rhs) const {
      return (data.w == rhs.data.w) && (data.h == rhs.data.h);
    }

    bool size::operator!= (const size& rhs) const {
      return !operator== (rhs);
    }

    bool size::operator< (const size& rhs) const {
      return (data.w < rhs.data.w) || (data.h < rhs.data.h);
    }

    bool size::operator<= (const size& rhs) const {
      return (data.w <= rhs.data.w) || (data.h <= rhs.data.h);
    }

    bool size::operator> (const size& rhs) const {
      return (data.w > rhs.data.w) && (data.h > rhs.data.h);
    }

    bool size::operator>= (const size& rhs) const {
      return (data.w >= rhs.data.w) && (data.h >= rhs.data.h);
    }

    size size::operator+ (const size& rhs) const {
      return {type(width() + rhs.width()), type(height() + rhs.height())};
    }

    size size::operator- (const size& rhs) const {
      return {type(width() - rhs.width()), type(height() - rhs.height())};
    }

    size size::operator* (size_type f) const {
      return {type(width() * f), type(height() * f)};
    }

    size size::operator/ (size_type f) const {
      return {type(width() / f), type(height() / f)};
    }

    size& size::operator+= (const size& s) {
      data.w += s.data.w;
      data.h += s.data.h;
      return *this;
    }

    size& size::operator-= (const size& s) {
      data.w -= s.data.w;
      data.h -= s.data.h;
      return *this;
    }

    size& size::operator*= (size_type f) {
      data.w *= f;
      data.h *= f;
      return *this;
    }

    size& size::operator/= (size_type f) {
      data.w /= f;
      data.h /= f;
      return *this;
    }

    size size::operator- () const {
      return {-width(), -height()};
    }

    size::operator os::size() const {
      return os();
    }

    os::size size::os () const {
      return {os_width(), os_height()};
    }

    size::operator os::point() const {
      return {static_cast<os::point_type>(width()), static_cast<os::point_type>(height())};
    }

    // --------------------------------------------------------------------------
    size::data::data (type w, type h)
      : w(w)
      , h(h)
    {}

    // --------------------------------------------------------------------------
    point::point (const os::point& rhs)
      : data(static_cast<type>(rhs.x),
             static_cast<type>(rhs.y))
    {}

#ifdef WIN32
    point::point (const os::rectangle& rhs)
      : data(static_cast<type>(rhs.left),
             static_cast<type>(rhs.top))
    {}

#endif // WIN32
#ifdef X11
    point::point (const os::rectangle& r)
      : data(static_cast<type>(r.x),
             static_cast<type>(r.y))
    {}

#endif // X11

    point point::operator+ (const size& s) const {
      return {type(x() + s.width()), type(y() + s.height())};
    }

    point point::operator- (const size& s) const {
      return {type(x() - s.width()), type(y() - s.height())};
    }

    point& point::operator+= (const size& s) {
      operator= (*this + s);
      return *this;
    }

    point& point::operator-= (const size& s) {
      operator= (*this - s);
      return *this;
    }

    size point::operator- (const point& pt) const {
      return {type(x() - pt.x()), type(y() - pt.y())};
    }

    point point::operator+ (const point& pt) const {
      return {type(x() + pt.x()), type(y() + pt.y())};
    }

    point point::operator* (point_type f) const {
      return {type(x() * f), type(y() * f)};
    }

    point point::operator/ (point_type f) const {
      return {type(x() / f), type(y() / f)};
    }

    point& point::operator+= (const point& pt) {
      x(x() + pt.x());
      y(y() + pt.y());
      return *this;
    }

    point& point::operator-= (const point& pt) {
      x(x() - pt.x());
      y(y() - pt.y());
      return *this;
    }

    point& point::operator*= (point_type f) {
      x(x() * f);
      y(y() * f);
      return *this;
    }

    point& point::operator/= (point_type f) {
      x(x() / f);
      y(y() / f);
      return *this;
    }

    point point::operator- () const {
      return {-x(), -y()};
    }

    void point::move (const point& pt) {
      x(x() + pt.x());
      y(y() + pt.y());
    }

    void point::move_x (type dx) {
      x(x() + dx);
    }

    void point::move_y (type dy) {
      y(y() + dy);
    }

    bool point::operator== (const point& rhs) const {
      return (x() == rhs.x()) && (y() == rhs.y());
    }

    bool point::operator!= (const point& rhs) const {
      return !operator== (rhs);
    }

    bool point::operator< (const point& rhs) const {
      return (x() < rhs.x()) && (y() < rhs.y());
    }

    bool point::operator<= (const point& rhs) const {
      return (x() <= rhs.x()) && (y() <= rhs.y());
    }

    bool point::operator> (const point& rhs) const {
      return (x() > rhs.x()) && (y() > rhs.y());
    }

    bool point::operator>= (const point& rhs) const {
      return (x() >= rhs.x()) && (y() >= rhs.y());
    }

    point::operator os::point() const {
      return {os_x(), os_y()};
    }

    os::point point::os () const {
      return {os_x(), os_y()};
    }

    // --------------------------------------------------------------------------
    point::data::data (type x, type y)
      : x(x)
      , y(y)
    {}

    // --------------------------------------------------------------------------
    rectangle::rectangle (const os::rectangle& r)
#ifdef WIN32
      : pos(static_cast<point::type>(r.left), static_cast<point::type>(r.top))
      , sz(static_cast<size::type>(r.right - r.left), static_cast<size::type>(r.bottom - r.top))
#endif // Win32
#ifdef X11
      : pos(static_cast<point::type>(r.x), static_cast<point::type>(r.y))
      , sz(static_cast<size::type>(r.width), static_cast<size::type>(r.height))
#endif // X11
    {}

    bool rectangle::empty () const {
      return size().empty();
    }

    bool rectangle::is_inside (const point& p) const {
      return (p >= pos) && (p <= bottom_right());
    }

    point::type rectangle::center_x () const {
      return x() + width() / point::type(2);
    }

    point::type rectangle::center_y () const {
      return y() + height() / point::type(2);
    }

    point rectangle::center () const {
      return {center_x(), center_y()};
    }

    rectangle rectangle::with_width (const core::size::type w) const {
      return {position(), core::size(w, height())};
    }

    rectangle rectangle::with_height (const core::size::type h) const {
      return {position(), core::size(width(), h)};
    }

    rectangle rectangle::with_size (const core::size& sz) const {
      return {position(), sz};
    }

    rectangle rectangle::with_x (const core::size::type x) const {
      return {core::point(x, y()), size()};
    }

    rectangle rectangle::with_y (const core::size::type y) const {
      return {core::point(x(), y), size()};
    }

    rectangle rectangle::with_pos (const core::point& pt) const {
      return {pt, size()};
    }

    rectangle rectangle::grown (const core::size& s) const {
      return {position() - s, size() + (s + s)};
    }

    rectangle rectangle::shrinked (const core::size& s) const {
      return {position() + s, size() - (s + s)};
    }

    rectangle& rectangle::grow (const core::size& s) {
      pos -= s;
      sz += (s + s);
      return *this;
    }

    rectangle& rectangle::shrink (const core::size& s) {
      pos += s;
      sz -= (s + s);
      return *this;
    }

    rectangle rectangle::operator- (const core::size& s) const {
      return {position(), size() - s};
    }

    rectangle& rectangle::operator-= (const core::size& s) {
      sz -= s;
      return *this;
    }

    rectangle rectangle::operator+ (const core::size& s) const {
      return {position(), sz + s};
    }

    rectangle& rectangle::operator+= (const core::size& s) {
      sz += s;
      return *this;
    }

    rectangle rectangle::operator- (const point& pt) const {
      return {position() + pt, size()};
    }

    rectangle& rectangle::operator-= (const point& pt) {
      pos -= pt;
      return *this;
    }

    rectangle rectangle::operator+ (const point& pt) const {
      return {position() + pt, size()};
    }

    rectangle& rectangle::operator+= (const point& pt) {
      pos += pt;
      return *this;
    }

    rectangle rectangle::operator- (const rectangle& r) const {
      return {point {x() - r.x(), y() - r.y()}, size() - r.size()};
    }

    rectangle& rectangle::operator-= (const rectangle& r) {
      operator= (*this - r);
      return *this;
    }

    rectangle rectangle::operator+ (const rectangle& r) const {
      return {position() + r.position(), size() + r.size()};
    }

    rectangle& rectangle::operator+= (const rectangle& r) {
      operator= (*this + r);
      return *this;
    }

    bool rectangle::operator== (const rectangle& rhs) const {
      return (position() == rhs.position()) && (size() == rhs.size());
    }

    bool rectangle::operator!= (const rectangle& rhs) const {
      return !operator== (rhs);
    }

    rectangle::operator os::rectangle() const {
      return os();
    }

    os::rectangle rectangle::os () const {
      return {static_cast<os::point_type>(x()), static_cast<os::point_type>(y()),
#ifdef WIN32
        static_cast<os::point_type>(x2()), static_cast<os::point_type>(y2())
#endif // Win32
#ifdef X11
        static_cast<os::size_type>(width()), static_cast<os::size_type>(height())
#endif // X11
      };
    }

    void rectangle::move (const point& pt) {
      pos += pt;
    }

    void rectangle::move (const core::size& sz) {
      pos += sz;
    }

    void rectangle::move_x (point::type dx) {
      pos.move_x(dx);
    }

    void rectangle::move_y (point::type dy) {
      pos.move_y(dy);
    }

    void rectangle::move_to_x (point::type new_x) {
      x(new_x);
    }

    void rectangle::move_to_y (point::type new_y) {
      y(new_y);
    }

    void rectangle::set_center (const core::point& pt) {
      set_position({ pt.x() - width() / point_type(2), pt.y() - height() / point_type(2) });
    }

    rectangle& rectangle::operator|= (const rectangle& rhs) {
      point::type x0 = std::min(x(), rhs.x());
      point::type y0 = std::min(y(), rhs.y());
      point::type x1 = std::max(x2(), rhs.x2());
      point::type y1 = std::max(y2(), rhs.y2());
      return operator= ({point {x0, y0}, point {x1, y1}});
    }

    rectangle& rectangle::operator&= (const rectangle& rhs) {
      point::type x0 = std::max(x(), rhs.x());
      point::type y0 = std::max(y(), rhs.y());
      point::type x1 = std::min(x2(), rhs.x2());
      point::type y1 = std::min(y2(), rhs.y2());
      return operator= ({point {x0, y0}, point {x1, y1}});
    }

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const size& sz) {
      out << sz.width() << ", " << sz.height();
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const point& p) {
      out << p.x() << ", " << p.y();
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const rectangle& r) {
      out << r.position() << ", " << r.size();
      return out;
    }

  } // core

} // gui
