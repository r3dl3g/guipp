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

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/layout.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    enum class What {
      left,
      right,
      top,
      bottom
    };

    // --------------------------------------------------------------------------
    enum class Where {
      x,
      x2,
      y,
      y2,
      width,
      height
    };

    namespace detail {

      template<int O, int R>
      struct scale {
        static core::point_type calc (core::point_type a);
      };

      template<int O>
      struct scale<O, 10000> {
        static core::point_type calc (core::point_type a);
      };

      template<int O>
      struct scale<O, 0> {
        static core::point_type calc (core::point_type a);
      };

      template<Where W, int O, int R = 10000>
      struct source {
        static core::point_type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::x, O, R> {
        static core::point_type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::x2, O, R> {
        static core::point_type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::y, O, R> {
        static core::point_type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::y2, O, R> {
        static core::point_type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::width, O, R> {
        static core::point_type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::height, O, R> {
        static core::point_type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<What T, Where W, int O, int R = 10000>
      struct target {
        void operator() (core::rectangle&,
                         const core::size&,
                         const core::rectangle&);
      };

      template<Where W, int O, int R>
      struct target<What::left, W, O, R> {
        void operator() (core::rectangle& rect,
                         const core::size& sz,
                         const core::rectangle& outer);
      };

      template<Where W, int O, int R>
      struct target<What::right, W, O, R> {
        void operator() (core::rectangle& rect,
                         const core::size& sz,
                         const core::rectangle& outer);
      };

      template<Where W, int O, int R>
      struct target<What::top, W, O, R> {
        void operator() (core::rectangle& rect,
                         const core::size& sz,
                         const core::rectangle& outer);
      };

      template<Where W, int O, int R>
      struct target<What::bottom, W, O, R> {
        void operator() (core::rectangle& rect,
                         const core::size& sz,
                         const core::rectangle& outer);
      };

      struct attachment {
        typedef void (adjust_function)(core::rectangle&,
                                       const core::size&,
                                       const core::rectangle&);

        win::window* target;
        win::window* source;
        std::function<adjust_function> adjust;
      };

      // --------------------------------------------------------------------------
      template<What>
      struct convert_from {};

      template<>
      struct convert_from<What::left> {
        static constexpr Where where = Where::width;
      };

      template<>
      struct convert_from<What::right> {
        static constexpr Where where = Where::width;
      };

      template<>
      struct convert_from<What::top> {
        static constexpr Where where = Where::height;
      };

      template<>
      struct convert_from<What::bottom> {
        static constexpr Where where = Where::height;
      };

      // --------------------------------------------------------------------------
    } // namespace detail

    constexpr int make_relative (double r) {
      return static_cast<int>(r * 10000.0);
    }

    // --------------------------------------------------------------------------
    class attach {
    public:
      attach (win::container* m);
      attach (win::container* m, const attach&);
      attach (win::container* m, attach&&);

      template<What what, Where where, int offset = 0>
      void attach_fix (win::window* target, win::window* source);

      template<What what, int relativ, int offset = 0>
      void attach_relative (win::window* target, win::window* source);

      void layout (const core::size& sz);

    private:
      void init (win::container*);

      std::vector<detail::attachment> attachments;
    };

    // --------------------------------------------------------------------------
    //
    // inlines
    //

    namespace detail {

      // --------------------------------------------------------------------------
      template<int O, int R>
      inline core::point_type scale<O, R>::calc (core::point_type a) {
        return core::point_type(a * core::point_type(R) / 10000.0 + O);
      }

      template<int O>
      inline core::point_type scale<O, 10000>::calc (core::point_type a) {
        return core::point_type(a + O);
      }

      template<int O>
      inline core::point_type scale<O, 0>::calc (core::point_type a) {
        return core::point_type(O);
      }

      // --------------------------------------------------------------------------
      template<int O, int R>
      inline core::point_type source<Where::x, O, R>::calc (const core::size& sz,
                                                            const core::rectangle& outer) {
        return scale<O, R>::calc(outer.x());
      }

      template<int O, int R>
      inline core::point_type source<Where::x2, O, R>::calc (const core::size&,
                                                             const core::rectangle& outer) {
        return scale<O, R>::calc(outer.x2());
      }

      template<int O, int R>
      inline core::point_type source<Where::y, O, R>::calc (const core::size& sz,
                                                            const core::rectangle& outer) {
        return scale<O, R>::calc(outer.y());
      }

      template<int O, int R>
      inline core::point_type source<Where::y2, O, R>::calc (const core::size& sz,
                                                             const core::rectangle& outer) {
        return scale<O, R>::calc(outer.y2());
      }

      template<int O, int R>
      inline core::point_type source<Where::width, O, R>::calc (const core::size& sz,
                                                                const core::rectangle& outer) {
        return scale<O, R>::calc(sz.width());
      }

      template<int O, int R>
      inline core::point_type source<Where::height, O, R>::calc (const core::size& sz,
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

    inline attach::attach (win::container* m) {
      init(m);
    }

    inline attach::attach (win::container* m, const attach&) {
      init(m);
    }

    inline attach::attach (win::container* m, attach&&) {
      init(m);
    }

    template<What what, Where where, int offset>
    inline void attach::attach_fix (win::window* target, win::window* source) {
      attachments.push_back({target, source, detail::target<what, where, offset, 10000>()});
    }

    template<What what, int relativ, int offset>
    inline void attach::attach_relative (win::window* target, win::window* source) {
      attachments.push_back({target, source, detail::target<what, detail::convert_from<what>::where, offset, relativ>()});
    }

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui
