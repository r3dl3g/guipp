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
      template<int O, int R>
      inline core::point::type scale<O, R>::calc (core::point::type a) {
        return core::point::type(a * core::point::type(R) / 10000.0 + O);
      }

      template<int O>
      inline core::point::type scale<O, 10000>::calc (core::point::type a) {
        return core::point::type(a + O);
      }

      template<int O>
      inline core::point::type scale<O, 0>::calc (core::point::type a) {
        return core::point::type(O);
      }

      // --------------------------------------------------------------------------
      template<int O, int R>
      inline core::point::type source<Where::x, O, R>::calc (const core::size& sz,
                                                            const core::rectangle& outer) {
        return scale<O, R>::calc(outer.x());
      }

      template<int O, int R>
      inline core::point::type source<Where::x2, O, R>::calc (const core::size&,
                                                             const core::rectangle& outer) {
        return scale<O, R>::calc(outer.x2());
      }

      template<int O, int R>
      inline core::point::type source<Where::y, O, R>::calc (const core::size& sz,
                                                            const core::rectangle& outer) {
        return scale<O, R>::calc(outer.y());
      }

      template<int O, int R>
      inline core::point::type source<Where::y2, O, R>::calc (const core::size& sz,
                                                             const core::rectangle& outer) {
        return scale<O, R>::calc(outer.y2());
      }

      template<int O, int R>
      inline core::point::type source<Where::width, O, R>::calc (const core::size& sz,
                                                                const core::rectangle& outer) {
        return scale<O, R>::calc(sz.width());
      }

      template<int O, int R>
      inline core::point::type source<Where::height, O, R>::calc (const core::size& sz,
                                                                 const core::rectangle& outer) {
        return scale<O, R>::calc(sz.height());
      }

      // --------------------------------------------------------------------------
      template<Where W, int O, int R>
      void target<What::left, W, O, R>::adjust (core::rectangle& rect,
                                                       const core::size& sz,
                                                       const core::rectangle& outer) {
        rect.x(source<W, O, R>::calc(sz, outer));
      }


      template<Where W, int O, int R>
      void target<What::right, W, O, R>::adjust (core::rectangle& rect,
                                                        const core::size& sz,
                                                        const core::rectangle& outer) {
        rect.x2(source<W, O, R>::calc(sz, outer));
      }

      template<Where W, int O, int R>
      void target<What::top, W, O, R>::adjust (core::rectangle& rect,
                                                      const core::size& sz,
                                                      const core::rectangle& outer) {
        rect.y(source<W, O, R>::calc(sz, outer));
      }

      template<Where W, int O, int R>
      void target<What::bottom, W, O, R>::adjust (core::rectangle& rect,
                                                         const core::size& sz,
                                                         const core::rectangle& outer) {
        rect.y2(source<W, O, R>::calc(sz, outer));
      }

    } //namespace detail

    // --------------------------------------------------------------------------
    template<typename S, typename T>
    template<What what, Where where, int offset>
    inline void attach_t<S, T>::attach_fix (T target, S source) {
      auto fkt = detail::target<what, where, offset, 10000>::adjust;
      attachments.push_back({target, source, fkt});
    }

    template<typename S, typename T>
    template<What what, int relativ, int offset>
    inline void attach_t<S, T>::attach_relative (T target, S source) {
      auto fkt = detail::target<what, detail::convert_from<what>::where, offset, relativ>::adjust;
      attachments.push_back({target, source, fkt});
    }

    // --------------------------------------------------------------------------
    template<typename S, typename T>
    void attach_t<S, T>::layout (const core::rectangle&) const {

      struct place_and_size {
        core::rectangle place;
        core::size size;
      };

      using st = attach_traits<S>;
      using tt = attach_traits<T>;

      using source_places_t = std::map<typename st::key_type, place_and_size>;
      using target_places_t = std::map<typename tt::key_type, place_and_size>;

      source_places_t splaces;
      target_places_t tplaces;

      for (const attachment& a : attachments) {
        auto t_key = tt::key(a.target);
        auto s_key = st::key(a.source);
        auto t_it = tplaces.find(t_key);
        auto s_it = splaces.find(s_key);

        place_and_size source;
        if (s_it != splaces.end()) {
          source = s_it->second;
        } else {
          source.place = st::place(a.source);
          source.size = st::size(a.source);
          splaces[s_key] = source;
        }

        if (t_it != tplaces.end()) {
          a.adjust(t_it->second.place, source.size, source.place);
        } else {
          place_and_size& r = tplaces[t_key];
          r.place = tt::place(a.target);
          r.size = tt::size(a.target);
          core::size diff = r.place.size() - r.size;
          a.adjust(r.place, source.size, source.place);
          r.size = r.place.size() - diff;
        }
      }
      for (auto& i : tplaces) {
        tt::place(i.first, i.second.place);
      }
    }

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui
