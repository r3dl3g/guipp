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
#include <type_traits>
#include <map>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/layout/layout.h>
#include <gui/win/window.h>


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

      // --------------------------------------------------------------------------
      template<int O, int R>
      struct scale {
        static core::point::type calc (core::point::type a);
      };

      template<int O>
      struct scale<O, 10000> {
        static core::point::type calc (core::point::type a);
      };

      template<int O>
      struct scale<O, 0> {
        static core::point::type calc (core::point::type a);
      };

      // --------------------------------------------------------------------------
      template<Where W, int O, int R = 10000>
      struct source {
        static core::point::type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::x, O, R> {
        static core::point::type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::x2, O, R> {
        static core::point::type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::y, O, R> {
        static core::point::type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::y2, O, R> {
        static core::point::type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::width, O, R> {
        static core::point::type calc (const core::size& sz, const core::rectangle& outer);
      };

      template<int O, int R>
      struct source<Where::height, O, R> {
        static core::point::type calc (const core::size& sz, const core::rectangle& outer);
      };

      // --------------------------------------------------------------------------
      template<What T, Where W, int O, int R = 10000>
      struct target {
        static void adjust (core::rectangle&,
                            const core::size&,
                            const core::rectangle&);
      };

      template<Where W, int O, int R>
      struct target<What::left, W, O, R> {
        static void adjust (core::rectangle& rect,
                            const core::size& sz,
                            const core::rectangle& outer);
      };

      template<Where W, int O, int R>
      struct target<What::right, W, O, R> {
        static void adjust (core::rectangle& rect,
                            const core::size& sz,
                            const core::rectangle& outer);
      };

      template<Where W, int O, int R>
      struct target<What::top, W, O, R> {
        static void adjust (core::rectangle& rect,
                            const core::size& sz,
                            const core::rectangle& outer);
      };

      template<Where W, int O, int R>
      struct target<What::bottom, W, O, R> {
        static void adjust (core::rectangle& rect,
                            const core::size& sz,
                            const core::rectangle& outer);
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
    template<typename S, typename T>
    class attach_t {
    public:
      struct attachment {
        using adjust_function = void(*)(core::rectangle&,
                                        const core::size&,
                                        const core::rectangle&);
        T target;
        S source;
        adjust_function adjust;
      };

      template<What what, Where where, int offset = 0>
      void attach_fix (T target, S source);

      template<What what, int relativ, int offset = 0>
      void attach_relative (T target, S source);

      void layout (const core::rectangle& sz) const;

    private:
      std::vector<attachment> attachments;
    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct attach_traits {
      using key_type = T*;
      static core::rectangle place (const T&);
      static core::size size (const T&);
      static key_type key (T&);
      static void place (key_type, const core::rectangle&);
    };

    // --------------------------------------------------------------------------
    template<>
    struct attach_traits<win::window&> {
      using key_type = win::window*;

      static core::rectangle place (const win::window& t) {
        return t.place();
      }

      static core::size size (const win::window& t) {
        return t.client_size();
      }

      static key_type key (win::window& t) {
        return &t;
      }

      static void place (win::window* t, const core::rectangle& r) {
        t->place(r);
      }
    };

    // --------------------------------------------------------------------------
    using attach = attach_t<win::window&, win::window&>;

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/attach_layout.inl>
