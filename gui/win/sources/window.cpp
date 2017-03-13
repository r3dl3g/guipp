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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window.h"
#include "window_event_proc.h"


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
                         core::window_id parent_id,
                         const core::rectangle& place,
                         core::menu_id menu) {

      this->cls = &type;

      id = CreateWindowEx(type.get_ex_style(),            // window style
                          type.get_class_name().c_str(),  // address of registered class name
                          nullptr,                        // address of window text
                          type.get_style(),               // window style
                          place.x(),                      // horizontal position of window
                          place.y(),                      // vertical position of window
                          place.width(),                  // window width
                          place.height(),                 // window height
                          parent_id,                      // handle of parent window
                          menu,                           // handle of menu or child-window identifier
                          core::global::get_instance(),   // handle of application instance
                          (LPVOID)this);
      type.prepare(this);
      SetWindowLongPtr(id, GWLP_USERDATA, (LONG_PTR)this);
    }

    void window::create (const window_class& type,
                         const window& parent,
                         const core::rectangle& place,
                         core::menu_id menu) {
      create(type, parent.get_id(), place, menu);
    }

    void window::create(const window_class& type,
                        const core::rectangle& place) {
      create(type, nullptr, place);
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

    bool window::is_top_most() const {
      return (GetWindowLong(get_id(), GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST;
    }

    bool window::is_minimized() const {
      return IsIconic(get_id()) != FALSE;
    }

    bool window::is_maximized() const {
      return IsZoomed(get_id()) != FALSE;
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

    void window::set_parent(const window& parent) {
      SetParent(get_id(), parent.get_id());
    }

    window* window::get_parent() const {
      return is_valid() ? window::get(GetParent(get_id())) : nullptr;
    }

    bool window::is_parent_of(const window& child) const {
      return is_valid() && child.is_valid() && IsChild(get_id(), child.get_id()) != FALSE;
    }

    bool window::is_child_of(const window& parent) const {
      return is_valid() && parent.is_valid() && IsChild(parent.get_id(), get_id()) != FALSE;
    }

    void window::hide() {
      ShowWindow(get_id(), SW_HIDE);
    }

    void window::show() {
      ShowWindow(get_id(), SW_SHOWNA);
    }

    void window::minimize() {
      ShowWindow(get_id(), SW_MINIMIZE);
    }

    void window::maximize() {
      ShowWindow(get_id(), SW_MAXIMIZE);
    }

    void window::restore() {
      ShowWindow(get_id(), SW_RESTORE);
    }

    void window::set_top_most(bool toplevel) {
      SetWindowPos(get_id(),
                   toplevel ? HWND_TOPMOST : HWND_NOTOPMOST,
                   0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }

    void window::enable(bool on) {
      EnableWindow(get_id(), on);
    }

    void window::take_focus () {
      SetFocus(get_id());
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
      return core::size(r.right - r.left, r.bottom - r.top);
    }

    core::point window::position() const {
      RECT r;
      GetWindowRect(get_id(), &r);
      return screen_to_window({ r.left, r.top });
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
      return{ r.left, r.top };
    }

    core::rectangle window::client_area() const {
      RECT r;
      GetClientRect(get_id(), &r);
      return core::rectangle(r);
    }

    void window::move(const core::point& pt, bool repaint) {
      SetWindowPos(get_id(), nullptr, pt.x(), pt.y(), 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
    }

    void window::resize(const core::size& sz, bool repaint) {
      SetWindowPos(get_id(), nullptr, 0, 0, sz.width(), sz.height(), SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
    }

    void window::place(const core::rectangle& r, bool repaint) {
      MoveWindow(get_id(), r.x(), r.y(), r.width(), r.height(), repaint);
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

    window* window::get(core::window_id id) {
      return reinterpret_cast<window*>(GetWindowLongPtr(id, GWLP_USERDATA));
    }
#endif // WIN32

#ifdef X11

    namespace detail {
        typedef std::map<core::window_id, win::window*> window_map;
        window_map global_window_map;
    }

    window::window ()
      : id(0)
#ifdef X11
      , redraw_disabled(false)
      , window_disabled(false)
#endif // X11
    {}

    window::~window () {
      destroy();
    }

    void window::create (const window_class& type,
                         core::window_id parent_id,
                         const core::rectangle& place,
                         core::menu_id) {
      cls = &type;
      core::instance_id display = core::global::get_instance();
      id = XCreateSimpleWindow(display,
                               parent_id,
                               place.x(),
                               place.y(),
                               place.width(),
                               place.height(),
                               type.get_class_style(),
                               type.get_foreground(),
                               type.get_background());
      detail::global_window_map[id] = this;

      XSetWindowAttributes wa;
      wa.event_mask = type.get_style();
      XChangeWindowAttributes(display, id, CWEventMask, &wa);
    }

    void window::create (const window_class& type,
                         const window& parent,
                         const core::rectangle& place,
                         core::menu_id menu) {
      create(type, parent.get_id(), place, menu);
    }

    void window::create (const window_class& type,
                        const core::rectangle& place) {
      create(type, DefaultRootWindow(core::global::get_instance()), place);
    }

    const window_class* window::get_window_class() const {
      return cls;
    }

    bool window::is_valid () const {
      return detail::global_window_map[get_id()] == this;
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

    bool window::is_top_most () const {
      return is_toplevel();
    }

    bool window::is_minimized () const {
      XWindowAttributes a;
      return (XGetWindowAttributes(core::global::get_instance(), get_id(), &a) &&
              (a.map_state == IsUnmapped));
    }

    bool window::is_maximized () const {
//      XSizeHints zhints = { PBaseSize | PMaxSize | USSize | PSize, 0 };
//      long suplied = 0;
//      if (!XGetWMNormalHints(core::global::get_instance(), get_id(), &zhints, &suplied)) {
//        core::size sz = size();
//        return (sz.width() == zhints.base_width) && (sz.height() == zhints.base_height);
//      }
      return false;
    }

    bool window::has_border () const {
      XWindowAttributes a;
      return (XGetWindowAttributes(core::global::get_instance(), get_id(), &a) &&
              (a.border_width > 0));
    }

    void window::destroy () {
      XDestroyWindow(core::global::get_instance(), get_id());
      detail::global_window_map.erase(get_id());
      id = 0;
    }

    void window::quit () {
    }

    void window::set_parent (const window& parent) {
      core::point pt = position();
      XReparentWindow(core::global::get_instance(), get_id(), parent.get_id(), pt.x(), pt.y());
    }

    window* window::get_parent () const {
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
      return window::get(parent_return);
    }

    bool window::is_parent_of (const window& child) const {
      return child.get_parent() == this;
    }

    bool window::is_child_of (const window& parent) const {
      return get_parent() == &parent;
    }

    void window::hide () {
      XUnmapWindow(core::global::get_instance(), get_id());
    }

    void window::show () {
      XMapWindow(core::global::get_instance(), get_id());
    }

    void window::minimize () {
      XIconifyWindow(core::global::get_instance(), get_id(), core::global::get_screen());
    }

    void window::maximize () {
//      XSizeHints zhints = { PBaseSize | PMaxSize | USSize | PSize, 0 };
//      long suplied = 0;
//      if (!XGetWMNormalHints(core::global::get_instance(), get_id(), &zhints, &suplied)) {
//        XResizeWindow(core::global::get_instance(), get_id(), zhints.base_width, zhints.base_height);
//      }
    }

    void window::restore () {
//      XSizeHints zhints = { PBaseSize | PMaxSize | USSize | PSize, 0 };
//      long suplied = 0;
//      if (!XGetWMNormalHints(core::global::get_instance(), get_id(), &zhints, &suplied)) {
//        XResizeWindow(core::global::get_instance(), get_id(), zhints.width, zhints.height);
//      }
    }

    void window::set_top_most (bool toplevel) {
      if (toplevel) {
        XRaiseWindow(core::global::get_instance(), get_id());
      } else {
        XLowerWindow(core::global::get_instance(), get_id());
      }
    }

    void window::enable (bool on) {
      window_disabled = !on;
      redraw_later();
    }

    void window::take_focus () {
      XSetInputFocus(core::global::get_instance(), get_id(), RevertToParent, CurrentTime);
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
      int x, y;
      unsigned int width, height;
      unsigned int border_width;
      unsigned int depth;
      XGetGeometry(core::global::get_instance(), get_id(), &root, &x, &y, &width, &height, &border_width, &depth);
      return {core::size::type(width), core::size::type(height)};
    }

    core::point window::position () const {
      Window root;
      int x, y;
      unsigned int width, height;
      unsigned int border_width;
      unsigned int depth;
      XGetGeometry(core::global::get_instance(), get_id(), &root, &x, &y, &width, &height, &border_width, &depth);
      return {core::point::type(x), core::point::type(y)};
    }

    core::rectangle window::place () const {
      Window root;
      int x, y;
      unsigned int width, height;
      unsigned int border_width;
      unsigned int depth;
      XGetGeometry(core::global::get_instance(), get_id(), &root, &x, &y, &width, &height, &border_width, &depth);
      return core::rectangle(core::point::type(x), core::point::type(y),
                             core::size::type(width), core::size::type(height));
    }

    core::rectangle window::absolute_place () const {
      return core::rectangle(absolute_position(), size());
    }

    core::point window::absolute_position () const {
      return window_to_screen(core::point::zero);
    }

    core::rectangle window::client_area () const {
      return core::rectangle(size() - core::size(1, 1));
    }

    void window::move (const core::point& pt, bool repaint) {
      XMoveWindow(core::global::get_instance(), get_id(), pt.x(), pt.y());
    }

    void window::resize (const core::size& sz, bool repaint) {
      XResizeWindow(core::global::get_instance(), get_id(), sz.width(), sz.height());
    }

    void window::place (const core::rectangle& r, bool repaint) {
      XMoveResizeWindow(core::global::get_instance(), get_id(), r.x(), r.y(), r.width(), r.height());
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

    window* window::get (core::window_id id) {
      return detail::global_window_map[id];
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

  } // win

} // gui
