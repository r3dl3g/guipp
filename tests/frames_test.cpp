
#include <iomanip>

#include <gui/draw/bitmap.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/frames.h>
#include <gui/draw/brush.h>
#include <gui/draw/pen.h>
#include <gui/draw/font.h>
#include <gui/ctrl/separator.h>
#include <testlib/image_test_lib.h>
#include <testlib/testlib.h>
#include <logging/core.h>


using namespace gui;
using namespace gui::draw;
using namespace testing;


// --------------------------------------------------------------------------
template<draw::frame::drawer F>
struct framer {
  framer (const core::size& sz)
    : sz(sz)
  {}

  void operator() (graphics& g, const core::point& pt) const {
    F(g, core::rectangle(pt, sz));
  }

private:
  const core::size sz;
};

// --------------------------------------------------------------------------
template<draw::frame::drawer F, os::color C = color::white>
void tester (float scale, int pix, const core::point& pt, const core::size& sz,
             colormap&& expected) {
  core::global::set_scale_factor(scale);
  pixmap img(pix, pix);
  graphics g(img);
  g.clear(C);
  F(g, core::rectangle(pt, sz));
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, expected);
}

// --------------------------------------------------------------------------
void test_no_frame () {
  tester<frame::no_frame>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {W,W,W,W,W},
    {W,W,W,W,W},
    {W,W,W,W,W},
    {W,W,W,W,W},
    {W,W,W,W,W}
  }));
}


// --------------------------------------------------------------------------
void test_black_frame () {
  tester<frame::black>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {W,W,W,W,W},
    {W,_,_,_,W},
    {W,_,W,_,W},
    {W,_,_,_,W},
    {W,W,W,W,W}
  }));
}


// --------------------------------------------------------------------------
void test_black_frame_x2 () {
  tester<frame::black>(2.0F, 10, {1, 1}, {3, 3}, CM({
    {W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W},
    {W,W,_,_,_,_,_,_,W,W},
    {W,W,_,_,_,_,_,_,W,W},
    {W,W,_,_,W,W,_,_,W,W},
    {W,W,_,_,W,W,_,_,W,W},
    {W,W,_,_,_,_,_,_,W,W},
    {W,W,_,_,_,_,_,_,W,W},
    {W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W}
  }));
}


// --------------------------------------------------------------------------
void test_black_frame_x3 () {
  tester<frame::black>(3.0F, 15, {1, 1}, {3, 3}, CM({
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,_,_,_,_,_,_,_,_,_,W,W,W},
    {W,W,W,_,_,_,_,_,_,_,_,_,W,W,W},
    {W,W,W,_,_,_,_,_,_,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,_,_,_,_,_,_,W,W,W},
    {W,W,W,_,_,_,_,_,_,_,_,_,W,W,W},
    {W,W,W,_,_,_,_,_,_,_,_,_,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W}
  }));
}


