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

namespace gui {

  namespace core {

#ifdef GUIPP_WIN
    template<typename T, typename S, coordinate_system C>
    const basic_rectangle<T, S, C> basic_rectangle<T, S, C>::def(-1, -1, -1, -1);
#elif GUIPP_X11
    template<typename T, typename S, coordinate_system C>
    const basic_rectangle<T, S, C> basic_rectangle<T, S, C>::def(0, 0, 1, 1);
#elif GUIPP_QT
    template<typename T, typename S, coordinate_system C>
    const basic_rectangle<T, S, C> basic_rectangle<T, S, C>::def(0, 0, -1, -1);
#else
#error Unknown target system: const basic_rectangle<T, S, C> basic_rectangle<T, S, C>::def
#endif // GUIPP_QT

    template<typename T, typename S, coordinate_system C>
    const basic_rectangle<T, S, C> basic_rectangle<T, S, C>::zero;

    template<typename T, typename S, coordinate_system C>
    inline basic_rectangle<T, S, C>::basic_rectangle (const point_t& pos,
                                                      const size_t& sz)
      : pos(pos)
      , sz(sz)
    {}

    template<typename T, typename S, coordinate_system C>
    inline basic_rectangle<T, S, C>::basic_rectangle (const size_t& sz)
      : sz(sz)
    {}

    template<typename T, typename S, coordinate_system C>
    inline basic_rectangle<T, S, C>::basic_rectangle (const point_t& topleft,
                                                      const point_t& bottomright)
      : pos(topleft)
      , sz(std::max<T>(0, bottomright.x() - topleft.x() + 1), std::max<T>(0, bottomright.y() - topleft.y() + 1))
    {}

    template<typename T, typename S, coordinate_system C>
    inline basic_rectangle<T, S, C>::basic_rectangle (point_type x,
                                                      point_type y,
                                                      size_type width,
                                                      size_type height)
      : pos(x, y)
      , sz(width, height)
    {}

    template<typename T, typename S, coordinate_system C>
    basic_rectangle<T, S, C>::basic_rectangle (const gui::os::rectangle& r, const context& ctx)
      : pos(r, ctx)
      , sz(r)
    {}

    template<typename T, typename S, coordinate_system C>
    basic_rectangle<T, S, C>::basic_rectangle (const gui::os::point& p, const gui::os::size& s, const context& ctx)
      : pos(p, ctx)
      , sz(s)
    {}

