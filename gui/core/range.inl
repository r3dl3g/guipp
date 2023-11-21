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
 * @brief     begin - end range
 *
 * @license   MIT license. See accompanying file LICENSE.
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
    range<T>::range (const type& f, const type& l)
      : first(f)
      , last(l)
    {}

    template<typename T>
    range<T>::range (type&& f, type&& l)
      : first(std::move(f))
      , last(std::move(l))
    {}

    template<typename T>
    bool range<T>::operator== (const range& rhs) const {
      return (first == rhs.first) && (last == rhs.last);
    }

    template<typename T>
    bool range<T>::operator!= (const range& rhs) const {
      return (first != rhs.first) || (last != rhs.last);
    }

    template<typename T>
    bool range<T>::is_inside (const type& i) const {
      return (i > first) && (i < last);
    }

    template<typename T>
    bool range<T>::is_overlapping (const type& i) const {
      return (i >= first) && (i <= last);
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
    auto range<T>::begin () -> type {
      return first;
    }

    template<typename T>
    auto range<T>::end () -> type {
      return last;
    }

    template<typename T>
    auto range<T>::begin () const -> const type {
      return first;
    }

    template<typename T>
    auto range<T>::end () const -> const type {
      return last;
    }

    template<typename T>
    range<T> min_max (const range<T>& lhs, const range<T>& rhs) {
      return range<T>(std::min(lhs.begin(), rhs.begin()),
                      std::max(lhs.end(), rhs.end()));
    }

    template<typename T>
    range<T> intersection (const range<T>& lhs, const range<T>& rhs) {
      return range<T>(std::max(lhs.begin(), rhs.begin()),
                      std::min(lhs.end(), rhs.end()));
    }

    template<typename T>
    std::ostream& operator<< (std::ostream& out, const range<T>& r) {
      out << r.begin() << ":" << r.end();
      return out;
    }

  } // namespace core

} // namespace gui
