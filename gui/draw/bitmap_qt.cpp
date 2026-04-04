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
 * @brief     monochrome and coloured fast bitmaps
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_QT

// --------------------------------------------------------------------------
//
// Common includes
//
#include <limits>
#include <algorithm>
#include <map>
#include <util/ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/draw/bitmap.h"
#include "gui/draw/graphics.h"
#include "gui/draw/pen.h"
#include "gui/draw/use.h"

#include <QtGui/QBitmap>


namespace gui {

  namespace native {

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr) {
      if (bmi.bits_per_pixel() == 1) {
        return new QBitmap(bmi.width, bmi.height);
      } else {
        return new QPixmap(bmi.width, bmi.height);
      }
    }

    void free_bitmap (os::bitmap& id) {
      if (id) {
        delete id;
        id = nullptr;
      }
    }

    draw::bitmap_info bitmap_get_info (const os::bitmap& id) {
      if (id) {
        pixel_format_t fmt = pixel_format_t::RGBA;
        const auto depth = id->depth();
        fmt = gui::get_pixel_format(depth, core::os::bitmap_byte_order);
        return draw::bitmap_info(id->width(), id->height(), fmt);
      }
      return {};
    }

    void bitmap_put_data (os::bitmap& id, cbyteptr data, const draw::bitmap_info& bmi) {
      if (id) {
        const auto fmt = draw::bitmap_info::convert(bmi.pixel_format);
        if (bmi.pixel_format == pixel_format_t::BW) {
          QImage img((const uchar *)data, bmi.width, bmi.height, bmi.bytes_per_line, fmt);
          img.setColorTable({ QColor(Qt::white).rgb(), QColor(Qt::black).rgb() });
//          img.setColorTable({ QColor(Qt::black).rgb(), QColor(Qt::white).rgb() });
          *id = QBitmap::fromImage(img);
        } else {
          *id = QPixmap::fromImage(QImage((const uchar *)data, bmi.width, bmi.height, bmi.bytes_per_line, fmt));
        }
      }
    }

    void bitmap_get_data (const os::bitmap& id, blob& data, draw::bitmap_info& bmi) {
      if (id) {
        QImage img = id->toImage();
        const auto sz = img.size();
        auto fmt = draw::bitmap_info::convert(img.format());
        bmi = draw::bitmap_info(sz.width(), sz.height(), img.bytesPerLine(), fmt);

        const uchar* bits = img.constBits();
        data.assign(bits, bits +
#if QT_VERSION > QT_VERSION_CHECK(5, 13, 0)
                    img.sizeInBytes());
#else
                    img.byteCount());
#endif
//        if (fmt == pixel_format_t::BW) {
//          for (byte& b : data) {
//            b = ~b;
//          }
//        }
      } else {
        bmi = {};
        data.clear();
      }
    }

    void copy_bitmap (draw::basic_map& lhs, const draw::basic_map& rhs) {
      draw::bitmap_info bmi = rhs.get_info();
      lhs.create(bmi);
      *lhs.get_os_bitmap() = *rhs.get_os_bitmap();
    }

  } // namespace native

}

#endif // GUIPP_QT
