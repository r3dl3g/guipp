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
    } // namespace detail

    constexpr int make_relative (double r) {
      return static_cast<int>(r * 10000.0);
    }

    // --------------------------------------------------------------------------
    template<typename S, typename T>
    class attach_t {
    public:
      using source_traits = S;
      using target_traits = T;
      using source_key = typename source_traits::key_type;
      using target_key = typename target_traits::key_type;
      using source_comperator = typename source_traits::comperator;
      using target_comperator = typename target_traits::comperator;
      using source_type = typename source_traits::type;
      using target_type = typename target_traits::type;
      using source_store_type = typename source_traits::store_type;
      using target_store_type = typename target_traits::store_type;

      struct attachment {
        using adjust_function = void(*)(core::rectangle&,
                                        const core::size&,
                                        const core::rectangle&);
        target_store_type target;
        source_store_type source;
        adjust_function adjust;
      };

      template<What what, Where where, int offset = 0>
      void attach_fix (target_type target, source_type source);

      template<What what, int relativ, int offset = 0>
      void attach_relative (target_type target, source_type source);

      void layout (const core::rectangle& sz) const;

    private:
      std::vector<attachment> attachments;
    };

    template<typename S, typename T>
    struct is_layout<attach_t<S, T>> {
      enum {
        value = true
      };
    };

    template<typename S, typename T>
    struct factory {
      using attach = attach_t<S, T>;
      using source_type = typename attach::source_type;
      using target_type = typename attach::target_type;
      using source_store_type = typename attach::source_store_type;
      using target_store_type = typename attach::target_store_type;
      using attachment = typename attach::attachment;
      using adjust_function = typename attachment::adjust_function;

      attachment operator() (target_type target, source_type source, adjust_function fkt) {
        return {target_store_type(target), source_store_type(source), fkt};
      }
    };

    // --------------------------------------------------------------------------
    struct win_attach_traits {
      using key_type = win::window*;
      using comperator = std::less<key_type>;
      using type = win::window&;
      using store_type = win::window&;

      static inline core::rectangle place (const win::window& t) {
        return t.place();
      }

      static inline core::size size (const win::window& t) {
        return t.client_size();
      }

      static inline key_type key (win::window& t) {
        return &t;
      }

      static inline void place (key_type t, const core::rectangle& r) {
        t->place(r);
      }
    };

    // --------------------------------------------------------------------------
    using win_attach = attach_t<win_attach_traits, win_attach_traits>;

    template<>
    struct factory<win_attach_traits, win_attach_traits> {
      using attach = attach_t<win_attach_traits, win_attach_traits>;
      using attachment = typename attach::attachment;
      using adjust_function = typename attachment::adjust_function;

      attachment operator() (win::window& target, win::window& source, adjust_function fkt) {
        return {target, source, fkt};
      }
    };

    // --------------------------------------------------------------------------
    template<typename L>
    struct attachable_layout {
      attachable_layout ()
      {}

      attachable_layout (const L& lay)
        : lay(lay)
      {}

      attachable_layout (L&& lay)
        : lay(std::move(lay))
      {}

      operator L& () {
        return lay;
      }

      void layout (const core::rectangle& r) const {
        place = r;
        lay.layout(r);
      }

      L lay;
      mutable core::rectangle place;
    };

    template<typename L>
    struct is_layout<attachable_layout<L>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
    /*abstract*/ struct attach_base {
      virtual core::rectangle place () const = 0;
      virtual core::size size () const = 0;
      virtual void place (const core::rectangle&) = 0;
      virtual const void* key () const = 0;
      virtual attach_base* clone () const = 0;
      virtual ~attach_base ()
      {}

      bool operator== (const attach_base& rhs) const {
        return key() == rhs.key();
      }
    };

    // --------------------------------------------------------------------------
    struct all_attach_traits {
      using key_type = attach_base*;

      struct key_compare {
        bool operator() (const key_type lhs, const key_type rhs) const {
          return lhs->key() < rhs->key();
        }
      };

      using comperator = key_compare;
      using type = const attach_base&;
      using store_type = std::unique_ptr<attach_base>;

      static inline core::rectangle place (const store_type& t) {
        return t.get()->place();
      }

      static inline core::size size (const store_type& t) {
        return t.get()->size();
      }

      static inline const key_type key (const store_type& t) {
        return t.get();
      }

      static inline void place (key_type k, const core::rectangle& r) {
        k->place(r);
      }
    };

    template<>
    struct factory<all_attach_traits, all_attach_traits> {
      using attach = attach_t<all_attach_traits, all_attach_traits>;
      using attachment = typename attach::attachment;
      using adjust_function = typename attachment::adjust_function;

      static std::unique_ptr<attach_base> make_unique (const attach_base& a) {
          return std::unique_ptr<attach_base>(a.clone());
      }
      attachment operator() (const attach_base& target, const attach_base& source, adjust_function fkt) {
        return {make_unique(target), make_unique(source), fkt};
      }
    };

    // --------------------------------------------------------------------------
    struct attach_win : public attach_base {
      attach_win (win::window& win)
        : win(win)
      {}

      core::rectangle place () const override {
        return win.place();
      }

      core::size size () const override {
        return win.client_size();
      }

      void place (const core::rectangle& r) override {
        win.place(r);
      }

      const void* key () const override {
        return &win;
      }

      attach_base* clone () const override {
        return new attach_win(win);
      }

    private:
      win::window& win;
    };

    // --------------------------------------------------------------------------
    template<typename L>
    struct attach_layout : public attach_base {
      using layout = attachable_layout<L>;

      attach_layout (layout& lay)
        : lay(lay)
      {}

      core::rectangle place () const override {
        return lay.place;
      }

      core::size size () const override {
        return lay.place.size();
      }

      void place (const core::rectangle& r) override {
        lay.layout(r);
      }

      const void* key () const override {
        return &lay;
      }

      attach_layout* clone () const override {
        return new attach_layout(lay);
      }

    private:
      attachable_layout<L>& lay;
    };

    // --------------------------------------------------------------------------
    using attach_all = attach_t<all_attach_traits, all_attach_traits>;

    namespace attach {
      using win = attach_win;

      template<typename L>
      using lay = attach_layout<L>;

    }
    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/attach_layout.inl>