    template<typename T, typename S, coordinate_system C>
    basic_rectangle<T, S, C>::basic_rectangle (const gui::os::size& s)
      : sz(s)
    {}

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::position () const -> const point_t& {
      return pos;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::size () const -> const size_t& {
      return sz;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::top_left () const -> const point_t& {
      return pos;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::top_right () const -> point_t {
      return {right(), y()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::bottom_right () const -> point_t {
      return {right(), bottom()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::bottom_left () const -> point_t {
      return {x(), bottom()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::x2y1 () const -> point_t {
      return {x2(), y()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::x2y2 () const -> point_t {
      return {x2(), y2()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::x1y2 () const -> point_t {
      return {x(), y2()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::x () const -> point_type {
      return pos.x();
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::y () const -> point_type {
      return pos.y();
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::width () const -> size_type {
      return sz.width();
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::height () const -> size_type {
      return sz.height();
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::x2 () const -> point_type {
      return x() + width();
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::y2 () const -> point_type {
      return y() + height();
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::right () const -> point_type {
      return x() + width() - 1;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::bottom () const -> point_type {
      return y() + height() - 1;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::set_position (const point_t& pt) -> self& {
      pos = pt;
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::set_size (const size_t& s) -> self& {
      sz = s;
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::x (point_type x) -> self& {
      pos.x(x);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::y (point_type y) -> self& {
      pos.y(y);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::x2 (point_type new_x) -> self& {
      sz.width(new_x - x());
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::y2 (point_type new_y) -> self& {
      sz.height(new_y - y());
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::width (size_type width) -> self& {
      sz.width(width);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::height (size_type height) -> self& {
      sz.height(height);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::right (point_type new_x) -> self& {
      sz.width(new_x - x() + 1);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::bottom (point_type new_y) -> self& {
      sz.height(new_y - y() + 1);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::top_left (const point_t& pt) -> self& {
      pos = pt;
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::bottom_right (const point_t& pt) -> self& {
      return bottom(pt.x()).right(pt.y());
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::x2y2 (const point_t& pt) -> self& {
      return x2(pt.x()).y2(pt.y());
    }

    template<typename T, typename S, coordinate_system C>
    inline gui::os::point_type basic_rectangle<T, S, C>::os_x (const context& ctx) const {
      return pos.os_x(ctx);
    }

    template<typename T, typename S, coordinate_system C>
    inline gui::os::point_type basic_rectangle<T, S, C>::os_y (const context& ctx) const {
      return pos.os_y(ctx);
    }

    template<typename T, typename S, coordinate_system C>
    inline gui::os::point_type basic_rectangle<T, S, C>::os_x2 (const context& ctx) const {
      return convert::point<T, C>::to_os_x(x2(), ctx);
    }

    template<typename T, typename S, coordinate_system C>
    inline gui::os::point_type basic_rectangle<T, S, C>::os_y2 (const context& ctx) const {
      return convert::point<T, C>::to_os_y(y2(), ctx);
    }

    template<typename T, typename S, coordinate_system C>
    inline gui::os::size_type basic_rectangle<T, S, C>::os_width () const {
      return size().os_width();
    }

    template<typename T, typename S, coordinate_system C>
    inline gui::os::size_type basic_rectangle<T, S, C>::os_height () const {
      return size().os_height();
    }

    template<typename T, typename S, coordinate_system C>
    inline gui::os::point_type basic_rectangle<T, S, C>::os_right (const context& ctx) const {
      return convert::point<T, C>::to_os_x(right(), ctx);
    }

    template<typename T, typename S, coordinate_system C>
    inline gui::os::point_type basic_rectangle<T, S, C>::os_bottom (const context& ctx) const {
      return convert::point<T, C>::to_os_y(bottom(), ctx);
    }

    template<typename T, typename S, coordinate_system C>
    inline bool basic_rectangle<T, S, C>::empty () const {
      return size().empty();
    }

    template<typename T, typename S, coordinate_system C>
    inline bool basic_rectangle<T, S, C>::isnan () const {
      return size().isnan() || position().isnan();
    }

    template<typename T, typename S, coordinate_system C>
    inline bool basic_rectangle<T, S, C>::is_inside (const point_t& p) const {
      return (p >= pos) && (p <= x2y2());
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::center_x () const -> point_type {
      return x() + width() / point_type(2);
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::center_y () const -> point_type {
      return y() + height() / point_type(2);
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::center () const -> point_t {
      return {center_x(), center_y()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::max_radius () const -> size_type {
      return std::min(height(), width()) / size_type(2);
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::with_width (const size_type w) const -> self {
      return {position(), size_t(w, height())};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::with_height (const size_type h) const -> self {
      return {position(), size_t(width(), h)};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::right_width (const size_type w) const -> self {
      return {x2() - w, y(), w, height()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::bottom_height (const size_type h) const -> self {
      return {x(), y2() - h, width(), h};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::with_size (const size_t& sz) const -> self {
      return {position(), sz};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::with_x (const point_type x) const -> self {
      return {point(x, y()), size()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::with_y (const point_type y) const -> self {
      return {point_t(x(), y), size()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::with_pos (const point_t& pt) const -> self {
      return {pt, size()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::with_horizontal (const point_type x, const size_type w) const -> self {
      return {point_t(x, y()), size_t(w, height())};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::with_vertical (const point_type y, const size_type h) const -> self {
      return {point_t(x(), y), size_t(width(), h)};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::grown (const size_t& s) const -> self {
      return {position() - s, size() + (s + s)};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::grown (const size_type top, const size_type bottom, const size_type left, const size_type right) const -> self {
      return self{position() - size_t{left,  top}, size() + size_t{(left + right), (top + bottom)}};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::shrinked (const size_t& s) const -> self {
      return {position() + s, size() - (s + s)};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::shrinked (const size_type top, const size_type bottom, const size_type left, const size_type right) const -> self {
      return self{position() + size_t{left,  top}, size() - size_t{(left + right), (top + bottom)}};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::grow (const size_t& s) -> self& {
      pos = point_t{point_type(pos.x() - s.width()), point_type(pos.y() - s.height())};
      sz += (s + s);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::shrink (const size_t& s) -> self& {
      pos = point_t{point_type(pos.x() + s.width()), point_type(pos.y() + s.height())};
      sz -= (s + s);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::shrink (const size_type top, const size_type bottom, const size_type left, const size_type right) -> self& {
      pos += {left,  top};
      sz -= {(left + right), (top + bottom)};
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::grow (const size_type top, const size_type bottom, const size_type left, const size_type right) -> self& {
      pos -= {left,  top};
      sz += {(left + right), (top + bottom)};
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::square () -> self& {
      const auto s = std::min(width(), height());
      sz = {s, s};
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::squared () const -> self {
      return self(*this).square();
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator- (const size_t& s) const -> self {
      return {position(), size() - s};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator-= (const size_t& s) -> self& {
      sz -= s;
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator+ (const size_t& s) const -> self {
      return {position(), sz + s};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator+= (const size_t& s) -> self& {
      sz += s;
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator- (const point_t& pt) const -> self {
      return {position() - size_t(pt.x(), pt.y()), size()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator-= (const point_t& pt) -> self& {
      pos -= pt;
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator+ (const point_t& pt) const -> self {
      return {position() + pt, size()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator+= (const point_t& pt) -> self& {
      pos += pt;
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator- (const self& r) const -> self {
      return {point_t {x() - r.x(), y() - r.y()}, size() - r.size()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator-= (const self& r) -> self& {
      operator= (*this - r);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator+ (const self& r) const -> self {
      return {position() + r.position(), size() + r.size()};
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator+= (const self& r) -> self& {
      operator= (*this + r);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator*= (double f) -> self& {
      const auto pt = center();
      sz *= f;
      set_center(pt);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator/= (double f) -> self& {
      const auto pt = center();
      sz /= f;
      set_center(pt);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator* (double f) const -> self {
      return self(*this) *= f;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator/ (double f) const -> self {
      return self(*this) /= f;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::operator= (const self& rhs) -> self& {
      pos = rhs.pos;
      sz = rhs.sz;
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline bool basic_rectangle<T, S, C>::operator== (const self& rhs) const {
      return (position() == rhs.position()) && (size() == rhs.size());
    }

    template<typename T, typename S, coordinate_system C>
    inline bool basic_rectangle<T, S, C>::operator!= (const self& rhs) const {
      return !operator== (rhs);
    }

    template<typename T, typename S, coordinate_system C>
    gui::os::rectangle basic_rectangle<T, S, C>::os (const context& ctx) const {
      return {
#ifdef GUIPP_WIN
        os_x(ctx), os_y(ctx),
        os_x2(ctx), os_y2(ctx)
#elif GUIPP_X11
        os_x(ctx), os_y(ctx),
        os_width(), os_height()
#elif GUIPP_QT
        top_left().os(ctx), size().os()
#else
# error Unknown target system: gui::os::rectangle basic_rectangle<T, S, C>::os () const
#endif // GUIPP_QT

      };
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::move (const point_t& pt) -> self& {
      pos += pt;
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::move (const size_t& sz) -> self& {
      pos += sz;
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::move_x (point_type dx) -> self& {
      pos.move_x(dx);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::move_y (point_type dy) -> self& {
      pos.move_y(dy);
      return *this;
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::move_to_x (point_type new_x) -> self& {
      return x(new_x);
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::move_to_y (point_type new_y) -> self& {
      return y(new_y);
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::dx (point_type dx) const -> self {
      return self(*this).move_x(dx);
    }

    template<typename T, typename S, coordinate_system C>
    inline auto  basic_rectangle<T, S, C>::dy (point_type dy) const -> self {
      return self(*this).move_y(dy);
    }

    template<typename T, typename S, coordinate_system C>
    inline auto  basic_rectangle<T, S, C>::dxy (point_type dx, point_type dy) const -> self {
      return self(*this).move({dx, dy});
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::set_center (const point_t& pt) -> self& {
      return set_position({ pt.x() - width() / point_type(2), pt.y() - height() / point_type(2) });
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::set_horizontal (const point_type x_, const size_type w_) -> self& {
      return x(x_).width(w_);
    }

    template<typename T, typename S, coordinate_system C>
    inline auto basic_rectangle<T, S, C>::set_vertical (const point_type y_, const size_type h_) -> self& {
      return y(y_).height(h_);
    }

    template<typename T, typename S, coordinate_system C>
    auto basic_rectangle<T, S, C>::operator|= (const self& rhs) -> self& {
      point_type x0 = std::min(x(), rhs.x());
      point_type y0 = std::min(y(), rhs.y());
      point_type x1 = std::max(right(), rhs.right());
      point_type y1 = std::max(bottom(), rhs.bottom());
      return operator= ({point_t {x0, y0}, point_t {x1, y1}});
    }

    template<typename T, typename S, coordinate_system C>
    auto basic_rectangle<T, S, C>::operator| (const self& rhs) const -> self {
      self r = *this;
      return r |= rhs;
    }

    template<typename T, typename S, coordinate_system C>
    auto basic_rectangle<T, S, C>::Union (const self& rhs) const -> self {
      return *this | rhs;
    }

    template<typename T, typename S, coordinate_system C>
    auto basic_rectangle<T, S, C>::operator&= (const self& rhs) -> self& {
      point_type x0 = std::max(x(), rhs.x());
      point_type y0 = std::max(y(), rhs.y());
      point_type x1 = std::min(x2(), rhs.x2());
      point_type y1 = std::min(y2(), rhs.y2());
      return operator= ({point_t {x0, y0}, size_t {size_type(x1 - x0), size_type(y1 - y0)}});
    }

    template<typename T, typename S, coordinate_system C>
    auto basic_rectangle<T, S, C>::operator& (const self& rhs) const -> self {
      self r = *this;
      return r &= rhs;
    }

    template<typename T, typename S, coordinate_system C>
    auto basic_rectangle<T, S, C>::intersection (const self& rhs) const -> self {
      return *this & rhs;
    }

    template<typename T, typename S, coordinate_system C>
    bool basic_rectangle<T, S, C>::overlap (const self& rhs) const {
      return (x() <= rhs.x2()) && ((rhs.x() <= x2())) && (y() <= rhs.y2()) && ((rhs.y() <= y2()));
    }

    template<typename T, typename S, coordinate_system C>
    std::ostream& operator<< (std::ostream& out, const basic_rectangle<T, S, C>& r) {
      out << r.position() << ", " << r.size();
      return out;
    }

    // --------------------------------------------------------------------------
    namespace global {

      inline rectangle scale_from_native (const native_rect& v) {
        return rectangle(scale_from_native(v.position()), scale_from_native(v.size()));
      }

      inline native_rect scale_to_native (const rectangle& v) {
        return native_rect(scale_to_native(v.position()), scale_to_native(v.size()));
      }

    } // namespace global

  } // namespace core

} // namespace gui
