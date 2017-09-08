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
* @brief     C++ API: basic window
*
* @file
*/


// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <map>

#ifdef X11
#include <X11/cursorfont.h>
#endif // X11
// --------------------------------------------------------------------------
//
// Library includes
//
#include "window.h"
#include "window_event_proc.h"
#include "window_event_handler.h"
#include "ostreamfmt.h"


namespace gui {

  namespace win {

    struct log_hierarchy {
      log_hierarchy (window* win)
        :win(win)
      {}

      window* win;
    };

    inline std::ostream& operator<< (std::ostream& out, const log_hierarchy& lh) {
      const window* w = lh.win;
      while (w) {
        out << " -> " << w->get_id();
        w = w->get_parent();
      }
      return out;
    }

    // --------------------------------------------------------------------------

    std::vector<os::window> capture_stack;

#ifdef WIN32

    struct disable_wheel_hook {
      disable_wheel_hook ()
        : hook_id(0)
      {}

      void enable ();
      void disable ();

    private:
      HHOOK hook_id;
    };

    disable_wheel_hook s_wheel_hook;

    LRESULT CALLBACK mouseInputHook (int nCode, WPARAM wParam, LPARAM lParam) {
      //"if nCode is less than zero, the hook procedure must pass the message to the CallNextHookEx function
      //without further processing and should return the value returned by CallNextHookEx"
      if ((nCode >= 0) && ((wParam == WM_MOUSEWHEEL || wParam == WM_MOUSEHWHEEL))) {
        return -1;
      }

      return ::CallNextHookEx(nullptr, nCode, wParam, lParam);
    }

    void disable_wheel_hook::enable () {
      if (!hook_id) {
        hook_id = SetWindowsHookEx(WH_MOUSE_LL, mouseInputHook, NULL, 0);
      }
    }

    void disable_wheel_hook::disable () {
      if (hook_id) {
        UnhookWindowsHookEx(hook_id);
        hook_id = 0;
      }
    }


    window::window ()
      : id(0)
      , cls(nullptr)
      , focus_accepting(false)
    {}

    window::window (const window& rhs)
      : id(0)
      , cls(rhs.cls)
      , focus_accepting(rhs.focus_accepting)
    {}

    window::window (window&& rhs)
      : id(0)
      , cls(rhs.cls)
      , focus_accepting(rhs.focus_accepting)
    {
      std::swap(id, rhs.id);
    }

    void window::init ()
    {}

    void window::create (const window_class& type,
                         os::window parent_id,
                         const core::rectangle& r) {

      this->cls = &type;

      id = CreateWindowEx(type.get_ex_style(),            // window style
                          type.get_class_name().c_str(),  // address of registered class name
                          nullptr,                        // address of window text
                          type.get_style(),               // window style
                          r.os_x(),                       // horizontal position of window
                          r.os_y(),                       // vertical position of window
                          r.os_width(),                   // window width
                          r.os_height(),                  // window height
                          parent_id,                      // handle of parent window
                          NULL,                           // handle of menu or child-window identifier
                          core::global::get_instance(),   // handle of application instance
                          (LPVOID)this);
      type.prepare(this, parent_id);
      SetWindowLongPtr(id, GWLP_USERDATA, (LONG_PTR)this);
    }

    void window::destroy () {
      if (get_id()) {
        DestroyWindow(get_id());
        id = 0;
      }
    }

    void window::quit () {
      PostQuitMessage(0);
    }

    bool window::is_valid () const {
      return IsWindow(get_id()) != FALSE;
    }

    bool window::is_visible () const {
      return is_valid() && IsWindowVisible(get_id());
    }

    bool window::is_enabled () const {
      return is_valid() && IsWindowEnabled(get_id());
    }

    bool window::has_focus () const {
      return is_valid() && (GetFocus() == get_id());
    }

    bool window::is_child () const {
      return (GetWindowLong(get_id(), GWL_STYLE) & WS_CHILD) != WS_CHILD;
    }

    bool window::is_popup () const {
      return (GetWindowLong(get_id(), GWL_STYLE) & WS_POPUP) == WS_POPUP;
    }

    bool window::is_toplevel () const {
      return (GetWindowLong(get_id(), GWL_STYLE) & WS_CHILD) != WS_CHILD;
    }

    bool window::has_border () const {
      return (GetWindowLong(get_id(), GWL_STYLE) & (WS_BORDER | WS_DLGFRAME | WS_THICKFRAME) ? true : false);
    }

    void window::set_parent (const container& parent) {
      SetParent(get_id(), parent.get_id());
    }

    container* window::get_parent () const {
      return is_valid() ? (container*)detail::get_window(GetParent(get_id())) : nullptr;
    }

    bool window::is_child_of (const container& parent) const {
      return is_valid() && parent.is_valid() && IsChild(parent.get_id(), get_id()) != FALSE;
    }

    void window::set_visible (bool s) {
      ShowWindow(get_id(), s ? SW_SHOWNA : SW_HIDE);
    }

    void window::enable (bool on) {
      EnableWindow(get_id(), on);
      redraw_later();
    }

    void window::take_focus () {
      SetFocus(get_id());
      redraw_later();
    }

