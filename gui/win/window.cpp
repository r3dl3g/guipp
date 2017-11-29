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
# include <X11/cursorfont.h>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/ostreamfmt.h>
#include <gui/win/container.h>
#include <gui/win/window_event_proc.h>
#include <gui/win/window_event_handler.h>


namespace gui {

  namespace win {

#ifdef X11
# define XLIB_ERROR_CODE(a) case a: LogFatal << # a;break;

    bool check_xlib_return (int r) {
# ifndef NDEBUG
      core::global::sync();
# endif
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
#endif // X11

    struct log_hierarchy {
      log_hierarchy (window* win)
        : win(win)
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
    namespace hidden {
      std::map<std::string, class_info> window_class_info_map;
      std::vector<os::window> capture_stack;
    }

    // --------------------------------------------------------------------------
    window::window ()
      : id(0)
    {
      init();
    }

    window::window (const window& rhs)
      : id(0)
      , flags(rhs.flags)
    {
      init();
      if (rhs.is_valid()) {
        container* parent = rhs.get_parent();
        create(rhs.get_window_class(),
               parent ? parent->get_id()
               : IF_WIN32_ELSE(NULL, DefaultRootWindow(core::global::get_instance())),
               rhs.place());
      }
    }

    window::window (window&& rhs)
      : id(0)
      , flags(std::move(rhs.flags))
    {
      init();
      std::swap(id, rhs.id);
    }

    window::~window () {
      destroy();
    }

    void window::create (const class_info& type,
                         const container& parent,
                         const core::rectangle& r) {
      if (parent.is_valid()) {
        create(type, parent.get_id(), r);
      }
    }

    void window::create (const class_info& type,
                         os::window parent_id,
                         const core::rectangle& r) {

      if (get_id()) {
        destroy();
      }

      id = create_window(type, r, parent_id, this);
#ifdef X11
      x11::send_client_message(this, x11::WM_CREATE_WINDOW, this, r);
#endif // X11
    }

    core::point window::window_to_screen (const core::point& pt) const {
      window* p = get_parent();
      return p ? p->client_to_screen(pt) : pt;
    }

    core::point window::screen_to_window (const core::point& pt) const {
      window* p = get_parent();
      return p ? p->screen_to_client(pt) : pt;
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

    bool window::handle_event (const core::event& e, os::event_result& result) const {
      if (any_key_up_event::match(e)) {
        os::key_symbol key = get_key_symbol(e);
        if (key == keys::tab) {
          os::key_state state = get_key_state(e);
          shift_focus(shift_key_bit_mask::is_set(state));
        }
      }
      return events.handle_event(e, result);
    }

    void window::shift_focus (bool backward) const {
      const container* parent = get_parent();
      if (parent) {
        parent->shift_focus(*this, backward);
        redraw_later();
      }
    }

    bool window::can_accept_focus () const {
      return is_focus_accepting() && is_enabled() && is_visible();
    }

    const class_info& window::get_window_class () const {
      return hidden::window_class_info_map[get_class_name()];
    }

    // --------------------------------------------------------------------------

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

    // --------------------------------------------------------------------------
    void window::init ()
    {}

    void window::destroy () {
      if (get_id()) {
        DestroyWindow(get_id());
        id = 0;
      }
    }

    void window::close () {
      if (get_id()) {
        CloseWindow(get_id());
      }
    }

    bool window::is_valid () const {
      return IsWindow(get_id()) != FALSE;
    }

    bool window::is_visible () const {
      return is_valid() && IsWindowVisible(get_id());
    }

    bool window::has_focus () const {
      return is_valid() && (GetFocus() == get_id());
    }

    bool window::is_child () const {
      return is_valid() && (GetWindowLong(get_id(), GWL_STYLE) & WS_CHILD) != WS_CHILD;
    }

    bool window::is_popup () const {
      return is_valid() && (GetWindowLong(get_id(), GWL_STYLE) & WS_POPUP) == WS_POPUP;
    }

    bool window::is_toplevel () const {
      return is_valid() && (GetWindowLong(get_id(), GWL_STYLE) & WS_CHILD) != WS_CHILD;
    }

    bool window::has_border () const {
      return is_valid() && (GetWindowLong(get_id(), GWL_STYLE) & (WS_BORDER | WS_DLGFRAME | WS_THICKFRAME) ? true : false);
    }

    void window::set_parent (const container& parent) {
      if (is_valid() && parent.is_valid()) {
        SetParent(get_id(), parent.get_id());
      }
    }

    container* window::get_parent () const {
      return is_valid() ? (container*)detail::get_window(GetParent(get_id())) : nullptr;
    }

    bool window::is_child_of (const container& parent) const {
      return is_valid() && parent.is_valid() && IsChild(parent.get_id(), get_id()) != FALSE;
    }

    void window::set_visible (bool s) {
      if (is_valid()) {
        ShowWindow(get_id(), s ? SW_SHOWNA : SW_HIDE);
      }
    }

    void window::take_focus () const {
      if (is_valid()) {
        SetFocus(get_id());
        redraw_later();
      }
    }

    void window::to_front () {
      if (is_valid()) {
        SetWindowPos(get_id(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
      }
    }

    void window::to_back () {
      if (is_valid()) {
        SetWindowPos(get_id(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
      }
    }

    void window::redraw_now () const {
      if (is_valid()) {
        RedrawWindow(get_id(), nullptr, nullptr, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_ERASENOW);
      }
    }

    void window::redraw_later () const {
      if (is_valid()) {
        InvalidateRect(get_id(), nullptr, TRUE);
      }
    }

    core::size window::size () const {
      RECT r;
      GetWindowRect(get_id(), &r);
      return core::size(r);
    }

    core::point window::position () const {
      RECT r;
      GetWindowRect(get_id(), &r);
      return screen_to_window(core::point(os::point {r.left, r.top}));
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
      return core::size(r); //
    }

    core::rectangle window::client_area () const {
      RECT r;
      GetClientRect(get_id(), &r);
      return core::rectangle(r); //
    }

    void window::move (const core::point& pt, bool repaint) {
      if (is_valid()) {
        SetWindowPos(get_id(), nullptr, pt.os_x(), pt.os_y(), 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
        if (repaint) {
          redraw_later();
        }
      }
    }

    void window::resize (const core::size& sz, bool repaint) {
      if (is_valid()) {
        SetWindowPos(get_id(), nullptr, 0, 0, sz.os_width(), sz.os_height(), SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
        if (repaint) {
          redraw_later();
        }
      }
    }

    void window::place (const core::rectangle& r, bool repaint) {
      if (is_valid()) {
        MoveWindow(get_id(), r.os_x(), r.os_y(), r.os_width(), r.os_height(), repaint);
      }
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
      if (is_valid()) {
        SetClassLongPtr(get_id(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(c));
      }
    }

    void window::capture_pointer () {
      if (is_valid()) {
        LogDebug << "capture_pointer:" << get_id();
        hidden::capture_stack.push_back(get_id());
        SetCapture(get_id());
        s_wheel_hook.enable();
      }
    }

    void window::uncapture_pointer () {
      if (!hidden::capture_stack.empty()) {
        if (hidden::capture_stack.back() != get_id()) {
          LogFatal << "uncapture_pointer:" << get_id() << " differs from stack back:(" << hidden::capture_stack.back() << ")";
        } else {
          LogDebug << "uncapture_pointer:" << get_id();
        }
        ReleaseCapture();
        hidden::capture_stack.pop_back();
        if (!hidden::capture_stack.empty()) {
          LogDebug << "re-capture_pointer:" << hidden::capture_stack.back();
          SetCapture(hidden::capture_stack.back());
        } else {
          s_wheel_hook.disable();
        }
      }
    }

# ifdef WIN32_DEPRECATED
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

# endif // WIN32_DEPRECATED

    void window::prepare_for_event (os::event_id id)
    {}

    os::window window::create_window (const class_info& type,
                                      const core::rectangle& r,
                                      os::window parent_id,
                                      window* data) {
      auto display = core::global::get_instance();

      os::brush back = ((type.get_background() > 0) && (type.get_background() < 20))
                       ? reinterpret_cast<os::brush>(static_cast<LPARAM>(type.get_background()))
                       : (color::extract<color::part::alpha>(type.get_background()) == 0xff)
                       ? NULL : CreateSolidBrush(type.get_background());

      WNDCLASS wc = {
        /* Register the window class. */
        type.get_class_style(),
        win32::WindowEventProc,
        0,
        0,
        display,
        nullptr,
        type.get_cursor(),
        back,
        nullptr,
        type.get_class_name().c_str()
      };

      ATOM cls_id = RegisterClass(&wc);
      if (cls_id) {
        hidden::window_class_info_map[type.get_class_name()] = type;
      }

      os::window id = CreateWindowEx(type.get_ex_style(),
                                     type.get_class_name().c_str(),
                                     nullptr,         // address of window text
                                     type.get_style(),  // window style
                                     r.os_x(),        // horizontal position of window
                                     r.os_y(),        // vertical position of window
                                     r.os_width(),    // window width
                                     r.os_height(),   // window height
                                     parent_id,       // handle of parent window
                                     NULL,            // handle of menu or child-window identifier
                                     display,         // handle of application instance
                                     data);
      SetWindowLongPtr(id, GWLP_USERDATA, (LONG_PTR)data);

      return id;
    }

    std::string window::get_class_name () const {
      char class_name[256];
      GetClassName(id, class_name, 256);
      return std::string(class_name);
    }

    // --------------------------------------------------------------------------

#endif // WIN32    

#ifdef X11
    namespace hidden {
      std::map<os::window, std::string> window_class_map;
      std::map<window*, os::event_id> window_event_mask;
    }

    void window::init () {
      static int initialized = x11::init_messages();
      prepare_for_event(KeyPressMask);
    }

    void window::destroy () {
      if (get_id()) {
        hidden::window_class_map.erase(get_id());
        check_xlib_return(XDestroyWindow(core::global::get_instance(), get_id()));
        detail::unset_window(get_id());
        id = 0;
      }
    }

    void window::close () {
      send_client_message(this, x11::WM_PROTOCOLS, x11::WM_DELETE_WINDOW);
    }

    bool window::is_valid () const {
      return detail::get_window(get_id()) == this;
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
      XWindowAttributes a = {0};
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

    bool window::is_visible () const {
      if (is_valid()) {
        XWindowAttributes a = {0};
        int result = XGetWindowAttributes(core::global::get_instance(), get_id(), &a);
        return (check_xlib_status(result) && (a.map_state == IsViewable));
      }
      return false;
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

    void window::take_focus () const {
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

    void window::redraw_now () const {
      redraw_later();
      core::global::sync();
    }

    void window::redraw_later () const {
      if (get_id()) {
        if (!x11::needs_redraw(get_id())) {
          x11::set_needs_redraw(get_id());
          check_xlib_return(XClearArea(core::global::get_instance(), get_id(),
                                       0, 0, 1, 1, true));
        }
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
      return size(); // - core::size(1, 1);
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
                                              r.os_x(), r.os_y(),
                                              r.os_width(), r.os_height()));
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
      hidden::capture_stack.push_back(get_id());
      check_xlib_return(XGrabPointer(core::global::get_instance(), get_id(),
                                     False,
                                     ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask,
                                     GrabModeAsync, GrabModeAsync, None, None, CurrentTime));
    }

    void window::uncapture_pointer () {
      if (!hidden::capture_stack.empty()) {
        if (hidden::capture_stack.back() != get_id()) {
          LogFatal << "uncapture_pointer:" << get_id() << " differs from stack back:(" << hidden::capture_stack.back() << ")";
        } else {
          LogDebug << "uncapture_pointer:" << get_id();
        }
        check_xlib_return(XUngrabPointer(core::global::get_instance(), CurrentTime));
        hidden::capture_stack.pop_back();
        if (!hidden::capture_stack.empty()) {
          LogDebug << "re-capture_pointer:" << hidden::capture_stack.back();
          check_xlib_return(XGrabPointer(core::global::get_instance(), hidden::capture_stack.back(),
                                         False,
                                         ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask,
                                         GrabModeAsync, GrabModeAsync, None, None, CurrentTime));
        }
      }
    }

    void window::prepare_for_event (os::event_id mask) {
      if (get_id()) {
        auto i = hidden::window_event_mask.find(this);
        if (i != hidden::window_event_mask.end()) {
          i->second |= mask;
          check_xlib_return(XSelectInput(core::global::get_instance(), get_id(), i->second));
        } else {
          hidden::window_event_mask[this] = mask;
          check_xlib_return(XSelectInput(core::global::get_instance(), get_id(), mask));
        }
      } else {
        os::event_id& old_mask = hidden::window_event_mask[this];
        old_mask |= mask;
      }
    }

    os::window window::create_window (const class_info& type,
                                      const core::rectangle& r,
                                      os::window parent_id,
                                      window* data) {
      auto display = core::global::get_instance();
      os::window id = XCreateSimpleWindow(display,
                                          parent_id,
                                          r.os_x(),
                                          r.os_y(),
                                          r.os_width(),
                                          r.os_height(),
                                          0,
                                          0,
                                          type.get_background());
      detail::set_window(id, data);

      auto i = hidden::window_event_mask.find(data);
      if (i != hidden::window_event_mask.end()) {
        check_xlib_return(XSelectInput(display, id, i->second));
      }

      unsigned long mask = 0;
      XSetWindowAttributes wa;

      if (color::extract<color::part::alpha>(type.get_background()) == 0xff) {
        mask |= CWBackPixmap;
        wa.background_pixmap = None;
      }
      if (type.get_cursor()) {
        mask |= CWCursor;
        wa.cursor = type.get_cursor();
      }
      if (type.get_style ()) {
        mask |= CWWinGravity;
        wa.win_gravity = type.get_style();
      }

      if (mask) {
        XChangeWindowAttributes(display, id, mask, &wa);
      }

      if (0 == hidden::window_class_info_map.count(type.get_class_name())) {
        hidden::window_class_info_map[type.get_class_name()] = type;
      }
      hidden::window_class_map[id] = type.get_class_name();

      return id;
    }

    std::string window::get_class_name () const {
      return hidden::window_class_map[get_id()];
    }

#endif // X11

    // --------------------------------------------------------------------------

  } // win

} // gui
