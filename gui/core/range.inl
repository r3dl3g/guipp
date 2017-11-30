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
    range<T>::range (type v)
      : first(v)
      , last(v)
    {}

    template<typename T>
    range<T>::range (type f, type l)
      : first(f)
      , last(l)
    {}

    template<typename T>
    bool range<T>::operator== (const range& rhs) {
      return (first == rhs.first) && (last == rhs.last);
    }

    template<typename T>
    bool range<T>::operator!= (const range& rhs) {
      return (first != rhs.first) || (last != rhs.last);
    }

    template<typename T>
    bool range<T>::is_inside (type i) const {
      return (i > first) && (i < last);
    }

    template<typename T>
    void range<T>::sort () {
      if (first > last) {
        std::swap(first, last);
      }
    }

    template<typename T>
    bool range<T>::empty () const {
      return first >= last;
    }

    template<typename T>
    void range<T>::clear () {
      first = last = type();
    }

    template<typename T>
    std::ostream& operator<< (std::ostream& out, const range<T>& r) {
      out << r.first << ":" << r.last;
      return out;
    }

  } // namespace core

} // namespace gui
