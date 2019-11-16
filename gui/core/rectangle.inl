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

    template<typename T, typename S>
    const basic_rectangle<T, S> basic_rectangle<T, S>::zero;

    template<typename T, typename S>
    inline basic_rectangle<T, S>::basic_rectangle (const point_t& pos,
                                                   const size_t& sz)
      : pos(pos)
      , sz(sz)
    {}

    template<typename T, typename S>
    inline basic_rectangle<T, S>::basic_rectangle (const size_t& sz)
      : sz(sz)
    {}

    template<typename T, typename S>
    inline basic_rectangle<T, S>::basic_rectangle (const point_t& topleft,
                                                   const point_t& bottomright)
      : pos(topleft)
      , sz(bottomright - topleft)
    {}

    template<typename T, typename S>
    inline basic_rectangle<T, S>::basic_rectangle (point_type x,
                                                   point_type y,
                                                   size_type width,
                                                   size_type height)
      : pos(x, y)
      , sz(width, height)
    {}

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::position () const -> const point_t& {
      return pos;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::size () const -> const size_t& {
      return sz;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::top_left () const -> const point_t& {
      return pos;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::top_right () const -> point_t {
      return {right(), y()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::bottom_right () const -> point_t {
      return {right(), bottom()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::bottom_left () const -> point_t {
      return {x(), bottom()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::x () const -> point_type {
      return pos.x();
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::y () const -> point_type {
      return pos.y();
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::width () const -> size_type {
      return sz.width();
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::height () const -> size_type {
      return sz.height();
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::x2 () const -> point_type {
      return x() + width();
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::y2 () const -> point_type {
      return y() + height();
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::right () const -> point_type {
      return x() + width() - 1;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::bottom () const -> point_type {
      return y() + height() - 1;
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::set_position (const point_t& pt) {
      pos = pt;
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::set_size (const size_t& s) {
      sz = s;
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::x (point_type x) {
      pos.x(x);
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::y (point_type y) {
      pos.y(y);
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::x2 (point_type new_x) {
      sz.width(new_x - x());
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::y2 (point_type new_y) {
      sz.height(new_y - y());
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::width (size_type width) {
      sz.width(width);
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::height (size_type height) {
      sz.height(height);
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::right (point_type new_x) {
      sz.width(new_x - x() + 1);
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::bottom (point_type new_y) {
      sz.height(new_y - y() + 1);
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::top_left (const point_t& pt) {
      pos = pt;
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::bottom_right (const point_t& pt) {
      sz.width(pt.x() - pos.x());
      sz.height(pt.y() - pos.y());
    }

    template<typename T, typename S>
    inline gui::os::point_type basic_rectangle<T, S>::os_x () const {
      return pos.os_x();
    }

    template<typename T, typename S>
    inline gui::os::point_type basic_rectangle<T, S>::os_y () const {
      return pos.os_y();
    }

    template<typename T, typename S>
    inline gui::os::point_type basic_rectangle<T, S>::os_x2 () const {
      return global::scale<gui::os::point_type>(x2());
    }

    template<typename T, typename S>
    inline gui::os::point_type basic_rectangle<T, S>::os_y2 () const {
      return global::scale<gui::os::point_type>(y2());
    }

    template<typename T, typename S>
    inline gui::os::size_type basic_rectangle<T, S>::os_width () const {
      return size().os_width();
    }

    template<typename T, typename S>
    inline gui::os::size_type basic_rectangle<T, S>::os_height () const {
      return size().os_height();
    }

    template<typename T, typename S>
    inline gui::os::point_type basic_rectangle<T, S>::os_right () const {
      return global::scale<gui::os::point_type>(right());
    }

    template<typename T, typename S>
    inline gui::os::point_type basic_rectangle<T, S>::os_bottom () const {
      return global::scale<gui::os::point_type>(bottom());
    }

#ifdef WIN32
    template<typename T, typename S>
    const basic_rectangle<T, S> basic_rectangle<T, S>::def(point_t(-1, -1), point_t(-2, -2));
#endif // WIN32
#ifdef X11
    template<typename T, typename S>
    const basic_rectangle<T, S> basic_rectangle<T, S>::def(0, 0, 1, 1);
#endif // X11

    template<typename T, typename S>
    basic_rectangle<T, S>::basic_rectangle (const gui::os::rectangle& r)
#ifdef WIN32
      : pos(global::scale<T>(r.left), global::scale<T>(r.top))
      , sz(global::scale<S>(r.right - r.left), global::scale<S>(r.bottom - r.top))
#endif // Win32
#ifdef X11
      : pos(global::scale<T>(r.x), global::scale<T>(r.y))
      , sz(global::scale<S>(r.width), global::scale<S>(r.height))
#endif // X11
    {}

    template<typename T, typename S>
    inline bool basic_rectangle<T, S>::empty () const {
      return size().empty();
    }

    template<typename T, typename S>
    inline bool basic_rectangle<T, S>::is_inside (const point_t& p) const {
      return (p >= pos) && (p <= bottom_right());
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::center_x () const -> point_type {
      return x() + width() / point_type(2);
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::center_y () const -> point_type {
      return y() + height() / point_type(2);
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::center () const -> point_t {
      return {center_x(), center_y()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::with_width (const size_type w) const -> self {
      return {position(), size_t(w, height())};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::with_height (const size_type h) const -> self {
      return {position(), size_t(width(), h)};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::right_width (const size_type w) const -> self {
      return {x2() - w, y(), w, height()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::bottom_height (const size_type h) const -> self {
      return {x(), y2() - h, width(), h};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::with_size (const size_t& sz) const -> self {
      return {position(), sz};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::with_x (const point_type x) const -> self {
      return {point(x, y()), size()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::with_y (const point_type y) const -> self {
      return {point(x(), y), size()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::with_pos (const point_t& pt) const -> self {
      return {pt, size()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::grown (const size_t& s) const -> self {
      return {position() - s, size() + (s + s)};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::shrinked (const size_t& s) const -> self {
      return {position() + s, size() - (s + s)};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::grow (const size_t& s) -> self& {
      pos -= s;
      sz += (s + s);
      return *this;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::shrink (const size_t& s) -> self& {
      pos += s;
      sz -= (s + s);
      return *this;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator- (const size_t& s) const -> self {
      return {position(), size() - s};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator-= (const size_t& s) -> self& {
      sz -= s;
      return *this;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator+ (const size_t& s) const -> self {
      return {position(), sz + s};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator+= (const size_t& s) -> self& {
      sz += s;
      return *this;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator- (const point_t& pt) const -> self {
      return {position() + pt, size()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator-= (const point_t& pt) -> self& {
      pos -= pt;
      return *this;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator+ (const point_t& pt) const -> self {
      return {position() + pt, size()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator+= (const point_t& pt) -> self& {
      pos += pt;
      return *this;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator- (const self& r) const -> self {
      return {point_t {x() - r.x(), y() - r.y()}, size() - r.size()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator-= (const self& r) -> self& {
      operator= (*this - r);
      return *this;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator+ (const self& r) const -> self {
      return {position() + r.position(), size() + r.size()};
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator+= (const self& r) -> self& {
      operator= (*this + r);
      return *this;
    }

    template<typename T, typename S>
    inline auto basic_rectangle<T, S>::operator= (const self& rhs) -> self& {
      pos = rhs.pos;
      sz = rhs.sz;
      return *this;
    }

    template<typename T, typename S>
    inline bool basic_rectangle<T, S>::operator== (const self& rhs) const {
      return (position() == rhs.position()) && (size() == rhs.size());
    }

    template<typename T, typename S>
    inline bool basic_rectangle<T, S>::operator!= (const self& rhs) const {
      return !operator== (rhs);
    }

    template<typename T, typename S>
    inline basic_rectangle<T, S>::operator gui::os::rectangle() const {
      return os();
    }

    template<typename T, typename S>
    gui::os::rectangle basic_rectangle<T, S>::os () const {
      return {
        os_x(), os_y(),
#ifdef WIN32
        os_x2(), os_y2()
#endif // Win32
#ifdef X11
        os_width(), os_height()
#endif // X11
      };
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::move (const point_t& pt) {
      pos += pt;
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::move (const size_t& sz) {
      pos += sz;
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::move_x (point_type dx) {
      pos.move_x(dx);
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::move_y (point_type dy) {
      pos.move_y(dy);
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::move_to_x (point_type new_x) {
      x(new_x);
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::move_to_y (point_type new_y) {
      y(new_y);
    }

    template<typename T, typename S>
    inline void basic_rectangle<T, S>::set_center (const point_t& pt) {
      set_position({ pt.x() - width() / point_type(2), pt.y() - height() / point_type(2) });
    }

    template<typename T, typename S>
    auto basic_rectangle<T, S>::operator|= (const self& rhs) -> self& {
      point_type x0 = std::min(x(), rhs.x());
      point_type y0 = std::min(y(), rhs.y());
      point_type x1 = std::max(x2(), rhs.x2());
      point_type y1 = std::max(y2(), rhs.y2());
      return operator= ({point_t {x0, y0}, point_t {x1, y1}});
    }

    template<typename T, typename S>
    auto basic_rectangle<T, S>::operator| (const self& rhs) const -> self {
      self r = *this;
      return r |= rhs;
    }

    template<typename T, typename S>
    auto basic_rectangle<T, S>::Union (const self& rhs) const -> self {
      return *this | rhs;
    }

    template<typename T, typename S>
    auto basic_rectangle<T, S>::operator&= (const self& rhs) -> self& {
      point_type x0 = std::max(x(), rhs.x());
      point_type y0 = std::max(y(), rhs.y());
      point_type x1 = std::min(x2(), rhs.x2());
      point_type y1 = std::min(y2(), rhs.y2());
      return operator= ({point_t {x0, y0}, point_t {x1, y1}});
    }

    template<typename T, typename S>
    auto basic_rectangle<T, S>::operator& (const self& rhs) const -> self {
      self r = *this;
      return r &= rhs;
    }

    template<typename T, typename S>
    auto basic_rectangle<T, S>::intersection (const self& rhs) const -> self {
      return *this & rhs;
    }

    template<typename T, typename S>
    std::ostream& operator<< (std::ostream& out, const basic_rectangle<T, S>& r) {
      out << r.position() << ", " << r.size();
      return out;
    }

    // --------------------------------------------------------------------------
    namespace global {

      inline rectangle scale (const native_rect& v) {
        return rectangle(scale(v.position()), scale(v.size()));
      }

      inline native_rect scale (const rectangle& v) {
        return native_rect(scale(v.position()), scale(v.size()));
      }

    } // namespace global

  } // namespace core

} // namespace gui