    void window::to_front () {
      SetWindowPos(get_id(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }

    void window::to_back () {
      SetWindowPos(get_id(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }

    void window::enable_redraw (bool on) {
      SendMessage(get_id(), WM_SETREDRAW, on, 0);
    }

    void window::redraw_now () {
      RedrawWindow(get_id(), nullptr, nullptr, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_ERASENOW);
    }

    void window::redraw_later () {
      InvalidateRect(get_id(), nullptr, TRUE);
    }

    core::size window::size () const {
      RECT r;
      GetWindowRect(get_id(), &r);
      return core::size(r);
    }

    core::point window::position () const {
      RECT r;
      GetWindowRect(get_id(), &r);
      return screen_to_window(core::point(os::point{ r.left, r.top }));
    }

    core::rectangle window::place () const {
      const core::rectangle pl = absolute_place();
      return core::rectangle(screen_to_window(pl.position()), pl.size());
    }

    core::rectangle window::absolute_place () const {
      RECT r;
      GetWindowRect(get_id(), &r);
      return core::rectangle(r);
    }

    core::point window::absolute_position () const {
      RECT r;
      GetWindowRect(get_id(), &r);
      return core::point(r);
    }

    core::size window::client_size () const {
      RECT r;
      GetClientRect(get_id(), &r);
      return core::size(r);//
    }

    core::rectangle window::client_area () const {
      RECT r;
      GetClientRect(get_id(), &r);
      return core::rectangle(r);//
    }

    void window::move (const core::point& pt, bool repaint) {
      SetWindowPos(get_id(), nullptr, pt.os_x(), pt.os_y(), 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
      if (repaint) {
        redraw_later();
      }
    }

    void window::resize (const core::size& sz, bool repaint) {
      SetWindowPos(get_id(), nullptr, 0, 0, sz.os_width(), sz.os_height(), SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
      if (repaint) {
        redraw_later();
      }
    }

    void window::place (const core::rectangle& r, bool repaint) {
      MoveWindow(get_id(), r.os_x(), r.os_y(), r.os_width(), r.os_height(), repaint);
    }

    core::point window::client_to_screen (const core::point& pt) const {
      POINT Point = pt;
      ClientToScreen(get_id(), &Point);
      return core::point(Point);
    }

    core::point window::screen_to_client (const core::point& pt) const {
      POINT Point = pt;
      ScreenToClient(get_id(), &Point);
      return core::point(Point);
    }

    void window::set_cursor (os::cursor c) {
      SetClassLongPtr(get_id(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(c));
    }

    void window::capture_pointer () {
      LogDebug << "capture_pointer:" << get_id();
      capture_stack.push_back(get_id());
      SetCapture(get_id());
      s_wheel_hook.enable();
    }

    void window::uncapture_pointer () {
      if (!capture_stack.empty()) {
        if (capture_stack.back() != get_id()) {
          LogFatal << "uncapture_pointer:" << get_id() << " differs from stack back:(" << capture_stack.back() << ")";
        } else {
          LogDebug << "uncapture_pointer:" << get_id();
        }
        ReleaseCapture();
        capture_stack.pop_back();
        if (!capture_stack.empty()) {
          LogDebug << "re-capture_pointer:" << capture_stack.back();
          SetCapture(capture_stack.back());
        } else {
          s_wheel_hook.disable();
        }
      }
    }

    os::style window::get_style (os::style mask) const {
      LONG old_style = GetWindowLong(get_id(), GWL_STYLE);
      os::style new_style = old_style & mask;
      return new_style;
    }

    void window::set_style (os::style style, bool enable) {
      LONG new_style = enable ? get_style() | style : get_style(~style);
      SetWindowLong(get_id(), GWL_STYLE, new_style);
      redraw_now();
    }

    void window::prepare_for_event (os::event_id id)
    {}

    // --------------------------------------------------------------------------
    bool container::is_parent_of (const window& child) const {
      return is_valid() && child.is_valid() && IsChild(get_id(), child.get_id()) != FALSE;
    }

    std::vector<window*> container::get_children () const {
      std::vector<window*> list;
      os::window id = GetWindow(get_id(), GW_CHILD);
      while (id) {
        list.push_back(detail::get_window(id));
        id = GetWindow(id, GW_HWNDNEXT);
      }
      return list;
    }

    void container::set_children_visible (bool show) {
      std::vector<window*> children = get_children();
      for (window* win : children) {
        win->set_visible(show);
      }
    }

#endif // WIN32
    window::~window () {
      destroy();
    }

    void window::create (const window_class& type,
                         const container& parent,
                         const core::rectangle& r) {
      create(type, parent.get_id(), r);
    }

    core::point window::window_to_screen (const core::point& pt) const {
      window* p = get_parent();
      return p ? p->client_to_screen(pt) : pt;
    }

    core::point window::screen_to_window (const core::point& pt) const {
      window* p = get_parent();
      return p ? p->screen_to_client(pt) : pt;
    }

    const window_class& window::get_window_class () const {
      return *cls;
    }

    void window::register_event_handler (char const name[], const event_handler_function& f, os::event_id mask) {
      events.register_event_handler(name, f);
      prepare_for_event(mask);
    }

    void window::register_event_handler (char const name[], event_handler_function&& f, os::event_id mask) {
      events.register_event_handler(name, std::move(f));
      prepare_for_event(mask);
    }

    void window::unregister_event_handler (const event_handler_function& f) {
      events.unregister_event_handler(f);
    }

    container* window::get_root () const {
      container* parent = get_parent();
      if (parent) {
        return parent->get_root();
      }
      return (container*)this;
    }

    bool window::handle_event (const core::event& e, os::event_result& result) {
      static win::key_down_event::Matcher matcher;
      if (matcher(e)) {
        os::key_symbol key = get_key_symbol(e);
        if (key == keys::tab) {
          os::key_state state = get_key_state(e);
          shift_focus(shift_key_bit_mask::is_set(state));
        }
      }
      return events.handle_event(e, result);
    }

    void window::shift_focus (bool backward) {
      container* parent = get_parent();
      if (parent) {
        parent->shift_focus(*this, backward);
        redraw_later();
      }
    }

    bool window::accept_focus () const {
      return focus_accepting && is_enabled() && is_visible();
    }

#ifdef X11
#define XLIB_ERROR_CODE(a) case a: LogFatal << #a; break;

    bool check_xlib_return (int r) {
#ifndef NDEBUG
      core::global::sync();
#endif
      switch (r) {
        case Success:
        case True:
          return true;
        XLIB_ERROR_CODE(BadValue)
        XLIB_ERROR_CODE(BadWindow)
        XLIB_ERROR_CODE(BadPixmap)
        XLIB_ERROR_CODE(BadAtom)
        XLIB_ERROR_CODE(BadCursor)
        XLIB_ERROR_CODE(BadFont)
        XLIB_ERROR_CODE(BadMatch)
        XLIB_ERROR_CODE(BadDrawable)
        XLIB_ERROR_CODE(BadAccess)
        XLIB_ERROR_CODE(BadAlloc)
        XLIB_ERROR_CODE(BadColor)
        XLIB_ERROR_CODE(BadGC)
        XLIB_ERROR_CODE(BadIDChoice)
        XLIB_ERROR_CODE(BadName)
        XLIB_ERROR_CODE(BadLength)
        XLIB_ERROR_CODE(BadImplementation)
      }
      return false;
//      return true;
    }

    bool check_xlib_status (Status s) {
      if (s) {
        return true;
      }
//      LogFatal << "xlib Status failed";
      return false;
    }

    typedef std::map<window*, os::event_id> window_event_mask_map;
    window_event_mask_map window_event_mask;

    window::window ()
      : id(0)
      , redraw_disabled(false)
      , window_disabled(false)
      , focus_accepting(false)
      , cls(nullptr)
    {
      init();
    }

    window::window (const window& rhs)
      : id(0)
      , redraw_disabled(rhs.redraw_disabled)
      , window_disabled(rhs.window_disabled)
      , focus_accepting(rhs.focus_accepting)
      , cls(rhs.cls)
    {
      init();
      if (rhs.is_valid()) {
        container* parent = rhs.get_parent();
        create(rhs.get_window_class(),
               parent ? parent->get_id()
                      : DefaultRootWindow(core::global::get_instance()),
               rhs.place());
      }
    }

    window::window (window&& rhs)
      : id(0)
      , redraw_disabled(rhs.redraw_disabled)
      , window_disabled(rhs.window_disabled)
      , focus_accepting(rhs.focus_accepting)
      , cls(rhs.cls)
    {
      std::swap(id, rhs.id);
    }

    void window::init () {
      detail::init_message(detail::WM_CREATE_WINDOW, "WM_CREATE_WINDOW");
      detail::init_message(detail::WM_DELETE_WINDOW, "WM_DELETE_WINDOW");
      detail::init_message(detail::WM_PROTOCOLS, "WM_PROTOCOLS");
      prepare_for_event(KeyPressMask);
    }

    void window::create (const window_class& type,
                         os::window parent_id,
                         const core::rectangle& r) {
      if (get_id()) {
        destroy();
      }

      cls = &type;

      auto display = core::global::get_instance();
//      auto visual = core::global::get_visual();

//      XSetWindowAttributes attr;
//      attr.colormap = XCreateColormap(display, DefaultRootWindow(display), visual->visual, AllocNone);
//      attr.border_pixel = 0;//type.get_background();
//      attr.background_pixel = 0;//type.get_background();

//      id = XCreateWindow(display,
//                         parent_id,
//                         r.os_x(),
//                         r.os_y(),
//                         r.os_width(),
//                         r.os_height(),
//                         0,
//                         visual->depth,
//                         InputOutput,
//                         visual->visual,
//                         CWColormap | CWBorderPixel | CWBackPixel,
//                         &attr);

      id = XCreateSimpleWindow(display,
                               parent_id,
                               r.os_x(),
                               r.os_y(),
                               r.os_width(),
                               r.os_height(),
                               0,
                               0,
                               type.get_background());
      detail::set_window(id, this);

//      core::global::sync();

      type.prepare(this, parent_id);

      window_event_mask_map::iterator i = window_event_mask.find(this);
      if (i != window_event_mask.end()) {
        check_xlib_return(XSelectInput(display, id, i->second));
      }

      send_client_message(this, detail::WM_CREATE_WINDOW, this, r);

    }

    void window::destroy () {
      if (get_id()) {
        check_xlib_return(XDestroyWindow(core::global::get_instance(), get_id()));
        detail::unset_window(get_id());
        id = 0;
      }
    }

    void window::quit () {
      send_client_message(this, detail::WM_PROTOCOLS, detail::WM_DELETE_WINDOW);
    }

    bool window::is_valid () const {
      return detail::get_window(get_id()) == this;
    }

    bool window::is_visible () const {
      if (is_valid()) {
        XWindowAttributes a = { 0 };
        int result = XGetWindowAttributes(core::global::get_instance(), get_id(), &a);
        return (check_xlib_status(result) &&
                (a.map_state == IsViewable));
      }
      return false;
    }

    bool window::is_enabled () const {
      return !window_disabled;
    }

    bool window::has_focus () const {
      Window focus = 0;
      int revert_to = 0;
      if (is_valid()) {
        if (check_xlib_return(XGetInputFocus(core::global::get_instance(), &focus, &revert_to))) {
          return focus == get_id();
        }
      }
      return false;
    }

    bool window::is_child () const {
      Window root = 0;
      Window parent = 0;
      Window *children = 0;
      unsigned int nchildren = 0;

      check_xlib_status(XQueryTree(core::global::get_instance(),
                 get_id(),
                 &root,
                 &parent,
                 &children,
                 &nchildren));
      return parent != root;
    }

    bool window::is_toplevel () const {
      Window root = 0;
      Window parent = 0;
      Window *children = 0;
      unsigned int nchildren = 0;

      check_xlib_status(XQueryTree(core::global::get_instance(),
                 get_id(),
                 &root,
                 &parent,
                 &children,
                 &nchildren));
      return parent == root;
    }

    bool window::is_popup () const {
      return is_toplevel();
    }

    bool window::has_border () const {
      XWindowAttributes a = { 0 };
      return (check_xlib_status(XGetWindowAttributes(core::global::get_instance(), get_id(), &a)) &&
              (a.border_width > 0));
    }

    void window::set_parent (const container& parent) {
      core::point pt = position();
      check_xlib_return(XReparentWindow(core::global::get_instance(), get_id(),
                                        parent.get_id(), pt.x(), pt.y()));
    }

    container* window::get_parent () const {
      Window root_return = 0;
      Window parent_return = 0;
      Window *children_return = 0;
      unsigned int nchildren_return = 0;

      check_xlib_return(XQueryTree(core::global::get_instance(),
                                   get_id(),
                                   &root_return,
                                   &parent_return,
                                   &children_return,
                                   &nchildren_return));
      return (container*)detail::get_window(parent_return);
    }

    bool window::is_child_of (const container& parent) const {
      return get_parent() == &parent;
    }

    void window::set_visible (bool s) {
      if (get_id()) {
        if (s) {
          check_xlib_return(XMapWindow(core::global::get_instance(), get_id()));
        } else {
          check_xlib_return(XUnmapWindow(core::global::get_instance(), get_id()));
        }
      }
    }

    void window::enable (bool on) {
      if (window_disabled == on) {
        window_disabled = !on;

        if (get_window_class().get_cursor()) {
          unsigned long mask = CWCursor;
          XSetWindowAttributes wa = { 0 };
          wa.cursor = on ? get_window_class().get_cursor()
                         : win::cursor::arrow();
          check_xlib_return(XChangeWindowAttributes(core::global::get_instance(), get_id(), mask, &wa));
        }
        redraw_later();
      }
    }

    void window::take_focus () {
      check_xlib_return(XSetInputFocus(core::global::get_instance(), get_id(),
                                       RevertToParent, CurrentTime));
      redraw_later();
    }

    void window::to_front () {
      check_xlib_return(XRaiseWindow(core::global::get_instance(), get_id()));
    }

    void window::to_back () {
      check_xlib_return(XLowerWindow(core::global::get_instance(), get_id()));
    }

    void window::enable_redraw (bool on) {
      redraw_disabled = !on;
    }

    void window::redraw_now () {
      if (get_id()) {
        check_xlib_return(XClearArea(core::global::get_instance(), get_id(),
                                     0, 0, 1, 1, true));
        core::global::sync();
      }
    }

    void window::redraw_later () {
      if (get_id()) {
        check_xlib_return(XClearArea(core::global::get_instance(), get_id(),
                                     0, 0, 1, 1, true));
      }
    }

    core::size window::size () const {
      Window root = 0;
      int x = 0, y = 0;
      unsigned int width = 0, height = 0;
      unsigned int border_width = 0;
      unsigned int depth = 0;
      Window wid = get_id();
      if (wid && check_xlib_status(XGetGeometry(core::global::get_instance(), wid,
                                   &root, &x, &y, &width, &height, &border_width, &depth))) {
        return {core::size::type(width), core::size::type(height)};
      }
      return core::size::zero;
    }

    core::point window::position () const {
      Window root = 0;
      int x = 0, y = 0;
      unsigned int width = 0, height = 0;
      unsigned int border_width = 0;
      unsigned int depth = 0;
      if (check_xlib_return(XGetGeometry(core::global::get_instance(), get_id(),
                            &root, &x, &y, &width, &height, &border_width, &depth))) {
        return {core::point::type(x), core::point::type(y)};
      }
      return core::point::undefined;
    }

    core::rectangle window::place () const {
      Window root = 0;
      int x = 0, y = 0;
      unsigned int width = 0, height = 0;
      unsigned int border_width = 0;
      unsigned int depth = 0;
      if (check_xlib_return(XGetGeometry(core::global::get_instance(), get_id(),
                            &root, &x, &y, &width, &height, &border_width, &depth))) {
        return core::rectangle(core::point::type(x), core::point::type(y),
                               core::size::type(width), core::size::type(height));
      }
      return core::rectangle::def;
    }

    core::rectangle window::absolute_place () const {
      return core::rectangle(absolute_position(), size());
    }

    core::point window::absolute_position () const {
      return client_to_screen(core::point::zero);
    }

    core::size window::client_size () const {
      return size();// - core::size(1, 1);
    }

    core::rectangle window::client_area () const {
      return core::rectangle(client_size());
    }

    void window::move (const core::point& pt, bool repaint) {
      if (position() != pt) {
        check_xlib_return(XMoveWindow(core::global::get_instance(), get_id(),
                                      pt.os_x(), pt.os_y()));
        if (repaint) {
          redraw_later();
        }
      }
    }

    void window::resize (const core::size& sz, bool repaint) {
      if (sz.empty()) {
        set_visible(false);
      } else {
        set_visible();
        if (size() != sz) {
          check_xlib_return(XResizeWindow(core::global::get_instance(), get_id(),
                                        sz.os_width(), sz.os_height()));
          if (repaint) {
            redraw_later();
          }
        }
      }
    }

    void window::place (const core::rectangle& r, bool repaint) {
      if (r.empty()) {
        set_visible(false);
      } else {
        set_visible();
        if (place() != r) {
          check_xlib_return(XMoveResizeWindow(core::global::get_instance(), get_id(),
                                            r.os_x(), r.os_y(), r.os_width(), r.os_height()));
          if (repaint) {
            redraw_later();
          }
        }
      }
    }

    core::point window::client_to_screen (const core::point& pt) const {
        int x, y;
        Window child_return;
        auto display = core::global::get_instance();
        check_xlib_return(XTranslateCoordinates(display,
                                                get_id(),
                                                DefaultRootWindow(display),
                                                pt.os_x(),
                                                pt.os_y(),
                                                &x,
                                                &y,
                                                &child_return));
        return {core::point::type(x), core::point::type(y)};
    }

    core::point window::screen_to_client (const core::point& pt) const {
        int x, y;
        Window child_return;
        auto display = core::global::get_instance();
        check_xlib_return(XTranslateCoordinates(display,
                                                DefaultRootWindow(display),
                                                get_id(),
                                                pt.os_x(),
                                                pt.os_y(),
                                                &x,
                                                &y,
                                                &child_return));
        return {core::point::type(x), core::point::type(y)};
    }

    void window::set_cursor (os::cursor c) {
      XDefineCursor(core::global::get_instance(), get_id(), c);
    }

    void window::capture_pointer () {
      LogDebug << "capture_pointer:" << get_id();
      capture_stack.push_back(get_id());
      check_xlib_return(XGrabPointer(core::global::get_instance(), get_id(),
                                     False,
                                     ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask,
                                     GrabModeAsync, GrabModeAsync, None, None, CurrentTime));
    }

    void window::uncapture_pointer () {
      if (!capture_stack.empty()) {
        if (capture_stack.back() != get_id()) {
          LogFatal << "uncapture_pointer:" << get_id() << " differs from stack back:(" << capture_stack.back() << ")";
        } else {
          LogDebug << "uncapture_pointer:" << get_id();
        }
        check_xlib_return(XUngrabPointer(core::global::get_instance(), CurrentTime));
        capture_stack.pop_back();
        if (!capture_stack.empty()) {
          LogDebug << "re-capture_pointer:" << capture_stack.back();
          check_xlib_return(XGrabPointer(core::global::get_instance(), capture_stack.back(),
                                         False,
                                         ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask,
                                         GrabModeAsync, GrabModeAsync, None, None, CurrentTime));
        }
      }
    }

    void window::prepare_for_event (os::event_id mask) {
      if (get_id()) {
        window_event_mask_map::iterator i = window_event_mask.find(this);
        if (i != window_event_mask.end()) {
          i->second |= mask;
          check_xlib_return(XSelectInput(core::global::get_instance(), id, i->second));
        } else {
          window_event_mask[this] = mask;
          check_xlib_return(XSelectInput(core::global::get_instance(), id, mask));
        }
      } else {
        os::event_id& old_mask = window_event_mask[this];
        old_mask |= mask;
      }
    }

    // --------------------------------------------------------------------------
    bool container::is_parent_of (const window& child) const {
      return child.get_parent() == this;
    }

    void container::set_children_visible (bool show) {
      if (show) {
        check_xlib_return(XMapSubwindows(core::global::get_instance(), get_id()));
      } else {
        check_xlib_return(XUnmapSubwindows(core::global::get_instance(), get_id()));
      }
    }

    std::vector<window*> container::get_children () const {
      std::vector<window*> list;

      if (is_visible()) {
        Window root = 0;
        Window parent = 0;
        Window *children = 0;
        unsigned int nchildren = 0;

        if (check_xlib_status(XQueryTree(core::global::get_instance(),
                                         get_id(),
                                         &root,
                                         &parent,
                                         &children,
                                         &nchildren))) {
          for (unsigned int n = 0; n < nchildren; ++n) {
            list.push_back(detail::get_window(children[n]));
          }
        }
      }
      return list;
    }

#endif // X11
    container::container () {
      init();
    }

    container::container (const container& rhs)
      : super(rhs) {
      init();
    }

    container::container (container&& rhs)
      : super(std::move(rhs)) {
      init();
    }

    void container::init () {
      set_accept_focus(true);
      register_event_handler(REGISTER_FUNCTION, set_focus_event([&](window* w){
        if (w == this) {
          forward_focus(shift_key_bit_mask::is_set(core::global::get_key_state()));
        }
      }));
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

    template<typename iterator>
    bool iterate_focus (iterator begin, iterator end, const window* focus) {
      auto i = std::find(begin, end, focus);
      if (i != end) {
        do {
          ++i;
        } while ((i != end) && !(*i)->accept_focus());
        if (i != end) {
          (*i)->take_focus();
          return true;
        }
      }
      return false;
    }

    void container::shift_focus (window& focus, bool backward) {
      std::vector<window*> children = get_children();
      if (children.size() > 0) {
        if (backward) {
          if (iterate_focus(children.rbegin(), children.rend(), &focus)) {
            return;
          }
        } else {
          if (iterate_focus(children.begin(), children.end(), &focus)) {
            return;
          }
        }
      }
      auto parent = get_parent();
      if (parent) {
        parent->shift_focus(*this, backward);
      } else {
        forward_focus(backward);
      }
    }

    void container::forward_focus (bool backward) {
      std::vector<window*> children = get_children();
      if (children.size() > 0) {
        if (backward) {
          for (auto i = children.rbegin(), e = children.rend(); i != e; ++i) {
            window* next = *i;
            if (next->accept_focus()) {
              next->take_focus();
              return;
            }
          }
        } else {
          for (auto i = children.begin(), e = children.end(); i != e; ++i) {
            window* next = *i;
            if (next->accept_focus()) {
              next->take_focus();
              return;
            }
          }
        }
      }
      window::shift_focus(backward);
    }

    // --------------------------------------------------------------------------
    window_with_text::window_with_text (const std::string& t)
      : text(const_text(t))
    {}

    window_with_text::window_with_text (const text_source& t)
      : text(t)
    {}

    window_with_text::window_with_text (const window_with_text& rhs)
      : super(rhs)
      , text(rhs.text)
    {}

    window_with_text::window_with_text (window_with_text&& rhs)
      : super(rhs)
    {
      std::swap(text, rhs.text);
    }

    void window_with_text::set_text (const std::string& t) {
      set_text(const_text(t));
    }

    void window_with_text::set_text(const text_source& t) {
      text = t;
      redraw_later();
    }

    std::string window_with_text::get_text() const {
      return text();
    }

    // --------------------------------------------------------------------------
    const window_class client_window::clazz("client_window",
#ifdef WIN32
    (os::color)(COLOR_BTNFACE + 1)
#endif // WIN32
#ifdef X11
                                            color::buttonColor()
#endif
    );

    // --------------------------------------------------------------------------
#ifdef WIN32
    void overlapped_window::create (const window_class& type,
                                    const window& parent,
                                    const core::rectangle& r) {
      window::create(type, parent.get_id(), r);
    }

    void overlapped_window::create (const window_class& type,
                                    const core::rectangle& r) {
      window::create(type, GetDesktopWindow(), r);
    }

    void overlapped_window::set_title (const std::string& title) {
      SendMessage(get_id(), WM_SETTEXT, 0, (LPARAM)title.c_str());
    }

    std::string overlapped_window::get_title () const {
      std::string s;
      s.resize(SendMessage(get_id(), WM_GETTEXTLENGTH, 0, 0) + 1);
      SendMessage(get_id(), WM_GETTEXT, (WPARAM)s.capacity(), (LPARAM)&s[0]);
      return s;
    }

    bool overlapped_window::is_top_most() const {
      return (GetWindowLong(get_id(), GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST;
    }

    bool overlapped_window::is_minimized() const {
      return IsIconic(get_id()) != FALSE;
    }

    bool overlapped_window::is_maximized() const {
      return IsZoomed(get_id()) != FALSE;
    }

    void overlapped_window::minimize() {
      ShowWindow(get_id(), SW_MINIMIZE);
    }

    void overlapped_window::maximize() {
      ShowWindow(get_id(), SW_MAXIMIZE);
    }

    void overlapped_window::restore() {
      ShowWindow(get_id(), SW_RESTORE);
    }

    void overlapped_window::set_top_most(bool toplevel) {
      SetWindowPos(get_id(),
                   toplevel ? HWND_TOPMOST : HWND_NOTOPMOST,
                   0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }
#endif // WIN32

#ifdef X11
    void overlapped_window::create (const window_class& clazz,
                                    const core::rectangle& r) {
      super::create(clazz, DefaultRootWindow(core::global::get_instance()), r);
    }

    void overlapped_window::create (const window_class& clazz,
                                    const window& parent,
                                    const core::rectangle& r) {
#ifdef WIN32
      super::create(clazz, parent, r);
#endif // WIN32
#ifdef X11
      os::instance display = core::global::get_instance();
      super::create(clazz, DefaultRootWindow(display), r);
      XSetTransientForHint(display, get_id(), parent.get_id());
#endif
    }

    void overlapped_window::set_title (const std::string& title) {
      check_xlib_status(XStoreName(core::global::get_instance(), get_id(), title.c_str()));
    }

    std::string overlapped_window::get_title () const {
      char *window_name;
      check_xlib_status(XFetchName(core::global::get_instance(), get_id(), &window_name));
      return std::string(window_name);
    }

    Atom ATOM_ATOM = 0;
    Atom NET_WM_STATE = 0;
    Atom NET_WM_STATE_MAXIMIZED_HORZ = 0;
    Atom NET_WM_STATE_MAXIMIZED_VERT = 0;
    Atom NET_WM_STATE_ABOVE = 0;
    Atom NET_WM_STATE_HIDDEN = 0;

#ifndef _NET_WM_STATE_REMOVE
# define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
#endif
#ifndef _NET_WM_STATE_ADD
# define _NET_WM_STATE_ADD           1    /* add/set property */
#endif
#ifndef _NET_WM_STATE_TOGGLE
# define _NET_WM_STATE_TOGGLE        2    /* toggle property  */
#endif

    void init_for_net_wm_state () {
      auto dpy = core::global::get_instance();

      detail::init_message(NET_WM_STATE, "_NET_WM_STATE");
      detail::init_message(NET_WM_STATE_MAXIMIZED_HORZ, "_NET_WM_STATE_MAXIMIZED_HORZ");
      detail::init_message(NET_WM_STATE_MAXIMIZED_VERT, "_NET_WM_STATE_MAXIMIZED_VERT");
      detail::init_message(NET_WM_STATE_ABOVE, "_NET_WM_STATE_ABOVE");
      detail::init_message(NET_WM_STATE_HIDDEN, "_NET_WM_STATE_HIDDEN");
      detail::init_message(ATOM_ATOM, "ATOM");
    }

    bool query_net_wm_state (os::window id,
                             Atom a1,
                             Atom a2 = 0,
                             Atom a3 = 0) {
      auto dpy = core::global::get_instance();

      Atom actual_type_return;
      int actual_format_return;
      unsigned long nitems_return;
      unsigned long bytes_after_return;
      unsigned char *prop_return;

      bool ret_a1 = (a1 ? false : true);
      bool ret_a2 = (a2 ? false : true);
      bool ret_a3 = (a3 ? false : true);
      if (Success == XGetWindowProperty(dpy, id, NET_WM_STATE,
                                        0, 99, false, AnyPropertyType,
                                        &actual_type_return,
                                        &actual_format_return,
                                        &nitems_return,
                                        &bytes_after_return,
                                        &prop_return)) {
        if (actual_type_return == ATOM_ATOM) {
          Atom* atoms = (Atom*)prop_return;
          for (unsigned long i = 0; i < nitems_return; ++i) {
            ret_a1 |= (atoms[i] == a1);
            ret_a2 |= (atoms[i] == a2);
            ret_a3 |= (atoms[i] == a3);
          }
        }
      }
      return ret_a1 && ret_a2 && ret_a3;
    }

    bool overlapped_window::is_maximized () const {
      init_for_net_wm_state();
      return query_net_wm_state(get_id(), NET_WM_STATE_MAXIMIZED_HORZ, NET_WM_STATE_MAXIMIZED_VERT);
    }

    bool overlapped_window::is_top_most () const {
      init_for_net_wm_state();
      return query_net_wm_state(get_id(), NET_WM_STATE_ABOVE);
    }

    bool overlapped_window::is_minimized () const {
      init_for_net_wm_state();
      return query_net_wm_state(get_id(), NET_WM_STATE_HIDDEN);
    }

    void overlapped_window::minimize () {
      XIconifyWindow(core::global::get_instance(), get_id(), core::global::get_screen());
    }

    void send_net_wm_state (os::window id,
                            long action,
                            Atom a1,
                            Atom a2 = 0,
                            Atom a3 = 0) {
      auto dpy = core::global::get_instance();

      XEvent xev;
      memset(&xev, 0, sizeof(xev));
      xev.type = ClientMessage;
      xev.xclient.window = id;
      xev.xclient.message_type = NET_WM_STATE;
      xev.xclient.format = 32;
      xev.xclient.data.l[0] = action;
      xev.xclient.data.l[1] = a1;
      xev.xclient.data.l[2] = a2;
      xev.xclient.data.l[3] = a3;

      XSendEvent(dpy, DefaultRootWindow(dpy), False, SubstructureNotifyMask, &xev);
    }

    void overlapped_window::maximize () {
      init_for_net_wm_state();
      send_net_wm_state(get_id(), _NET_WM_STATE_ADD,
                        NET_WM_STATE_MAXIMIZED_HORZ,
                        NET_WM_STATE_MAXIMIZED_VERT);

    }

    void overlapped_window::restore () {
      init_for_net_wm_state();
      send_net_wm_state(get_id(), _NET_WM_STATE_REMOVE,
                        NET_WM_STATE_MAXIMIZED_HORZ,
                        NET_WM_STATE_MAXIMIZED_VERT);
    }

    void overlapped_window::set_top_most (bool toplevel) {
      init_for_net_wm_state();
      send_net_wm_state(get_id(),
                        toplevel ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE,
                        NET_WM_STATE_ABOVE);
    }

#endif //X11

    win::window_class create_group_window_clazz (os::color v) {
      static int group_window_id = 0;
      return win::window_class(ostreamfmt("group_window-" << group_window_id++), v);
    }

    modal_window::modal_window ()
      : is_modal(false)
    {
      init();
    }

    modal_window::modal_window (const modal_window& rhs)
      : super(rhs)
    {
      init();
    }

    modal_window::modal_window (modal_window&& rhs)
      : super(std::move(rhs))
    {
      init();
    }

    void modal_window::init () {
#ifdef WIN32
      register_event_handler(REGISTER_FUNCTION, close_event([&]() {
        is_modal = false;
      }));
#endif // WIN32
    }

    void modal_window::end_modal () {
      is_modal = false;
#ifdef X11
      redraw_now();
#endif // X11
    }

#ifdef X11
    bool is_deeper_window (os::window main, os::window sub) {
      if (sub == main) {
        return false;
      }

      Window root = 0, parent = 0;
      Window *children = 0;
      unsigned int nchildren = 0;

      XQueryTree(core::global::get_instance(), sub, &root, &parent, &children, &nchildren);
      if (parent) {
        return is_deeper_window(main, parent);
      }
      return true;
    }
#endif // X11

    void modal_window::run_modal () {
      LogDebug << "Enter modal loop";

      os::window win = get_id();

      redraw_later();

      is_modal = true;

      run_loop(is_modal, [&, win](const core::event& e) -> bool {
#ifdef X11
        switch (e.type) {
          case MotionNotify:
          case ButtonPress:
          case ButtonRelease:
          case FocusIn:
          case FocusOut:
          case EnterNotify:
          case LeaveNotify:
            return is_deeper_window(win, e.xany.window);
          break;
        }
#endif // X11

#ifdef WIN32
        if (e.type == WM_HOTKEY) {
#endif // WIN32
#ifdef X11
        if ((e.type == KeyPress) && !is_deeper_window(win, e.xany.window)) {
#endif // X11
          return check_hot_key(e);
        }
        return false;
      });

      LogDebug << "Exit modal loop";
    }

    namespace detail {

      // --------------------------------------------------------------------------
      main_window_class::main_window_class ()
        : window_class("main_window",
#ifdef WIN32
                       (os::color)(COLOR_APPWORKSPACE + 1),
                       cursor::arrow(),
                       WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME,
                       WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_COMPOSITED
#endif // WIN32
#ifdef X11
                       color::workSpaceColor()
#endif
         )
      {}

      void main_window_class::prepare (window* w, os::window parent_id) const {
        window_class::prepare(w, parent_id);
#ifdef X11
        os::instance display = core::global::get_instance();
        Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
        Atom value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NORMAL", False);
        XChangeProperty(display, w->get_id(), type, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&value), 1);

        Atom protocols[] = {
          XInternAtom(display, "WM_TAKE_FOCUS", False),
          detail::WM_DELETE_WINDOW,
        };
        XSetWMProtocols(display, w->get_id(), protocols, 2);
        XWMHints* hints = XGetWMHints(display, w->get_id());
        if (!hints) {
          hints = XAllocWMHints();
        }
        hints->flags |= InputHint;
        hints->input = True;
        XSetWMHints(display, w->get_id(), hints);
#endif
      }

      template<os::platform P = os::system_platform>
      struct popup_window_class_defaults {};

      template<>
      struct popup_window_class_defaults<os::platform::win32> {
        static constexpr os::style style = IF_WIN32_ELSE(WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
        static constexpr os::style ex_style = IF_WIN32_ELSE(WS_EX_PALETTEWINDOW | WS_EX_NOPARENTNOTIFY | WS_EX_COMPOSITED, 0);
        static constexpr os::style class_style = IF_WIN32_ELSE(CS_DBLCLKS | CS_DROPSHADOW, 0);
      };

      template<>
      struct popup_window_class_defaults<os::platform::x11> {
        static constexpr os::style style = 0;
        static constexpr os::style ex_style = 0;
        static constexpr os::style class_style = 0;
      };

      // --------------------------------------------------------------------------
      popup_window_class::popup_window_class ()
        : window_class("POPUP",
                       color::light_gray,
                       window_class_defaults<>::cursor(),
                       popup_window_class_defaults<>::style,
                       popup_window_class_defaults<>::ex_style,
                       popup_window_class_defaults<>::class_style)
      {}

      void popup_window_class::prepare (window* w, os::window parent_id) const {
        window_class::prepare(w, parent_id);
#ifdef X11
        os::instance display = core::global::get_instance();
        Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
        Atom value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU", False);
        XChangeProperty(display, w->get_id(), type, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&value), 1);

        XSetWindowAttributes wa;
        wa.override_redirect = 1;
        XChangeWindowAttributes(display, w->get_id(), CWOverrideRedirect, &wa);
#endif
      }

      template<os::platform P = os::system_platform>
      struct dialog_window_class_defaults {};

      template<>
      struct dialog_window_class_defaults<os::platform::win32> {
        static constexpr os::style style = IF_WIN32_ELSE(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SYSMENU | 
                                                         WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME, 0);
        static constexpr os::style ex_style = IF_WIN32_ELSE(WS_EX_NOPARENTNOTIFY | WS_EX_COMPOSITED, 0);
      };

      template<>
      struct dialog_window_class_defaults<os::platform::x11> {
        static constexpr os::style style = 0;
        static constexpr os::style ex_style = 0;
      };

      dialog_window_class::dialog_window_class ()
        : window_class("dialog_window",
                       color::light_gray,
                       window_class_defaults<>::cursor(),
                       dialog_window_class_defaults<>::style,
                       dialog_window_class_defaults<>::ex_style)
      {}

      void dialog_window_class::prepare (window* w, os::window parent_id) const  {
        window_class::prepare(w, parent_id);
#ifdef X11
        os::instance display = core::global::get_instance();
        Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
        Atom value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
        XChangeProperty(display, w->get_id(), type, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&value), 1);

        type = XInternAtom(display, "_NET_WM_STATE", False);
        value = XInternAtom(display, "_NET_WM_STATE_MODAL", False),
        XChangeProperty(display, w->get_id(), type, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&value), 1);

        type = XInternAtom(display, "WM_CLIENT_LEADER", False);
        XChangeProperty(display, w->get_id(), type, XA_WINDOW, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&parent_id), 1);
#endif
      }

    }

    // --------------------------------------------------------------------------
    detail::popup_window_class popup_window::clazz;
    detail::dialog_window_class dialog_window::clazz;
    detail::main_window_class main_window::clazz;

    // --------------------------------------------------------------------------

  } // win

} // gui
