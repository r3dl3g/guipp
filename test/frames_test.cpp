
#include <iomanip>

#include <gui/draw/bitmap.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/frames.h>
#include <testlib/image_test_lib.h>
#include <testlib/testlib.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;
using namespace testing;

// --------------------------------------------------------------------------
DECLARE_TEST(test_no_frame);

DECLARE_TEST(test_black_frame);
DECLARE_TEST(test_black_frame_x2);
DECLARE_TEST(test_black_frame_x3);

DECLARE_TEST(test_white_frame);
DECLARE_TEST(test_white_frame_x2);
DECLARE_TEST(test_white_frame_x3);

DECLARE_TEST(test_dots_frame);
DECLARE_TEST(test_dots_frame_x2);
DECLARE_TEST(test_dots_frame_x3);

DECLARE_TEST(test_lines_frame);
DECLARE_TEST(test_lines_frame_x2);
DECLARE_TEST(test_lines_frame_x3);
DECLARE_TEST(test_lines_frame_x4);

DECLARE_TEST(test_vline);
DECLARE_TEST(test_vline_x2);
DECLARE_TEST(test_vline_x3);

DECLARE_TEST(test_hline);
DECLARE_TEST(test_hline_x2);
DECLARE_TEST(test_hline_x3);

DECLARE_TEST(test_vraise);
DECLARE_TEST(test_vraise_x2);
DECLARE_TEST(test_vraise_x3);

DECLARE_TEST(test_hraise);
DECLARE_TEST(test_hraise_x2);
DECLARE_TEST(test_hraise_x3);

DECLARE_TEST(test_vgroove);
DECLARE_TEST(test_vgroove_x2);
DECLARE_TEST(test_vgroove_x3);

DECLARE_TEST(test_hgroove);
DECLARE_TEST(test_hgroove_x2);
DECLARE_TEST(test_hgroove_x3);

DECLARE_TEST(test_raised_relief);
DECLARE_TEST(test_raised_relief_x2);
DECLARE_TEST(test_raised_relief_x3);

DECLARE_TEST(test_sunken_relief);
DECLARE_TEST(test_sunken_relief_x2);
DECLARE_TEST(test_sunken_relief_x3);

DECLARE_TEST(test_raised_deep_relief);
DECLARE_TEST(test_raised_deep_relief_x2);
DECLARE_TEST(test_raised_deep_relief_x3);

DECLARE_TEST(test_sunken_deep_relief);
DECLARE_TEST(test_sunken_deep_relief_x2);
DECLARE_TEST(test_sunken_deep_relief_x3);

// --------------------------------------------------------------------------
TEST_MAIN(frames_test) {
  RUN_TEST(test_no_frame);

  RUN_TEST(test_black_frame);
  RUN_TEST(test_black_frame_x2);
  RUN_TEST(test_black_frame_x3);

  RUN_TEST(test_white_frame);
  RUN_TEST(test_white_frame_x2);
  RUN_TEST(test_white_frame_x3);

  RUN_TEST(test_dots_frame);
  RUN_TEST(test_dots_frame_x2);
  RUN_TEST(test_dots_frame_x3);

  RUN_TEST(test_lines_frame);
  RUN_TEST(test_lines_frame_x2);
  RUN_TEST(test_lines_frame_x3);
  RUN_TEST(test_lines_frame_x4);

  RUN_TEST(test_vline);
  RUN_TEST(test_vline_x2);
  RUN_TEST(test_vline_x3);

  RUN_TEST(test_hline);
  RUN_TEST(test_hline_x2);
  RUN_TEST(test_hline_x3);

  RUN_TEST(test_vraise);
  RUN_TEST(test_vraise_x2);
  RUN_TEST(test_vraise_x3);

  RUN_TEST(test_hraise);
  RUN_TEST(test_hraise_x2);
  RUN_TEST(test_hraise_x3);

  RUN_TEST(test_vgroove);
  RUN_TEST(test_vgroove_x2);
  RUN_TEST(test_vgroove_x3);

  RUN_TEST(test_hgroove);
  RUN_TEST(test_hgroove_x2);
  RUN_TEST(test_hgroove_x3);

  RUN_TEST(test_raised_relief);
  RUN_TEST(test_raised_relief_x2);
  RUN_TEST(test_raised_relief_x3);

  RUN_TEST(test_sunken_relief);
  RUN_TEST(test_sunken_relief_x2);
  RUN_TEST(test_sunken_relief_x3);

  RUN_TEST(test_raised_deep_relief);
  RUN_TEST(test_raised_deep_relief_x2);
  RUN_TEST(test_raised_deep_relief_x3);

  RUN_TEST(test_sunken_deep_relief);
  RUN_TEST(test_sunken_deep_relief_x2);
  RUN_TEST(test_sunken_deep_relief_x3);

} TEST_MAIN_END(frames_test)

// --------------------------------------------------------------------------
template<draw::frame::drawer F>
struct framer {
  framer (const core::size& sz)
    : sz(sz)
  {}

  void operator() (const graphics& g, const core::point& pt) const {
    F(g, core::rectangle(pt, sz));
  }

private:
  const core::size sz;
};