// --------------------------------------------------------------------------
void test_white_frame () {
  tester<frame::white, color::black>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_},
    {_,W,W,W,_},
    {_,W,_,W,_},
    {_,W,W,W,_},
    {_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_white_frame_x2 () {
  tester<frame::white, color::black>(2.0F, 10, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,W,W,W,W,W,W,_,_},
    {_,_,W,W,W,W,W,W,_,_},
    {_,_,W,W,_,_,W,W,_,_},
    {_,_,W,W,_,_,W,W,_,_},
    {_,_,W,W,W,W,W,W,_,_},
    {_,_,W,W,W,W,W,W,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_white_frame_x3 () {
  tester<frame::white, color::black>(3.0F, 15, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
    {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
    {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
    {_,_,_,W,W,W,_,_,_,W,W,W,_,_,_},
    {_,_,_,W,W,W,_,_,_,W,W,W,_,_,_},
    {_,_,_,W,W,W,_,_,_,W,W,W,_,_,_},
    {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
    {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
    {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_dots_frame () {
  tester<frame::dots>(1.0F, 7, {1, 1}, {5, 5}, CM({
#ifdef GUIPP_WIN
    {W,W,W,W,W,W,W},
    {W,_,_,W,_,W,W},
    {W,W,W,W,W,_,W},
    {W,_,W,W,W,W,W},
    {W,W,W,W,W,_,W},
    {W,_,W,_,W,W,W},
    {W,W,W,W,W,W,W}
#elif GUIPP_X11
    {W,W,W,W,W,W,W},
    {W,_,W,_,W,_,W},
    {W,_,W,W,W,W,W},
    {W,W,W,W,W,_,W},
    {W,_,W,W,W,W,W},
    {W,W,_,W,_,W,W},
    {W,W,W,W,W,W,W}
#elif GUIPP_QT
    {W,W,W,W,W,W,W},
    {W,_,_,W,W,_,W},
    {W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W},
    {W,_,W,W,W,_,W},
    {W,W,W,_,W,W,W},
    {W,W,W,W,W,W,W}
#else
    {W,W,W,W,W,W,W},
    {W,_,W,_,W,_,W},
    {W,W,W,W,W,W,W},
    {W,_,W,W,W,_,W},
    {W,W,W,W,W,W,W},
    {W,_,W,_,W,_,W},
    {W,W,W,W,W,W,W}
#endif // GUIPP_WIN
  }));
}


// --------------------------------------------------------------------------
void test_dots_frame_x2 () {
  tester<frame::dots>(2.0F, 14, {1, 1}, {5, 5}, CM({
#ifdef GUIPP_WIN
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,_,_,W,W,_,_,W,W,W},
    {W,W,_,_,W,_,_,W,W,_,_,W,W,W},
    {W,W,_,_,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,_,_,W,W},
    {W,W,W,W,W,W,W,W,W,W,_,_,W,W},
    {W,W,_,_,W,W,W,W,W,W,W,W,W,W},
    {W,W,_,_,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,_,_,W,W},
    {W,W,W,_,_,W,W,_,_,W,_,_,W,W},
    {W,W,W,_,_,W,W,_,_,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W}
#elif GUIPP_X11
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,_,W,_,W,_,W,_,W,W,W,W},
    {W,W,W,_,W,_,W,_,W,_,_,_,W,W},
    {W,W,_,_,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,_,_,W,W},
    {W,W,_,_,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,_,_,W,W},
    {W,W,_,_,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,_,_,W,W},
    {W,W,_,_,_,W,_,W,_,W,_,W,W,W},
    {W,W,W,W,_,W,_,W,_,W,_,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W}
#elif GUIPP_QT
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,_,_,W,W,W,W,_,_,W,W,W},
    {W,W,_,_,_,W,W,W,W,_,_,W,W,W},
    {W,W,_,_,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,_,_,W,W},
    {W,W,W,W,W,W,W,W,W,W,_,_,W,W},
    {W,W,_,_,W,W,W,W,W,W,W,W,W,W},
    {W,W,_,_,W,W,W,_,_,W,W,W,W,W},
    {W,W,W,W,W,W,W,_,_,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W}
#else
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,_,_,W,W,_,_,W,W,_,_,W,W},
    {W,W,_,_,W,W,_,_,W,W,_,_,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,_,_,W,W,W,W,W,W,_,_,W,W},
    {W,W,_,_,W,W,W,W,W,W,_,_,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,_,_,W,W,_,_,W,W,_,_,W,W},
    {W,W,_,_,W,W,_,_,W,W,_,_,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W}
#endif // GUIPP_WIN
  }));
}


// --------------------------------------------------------------------------
void test_dots_frame_x3 () {
  tester<frame::dots>(3.0F, 21, {1, 1}, {5, 5}, CM({
#ifdef GUIPP_WIN
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,_,_,_,W,W,W,_,_,_,W,W,W,W,W},
    {W,W,W,_,_,_,W,_,_,_,W,W,W,_,_,_,W,W,W,W,W},
    {W,W,W,_,_,_,W,_,_,_,W,W,W,_,_,_,W,W,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,W,_,_,_,W,W,W,_,_,_,W,W,_,_,_,W,W,W},
    {W,W,W,W,_,_,_,W,W,W,_,_,_,W,W,W,W,W,W,W,W},
    {W,W,W,W,_,_,_,W,W,W,_,_,_,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W}
#elif GUIPP_X11
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,_,W,_,W,_,W,_,W,_,W,_,W,W,W,W,W,W},
    {W,W,W,W,_,W,_,W,_,W,_,W,_,W,_,_,_,_,W,W,W},
    {W,W,W,_,_,_,_,W,_,W,_,W,_,W,_,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,_,W,_,W,_,W,_,W,_,W,W,W,W,W},
    {W,W,W,W,W,_,W,_,W,_,W,_,W,_,W,_,W,W,W,W,W},
    {W,W,W,W,W,_,W,_,W,_,W,_,W,_,W,_,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W}
#elif GUIPP_QT
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,_,_,_,W,W,W,W,W,W,_,_,_,W,W,W,W,W},
    {W,W,W,_,_,_,_,W,W,W,W,W,W,_,_,_,W,W,W,W,W},
    {W,W,W,_,_,_,_,W,W,W,W,W,W,_,_,_,W,W,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,_,_,_,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W}
#else
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,_,_,_,W,W,W,_,_,_,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,_,_,_,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,_,_,_,W,W,W,_,_,_,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,W,W,W,W,W,W,_,_,_,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,_,_,_,W,W,W,_,_,_,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,_,_,_,W,W,W,_,_,_,W,W,W},
    {W,W,W,_,_,_,W,W,W,_,_,_,W,W,W,_,_,_,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W}
#endif // GUIPP_WIN
  }));
}


// --------------------------------------------------------------------------
void test_lines_frame () {
  tester<frame::lines, color::black>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_},
    {_,_,_,V,_},
    {_,_,_,V,_},
    {_,V,V,V,_},
    {_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_lines_frame_x2 () {
  tester<frame::lines, color::black>(2.0F, 10, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,V,V,_,_},
    {_,_,_,_,_,_,V,V,_,_},
    {_,_,_,_,_,_,V,V,_,_},
    {_,_,_,_,_,_,V,V,_,_},
    {_,_,V,V,V,V,V,V,_,_},
    {_,_,V,V,V,V,V,V,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_lines_frame_x3 () {
  tester<frame::lines, color::black>(3.0F, 15, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,V,V,V,V,V,V,V,V,V,_,_,_},
    {_,_,_,V,V,V,V,V,V,V,V,V,_,_,_},
    {_,_,_,V,V,V,V,V,V,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_lines_frame_x4 () {
  tester<frame::lines, color::black>(4.0F, 20, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,V,V,V,V,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,V,V,V,V,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,V,V,V,V,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,V,V,V,V,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,V,V,V,V,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,V,V,V,V,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,V,V,V,V,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,V,V,V,V,_,_,_,_},
    {_,_,_,_,V,V,V,V,V,V,V,V,V,V,V,V,_,_,_,_},
    {_,_,_,_,V,V,V,V,V,V,V,V,V,V,V,V,_,_,_,_},
    {_,_,_,_,V,V,V,V,V,V,V,V,V,V,V,V,_,_,_,_},
    {_,_,_,_,V,V,V,V,V,V,V,V,V,V,V,V,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_vline () {
  tester<frame::vline, color::black>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_},
    {_,_,_,V,_},
    {_,_,_,V,_},
    {_,_,_,V,_},
    {_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_vline_x2 () {
  tester<frame::vline, color::black>(2.0F, 10, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,V,V,_,_},
    {_,_,_,_,_,_,V,V,_,_},
    {_,_,_,_,_,_,V,V,_,_},
    {_,_,_,_,_,_,V,V,_,_},
    {_,_,_,_,_,_,V,V,_,_},
    {_,_,_,_,_,_,V,V,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_vline_x3 () {
  tester<frame::vline, color::black>(3.0F, 15, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_hline () {
  tester<frame::hline, color::black>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_},
    {_,_,_,_,_},
    {_,_,_,_,_},
    {_,V,V,V,_},
    {_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_hline_x2 () {
  tester<frame::hline, color::black>(2.0F, 10, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,V,V,V,V,V,V,_,_},
    {_,_,V,V,V,V,V,V,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_hline_x3 () {
  tester<frame::hline, color::black>(3.0F, 15, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,V,V,V,V,V,V,V,V,V,_,_,_},
    {_,_,_,V,V,V,V,V,V,V,V,V,_,_,_},
    {_,_,_,V,V,V,V,V,V,V,V,V,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_vraise () {
  tester<frame::vraise, color::black>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_},
    {_,W,_,Y,_},
    {_,W,_,Y,_},
    {_,W,_,Y,_},
    {_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_vraise_x2 () {
  tester<frame::vraise, color::black>(2.0F, 10, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,W,W,_,_,Y,Y,_,_},
    {_,_,W,W,_,_,Y,Y,_,_},
    {_,_,W,W,_,_,Y,Y,_,_},
    {_,_,W,W,_,_,Y,Y,_,_},
    {_,_,W,W,_,_,Y,Y,_,_},
    {_,_,W,W,_,_,Y,Y,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_vraise_x3 () {
  tester<frame::vraise, color::black>(3.0F, 15, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
    {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
    {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
    {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
    {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
    {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
    {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
    {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
    {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_hraise () {
  tester<frame::hraise, color::black>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_},
    {_,W,W,W,_},
    {_,_,_,_,_},
    {_,Y,Y,Y,_},
    {_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_hraise_x2 () {
  tester<frame::hraise, color::black>(2.0F, 10, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,W,W,W,W,W,W,_,_},
    {_,_,W,W,W,W,W,W,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,Y,Y,Y,Y,Y,Y,_,_},
    {_,_,Y,Y,Y,Y,Y,Y,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_hraise_x3 () {
  tester<frame::hraise, color::black>(3.0F, 15, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
    {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
    {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,Y,Y,Y,Y,Y,Y,Y,Y,Y,_,_,_},
    {_,_,_,Y,Y,Y,Y,Y,Y,Y,Y,Y,_,_,_},
    {_,_,_,Y,Y,Y,Y,Y,Y,Y,Y,Y,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}
  }));
}


// --------------------------------------------------------------------------
void test_vgroove () {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::vgroove>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,Y,_,W,_},
                           {_,Y,_,W,_},
                           {_,Y,_,W,_},
                           {_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_vgroove_x2 () {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<frame::vgroove>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,Y,Y,_,_,W,W,_,_},
                           {_,_,Y,Y,_,_,W,W,_,_},
                           {_,_,Y,Y,_,_,W,W,_,_},
                           {_,_,Y,Y,_,_,W,W,_,_},
                           {_,_,Y,Y,_,_,W,W,_,_},
                           {_,_,Y,Y,_,_,W,W,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_vgroove_x3 () {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::vgroove>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_hgroove () {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::hgroove>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,Y,Y,Y,_},
                           {_,_,_,_,_},
                           {_,W,W,W,_},
                           {_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_hgroove_x2 () {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<frame::hgroove>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,Y,Y,Y,Y,Y,Y,_,_},
                           {_,_,Y,Y,Y,Y,Y,Y,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,W,W,W,W,W,W,_,_},
                           {_,_,W,W,W,W,W,W,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_hgroove_x3 () {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::hgroove>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,Y,Y,Y,Y,Y,Y,Y,Y,Y,_,_,_},
                           {_,_,_,Y,Y,Y,Y,Y,Y,Y,Y,Y,_,_,_},
                           {_,_,_,Y,Y,Y,Y,Y,Y,Y,Y,Y,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
                           {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
                           {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_raised_relief () {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::raised_relief>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,W,W,Y,_},
                           {_,W,_,Y,_},
                           {_,Y,Y,Y,_},
                           {_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_raised_relief_x2 () {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<frame::raised_relief>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,W,W,W,W,Y,Y,_,_},
                           {_,_,W,W,W,W,Y,Y,_,_},
                           {_,_,W,W,_,_,Y,Y,_,_},
                           {_,_,W,W,_,_,Y,Y,_,_},
                           {_,_,Y,Y,Y,Y,Y,Y,_,_},
                           {_,_,Y,Y,Y,Y,Y,Y,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_raised_relief_x3 () {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::raised_relief>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,W,W,W,W,W,W,Y,Y,Y,_,_,_},
                           {_,_,_,W,W,W,W,W,W,Y,Y,Y,_,_,_},
                           {_,_,_,W,W,W,W,W,W,Y,Y,Y,_,_,_},
                           {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
                           {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
                           {_,_,_,W,W,W,_,_,_,Y,Y,Y,_,_,_},
                           {_,_,_,Y,Y,Y,Y,Y,Y,Y,Y,Y,_,_,_},
                           {_,_,_,Y,Y,Y,Y,Y,Y,Y,Y,Y,_,_,_},
                           {_,_,_,Y,Y,Y,Y,Y,Y,Y,Y,Y,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_sunken_relief () {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::sunken_relief>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,Y,Y,W,_},
                           {_,Y,_,W,_},
                           {_,W,W,W,_},
                           {_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_sunken_relief_x2 () {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<frame::sunken_relief>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,Y,Y,Y,Y,W,W,_,_},
                           {_,_,Y,Y,Y,Y,W,W,_,_},
                           {_,_,Y,Y,_,_,W,W,_,_},
                           {_,_,Y,Y,_,_,W,W,_,_},
                           {_,_,W,W,W,W,W,W,_,_},
                           {_,_,W,W,W,W,W,W,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_sunken_relief_x3 () {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::sunken_relief>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,Y,Y,Y,Y,Y,Y,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,Y,Y,Y,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,Y,Y,Y,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,Y,Y,Y,_,_,_,W,W,W,_,_,_},
                           {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
                           {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
                           {_,_,_,W,W,W,W,W,W,W,W,W,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}}));
}


// --------------------------------------------------------------------------
void test_raised_deep_relief () {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::raised_deep_relief>({5, 5}), {0, 0});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{W,W,W,W,Y},
                           {W,M,M,D,Y},
                           {W,M,_,D,Y},
                           {W,D,D,D,Y},
                           {Y,Y,Y,Y,Y}}));
}


// --------------------------------------------------------------------------
void test_raised_deep_relief_x2 () {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<frame::raised_deep_relief>({5, 5}), {0, 0});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{W,W,W,W,W,W,W,W,Y,Y},
                           {W,W,W,W,W,W,W,W,Y,Y},
                           {W,W,M,M,M,M,D,D,Y,Y},
                           {W,W,M,M,M,M,D,D,Y,Y},
                           {W,W,M,M,_,_,D,D,Y,Y},
                           {W,W,M,M,_,_,D,D,Y,Y},
                           {W,W,D,D,D,D,D,D,Y,Y},
                           {W,W,D,D,D,D,D,D,Y,Y},
                           {Y,Y,Y,Y,Y,Y,Y,Y,Y,Y},
                           {Y,Y,Y,Y,Y,Y,Y,Y,Y,Y}
                          }));
}


// --------------------------------------------------------------------------
void test_raised_deep_relief_x3 () {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::raised_deep_relief>({5, 5}), {0, 0});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{W,W,W,W,W,W,W,W,W,W,W,W,Y,Y,Y},
                           {W,W,W,W,W,W,W,W,W,W,W,W,Y,Y,Y},
                           {W,W,W,W,W,W,W,W,W,W,W,W,Y,Y,Y},
                           {W,W,W,M,M,M,M,M,M,D,D,D,Y,Y,Y},
                           {W,W,W,M,M,M,M,M,M,D,D,D,Y,Y,Y},
                           {W,W,W,M,M,M,M,M,M,D,D,D,Y,Y,Y},
                           {W,W,W,M,M,M,_,_,_,D,D,D,Y,Y,Y},
                           {W,W,W,M,M,M,_,_,_,D,D,D,Y,Y,Y},
                           {W,W,W,M,M,M,_,_,_,D,D,D,Y,Y,Y},
                           {W,W,W,D,D,D,D,D,D,D,D,D,Y,Y,Y},
                           {W,W,W,D,D,D,D,D,D,D,D,D,Y,Y,Y},
                           {W,W,W,D,D,D,D,D,D,D,D,D,Y,Y,Y},
                           {Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y},
                           {Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y},
                           {Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y}
                          }));
}


// --------------------------------------------------------------------------
void test_sunken_deep_relief () {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::sunken_deep_relief>({5, 5}), {0, 0});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{Y,Y,Y,Y,W},
                           {Y,D,D,M,W},
                           {Y,D,_,M,W},
                           {Y,M,M,M,W},
                           {W,W,W,W,W}
                          }));
}


// --------------------------------------------------------------------------
void test_sunken_deep_relief_x2 () {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<frame::sunken_deep_relief>({5, 5}), {0, 0});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{Y,Y,Y,Y,Y,Y,Y,Y,W,W},
                           {Y,Y,Y,Y,Y,Y,Y,Y,W,W},
                           {Y,Y,D,D,D,D,M,M,W,W},
                           {Y,Y,D,D,D,D,M,M,W,W},
                           {Y,Y,D,D,_,_,M,M,W,W},
                           {Y,Y,D,D,_,_,M,M,W,W},
                           {Y,Y,M,M,M,M,M,M,W,W},
                           {Y,Y,M,M,M,M,M,M,W,W},
                           {W,W,W,W,W,W,W,W,W,W},
                           {W,W,W,W,W,W,W,W,W,W}
                          }));
}


// --------------------------------------------------------------------------
void test_sunken_deep_relief_x3 () {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::sunken_deep_relief>({5, 5}), {0, 0});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,W,W,W},
                           {Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,W,W,W},
                           {Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,Y,W,W,W},
                           {Y,Y,Y,D,D,D,D,D,D,M,M,M,W,W,W},
                           {Y,Y,Y,D,D,D,D,D,D,M,M,M,W,W,W},
                           {Y,Y,Y,D,D,D,D,D,D,M,M,M,W,W,W},
                           {Y,Y,Y,D,D,D,_,_,_,M,M,M,W,W,W},
                           {Y,Y,Y,D,D,D,_,_,_,M,M,M,W,W,W},
                           {Y,Y,Y,D,D,D,_,_,_,M,M,M,W,W,W},
                           {Y,Y,Y,M,M,M,M,M,M,M,M,M,W,W,W},
                           {Y,Y,Y,M,M,M,M,M,M,M,M,M,W,W,W},
                           {Y,Y,Y,M,M,M,M,M,M,M,M,M,W,W,W},
                           {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
                           {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
                           {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W}
                          }));
}


constexpr gui::os::color B7 = gui::color::rgba<0xb7, 0xb7, 0xb7, 0xFF>::value;
constexpr gui::os::color S8 = gui::color::rgba<0x78, 0x78, 0x78, 0xFF>::value;

// --------------------------------------------------------------------------
template<orientation_t O, bool sunken, os::color C = color::medium_gray>
void sep_tester (float scale, int pix, const core::point& pt, const core::size& sz,
                 colormap&& expected) {
  core::global::set_scale_factor(scale);
  pixmap img(pix, pix);
  graphics g(img);
  g.clear(color::black);

  typedef gui::ctrl::detail::line_traits<O> lt;
  typedef gui::ctrl::detail::color_traits<sunken> ct;

  gui::core::rectangle r(pt, sz);
  g.frame(lt::first(r), ct::first(C));
  g.frame(lt::second(r), ct::second(C));

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, expected);
}

// --------------------------------------------------------------------------
void test_vseparator_sunken () {
  sep_tester<orientation_t::vertical, true>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {_,_ ,_,_ ,_},
    {_,S8,_,B7,_},
    {_,S8,_,B7,_},
    {_,S8,_,B7,_},
    {_,_ ,_,_ ,_}
  }));
}


// --------------------------------------------------------------------------
void test_vseparator_raised () {
  sep_tester<orientation_t::vertical, false>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {_,_ ,_,_ ,_},
    {_,B7,_,S8,_},
    {_,B7,_,S8,_},
    {_,B7,_,S8,_},
    {_,_ ,_,_ ,_}
  }));
}


// --------------------------------------------------------------------------
void test_hseparator_sunken () {
  sep_tester<orientation_t::horizontal, true>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {_,_ ,_ ,_ ,_},
    {_,S8,S8,S8,_},
    {_,_ ,_ ,_ ,_},
    {_,B7,B7,B7,_},
    {_,_ ,_ ,_ ,_}
  }));
}


// --------------------------------------------------------------------------
void test_hseparator_raised () {
  sep_tester<orientation_t::horizontal, false>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {_,_ ,_ ,_ ,_},
    {_,B7,B7,B7,_},
    {_,_ ,_ ,_ ,_},
    {_,S8,S8,S8,_},
    {_,_ ,_ ,_ ,_}
  }));
}

// --------------------------------------------------------------------------
void test_main (const testing::start_params& params) {
  testing::init_gui(params);
  testing::log_info("Running frames_test");

  run_test(test_no_frame);

  run_test(test_black_frame);
  run_test(test_black_frame_x2);
  run_test(test_black_frame_x3);

  run_test(test_white_frame);
  run_test(test_white_frame_x2);
  run_test(test_white_frame_x3);

  run_test(test_dots_frame);
  run_test(test_dots_frame_x2);
  run_test(test_dots_frame_x3);

  run_test(test_lines_frame);
  run_test(test_lines_frame_x2);
  run_test(test_lines_frame_x3);
  run_test(test_lines_frame_x4);

  run_test(test_vline);
  run_test(test_vline_x2);
  run_test(test_vline_x3);

  run_test(test_hline);
  run_test(test_hline_x2);
  run_test(test_hline_x3);

  run_test(test_vraise);
  run_test(test_vraise_x2);
  run_test(test_vraise_x3);

  run_test(test_hraise);
  run_test(test_hraise_x2);
  run_test(test_hraise_x3);

  run_test(test_vgroove);
  run_test(test_vgroove_x2);
  run_test(test_vgroove_x3);

  run_test(test_hgroove);
  run_test(test_hgroove_x2);
  run_test(test_hgroove_x3);

  run_test(test_raised_relief);
  run_test(test_raised_relief_x2);
  run_test(test_raised_relief_x3);

  run_test(test_sunken_relief);
  run_test(test_sunken_relief_x2);
  run_test(test_sunken_relief_x3);

  run_test(test_raised_deep_relief);
  run_test(test_raised_deep_relief_x2);
  run_test(test_raised_deep_relief_x3);

  run_test(test_sunken_deep_relief);
  run_test(test_sunken_deep_relief_x2);
  run_test(test_sunken_deep_relief_x3);

  run_test(test_vseparator_sunken);
  run_test(test_vseparator_raised);

  run_test(test_hseparator_sunken);
  run_test(test_hseparator_raised);

}

// --------------------------------------------------------------------------
