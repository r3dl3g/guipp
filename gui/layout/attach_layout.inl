/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: layout
 *
 * @file
 */

#pragma once


namespace gui {

  namespace layout {

    namespace detail {

      // --------------------------------------------------------------------------
      template<int R>
      inline core::point::type scale<R>::calc (core::point::type a, int o) {
        return core::point::type(a * core::point::type(R) / 10000.0 + o);
      }

      template<>
      inline core::point::type scale<10000>::calc (core::point::type a, int o) {
        return core::point::type(a + o);
      }

      template<>
      inline core::point::type scale<0>::calc (core::point::type, int o) {
        return core::point::type(o);
      }

      // --------------------------------------------------------------------------
      template<int R>
      inline core::point::type source<Where::x, R>::calc (const core::size& sz,
                                                          const core::rectangle& outer,
                                                          int o) {
        return scale<R>::calc(outer.x(), o);
      }

      template<int R>
      inline core::point::type source<Where::x2, R>::calc (const core::size&,
                                                           const core::rectangle& outer,
                                                           int o) {
        return scale<R>::calc(outer.x2(), o);
      }

      template<int R>
      inline core::point::type source<Where::y, R>::calc (const core::size& sz,
                                                          const core::rectangle& outer,
                                                          int o) {
        return scale<R>::calc(outer.y(), o);
      }

      template<int R>
      inline core::point::type source<Where::y2, R>::calc (const core::size& sz,
                                                           const core::rectangle& outer,
                                                           int o) {
        return scale<R>::calc(outer.y2(), o);
      }

      template<int R>
      inline core::point::type source<Where::width, R>::calc (const core::size& sz,
                                                              const core::rectangle& outer,
                                                              int o) {
        return scale<R>::calc(sz.width(), o);
      }

      template<int R>
      inline core::point::type source<Where::height, R>::calc (const core::size& sz,
                                                               const core::rectangle& outer,
                                                               int o) {
        return scale<R>::calc(sz.height(), o);
      }

      // --------------------------------------------------------------------------
      template<Where W, int O, int R>
      void target<What::left, W, O, R>::adjust (core::rectangle& rect,
                                                       const core::size& sz,
                                                       const core::rectangle& outer) {
        rect.x(source<W, R>::calc(sz, outer, O));
      }


      template<Where W, int O, int R>
      void target<What::right, W, O, R>::adjust (core::rectangle& rect,
                                                        const core::size& sz,
                                                        const core::rectangle& outer) {
        rect.x2(source<W, R>::calc(sz, outer, O));
      }

      template<Where W, int O, int R>
      void target<What::top, W, O, R>::adjust (core::rectangle& rect,
                                                      const core::size& sz,
                                                      const core::rectangle& outer) {
        rect.y(source<W, R>::calc(sz, outer, O));
      }

      template<Where W, int O, int R>
      void target<What::bottom, W, O, R>::adjust (core::rectangle& rect,
                                                         const core::size& sz,
                                                         const core::rectangle& outer) {
        rect.y2(source<W, R>::calc(sz, outer, O));
      }

    } //namespace detail

    // --------------------------------------------------------------------------
    template<typename S, typename T>
    template<What what, Where where, int offset>
    inline void attach_t<S, T>::attach_fix (target_type target, source_type source) {
      auto fkt = detail::target<what, where, offset, 10000>::adjust;
      attachments.push_back(factory<S, T>()(target, source, fkt));
    }

    template<typename S, typename T>
    template<What what, int relativ, int offset>
    inline void attach_t<S, T>::attach_relative (target_type target, source_type source) {
      auto fkt = detail::target<what, detail::convert_from<what>::where, offset, relativ>::adjust;
      attachments.push_back(factory<S, T>()(target, source, fkt));
    }

    // --------------------------------------------------------------------------
    template<typename S, typename T>
    void attach_t<S, T>::layout (const core::rectangle&) const {

      struct place_and_size {
        core::rectangle place;
        core::size size;
      };

      using source_places_t = std::map<source_key, place_and_size, source_comperator>;
      using target_places_t = std::map<target_key, place_and_size, target_comperator>;

      source_places_t splaces;
      target_places_t tplaces;

      for (const attachment& a : attachments) {
        auto t_key = target_traits::key(a.target);
        auto s_key = source_traits::key(a.source);
        auto t_it = tplaces.find(t_key);
        auto s_it = splaces.find(s_key);

        place_and_size source;
        if (s_it != splaces.end()) {
          source = s_it->second;
        } else {
          source.place = source_traits::place(a.source);
          source.size = source_traits::size(a.source);
          splaces[s_key] = source;
        }

        if (t_it != tplaces.end()) {
          a.adjust(t_it->second.place, source.size, source.place);
        } else {
          place_and_size& r = tplaces[t_key];
          r.place = target_traits::place(a.target);
          r.size = target_traits::size(a.target);
          core::size diff = r.place.size() - r.size;
          a.adjust(r.place, source.size, source.place);
          r.size = r.place.size() - diff;
        }
      }
      for (auto& i : tplaces) {
        target_traits::place(i.first, i.second.place);
      }
    }

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui
