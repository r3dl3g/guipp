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
 * @brief     C++ API: basic window
 *
 * @file
 */


// --------------------------------------------------------------------------
//
// Common includes
//


// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/ostreamfmt.h>
#include <gui/win/container.h>
#include <gui/win/native.h>


namespace gui {

  namespace win {

    namespace {

      template<typename T>
      struct reverse {
        explicit reverse (T& iterable)
          : iterable{iterable}
        {}

        inline auto begin() const {
          return std::rbegin(iterable);
        }

        inline auto end() const {
          return std::rend(iterable);
        }

      private:
        T& iterable;
      };

    }

    // --------------------------------------------------------------------------

    container::container () {
      init();
    }

    container::container (const container& rhs)
      : super(rhs) {
      init();
    }

    container::container (container&& rhs) noexcept
      : super(std::move(rhs)) {
      init();
      for(auto& w : rhs.children) {
        w->set_parent(*this);
      }
    }

    container::~container () {
      for(window* win : children) {
        win->remove_parent();
      }
    }

    void container::init () {
      on_paint([&] (core::context* ctx, core::native_rect* r) {
        native::erase(ctx->drawable(), ctx->graphics(), surface_area() & *r, get_window_class().get_background());
      });
      on_show([&] () {
        set_children_visible();
      });
    }

    bool container::is_parent_of (const window& child) const {
      return child.get_parent() == this;
    }

    bool container::is_sub_window (const window* child) const {
      if (!child) {
        return false;
      }
      if (child == this) {
        return true;
      }
      return is_sub_window(child->get_parent());
    }

    void container::set_children_visible (bool show) {
      for(window* win : children) {
        win->set_visible(show);
      }
    }

    container::window_list_t container::get_children () const {
      return children;
    }

    void container::collect_children (window_list_t& list, const std::function<window_filter>& filter) const {
      for (window* win : children) {
        if (filter(win)) {
          list.push_back(win);
          const container* cont = dynamic_cast<const container*>(win);
          if (cont) {
            cont->collect_children(list, filter);
          }
        }
      }
    }

    void container::add_child (window* w) {
      auto i = std::find(children.begin(), children.end(), w);
      if (i == children.end()) {
        children.push_back(w);
        invalidate();
      }
    }

    void container::remove_child (window* w) {
      auto i = std::find(children.begin(), children.end(), w);
      if (i != children.end()) {
        children.erase(i);
      }
    }

    window* container::window_at_point (const core::native_point& pt) {
      for (window* w : reverse(children)) {
        auto state = w->get_state();
        if (state.created() && state.visible() && state.enabled() && !state.overlapped() && w->surface_area().is_inside(pt)) {
          container* cont = dynamic_cast<container*>(w);
          if (cont) {
            return cont->window_at_point(pt);
          }
          return w;
        }
      }
      return this;
    }

    void container::to_front (window* w) {
      remove_child(w);
      children.push_back(w);
      invalidate();
    }

    void container::to_back (window* w) {
      remove_child(w);
      children.insert(children.begin(), w);
      invalidate();
    }

    bool container::handle_event (const core::event& e, gui::os::event_result& r) {
      if (paint_event::match(e)) {
        core::context* cntxt = paint_event::Caller::get_param<0>(e);
        core::native_rect* clip_rect = paint_event::Caller::get_param<1>(e);
        core::global::set_surface_offset(surface_offset());
        bool ret = super::handle_event(e, r);
        for (auto& w : children) {
          auto rect = w->surface_area();
          if (!clip_rect || (clip_rect->overlap(rect))) {
            auto state = w->get_state();
            if (state.created() && state.visible() && !state.overlapped()) {
              core::clip clp(*cntxt, rect);
              core::global::set_surface_offset(rect.position());
              ret |= w->handle_event(e, r);
            }
          }
        }
        return ret;
      }
      return super::handle_event(e, r);;
    }

    os::event_id container::collect_event_mask () const {
      os::event_id mask = get_event_mask();
      for (auto& w : children) {
        if (!w->get_state().overlapped()) {
          mask |= w->collect_event_mask();
        }
      }
      return mask;
    }

    // --------------------------------------------------------------------------
  } // win

} // gui
