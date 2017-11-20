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
 * @brief     C++ API: edit controls
 *
 * @file
 */

#pragma once


namespace gui {

  namespace win {

    template<text_origin A>
    inline basic_edit<A>::basic_edit () {
      register_handler(A);
    }

    template<text_origin A>
    inline basic_edit<A>::basic_edit (const basic_edit& rhs)
      : super(rhs)
    {
      register_handler(A);
    }

    template<text_origin A>
    inline basic_edit<A>::basic_edit (basic_edit&& rhs)
      : super(std::move(rhs))
    {
      register_handler(A);
    }

  } // win

} // gui
