/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
#include <iosfwd>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"


namespace gui {

  namespace core {

    typedef float size_type;

    struct point;

    inline os::size_type convert_size_dimension (size_type v) {
#ifdef WIN32
      return static_cast<os::size_type>(v);
#endif // WIN32
#ifdef X11
      return static_cast<os::size_type>(std::max(v, size_type(0)));
#endif // X11
    }

    struct size {
      typedef size_type type;

      static const size zero;
      static const size one;

      inline size () 
        : m_w(0)
        , m_h(0) 
      {}

      explicit inline size (type i) 
        : m_w(i)
        , m_h(i) 
      {}

      inline size (type width, type height)
        : m_w(width)
        , m_h(height)
      {}

      explicit size (const os::size& sz);

      explicit size (const os::point& pt);

      explicit size (const os::rectangle& r);

#ifdef WIN32
      explicit size(const os::win32::event_param_1& p);
      explicit size(const os::win32::event_param_2& p);
#endif // WIN32
#ifdef X11

      template<typename T>
      explicit size (const T& rhs)
        : m_w(static_cast<type>(rhs.width))
        , m_h(static_cast<type>(rhs.height))
      {}

#endif // X11

      operator os::size () const;

      operator os::point () const;

      os::size os () const;

      inline bool empty () const {
        return (m_w <= 0) || (m_h <= 0);
      }

      inline bool operator== (const size& rhs) const {
        return (width() == rhs.width()) && (height() == rhs.height());
      }

      inline bool operator!= (const size& rhs) const {
        return !operator==(rhs);
      }

      inline size operator+ (const size& rhs) const {
        return {type(width() + rhs.width()), type(height() + rhs.height())};
      }

      inline size operator- (const size& rhs) const {
        return {type(width() - rhs.width()), type(height() - rhs.height())};
      }

      inline size& operator+= (const size& s) {
        m_w += s.m_w;
        m_h += s.m_h;
        return *this;
      }

      inline size& operator-= (const size& s) {
        m_w -= s.m_w;
        m_h -= s.m_h;
        return *this;
      }

      inline type width () const {
        return m_w;
      }

      inline type height () const {
        return m_h;
      }

      inline os::size_type os_width () const {
        return convert_size_dimension(m_w);
      }

      inline os::size_type os_height () const {
        return convert_size_dimension(m_h);
      }

      inline void width (type w) {
        m_w = w;
      }

      inline void height (type h) {
        m_h = h;
      }

    private:
      type m_w;
      type m_h;
    };

    std::ostream& operator<< (std::ostream& out,
                              const size&);

    typedef float point_type;

    struct point {
      typedef point_type type;
      static const point zero;
      static const point one;
      static const point undefined;

      inline point ()
        : m_x(0)
        , m_y(0)
      {}

      explicit inline point (type i)
        : m_x(i)
        , m_y(i)
      {}

      inline point (type x, type y)
        : m_x(x)
        , m_y(y)
      {}

      explicit point (const os::point& pt);

      explicit point (const os::rectangle& r);

#ifdef WIN32
      explicit point(const os::win32::event_param_2& p);
#endif // WIN32
#ifdef X11

      template<typename T>
      explicit point (const T& rhs)
        : m_x(static_cast<type>(rhs.x))
        , m_y(static_cast<type>(rhs.y))
      {}

#endif // X11

      operator os::point () const;
      os::point os () const;

      inline point operator+ (const size& s) const {
        return {type(x() + s.width()), type(y() + s.height())};
      }

      inline point operator- (const size& s) const {
        return {type(x() - s.width()), type(y() - s.height())};
      }

      inline point& operator+= (const size& s) {
        operator=(*this + s);
        return *this;
      }

      inline point& operator-= (const size& s) {
        operator=(*this - s);
        return *this;
      }

      inline point operator- (const point& pt) const {
        return { type(x() - pt.x()), type(y() - pt.y()) };
      }

      inline point operator+ (const point& pt) const {
        return { type(x() + pt.x()), type(y() + pt.y()) };
      }

      inline point& operator+= (const point& pt) {
        x(x() + pt.x());
        y(y() + pt.y());
        return *this;
      }

      inline point& operator-= (const point& pt) {
        x(x() - pt.x());
        y(y() - pt.y());
        return *this;
      }

      inline void move (const point& pt) {
        x(x() + pt.x());
        y(y() + pt.y());
      }

      inline void move_x (type dx) {
        x(x() + dx);
      }

      inline void move_y (type dy) {
        y(y() + dy);
      }

      inline bool operator== (const point& rhs) const {
        return (x() == rhs.x()) && (y() == rhs.y());
      }

      inline bool operator!= (const point& rhs) const {
        return !operator==(rhs);
      }

      inline bool operator< (const point& rhs) const {
        return (x() < rhs.x()) && (y() < rhs.y());
      }

      inline bool operator<= (const point& rhs) const {
        return (x() <= rhs.x()) && (y() <= rhs.y());
      }

      inline bool operator> (const point& rhs) const {
        return (x() > rhs.x()) && (y() > rhs.y());
      }

      inline bool operator>= (const point& rhs) const {
        return (x() >= rhs.x()) && (y() >= rhs.y());
      }

      inline type x () const {
        return m_x;
      }

      inline void x (type x_) {
        m_x = x_;
      }

      inline type y () const {
        return m_y;
      }

      inline void y (type y_) {
        m_y = y_;
      }

