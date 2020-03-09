
#include "cvmat2pixmap.h"

using namespace gui;
using namespace gui::draw;

pixmap cvMat2pixmap (const cv::Mat& source) {
  auto sz = source.size();
  core::native_size nsize = {(uint32_t)sz.width, (uint32_t)sz.height};

  if (CV_8UC3 == source.type()) {
    bitmap_info info(nsize.width(), nsize.height(), (int)(source.cols * source.elemSize()), pixel_format_t::RGB);

    typedef const_image_data<pixel_format_t::RGB> img_data;
    img_data::raw_type data(source.data, info.mem_size());

    pixmap image = datamap<pixel_format_t::RGB>(img_data(data, info));
    return image;
  } else if (CV_8UC4 == source.type()) {
    bitmap_info info(nsize.width(), nsize.height(), (int)(source.cols * source.elemSize()), pixel_format_t::RGBA);

    typedef const_image_data<pixel_format_t::RGBA> img_data;
    img_data::raw_type data(source.data, info.mem_size());

    pixmap image = datamap<pixel_format_t::RGBA>(img_data(data, info));
    return image;
  } else if (CV_8UC1 == source.type()) {
    bitmap_info info(nsize.width(), nsize.height(), (int)(source.cols * source.elemSize()), pixel_format_t::GRAY);

    typedef const_image_data<pixel_format_t::GRAY> img_data;
    img_data::raw_type data(source.data, info.mem_size());

    pixmap image = datamap<pixel_format_t::GRAY>(img_data(data, info));
    return image;
  }

  return pixmap();
}
