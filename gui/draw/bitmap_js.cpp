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

 #ifdef GUIPP_JS

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


#include <emscripten/bind.h>

namespace std {
  ostream& operator<< (ostream& os, const emscripten::val& v) {
    return os << (v.isUndefined() ? "undefined": "defined")  << " and " << (v.isNull() ? "null" : "not null");
  }
}


namespace gui {

  namespace native {

    using namespace emscripten;


    void free_bitmap (os::bitmap& id) {
    }
    
    void bitmap_put_data (os::bitmap& id, cbyteptr dataptr, const draw::bitmap_info& bmi) {
      auto ctx = core::native::create_graphics_context(id);

      logging::trace() << "2D-Context is " << ctx;

      val memoryView = val(typed_memory_view(bmi.mem_size(), dataptr));

      logging::trace() << "memoryView is " << memoryView;

      val uint8ClampedArray = val::global("Uint8ClampedArray").new_(memoryView);

      logging::trace() << "Uint8ClampedArray is " << uint8ClampedArray;

      val imageData = val::global("ImageData").new_(uint8ClampedArray, bmi.width, bmi.height);

      logging::trace() << "ImageData is " << imageData;
      
      ctx.call<void>("putImageData", imageData, 0, 0);
    }
    
    void bitmap_get_data (const os::bitmap& id, blob& dataptr, draw::bitmap_info& bmi) {
      auto ctx = core::native::create_graphics_context(id);

      logging::trace() << "2D-Context is " << ctx;

      dataptr.resize(bmi.mem_size());

      val imageData = ctx.call<val>("getImageData", 0, 0, bmi.width, bmi.height);

      logging::trace() << "ImageData is " << imageData;

      val dataArray = imageData["data"];

      logging::trace() << "dataArray is " << dataArray;

      // Effizientes Kopieren von JS-TypedArray in C++ Vektor
      val memoryView = val(typed_memory_view(dataptr.size(), dataptr.data()));

      logging::trace() << "memoryView is " << memoryView;

      memoryView.call<void>("set", dataArray);
    }
    
    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr data) {
      auto img = val::global("OffscreenCanvas").new_(bmi.width, bmi.height);
      logging::trace() << "OffscreenCanvas is " << img;
      if (data) {
        bitmap_put_data(img, data, bmi);
      }
      return img;
    }

    draw::bitmap_info bitmap_get_info (const os::bitmap& id) {
      logging::trace() << "bitmap_get_info is " << id;

      val width = id["width"];
      logging::trace() << "width is " << width;

      val height = id["height"];
      logging::trace() << "height is " << height;

      uint32_t w = width.as<int>();
      uint32_t h = height.as<int>();
      return {w, h, pixel_format_t::RGBA};
    }

    void copy_bitmap (draw::basic_map& lhs, const draw::basic_map& rhs) {
      draw::bitmap_info bmi = rhs.get_info();
      lhs.create(bmi);
      logging::trace() << "rhs.get_os_bitmap() is " << rhs.get_os_bitmap();
      auto imageData = rhs.get_os_bitmap().call<val>("getImageData", 0, 0, bmi.width, bmi.height);
      logging::trace() << "ImageData is " << imageData;
      lhs.get_os_bitmap().call<void>("putImageData", imageData, 0, 0);
    }

  } // namespace native

}

#endif // GUIPP_JS
