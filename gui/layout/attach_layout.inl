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
      inline void target<What::left, W, O, R>::operator() (core::rectangle& rect,
                                                           const core::size& sz,
                                                           const core::rectangle& outer) {
        rect.x(source<W, O, R>::calc(sz, outer));
      }


      template<Where W, int O, int R>
      inline void target<What::right, W, O, R>::operator() (core::rectangle& rect,
                                                            const core::size& sz,
                                                            const core::rectangle& outer) {
        rect.x2(source<W, O, R>::calc(sz, outer));
      }

      template<Where W, int O, int R>
      inline void target<What::top, W, O, R>::operator() (core::rectangle& rect,
                                                          const core::size& sz,
                                                          const core::rectangle& outer) {
        rect.y(source<W, O, R>::calc(sz, outer));
      }

      template<Where W, int O, int R>
      inline void target<What::bottom, W, O, R>::operator() (core::rectangle& rect,
                                                             const core::size& sz,
                                                             const core::rectangle& outer) {
        rect.y2(source<W, O, R>::calc(sz, outer));
      }

    } //namespace detail

    template<What what, Where where, int offset>
    inline void attach::attach_fix (win::window* target, win::window* source) {
      attachments.push_back({target, source, detail::target<what, where, offset, 10000>()});
    }

    template<What what, int relativ, int offset>
    inline void attach::attach_relative (win::window* target, win::window* source) {
      attachments.push_back({target, source, detail::target<what, detail::convert_from<what>::where, offset, relativ>()});
    }

  } // namespace layout

} // namespace gui
