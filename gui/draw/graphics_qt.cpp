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
 * @brief     graphic functions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_QT

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>
#include <QtGui/QBitmap>
#include <QtGui/QScreen>

#include <util/ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/draw/graphics.h"
#include "gui/draw/bitmap.h"
#include "gui/draw/shared_datamap.h"
#include "gui/draw/drawers.h"
#include "gui/draw/pen.h"
#include "gui/draw/brush.h"
#include "gui/draw/use.h"


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    graphics& graphics::draw_pixel (const core::native_point& pt,
                                          os::color c) {
      gc()->setPen(c);
      gc()->drawPoint(pt.x(), pt.y());
      return *this;
    }

    os::color graphics::get_pixel (const core::native_point& pt) const {
      QWindow* w = dynamic_cast<QWindow*>(target());
      if (w) {
        QPixmap px;
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#elif QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
        px = w->grab({pt.x(), pt.y(), 1, 1});
#else
        px.grabWindow(w->winId(), pt.x(), pt.y(), 1, 1);
#endif
        return px.toImage().pixel(0, 0);
      } else {
        QPixmap* p = dynamic_cast<QPixmap*>(target());
        if (p) {
          if (p->depth() == 1) {
            return p->toImage().pixelIndex(pt.x(), pt.y()) ? color::white : color::black;
          } else {
            return p->toImage().pixel(pt.x(), pt.y());
          }
        }
      }
      return color::black;
    }

    os::point operator+ (const os::point& pt, int i) {
      return { (pt.x() + i), (pt.y() + i) };
    }

    graphics& graphics::copy_from (graphics& src, const core::native_rect& r, const core::native_point& pt) {
      return copy_from(src.target(), r, pt);
    }

    graphics& graphics::copy_from (os::drawable d,
                                         const core::native_rect& r,
                                         const core::native_point& pt,
                                         const copy_mode mode) {
      QWindow* w = dynamic_cast<QWindow*>(d);
      if (w) {
        QPixmap pix = QGuiApplication::primaryScreen()->grabWindow(w->winId(), r.x(), r.y(), r.width(), r.height());
        const QPainter::CompositionMode oldMode = gc()->compositionMode();
        gc()->setCompositionMode(static_cast<QPainter::CompositionMode>(mode));
        gc()->drawPixmap(pt.x(), pt.y(), pix);
        gc()->setCompositionMode(oldMode);
      } else {
        QPixmap* p = dynamic_cast<QPixmap*>(d);
        if (p) {
          const QPainter::CompositionMode oldMode = gc()->compositionMode();
          gc()->setCompositionMode(static_cast<QPainter::CompositionMode>(mode));
          gc()->drawPixmap(pt.x(), pt.y(), *p);
          gc()->setCompositionMode(oldMode);
        }
      }
      return *this;
    }

    graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::native_point& pt) {
      if (bmp.mask && bmp.image) {
        QRegion clip(*(const QBitmap*)bmp.mask.get_os_bitmap());
        clip.translate(pt.x(), pt.y());
        gc()->setClipRegion(clip);
        gc()->drawPixmap(pt.x(), pt.y(), *bmp.image.get_os_bitmap());
        gc()->setClipping(false);
      } else if (bmp.image) {
        gc()->drawPixmap(pt.x(), pt.y(), *bmp.image.get_os_bitmap());
      } else if (bmp.mask) {
        gc()->drawPixmap(pt.x(), pt.y(), *bmp.mask.get_os_bitmap());
      }
      return *this;
    }

    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::rectangle& src, const core::point& pt) {
      if (bmp) {
        const auto r = core::global::scale_to_native(src);
        const auto p = pt.os(context());
        gc()->drawPixmap(p.x(), p.y(), *bmp.get_os_bitmap(),
                         r.x(), r.y(), r.width(), r.height());
      }
      return *this;
    }

    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::native_rect& src, const core::native_point& pt) {
      if (bmp) {
        gc()->drawPixmap(pt.x(), pt.y(), *bmp.get_os_bitmap(), src.x(), src.y(), src.width(), src.height());
      }
      return *this;
    }

    graphics& graphics::draw_streched (const draw::pixmap& bmp,
                                       const core::native_rect& dst,
                                       const core::native_point& src,
                                       const std::string& filter) {
      if (bmp) {
        auto sz = bmp.native_size();
        gc()->drawPixmap(dst.x(), dst.y(), dst.width(), dst.height(),
                         *bmp.get_os_bitmap(),
                         src.x(), src.y(), sz.width(), sz.height());
      }
      return *this;
    }

    std::vector<std::string> graphics::get_filter_list(os::drawable d) {
      static std::vector<std::string> list = {"STRETCH"};
      return list;
    }

    void graphics::invert (const core::rectangle& r) {
      const QPainter::CompositionMode oldMode = gc()->compositionMode();
      gc()->setCompositionMode(QPainter::RasterOp_NotDestination);
      gc()->fillRect(r.os(context()), Qt::white);
      gc()->setCompositionMode(oldMode);
    }

    void graphics::flush () {
    }

    int graphics::depth () const {
      return gc()->device()->depth();
    }

    core::native_rect graphics::native_area () const {
      auto vp = gc()->viewport();
      return core::native_rect(vp.x(),
                               vp.y(),
                               static_cast<core::native_rect::size_type>(vp.width()),
                               static_cast<core::native_rect::size_type>(vp.height()));
    }

  } // namespace draw

} // namespace gui

#endif // GUIPP_QT
