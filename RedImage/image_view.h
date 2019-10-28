/**
 * @copyright (c) 2016-2019 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    redimage
 *
 * Customer   -
 *
 * @brief     C++ API: red image view
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/image.h>
#include <opencv2/core/core.hpp>

namespace view {

  class image_view : public gui::ctrl::image {
  public:
    typedef gui::ctrl::image super;

    void set_image_and_scale (const cv::Mat& source);
  };

} // namespace view
