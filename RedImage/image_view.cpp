
#include "image_view.h"
#include "cvmat2pixmap.h"

using namespace gui::draw;
using namespace gui::win;
using namespace gui::ctrl;

// --------------------------------------------------------------------------
void image_view::create (const gui::win::container& parent,
                         const gui::core::rectangle& place) {
  super::create(clazz::get(), parent, place);
}

// --------------------------------------------------------------------------
void image_view::paint (const graphics& graph) {
  graph.clear(gui::color::dark_gray);
  if (image.is_valid()) {
    graph.copy_from(image);
  }
}

// --------------------------------------------------------------------------
void image_view::set_image (const pixmap& source) {
  image = source;
  invalidate();
}

// --------------------------------------------------------------------------
void image_view::set_image_and_scale (const cv::Mat& src) {
  if (src.empty()) {
    set_image(pixmap());
  } else {
    auto native_size = gui::core::global::scale(size());
    cv::Size sz(native_size.width(), native_size.height());
    cv::Mat target = cv::Mat(sz, src.type());
    cv::resize(src, target, sz, 0, 0, cv::INTER_NEAREST);
    set_image(cvMat2pixmap(target));
  }
}

// --------------------------------------------------------------------------
image_view::image_view () {
  on_paint(gui::draw::paint(this, &image_view::paint));
}
