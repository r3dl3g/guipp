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
 * @brief     global gui defines and typedefs
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_QT

// --------------------------------------------------------------------------
//
// Common includes
//
#include <QtWidgets/QApplication>
#include <QtGui/QScreen>
#include <logging/logger.h>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/guidefs.h"
#include "gui/core/context.h"
#include "gui/core/color.h"


namespace gui {

  // --------------------------------------------------------------------------
  namespace core {

    // --------------------------------------------------------------------------
    namespace global {

      double calc_scale_factor ();

      namespace {
        gui::os::instance instance = nullptr;
        
      }

      void init (gui::os::instance i) {
        instance = i;
        info(i);
        set_scale_factor(calc_scale_factor());
      }

      gui::os::instance get_instance () {
        if (instance != nullptr) {
          return instance;
        }
        throw std::runtime_error("gui::core::global::init must be called before first use!");
      }

      void fini () {
      }

      void sync () {
      }

      int get_device_depth () {
        static int depth =
          QGuiApplication::primaryScreen()->depth();
        return depth;
      }

      pixel_format_t get_device_pixel_format () {
        static pixel_format_t format =
          get_pixel_format(QPixmap(1, 1).depth(), os::bitmap_byte_order);
        return format;
      }

      gui::os::key_state get_key_state () {
        return QGuiApplication::keyboardModifiers();
      }

      double calc_scale_factor () {
        const char* xscale = getenv("XSCALE");
        if (xscale) {
          double scale = 1.0;
          std::stringstream(xscale) >> scale;
          return scale;
        } else {
          QScreen* d = QGuiApplication::primaryScreen();
          auto r =  std::max(ceil((double)d->logicalDotsPerInchX() * 2.0 / 96.0) / 2.0, 0.5);
          logging::debug()  << "Display "
                           "W:" << d->size().width() << ", H:" << d->size().height()
                        << ", MM-W:" << d->physicalSize().width() << ", MM-H:" << d->physicalSize().height()
                        << ", LogDPI-X:" << d->logicalDotsPerInchX() << ", LogDPI-Y:" << d->logicalDotsPerInchY()
                        << ", PhysDPI-X:" << d->physicalDotsPerInchX() << ", PhysDPI-Y:" << d->physicalDotsPerInchY()
                        << ", Pixel Ratio:" << d->devicePixelRatio();
//                        << ", Pixel Ratio Scale:" << d->devicePixelRatioFScale();
          return r;
        }
      }

    } // global

  } // core

} // gui

#endif // GUIPP_QT