// --------------------------------------------------------------------------
DEFINE_TEST(test_no_frame) {

  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::no_frame>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_black_frame) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::white).copy(framer<frame::black>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{W,W,W,W,W},
                           {W,_,_,_,W},
                           {W,_,W,_,W},
                           {W,_,_,_,W},
                           {W,W,W,W,W}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_black_frame_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::white).copy(framer<frame::black>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{W,W,W,W,W,W,W,W,W,W},
                           {W,W,W,W,W,W,W,W,W,W},
                           {W,W,_,_,_,_,_,_,W,W},
                           {W,W,_,_,_,_,_,_,W,W},
                           {W,W,_,_,W,W,_,_,W,W},
                           {W,W,_,_,W,W,_,_,W,W},
                           {W,W,_,_,_,_,_,_,W,W},
                           {W,W,_,_,_,_,_,_,W,W},
                           {W,W,W,W,W,W,W,W,W,W},
                           {W,W,W,W,W,W,W,W,W,W}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_black_frame_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::white).copy(framer<frame::black>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
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
                           {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_white_frame) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::white>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,W,W,W,_},
                           {_,W,_,W,_},
                           {_,W,W,W,_},
                           {_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_white_frame_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<draw::frame::white>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,W,W,W,W,W,W,_,_},
                           {_,_,W,W,W,W,W,W,_,_},
                           {_,_,W,W,_,_,W,W,_,_},
                           {_,_,W,W,_,_,W,W,_,_},
                           {_,_,W,W,W,W,W,W,_,_},
                           {_,_,W,W,W,W,W,W,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_white_frame_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::white>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
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
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_dots_frame) {
  core::global::set_scale_factor(1.0);
  pixmap img(7, 7);

  graphics(img).clear(color::white).copy(framer<frame::dots>({5, 5}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{W,W,W,W,W,W,W},
                           {W,_,W,_,W,_,W},
                           {W,_,W,W,W,W,W},
                           {W,W,W,W,W,_,W},
                           {W,_,W,W,W,W,W},
                           {W,W,_,W,_,W,W},
                           {W,W,W,W,W,W,W}
                          }));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_dots_frame_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(14, 14);

  graphics(img).clear(color::white).copy(framer<frame::dots>({5, 5}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{W,W,W,W,W,W,W,W,W,W,W,W,W,W},
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
                          }));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_dots_frame_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(21, 21);

  graphics(img).clear(color::white).copy(framer<frame::dots>({5, 5}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
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
                           {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
                           }));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_lines_frame) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::lines>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,V,_},
                           {_,_,_,V,_},
                           {_,V,V,V,_},
                           {_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_lines_frame_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<frame::lines>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,V,V,_,_},
                           {_,_,_,_,_,_,V,V,_,_},
                           {_,_,_,_,_,_,V,V,_,_},
                           {_,_,_,_,_,_,V,V,_,_},
                           {_,_,V,V,V,V,V,V,_,_},
                           {_,_,V,V,V,V,V,V,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_lines_frame_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::lines>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
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
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_lines_frame_x4) {
  core::global::set_scale_factor(4.0);
  pixmap img(20, 20);

  graphics(img).clear(color::black).copy(framer<frame::lines>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_vline) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::vline>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,V,_},
                           {_,_,_,V,_},
                           {_,_,_,V,_},
                           {_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_vline_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<frame::vline>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,V,V,_,_},
                           {_,_,_,_,_,_,V,V,_,_},
                           {_,_,_,_,_,_,V,V,_,_},
                           {_,_,_,_,_,_,V,V,_,_},
                           {_,_,_,_,_,_,V,V,_,_},
                           {_,_,_,_,_,_,V,V,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_vline_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::vline>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
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
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_hline) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::hline>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,V,V,V,_},
                           {_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_hline_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<frame::hline>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,V,V,V,V,V,V,_,_},
                           {_,_,V,V,V,V,V,V,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_hline_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::hline>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
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
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_vraise) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::vraise>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,W,_,Y,_},
                           {_,W,_,Y,_},
                           {_,W,_,Y,_},
                           {_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_vraise_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<frame::vraise>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,W,W,_,_,Y,Y,_,_},
                           {_,_,W,W,_,_,Y,Y,_,_},
                           {_,_,W,W,_,_,Y,Y,_,_},
                           {_,_,W,W,_,_,Y,Y,_,_},
                           {_,_,W,W,_,_,Y,Y,_,_},
                           {_,_,W,W,_,_,Y,Y,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_vraise_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::vraise>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
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
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_hraise) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);

  graphics(img).clear(color::black).copy(framer<frame::hraise>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,W,W,W,_},
                           {_,_,_,_,_},
                           {_,Y,Y,Y,_},
                           {_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_hraise_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);

  graphics(img).clear(color::black).copy(framer<frame::hraise>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,W,W,W,W,W,W,_,_},
                           {_,_,W,W,W,W,W,W,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,Y,Y,Y,Y,Y,Y,_,_},
                           {_,_,Y,Y,Y,Y,Y,Y,_,_},
                           {_,_,_,_,_,_,_,_,_,_},
                           {_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_hraise_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);

  graphics(img).clear(color::black).copy(framer<frame::hraise>({3, 3}), {1, 1});

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
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
                           {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}}));
}
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_vgroove) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_vgroove_x2) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_vgroove_x3) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_hgroove) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_hgroove_x2) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_hgroove_x3) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_raised_relief) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_raised_relief_x2) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_raised_relief_x3) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_sunken_relief) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_sunken_relief_x2) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_sunken_relief_x3) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_raised_deep_relief) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_raised_deep_relief_x2) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_raised_deep_relief_x3) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_sunken_deep_relief) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_sunken_deep_relief_x2) {
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
END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_sunken_deep_relief_x3) {
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
END_TEST()

// --------------------------------------------------------------------------
