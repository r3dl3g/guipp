/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
    template<What what, Where where, int offset, typename T, typename S>
    inline void attach::attach_fix (T target, S source)  {
      auto fkt = detail::target<what, where, offset, 10000>::adjust;
      attachments.push_back(std::unique_ptr<attachment_t<T, S>>(new attachment_t<T, S>(target, source, fkt)));
    }

    template<What what, int relativ, int offset, typename T, typename S>
    inline void attach::attach_relative (T target, S source) {
      auto fkt = detail::target<what, detail::convert_from<what>::where, offset, relativ>::adjust;
      attachments.push_back(std::unique_ptr<attachment_t<T, S>>(new attachment_t<T, S>(target, source, fkt)));
    }

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui
