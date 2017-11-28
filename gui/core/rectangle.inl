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
    const basic_rectangle<T> basic_rectangle<T>::zero;

    template<typename T>
    inline basic_rectangle<T>::basic_rectangle (const point_t& pos,
                                        const size_t& sz)
      : pos(pos)
      , sz(sz)
    {}

    template<typename T>
    inline basic_rectangle<T>::basic_rectangle (const size_t& sz)
      : sz(sz)
    {}

    template<typename T>
    inline basic_rectangle<T>::basic_rectangle (const point_t& topleft,
                                        const point_t& bottomright)
      : pos(topleft)
      , sz(bottomright - topleft)
    {}

    template<typename T>
    inline basic_rectangle<T>::basic_rectangle (type x,
                                        type y,
                                        type width,
                                        type height)
      : pos(x, y)
      , sz(width, height)
    {}

    template<typename T>
    inline auto basic_rectangle<T>::position () const -> const point_t& {
      return pos;
    }

    template<typename T>
    inline auto basic_rectangle<T>::size () const -> const size_t& {
      return sz;
    }

    template<typename T>
    inline auto basic_rectangle<T>::top_left () const -> const point_t& {
      return pos;
    }

    template<typename T>
    inline auto basic_rectangle<T>::top_right () const -> point_t {
      return {x2(), y()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::bottom_right () const -> point_t {
      return {x2(), y2()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::bottom_left () const -> point_t {
      return {x(), y2()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::x () const -> type {
      return pos.x();
    }

    template<typename T>
    inline auto basic_rectangle<T>::y () const -> type {
      return pos.y();
    }

    template<typename T>
    inline auto basic_rectangle<T>::width () const -> type {
      return sz.width();
    }

    template<typename T>
    inline auto basic_rectangle<T>::height () const -> type {
      return sz.height();
    }

    template<typename T>
    inline auto basic_rectangle<T>::x2 () const -> type {
      return x() + width();
    }

    template<typename T>
    inline auto basic_rectangle<T>::y2 () const -> type {
      return y() + height();
    }

    template<typename T>
    inline void basic_rectangle<T>::set_position (const point_t& pt) {
      pos = pt;
    }

    template<typename T>
    inline void basic_rectangle<T>::set_size (const size_t& s) {
      sz = s;
    }

    template<typename T>
    inline void basic_rectangle<T>::x (type x) {
      pos.x(x);
    }

    template<typename T>
    inline void basic_rectangle<T>::y (type y) {
      pos.y(y);
    }

    template<typename T>
    inline void basic_rectangle<T>::x2 (type new_x) {
      sz.width(new_x - x());
    }

    template<typename T>
    inline void basic_rectangle<T>::y2 (type new_y) {
      sz.height(new_y - y());
    }

    template<typename T>
    inline void basic_rectangle<T>::width (type width) {
      sz.width(width);
    }

    template<typename T>
    inline void basic_rectangle<T>::height (type height) {
      sz.height(height);
    }

    template<typename T>
    inline void basic_rectangle<T>::top_left (const point_t& pt) {
      pos = pt;
    }

    template<typename T>
    inline void basic_rectangle<T>::bottom_right (const point_t& pt) {
      sz.width(pt.x() - pos.x());
      sz.height(pt.y() - pos.y());
    }

    template<typename T>
    inline os::point_type basic_rectangle<T>::os_x () const {
      return pos.os_x();
    }

    template<typename T>
    inline os::point_type basic_rectangle<T>::os_y () const {
      return pos.os_y();
    }

    template<typename T>
    inline os::point_type basic_rectangle<T>::os_x2 () const {
      return os_dimension_cast<os::point_type>(x2());
    }

    template<typename T>
    inline os::point_type basic_rectangle<T>::os_y2 () const {
      return os_dimension_cast<os::point_type>(y2());
    }

    template<typename T>
    inline os::size_type basic_rectangle<T>::os_width () const {
      return size().os_width();
    }

    template<typename T>
    inline os::size_type basic_rectangle<T>::os_height () const {
      return size().os_height();
    }

#ifdef WIN32
    template<typename T>
    const basic_rectangle<T> basic_rectangle<T>::def(point_t(-1, -1), point_t(-1, -1));
#endif // WIN32
#ifdef X11
    template<typename T>
    const basic_rectangle<T> basic_rectangle<T>::def(0, 0, 1, 1);
#endif // X11

    template<typename T>
    basic_rectangle<T>::basic_rectangle (const os::rectangle& r)
#ifdef WIN32
      : pos(static_cast<type>(r.left), static_cast<type>(r.top))
      , sz(static_cast<type>(r.right - r.left), static_cast<type>(r.bottom - r.top))
#endif // Win32
#ifdef X11
      : pos(static_cast<type>(r.x), static_cast<type>(r.y))
      , sz(static_cast<type>(r.width), static_cast<type>(r.height))
#endif // X11
    {}

    template<typename T>
    inline bool basic_rectangle<T>::empty () const {
      return size().empty();
    }

    template<typename T>
    inline bool basic_rectangle<T>::is_inside (const point_t& p) const {
      return (p >= pos) && (p <= bottom_right());
    }

    template<typename T>
    inline auto basic_rectangle<T>::center_x () const -> type {
      return x() + width() / type(2);
    }

    template<typename T>
    inline auto basic_rectangle<T>::center_y () const -> type {
      return y() + height() / type(2);
    }

    template<typename T>
    inline auto basic_rectangle<T>::center () const -> point_t {
      return {center_x(), center_y()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::with_width (const type w) const -> self {
      return {position(), size_t(w, height())};
    }

    template<typename T>
    inline auto basic_rectangle<T>::with_height (const type h) const -> self {
      return {position(), size_t(width(), h)};
    }

    template<typename T>
    inline auto basic_rectangle<T>::right_width (const type w) const -> self {
      return {x2() - w, y(), w, height()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::bottom_height (const type h) const -> self {
      return {x(), y2() - h, width(), h};
    }

    template<typename T>
    inline auto basic_rectangle<T>::with_size (const size_t& sz) const -> self {
      return {position(), sz};
    }

    template<typename T>
    inline auto basic_rectangle<T>::with_x (const type x) const -> self {
      return {point(x, y()), size()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::with_y (const type y) const -> self {
      return {point(x(), y), size()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::with_pos (const point_t& pt) const -> self {
      return {pt, size()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::grown (const size_t& s) const -> self {
      return {position() - s, size() + (s + s)};
    }

    template<typename T>
    inline auto basic_rectangle<T>::shrinked (const size_t& s) const -> self {
      return {position() + s, size() - (s + s)};
    }

    template<typename T>
    inline auto basic_rectangle<T>::grow (const size_t& s) -> self& {
      pos -= s;
      sz += (s + s);
      return *this;
    }

    template<typename T>
    inline auto basic_rectangle<T>::shrink (const size_t& s) -> self& {
      pos += s;
      sz -= (s + s);
      return *this;
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator- (const size_t& s) const -> self {
      return {position(), size() - s};
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator-= (const size_t& s) -> self& {
      sz -= s;
      return *this;
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator+ (const size_t& s) const -> self {
      return {position(), sz + s};
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator+= (const size_t& s) -> self& {
      sz += s;
      return *this;
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator- (const point_t& pt) const -> self {
      return {position() + pt, size()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator-= (const point_t& pt) -> self& {
      pos -= pt;
      return *this;
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator+ (const point_t& pt) const -> self {
      return {position() + pt, size()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator+= (const point_t& pt) -> self& {
      pos += pt;
      return *this;
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator- (const self& r) const -> self {
      return {point_t {x() - r.x(), y() - r.y()}, size() - r.size()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator-= (const self& r) -> self& {
      operator= (*this - r);
      return *this;
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator+ (const self& r) const -> self {
      return {position() + r.position(), size() + r.size()};
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator+= (const self& r) -> self& {
      operator= (*this + r);
      return *this;
    }

    template<typename T>
    inline auto basic_rectangle<T>::operator= (const self& rhs) -> self& {
      pos = rhs.pos;
      sz = rhs.sz;
      return *this;
    }

    template<typename T>
    inline bool basic_rectangle<T>::operator== (const self& rhs) const {
      return (position() == rhs.position()) && (size() == rhs.size());
    }

    template<typename T>
    inline bool basic_rectangle<T>::operator!= (const self& rhs) const {
      return !operator== (rhs);
    }

    template<typename T>
    inline basic_rectangle<T>::operator os::rectangle() const {
      return os();
    }

    template<typename T>
    os::rectangle basic_rectangle<T>::os () const {
      return {
        static_cast<os::point_type>(x()), static_cast<os::point_type>(y()),
#ifdef WIN32
        static_cast<os::point_type>(x2()), static_cast<os::point_type>(y2())
#endif // Win32
#ifdef X11
        static_cast<os::size_type>(width()), static_cast<os::size_type>(height())
#endif // X11
      };
    }

    template<typename T>
    inline void basic_rectangle<T>::move (const point_t& pt) {
      pos += pt;
    }

    template<typename T>
    inline void basic_rectangle<T>::move (const size_t& sz) {
      pos += sz;
    }

    template<typename T>
    inline void basic_rectangle<T>::move_x (type dx) {
      pos.move_x(dx);
    }

    template<typename T>
    inline void basic_rectangle<T>::move_y (type dy) {
      pos.move_y(dy);
    }

    template<typename T>
    inline void basic_rectangle<T>::move_to_x (type new_x) {
      x(new_x);
    }

    template<typename T>
    inline void basic_rectangle<T>::move_to_y (type new_y) {
      y(new_y);
    }

    template<typename T>
    inline void basic_rectangle<T>::set_center (const point_t& pt) {
      set_position({ pt.x() - width() / type(2), pt.y() - height() / type(2) });
    }

    template<typename T>
    auto basic_rectangle<T>::operator|= (const self& rhs) -> self& {
      type x0 = std::min(x(), rhs.x());
      type y0 = std::min(y(), rhs.y());
      type x1 = std::max(x2(), rhs.x2());
      type y1 = std::max(y2(), rhs.y2());
      return operator= ({point_t {x0, y0}, point_t {x1, y1}});
    }

    template<typename T>
    auto basic_rectangle<T>::operator&= (const self& rhs) -> self& {
      type x0 = std::max(x(), rhs.x());
      type y0 = std::max(y(), rhs.y());
      type x1 = std::min(x2(), rhs.x2());
      type y1 = std::min(y2(), rhs.y2());
      return operator= ({point_t {x0, y0}, point_t {x1, y1}});
    }

    template<typename T>
    std::ostream& operator<< (std::ostream& out, const basic_rectangle<T>& r) {
      out << r.position() << ", " << r.size();
      return out;
    }

  } // namespace core

} // namespace gui
