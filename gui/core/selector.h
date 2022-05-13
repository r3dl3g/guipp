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
 * @brief     list specific state informtion
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <set>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui++-core-export.h>


namespace gui {

  namespace core {

    namespace selector {

      // --------------------------------------------------------------------------
      struct single {

        struct iterator {

          using iterator_category = std::input_iterator_tag;
          using value_type = int;
          using difference_type = std::ptrdiff_t;
          using pointer = int*;
          using reference = int&;

          inline explicit iterator (int idx = 0)
            : idx(idx)
          {}

          inline iterator& operator++ () {
            idx = -1;
            return *this;
          }

          inline iterator operator++ (int) {
            iterator retval = *this;
            idx = -1;
            return retval;
          }

          inline bool operator== (iterator other) const {
            return idx == other.idx;
          }

          inline bool operator!= (iterator other) const {
            return idx != other.idx;
          }

          inline int operator* () const {
            return idx;
          }

        private:
          int idx;
        };

        inline single ()
          : selection(-1)
        {}

        inline bool is_selected (int i) const {
          return selection == i;
        }

        inline void set_selected (int i) {
          selection = i;
        }

        inline void set_unselected (int i) {
          selection = i;
        }

        inline void clear_selection () {
          selection = -1;
        }

        inline void select_range (int, int) {
        }

        inline void expand_to (int i) {
          selection = i;
        }

        inline bool has_selection () const {
          return selection != -1;
        }

        inline int get_first_index () const {
          return selection;
        }

        inline int get_last_index () const {
          return selection;
        }

        inline iterator begin () const {
          return iterator(selection);
        }

        inline iterator end () const {
          return iterator(-1);
        }

        inline std::size_t size () const {
          return selection < 0 ? 0 : 1;
        }

        inline bool is_multi_select () const {
          return false;
        }

      private:
        int selection;
      };

      // --------------------------------------------------------------------------
      struct multi {
        typedef std::set<int>::const_iterator iterator;

        inline multi ()
        {}

        inline bool is_selected (int i) const {
          return selection.find(i) != selection.end();
        }

        inline void set_selected (int i) {
          selection.insert(i);
        }

        inline void set_unselected (int i) {
          selection.erase(i);
        }

        inline void clear_selection () {
          selection.clear();
        }

        inline void select_range (int from, int to) {
          for (int i = from; i <= to; ++i) {
            selection.insert(selection.end(), i);
          }
        }

        inline void expand_to (int to) {
          if (to < get_first_index()) {
            select_range(to, get_first_index());
          } else if (to > get_last_index()) {
            select_range(get_last_index(), to);
          }
        }

        inline bool has_selection () const {
          return !selection.empty();
        }

        inline int get_first_index () const {
          return selection.empty() ? 0 : *begin();
        }

        inline int get_last_index () const {
          return selection.empty() ? 0 : *selection.rbegin();
        }

        inline iterator begin () const {
          return selection.begin();
        }

        inline iterator end () const {
          return selection.end();
        }

        inline std::size_t size () const {
          return selection.size();
        }

        inline bool is_multi_select () const {
          return false;
        }

      private:
        std::set<int> selection;
      };

      // --------------------------------------------------------------------------
      template<typename V, typename S>
      struct iterator {
        using container_type = V;
        using selector_type = S;
        using selector_iterator = typename selector_type::iterator;

        using iterator_category = std::input_iterator_tag;
        using value_type = typename container_type::value_type;
        using difference_type = typename container_type::difference_type;
        using pointer = typename container_type::pointer;
        using reference = typename container_type::reference;

        inline explicit iterator (const container_type& vec, selector_iterator i)
          : vec(vec)
          , idx(i)
        {}

        inline iterator& operator++ () {
          ++idx;
          return *this;
        }

        inline iterator operator++ (int) {
          return iterator(vec, idx++);
        }

        inline bool operator== (const iterator& other) const {
          return (&vec == &(other.vec)) && (idx == other.idx);
        }

        inline bool operator!= (const iterator& other) const {
          return (&vec != &(other.vec)) || (idx != other.idx);
        }

        inline const value_type& operator* () const {
          return vec[*idx];
        }

      private:
        const container_type& vec;
        selector_iterator idx;
      };

    } // namespace selector

    // --------------------------------------------------------------------------
    template<typename V, typename S>
    inline selector::iterator<V, S> selection_begin (const V& v, const S& s) {
      return selector::iterator<V, S>(v, s.begin());
    }

    template<typename V, typename S>
    inline selector::iterator<V, S> selection_end (const V& v, const S& s) {
      return selector::iterator<V, S>(v, s.end());
    }

    template<typename V, typename S>
    inline V get_selected_elements (const V& v, const S& s) {
      return V(selection_begin(v, s), selection_end(v, s));
    }

    // --------------------------------------------------------------------------
  } // namespace core

} // namespace gui
