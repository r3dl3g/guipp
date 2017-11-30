/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: event
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
#include <gui/core/event.h>
#include <gui/core/logger.h>


namespace {

  const char* event_names[] = {
    "Unknown0",
    "Unknown1",
    "KeyPress",
    "KeyRelease",
    "ButtonPress",
    "ButtonRelease",
    "MotionNotify",
    "EnterNotify",
    "LeaveNotify",
    "FocusIn",
    "FocusOut",
    "KeymapNotify",
    "Expose",
    "GraphicsExpose",
    "NoExpose",
    "VisibilityNotify",
    "CreateNotify",
    "DestroyNotify",
    "UnmapNotify",
    "MapNotify",
    "MapRequest",
    "ReparentNotify",
    "ConfigureNotify",
    "ConfigureRequest",
    "GravityNotify",
    "ResizeRequest",
    "CirculateNotify",
    "CirculateRequest",
    "PropertyNotify",
    "SelectionClear",
    "SelectionRequest",
    "SelectionNotify",
    "ColormapNotify",
    "ClientMessage",
    "MappingNotify",
    "GenericEvent",
  };

}

namespace std {

  typedef function<void(ostream&)> streamfkt;

  inline ostream& operator<< (ostream& out, const streamfkt& f) {
    f(out);
    return out;
  }

  template<typename T>
  inline streamfkt position (const T& t) {
    return [&] (ostream& out) { out << " pos: (" << t.x << ", " << t.y << ")"; };
  }

  template<typename T>
  inline streamfkt size (const T& t) {
    return [&] (ostream& out) { out << " size: (" << t.width << ", " << t.height << ")"; };
  }

  template<typename T>
  inline streamfkt place (const T& t) {
    return [&] (ostream& out) { out << " area: (" << t.x << ", " << t.y << ")-(" << t.width << ", " << t.height << ")"; };
  }

  template<typename T>
  inline streamfkt count (const T& t) {
    return [&] (ostream& out) { out << " count: " << t.count; };
  }

  template<typename T>
  inline streamfkt state (const T& t) {
    return [&] (ostream& out) { out << " state: " << t.state; };
  }

  template<typename T>
  inline streamfkt window (const T& t) {
    return [&] (ostream& out) { out << " window: " << t.window; };
  }

  template<typename T>
  inline streamfkt drawable (const T& t) {
    return [&] (ostream& out) { out << " drawable: " << t.drawable; };
  }

  std::ostream& operator<< (std::ostream& out, const gui::core::event& e) {
    if (e.type < LASTEvent) {
      out << event_names[e.type];
    } else {
      out << "Unknown Event Type " << e.type;
    }
    switch (e.type) {
      case KeyPress:
      case KeyRelease:
        out << window(e.xkey) << " keycode:" << e.xkey.keycode << state(e.xkey);
      break;
      case ButtonPress:
      case ButtonRelease:
        out << window(e.xbutton) << " button:" << e.xbutton.button << state(e.xbutton) << position(e.xbutton);
      break;
      case MotionNotify:
        out << window(e.xmotion) << state(e.xmotion) << position(e.xmotion);
      break;
      case EnterNotify:
      case LeaveNotify:
        out << window(e.xcrossing) << position(e.xcrossing);
      break;
      case FocusIn:
      case FocusOut:
        out << window(e.xfocus) << " mode:" << e.xfocus.mode << " detail:" << e.xfocus.detail;
      break;
      case KeymapNotify:
        out << window(e.xkeymap) << " key_vector:'" << e.xkeymap.key_vector << "'";
      break;
      case Expose:
        out << window(e.xexpose) << place(e.xexpose) << count(e.xexpose);
      break;
      case GraphicsExpose:
        out << drawable(e.xgraphicsexpose) << place(e.xgraphicsexpose) << count(e.xgraphicsexpose);
      break;
      case NoExpose:
        out << drawable(e.xnoexpose);
      break;
      case VisibilityNotify:
        out << window(e.xvisibility) << state(e.xvisibility);
      break;
      case CreateNotify:
        out << window(e.xcreatewindow) << place(e.xcreatewindow);
      break;
      case DestroyNotify:
        out << window(e.xdestroywindow);
      break;
      case UnmapNotify:
        out << window(e.xunmap);
      break;
      case MapNotify:
        out << window(e.xmap);
      break;
      case MapRequest:
        out << window(e.xmaprequest);
      break;
      case ReparentNotify:
        out << window(e.xreparent) << position(e.xreparent);
      break;
      case ConfigureNotify:
        out << window(e.xconfigure) << place(e.xconfigure);
      break;
      case ConfigureRequest:
        out << window(e.xconfigurerequest);
      break;
      case GravityNotify:
        out << window(e.xgravity) << position(e.xgravity);
      break;
      case ResizeRequest:
        out << window(e.xresizerequest) << size(e.xresizerequest);
      break;
      case CirculateNotify:
        out << window(e.xcirculate)<< " place:" << e.xcirculate.place;
      break;
      case CirculateRequest:
        out << window(e.xcirculaterequest) << " place:" << e.xcirculaterequest.place;
      break;
      case PropertyNotify:
        out << window(e.xproperty) << state(e.xproperty);
      break;
      case SelectionClear:
        out << window(e.xselectionclear);
      break;
      case SelectionRequest:
        out << " owner:" << e.xselectionrequest.owner;
      break;
      case SelectionNotify:
        out << " requestor:" << e.xselection.requestor;
      break;
      case ColormapNotify:
        out << window(e.xcolormap);
      break;
      case ClientMessage:
        out << window(e.xclient);
      break;
      case MappingNotify:
        out << window(e.xmapping) << count(e.xmapping);
      break;
      case GenericEvent:
        out << " extension:" << e.xgeneric.extension << " evtype:" << e.xgeneric.evtype;
      break;
    }
    return out;
  }
}
