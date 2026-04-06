/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     system clipboard helper
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_QT

#include <limits>
#include <QtGui/QClipboard>
#include <QtGui/QGuiApplication>
#include <util/string_util.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/clipboard.h"
#include "gui/win/overlapped_window.h"
#include "gui/win/native.h"

namespace gui {

  namespace win {

    clipboard& clipboard::get () {
      static clipboard c;
      return c;
    }

    clipboard::clipboard ()
    {}

    void clipboard::set_text (window& win, const std::string& t) {
      QGuiApplication::clipboard()->setText(QString::fromStdString(t));
    }

    void clipboard::get_text (window& win, std::function<clipboard::text_callback>&& cb) {
      cb(QGuiApplication::clipboard()->text().toStdString());
    }

  } // win

} // gui

#endif // GUIPP_QT
