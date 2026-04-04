/**
 * @copyright (c) 2016-2026 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     Helper to use pen, brush and font
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#if GUIPP_QT
#include <QtGui/QPainter>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/draw/use.h"
#include "gui/draw/pen.h"
#include "gui/draw/brush.h"
#include "gui/draw/font.h"

namespace gui {

  namespace draw {
    // --------------------------------------------------------------------------
    template<>
    void Use<pen>::set (const pen& p) {
      g->setPen(QPen(QBrush(p.color()), p.os_size(),
                     static_cast<Qt::PenStyle>(p.style()),
                     static_cast<Qt::PenCapStyle>(p.cap()),
                     static_cast<Qt::PenJoinStyle>(p.join())));
    }

    template<>
    void Use<brush>::set (const brush& b) {
      g->setBrush(QBrush(b.color(), static_cast<Qt::BrushStyle>(b.style())));
    }

    template<>
    void Use<font>::set (const font& f) {
      g->setFont(QFont(QString::fromStdString(f.name()), f.size(), f.thickness(), f.italic()));
    }

    
  } // namespace draw
  
} // namespace gui

#endif // GUIPP_QT