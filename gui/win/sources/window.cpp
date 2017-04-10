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
#include "color.h"
#include "window_event_handler.h"
#include "ostreamfmt.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------

#ifdef WIN32

    window::window()
      : id(0)
      , cls(nullptr)
    {}

    window::~window() {
      destroy();
    }

    void window::create (const window_class& type,
                         os::window parent_id,
                         const core::rectangle& place,
                         os::menu menu) {

      this->cls = &type;

      id = CreateWindowEx(type.get_ex_style(),            // window style
                          type.get_class_name().c_str(),  // address of registered class name
                          nullptr,                        // address of window text
                          type.get_style(),               // window style
                          place.os_x(),                   // horizontal position of window
                          place.os_y(),                   // vertical position of window
                          place.os_width(),               // window width
                          place.os_height(),              // window height
                          parent_id,                      // handle of parent window
                          menu,                           // handle of menu or child-window identifier
                          core::global::get_instance(),   // handle of application instance
                          (LPVOID)this);
      type.prepare(this);
      SetWindowLongPtr(id, GWLP_USERDATA, (LONG_PTR)this);
    }

    void window::create (const window_class& type,
                         const container& parent,
                         const core::rectangle& place,
                         os::menu menu) {
      create(type, parent.get_id(), place, menu);
    }

    const window_class* window::get_window_class() const {
      return cls;
    }

    bool window::is_valid() const {
      return IsWindow(get_id()) != FALSE;
    }

    bool window::is_visible() const {
      return is_valid() && IsWindowVisible(get_id());
    }

    bool window::is_enabled() const {
      return is_valid() && IsWindowEnabled(get_id());
    }

    bool window::has_focus() const {
      return is_valid() && (GetFocus() == get_id());
    }

    bool window::is_child() const {
      return (GetWindowLong(get_id(), GWL_STYLE) & WS_CHILD) != WS_CHILD;
    }

    bool window::is_popup() const {
      return (GetWindowLong(get_id(), GWL_STYLE) & WS_POPUP) == WS_POPUP;
    }

    bool window::is_toplevel() const {
      return (GetWindowLong(get_id(), GWL_STYLE) & WS_CHILD) != WS_CHILD;
    }

    bool window::has_border() const {
      return (GetWindowLong(get_id(), GWL_STYLE) & (WS_BORDER | WS_DLGFRAME | WS_THICKFRAME) ? true : false);
    }

    void window::destroy() {
      DestroyWindow(get_id());
    }

    void window::quit() {
      PostQuitMessage(0);
    }

    void window::set_parent(const container& parent) {
      SetParent(get_id(), parent.get_id());
    }

    container* window::get_parent() const {
      return is_valid() ? (container*)detail::get_window(GetParent(get_id())) : nullptr;
    }

    bool window::is_child_of(const container& parent) const {
      return is_valid() && parent.is_valid() && IsChild(parent.get_id(), get_id()) != FALSE;
    }

    void window::set_visible(bool s) {
      ShowWindow(get_id(), s ? SW_SHOWNA : SW_HIDE);
    }

    void window::enable(bool on) {
      EnableWindow(get_id(), on);
    }

    void window::take_focus () {
      SetFocus(get_id());
    }

    void window::to_front () {
      SetWindowPos(get_id(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }

    void window::to_back () {
      SetWindowPos(get_id(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }

    void window::enable_redraw(bool on) {
      SendMessage(get_id(), WM_SETREDRAW, on, 0);
    }

    void window::redraw_now() {
      RedrawWindow(get_id(), nullptr, nullptr, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_ERASENOW);
    }

    void window::redraw_later() {
      InvalidateRect(get_id(), nullptr, TRUE);
    }

    core::size window::size() const {
      RECT r;
      GetWindowRect(get_id(), &r);
      return core::size(r);
    }

    core::point window::position() const {
      RECT r;
      GetWindowRect(get_id(), &r);
      return screen_to_window(core::point(os::point{ r.left, r.top }));
    }

    core::rectangle window::place() const {
      const core::rectangle pl = absolute_place();
      return core::rectangle(screen_to_window(pl.position()), pl.size());
    }

    core::rectangle window::absolute_place() const {
      RECT r;
      GetWindowRect(get_id(), &r);
      return core::rectangle(r);
    }

    core::point window::absolute_position() const {
      RECT r;
      GetWindowRect(get_id(), &r);
      return core::point(r);
    }

    core::size window::client_size() const {
      RECT r;
      GetClientRect(get_id(), &r);
      return core::size(r);//
    }

    core::rectangle window::client_area() const {
      RECT r;
      GetClientRect(get_id(), &r);
      return core::rectangle(r);//
    }

    void window::move(const core::point& pt, bool repaint) {
      SetWindowPos(get_id(), nullptr, pt.os_x(), pt.os_y(), 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
      if (repaint) {
        redraw_later();
      }
    }

    void window::resize(const core::size& sz, bool repaint) {
      SetWindowPos(get_id(), nullptr, 0, 0, sz.os_width(), sz.os_height(), SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
      if (repaint) {
        redraw_later();
      }
    }

    void window::place(const core::rectangle& r, bool repaint) {
      MoveWindow(get_id(), r.os_x(), r.os_y(), r.os_width(), r.os_height(), repaint);
    }

    core::point window::window_to_screen(const core::point& pt) const {
      window* p = get_parent();
      return p ? p->client_to_screen(pt) : pt;
    }

    core::point window::screen_to_window(const core::point& pt) const {
      window* p = get_parent();
      return p ? p->screen_to_client(pt) : pt;
    }

    core::point window::client_to_screen(const core::point& pt) const {
      POINT Point = pt;
      ClientToScreen(get_id(), &Point);
      return core::point(Point);
    }

    core::point window::screen_to_client(const core::point& pt) const {
      POINT Point = pt;
      ScreenToClient(get_id(), &Point);
      return core::point(Point);
    }

    void window::capture_pointer() {
      SetCapture(get_id());
    }

    void window::uncapture_pointer() {
      ReleaseCapture();
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

    bool container::is_parent_of(const window& child) const {
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

#endif // WIN32

#ifdef X11

    window::window ()
      : id(0)
      , redraw_disabled(false)
      , window_disabled(false)
    {
      if (!detail::WM_CREATE_WINDOW) {
        detail::WM_CREATE_WINDOW = XInternAtom(core::global::get_instance(), "WM_CREATE_WINDOW", False);
      }
    }

    window::~window () {
      destroy();
    }

    void window::create (const window_class& type,
                         os::window parent_id,
                         const core::rectangle& place,
                         os::menu) {
      destroy();
      cls = &type;
      os::instance display = core::global::get_instance();
      id = XCreateSimpleWindow(display,
                               parent_id,
                               place.x(),
                               place.y(),
                               place.width(),
                               place.height(),
                               type.get_class_style(),
                               type.get_foreground(),
                               type.get_background().color());
      detail::set_window(id, this);
      type.prepare(this);
      send_client_message(this, detail::WM_CREATE_WINDOW, this, place);
    }

    void window::create (const window_class& type,
                         const container& parent,
                         const core::rectangle& place,
                         os::menu menu) {
      create(type, parent.get_id(), place, menu);
    }

    const window_class* window::get_window_class() const {
      return cls;
    }

    bool window::is_valid () const {
      return detail::get_window(get_id()) == this;
    }

    bool window::is_visible () const {
      XWindowAttributes a;
      return (XGetWindowAttributes(core::global::get_instance(), get_id(), &a) &&
              (a.map_state == IsViewable));
    }

    bool window::is_enabled () const {
      return !window_disabled;
    }

    bool window::has_focus() const {
      Window focus;
      int revert_to;
      if (is_valid()) {
        int result = XGetInputFocus(core::global::get_instance(), &focus, &revert_to);
        return focus == get_id();
      }
      return false;
    }

    bool window::is_child () const {
      Window root;
      Window parent;
      Window *children;
      unsigned int nchildren;

      XQueryTree(core::global::get_instance(),
                 get_id(),
                 &root,
                 &parent,
                 &children,
                 &nchildren);
      return parent != root;
    }

    bool window::is_toplevel () const {
      Window root;
      Window parent;
      Window *children;
      unsigned int nchildren;

      XQueryTree(core::global::get_instance(),
                 get_id(),
                 &root,
                 &parent,
                 &children,
                 &nchildren);
      return parent == root;
    }

    bool window::is_popup () const {
      return is_toplevel();
    }

    bool window::has_border () const {
      XWindowAttributes a;
      return (XGetWindowAttributes(core::global::get_instance(), get_id(), &a) &&
              (a.border_width > 0));
    }

    void window::destroy () {
      if (get_id()) {
        XDestroyWindow(core::global::get_instance(), get_id());
        detail::unset_window(get_id());
        id = 0;
      }
    }

    void window::quit () {
    }

    void window::set_parent (const container& parent) {
      core::point pt = position();
      XReparentWindow(core::global::get_instance(), get_id(), parent.get_id(), pt.x(), pt.y());
    }

    container* window::get_parent () const {
      Window root_return;
      Window parent_return;
      Window *children_return;
      unsigned int nchildren_return;

      XQueryTree(core::global::get_instance(),
                 get_id(),
                 &root_return,
                 &parent_return,
                 &children_return,
                 &nchildren_return);
      return (container*)detail::get_window(parent_return);
    }

    bool window::is_child_of (const container& parent) const {
      return get_parent() == &parent;
    }

    void window::set_visible (bool s) {
      if (s) {
        XMapWindow(core::global::get_instance(), get_id());
      } else {
        XUnmapWindow(core::global::get_instance(), get_id());
      }
    }

    void window::enable (bool on) {
      if (window_disabled == on) {
        window_disabled = !on;

        if (get_window_class()->get_cursor()) {
          unsigned long mask = CWCursor;
          XSetWindowAttributes wa;
          wa.cursor = on ? get_window_class()->get_cursor()
                         : XCreateFontCursor(core::global::get_instance(), XC_arrow);
          XChangeWindowAttributes(core::global::get_instance(), get_id(), mask, &wa);
        }
        redraw_later();
      }
    }

    void window::take_focus () {
      XSetInputFocus(core::global::get_instance(), get_id(), RevertToParent, CurrentTime);
    }

    void window::to_front () {
      XRaiseWindow(core::global::get_instance(), get_id());
    }

    void window::to_back () {
      XLowerWindow(core::global::get_instance(), get_id());
    }

    void window::enable_redraw (bool on) {
      redraw_disabled = !on;
    }

    void window::redraw_now () {
      XClearArea(core::global::get_instance(), get_id(), 0, 0, 0, 0, true);
      XFlush(core::global::get_instance());
    }

    void window::redraw_later () {
      XClearArea(core::global::get_instance(), get_id(), 0, 0, 0, 0, true);
    }

    core::size window::size () const {
      Window root;
      int x = 0, y = 0;
      unsigned int width = 0, height = 0;
      unsigned int border_width = 0;
      unsigned int depth = 0;
      if (XGetGeometry(core::global::get_instance(), get_id(),
                       &root, &x, &y, &width, &height, &border_width, &depth)) {
        return {core::size::type(width), core::size::type(height)};
      }
      return core::size::zero;
    }

    core::point window::position () const {
      Window root;
      int x = 0, y = 0;
      unsigned int width = 0, height = 0;
      unsigned int border_width = 0;
      unsigned int depth = 0;
      if (XGetGeometry(core::global::get_instance(), get_id(),
                       &root, &x, &y, &width, &height, &border_width, &depth)) {
        return {core::point::type(x), core::point::type(y)};
      }
      return core::point::undefined;
    }

    core::rectangle window::place () const {
      Window root;
      int x = 0, y = 0;
      unsigned int width = 0, height = 0;
      unsigned int border_width = 0;
      unsigned int depth = 0;
      if (XGetGeometry(core::global::get_instance(), get_id(),
                       &root, &x, &y, &width, &height, &border_width, &depth)) {
        return core::rectangle(core::point::type(x), core::point::type(y),
                               core::size::type(width), core::size::type(height));
      }
      return core::rectangle::def;
    }

    core::rectangle window::absolute_place () const {
      return core::rectangle(absolute_position(), size());
    }

    core::point window::absolute_position () const {
      return window_to_screen(core::point::zero);
    }

    core::size window::client_size () const {
      return size();// - core::size(1, 1);
    }

    core::rectangle window::client_area () const {
      return core::rectangle(client_size());
    }

    void window::move (const core::point& pt, bool repaint) {
      XMoveWindow(core::global::get_instance(), get_id(), pt.x(), pt.y());
      if (repaint) {
        redraw_later();
      }
    }

    void window::resize (const core::size& sz, bool repaint) {
      XResizeWindow(core::global::get_instance(), get_id(), sz.width(), sz.height());
      if (repaint) {
        redraw_later();
      }
    }

    void window::place (const core::rectangle& r, bool repaint) {
      XMoveResizeWindow(core::global::get_instance(), get_id(), r.x(), r.y(), r.width(), r.height());
      if (repaint) {
        redraw_later();
      }
    }

    core::point window::window_to_screen (const core::point& pt) const {
      int x, y;
      Window child_return;
      XTranslateCoordinates(core::global::get_instance(),
                            get_id(),
                            RootWindow(core::global::get_instance(),
                                       DefaultScreen(core::global::get_instance())),
                            pt.x(),
                            pt.y(),
                            &x,
                            &y,
                            &child_return);
      return {core::point::type(x), core::point::type(y)};
    }

    core::point window::screen_to_window (const core::point& pt) const {
      int x, y;
      Window child_return;
      XTranslateCoordinates(core::global::get_instance(),
                            RootWindow(core::global::get_instance(),
                                       DefaultScreen(core::global::get_instance())),
                            get_id(),
                            pt.x(),
                            pt.y(),
                            &x,
                            &y,
                            &child_return);
      return {core::point::type(x), core::point::type(y)};
    }

    core::point window::client_to_screen (const core::point& pt) const {
      return window_to_screen(pt);
    }

    core::point window::screen_to_client (const core::point& pt) const {
      return screen_to_window(pt);
    }

    void window::capture_pointer() {
      XGrabPointer(core::global::get_instance(), get_id(), False,
        ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
        GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
    }

    void window::uncapture_pointer() {
      XUngrabPointer(core::global::get_instance(), CurrentTime);
    }

    bool container::is_parent_of (const window& child) const {
      return child.get_parent() == this;
    }

    std::vector<window*> container::get_children () const {
      std::vector<window*> list;

      Window root;
      Window parent;
      Window *children;
      unsigned int nchildren;

      if (XQueryTree(core::global::get_instance(),
                     get_id(),
                     &root,
                     &parent,
                     &children,
                     &nchildren)) {
        for (unsigned int n = 0; n < nchildren; ++n) {
          list.push_back(detail::get_window(children[n]));
        }
      }
      return list;
    }


#endif // X11

#ifdef WIN32
    void window_with_text::set_text(const std::string& s) {
      SendMessage(get_id(), WM_SETTEXT, 0, (LPARAM)s.c_str());
    }

    std::string window_with_text::get_text() const {
      std::string s;
      s.resize(SendMessage(get_id(), WM_GETTEXTLENGTH, 0, 0) + 1);
      SendMessage(get_id(), WM_GETTEXT, (WPARAM)s.capacity(), (LPARAM)&s[0]);
      return s;
    }
#endif // WIN32

#ifdef X11
    void window_with_text::set_text(const std::string& t) {
      text = t;
      redraw_later();
    }
    std::string window_with_text::get_text() const {
      return text;
    }
#endif //X11

    // --------------------------------------------------------------------------
    window_class main_window::clazz(
#ifdef WIN32
      win::window_class::custom_class("main_window",
                                      CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW,
                                      WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
                                      WS_THICKFRAME | WS_VISIBLE,
                                      WS_EX_NOPARENTNOTIFY | WS_EX_COMPOSITED,
                                      nullptr,
                                      IDC_ARROW,
                                      (HBRUSH)(COLOR_APPWORKSPACE + 1))
#endif // WIN32
#ifdef X11
      win::window_class::custom_class("main_window", draw::brush(draw::color::workSpaceColor()))
#endif
    );

    // --------------------------------------------------------------------------
    window_class client_window::clazz(
#ifdef WIN32
      win::window_class::custom_class("client_window",
                                      CS_DBLCLKS,
                                      WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                      WS_EX_NOPARENTNOTIFY | WS_EX_WINDOWEDGE,
                                      nullptr,
                                      IDC_ARROW,
                                      (HBRUSH)(COLOR_BTNFACE + 1))
#endif // WIN32
#ifdef X11
      win::window_class::custom_class("client_window", draw::brush(draw::color::buttonColor()))
#endif
    );

    // --------------------------------------------------------------------------
#ifdef WIN32
    void main_window::create (const core::rectangle& place) {
      window::create(clazz, GetDesktopWindow(), place);
    }

    void main_window::set_title (const std::string& title) {
      SendMessage(get_id(), WM_SETTEXT, 0, (LPARAM)title.c_str());
    }

    std::string main_window::get_title () const {
      std::string s;
      s.resize(SendMessage(get_id(), WM_GETTEXTLENGTH, 0, 0) + 1);
      SendMessage(get_id(), WM_GETTEXT, (WPARAM)s.capacity(), (LPARAM)&s[0]);
      return s;
    }

    bool main_window::is_top_most() const {
      return (GetWindowLong(get_id(), GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST;
    }

    bool main_window::is_minimized() const {
      return IsIconic(get_id()) != FALSE;
    }

    bool main_window::is_maximized() const {
      return IsZoomed(get_id()) != FALSE;
    }

    void main_window::minimize() {
      ShowWindow(get_id(), SW_MINIMIZE);
    }

    void main_window::maximize() {
      ShowWindow(get_id(), SW_MAXIMIZE);
    }

    void main_window::restore() {
      ShowWindow(get_id(), SW_RESTORE);
    }

    void main_window::set_top_most(bool toplevel) {
      SetWindowPos(get_id(),
                   toplevel ? HWND_TOPMOST : HWND_NOTOPMOST,
                   0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }
#endif // WIN32

#ifdef X11
    void main_window::create (const core::rectangle& place) {
      window::create(clazz, DefaultRootWindow(core::global::get_instance()), place);
    }

    void main_window::set_title (const std::string& title) {
      XStoreName(core::global::get_instance(), get_id(), title.c_str());
    }

    std::string main_window::get_title () const {
      char *window_name;
      XFetchName(core::global::get_instance(), get_id(), &window_name);
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

      if (!NET_WM_STATE) {
        NET_WM_STATE = XInternAtom(dpy, "_NET_WM_STATE", false);
      }
      if (!NET_WM_STATE_MAXIMIZED_HORZ) {
        NET_WM_STATE_MAXIMIZED_HORZ = XInternAtom(dpy, "_NET_WM_STATE_MAXIMIZED_HORZ", false);
      }
      if (!NET_WM_STATE_MAXIMIZED_VERT) {
        NET_WM_STATE_MAXIMIZED_VERT = XInternAtom(dpy, "_NET_WM_STATE_MAXIMIZED_VERT", false);
      }
      if (!NET_WM_STATE_ABOVE) {
        NET_WM_STATE_ABOVE = XInternAtom(dpy, "_NET_WM_STATE_ABOVE", false);
      }
      if (!NET_WM_STATE_HIDDEN) {
        NET_WM_STATE_HIDDEN = XInternAtom(dpy, "_NET_WM_STATE_HIDDEN", false);
      }
      if (!ATOM_ATOM) {
        ATOM_ATOM = XInternAtom(dpy, "ATOM", false);
      }
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

    bool main_window::is_maximized () const {
      init_for_net_wm_state();
      return query_net_wm_state(get_id(), NET_WM_STATE_MAXIMIZED_HORZ, NET_WM_STATE_MAXIMIZED_VERT);
    }

    bool main_window::is_top_most () const {
      init_for_net_wm_state();
      return query_net_wm_state(get_id(), NET_WM_STATE_ABOVE);
    }

    bool main_window::is_minimized () const {
      init_for_net_wm_state();
      return query_net_wm_state(get_id(), NET_WM_STATE_HIDDEN);
    }

    void main_window::minimize () {
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

    void main_window::maximize () {
      init_for_net_wm_state();
      send_net_wm_state(get_id(), _NET_WM_STATE_ADD,
                        NET_WM_STATE_MAXIMIZED_HORZ,
                        NET_WM_STATE_MAXIMIZED_VERT);

    }

    void main_window::restore () {
      init_for_net_wm_state();
      send_net_wm_state(get_id(), _NET_WM_STATE_REMOVE,
                        NET_WM_STATE_MAXIMIZED_HORZ,
                        NET_WM_STATE_MAXIMIZED_VERT);
    }

    void main_window::set_top_most (bool toplevel) {
      init_for_net_wm_state();
      send_net_wm_state(get_id(),
                        toplevel ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE,
                        NET_WM_STATE_ABOVE);
    }

#endif //X11

#ifdef WIN32
    win::window_class create_group_window_clazz (unsigned long v) {
      static int group_window_id = 0;
      return win::window_class::custom_class(ostreamfmt("group_window-" << group_window_id++),
                                              CS_DBLCLKS,
                                              WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                              WS_EX_NOPARENTNOTIFY | WS_EX_WINDOWEDGE,
                                              nullptr,
                                              IDC_ARROW,
                                              draw::brush(draw::color(v)));
    }
#endif // WIN32

#ifdef X11
    win::window_class create_group_window_clazz (draw::color::value_type v) {
      static int group_window_id = 0;
      return win::window_class::custom_class(ostreamfmt("group_window-" << group_window_id++), draw::brush(v));
    }
#endif //X11

  } // win

} // gui
