/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic structures
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

namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    inline bool clipping_stack::empty () const {
      return stack.empty();
    }
    // --------------------------------------------------------------------------
    inline const gui::os::rectangle& clipping_stack::back () const {
      return stack.back();
    }
    // --------------------------------------------------------------------------
    inline context::operator gui::os::graphics () const {
      return g;
    }
    // --------------------------------------------------------------------------
    inline context::operator gui::os::drawable() const {
      return id;
    }
    // --------------------------------------------------------------------------
    inline gui::os::graphics context::graphics () const {
      return g;
    }
    // --------------------------------------------------------------------------
    inline gui::os::drawable context::drawable () const {
      return id;
    }
    // --------------------------------------------------------------------------
    inline void context::push_clipping (const gui::os::rectangle& r) {
      clippings.push(*this, r);
    }
    // --------------------------------------------------------------------------
    inline void context::pop_clipping () {
      clippings.pop(*this);
    }
    // --------------------------------------------------------------------------
    inline void context::restore_clipping () {
      clippings.restore(*this);
    }
    // --------------------------------------------------------------------------
    inline const clipping_stack& context::clipping () const {
      return clippings;
    }
    // --------------------------------------------------------------------------
    inline int context::offset_x () const {
      return offs_x;
    }
    // --------------------------------------------------------------------------
    inline int context::offset_y () const {
      return offs_y;
    }
    // --------------------------------------------------------------------------
    inline void context::set_offset (int x, int y) {
      offs_x = x;
      offs_y = y;
    }

  } // namespace core

} // namespace gui