      inline os::point_type os_x () const {
        return static_cast<os::point_type>(m_x);
      }

      inline os::point_type os_y () const {
        return static_cast<os::point_type>(m_y);
      }

    private:
      type m_x;
      type m_y;
    };

    std::ostream& operator<< (std::ostream& out,
                              const point&);

    struct rectangle {
      static const rectangle zero;
      static const rectangle def;

      inline rectangle (const point& pos = point::zero,
                        const size& sz = size::zero)
        : tl(pos)
        , br(pos.x() + sz.width(), pos.y() + sz.height()) {
      }

      inline explicit rectangle (const size& sz)
        : br(sz.width()
        , sz.height()) {
      }

      inline rectangle (const point& topleft,
                        const point& bottomright)
        : tl(topleft)
        , br(bottomright) {
      }

      inline rectangle (point::type x,
                        point::type y,
                        size::type width,
                        size::type height)
        : tl(x, y)
        , br(x + width, y + height) {
      }

      explicit rectangle (const os::rectangle& r);

#ifdef WIN32
      explicit rectangle(const os::win32::event_param_2& p);
#endif // WIN32
#ifdef X11

      template<typename T>
      explicit rectangle (const T& rhs)
        : tl(rhs.x, rhs.y), br(rhs.x + rhs.width, rhs.y + rhs.height) {}

#endif // X11

      operator os::rectangle () const;

      os::rectangle os () const;

      inline bool empty () const {
        return (tl.x() >= br.x()) || (tl.y() >= br.y());
      }

      inline bool is_inside (const point& p) const {
        return (p >= tl) && (p < br);
      }

      inline point position () const {
        return tl;
      }

      inline rectangle grown (const core::size& sz) const {
        return {tl - sz, br + sz};
      }

      inline rectangle shrinked (const core::size& sz) const {
        return {tl + sz, br - sz};
      }

      inline rectangle& grow (const core::size& sz) {
        tl -= sz;
        br += sz;
        return *this;
      }

      inline rectangle& shrink (const core::size& sz) {
        tl += sz;
        br -= sz;
        return *this;
      }

      inline point center () const {
        return {point::type((tl.x() + br.x()) / 2), point::type((tl.y() + br.y()) / 2)};
      }

      inline point::type center_x () const {
        return (tl.x() + br.x()) / point::type(2);
      }

      inline point::type center_y () const {
        return (tl.y() + br.y()) / point::type(2);
      }

      inline core::size size () const {
        return {(core::size::type)(br.x() - tl.x()), (core::size::type)(br.y() - tl.y())};
      }

      inline rectangle operator- (const core::size& sz) const {
        return {tl, br - sz};
      }

      inline rectangle& operator-= (const core::size& sz) {
        br -= sz;
        return *this;
      }

      inline rectangle operator+ (const core::size& sz) const {
        return {tl, br + sz};
      }

      inline rectangle& operator+= (const core::size& sz) {
        br += sz;
        return *this;
      }

      inline rectangle operator- (const point& pt) const {
        return {tl + pt, br};
      }

      inline rectangle& operator-= (const point& pt) {
        tl -= pt;
        return *this;
      }

      inline rectangle operator+ (const point& pt) const {
        return {tl + pt, br};
      }

      inline rectangle& operator+= (const point& pt) {
        tl += pt;
        return *this;
      }

      inline bool operator== (const rectangle& rhs) const {
        return (tl == rhs.tl) && (br == rhs.br);
      }

      inline bool operator!= (const rectangle& rhs) const {
        return !operator==(rhs);
      }

      void move (const point& pt);

      void move (const core::size& sz);

      void move_x (point::type dx);

      void move_y (point::type dy);

      void move_to_x (point::type x);

      void move_to_y (point::type y);

      void set_size (const core::size& sz);

      const point& top_left () const {
        return tl;
      }

      point top_right () const {
        return { br.x(), tl.y() };
      }

      const point& bottom_right () const {
        return br;
      }

      point bottom_left () const {
        return { tl.x(), br.y() };
      }

      void top_left (const point& pt) {
        tl = pt;
      }

      void bottom_right (const point& pt) {
        br = pt;
      }

      inline point::type x () const {
        return tl.x();
      }

      inline point::type y () const {
        return tl.y();
      }

      inline point::type x2 () const {
        return br.x();
      }

      inline point::type y2 () const {
        return br.y();
      }

      inline size::type width () const {
        return size::type(br.x() - tl.x());
      }

      inline size::type height () const {
        return size::type(br.y() - tl.y());
      }

      void x (point::type x);

      void y (point::type y);

      void x2 (point::type x);

      void y2 (point::type y);

      void height (size::type height);

      void width (size::type width);

      inline os::point_type os_x () const {
        return static_cast<os::point_type>(x());
      }

      inline os::point_type os_y () const {
        return static_cast<os::point_type>(y());
      }

      inline os::point_type os_x2 () const {
        return static_cast<os::point_type>(x2());
      }

      inline os::point_type os_y2 () const {
        return static_cast<os::point_type>(y2());
      }

      inline os::size_type os_width () const {
        return convert_size_dimension(width());
      }

      inline os::size_type os_height () const {
        return convert_size_dimension(height());
      }

      // union
      rectangle& operator&= (const rectangle& rhs);

      // intersection
      rectangle& operator|= (const rectangle& rhs);

    private:
      point tl;
      point br;
    };

    std::ostream& operator<< (std::ostream& out,
                              const rectangle&);

  } // core

} // gui
