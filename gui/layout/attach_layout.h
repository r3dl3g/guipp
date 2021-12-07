/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     layout to attach windows relative or absolute to other windows
 *
 * @license   MIT license. See accompanying file LICENSE.
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
      template<int R>
      struct scale {
        static core::point::type calc (core::point::type a, int o);
      };

      // --------------------------------------------------------------------------
      template<Where W, int R = 10000>
      struct source {
        static core::point::type calc (const core::size& sz,
                                       const core::rectangle& outer,
                                       int o);
      };

      template<int R>
      struct source<Where::x, R> {
        static core::point::type calc (const core::size& sz,
                                       const core::rectangle& outer,
                                       int o);
      };

      template<int R>
      struct source<Where::x2, R> {
        static core::point::type calc (const core::size& sz,
                                       const core::rectangle& outer,
                                       int o);
      };

      template<int R>
      struct source<Where::y, R> {
        static core::point::type calc (const core::size& sz,
                                       const core::rectangle& outer,
                                       int o);
      };

      template<int R>
      struct source<Where::y2, R> {
        static core::point::type calc (const core::size& sz,
                                       const core::rectangle& outer,
                                       int o);
      };

      template<int R>
      struct source<Where::width, R> {
        static core::point::type calc (const core::size& sz,
                                       const core::rectangle& outer,
                                       int o);
      };

      template<int R>
      struct source<Where::height, R> {
        static core::point::type calc (const core::size& sz,
                                       const core::rectangle& outer,
                                       int o);
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
      inline core::rectangle geometry (const win::window* w) {
        return {w->position(), w->client_size()};
      }

      inline void geometry (win::window* w, const core::rectangle& r) {
        w->geometry(r);
      }

      inline const void* key (const win::window* w) {
        return w;
      }

      // --------------------------------------------------------------------------
      template<typename T>
      inline core::rectangle geometry (const attachable_layout<T>* w) {
        return w->place;
      }

      template<typename T>
      inline void geometry (attachable_layout<T>* w, const core::rectangle& r) {
        w->layout(r);
      }

      template<typename T>
      inline const void* key (const attachable_layout<T>* w) {
        return w;
      }

    } // namespace detail

    constexpr int make_relative (double r) {
      return static_cast<int>(r * 10000.0);
    }

    // --------------------------------------------------------------------------
    struct attachment {
      using adjust_function = void(*)(core::rectangle&,
                                      const core::size&,
                                      const core::rectangle&);

      explicit attachment (adjust_function adjust)
        : adjust(adjust)
      {}

      virtual ~attachment () = default;

      adjust_function adjust;

      virtual core::rectangle source_geometry () const = 0;
      virtual const void* source_key () const = 0;
      virtual core::rectangle target_geometry () const = 0;
      virtual const void* target_key () const = 0;
      virtual void geometry (const core::rectangle&) const = 0;
      virtual std::unique_ptr<attachment> clone () const = 0;
    };

    // --------------------------------------------------------------------------
    template<typename T, typename S>
    struct attachment_t : public attachment {

      attachment_t (T target, S source, adjust_function adjust)
        : attachment(adjust)
        , target(target)
        , source(source)
      {}

      T target;
      S source;

      core::rectangle source_geometry () const override {
        return detail::geometry(source);
      }

      const void* source_key () const override {
        return detail::key(source);
      }

      core::rectangle target_geometry () const override {
        return detail::geometry(target);
      }

      const void* target_key () const override {
        return detail::key(target);
      }

      void geometry (const core::rectangle& r) const override {
        detail::geometry(target, r);
      }

      std::unique_ptr<attachment> clone () const override {
        return std::unique_ptr<attachment>(new attachment_t(target, source, adjust));
      }
    };

    // --------------------------------------------------------------------------
    class GUIPP_LAYOUT_EXPORT attach {
    public:
      attach () = default;
      attach (const attach&);
      attach (attach&&) = default;

      template<What what, Where where, int offset = 0, typename T, typename S>
      void attach_fix (T target, S source);

      template<What what, int relativ, int offset = 0, typename T, typename S>
      void attach_relative (T target, S source);

      void layout (const core::rectangle& r) const;

    private:
      std::vector<std::unique_ptr<attachment>> attachments;
    };

    template<>
    struct is_layout<attach> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/attach_layout.inl>
