
#include <iomanip>

#include <gui/draw/bitmap.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/use.h>
#include <testlib/image_test_lib.h>
#include <testlib/testlib.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

#define TEST_RAWx
#define TEST_RECT
#define TEST_LINE
#define TEST_ELLIPSE
#define TEST_ROUND_RECT
#define TEST_ARC
#define TEST_POLYGON

using namespace gui;
using namespace gui::draw;
using namespace testing;

// --------------------------------------------------------------------------
DECLARE_TEST(test_data2colormap);
DECLARE_TEST(test_raw_rect);

DECLARE_TEST(test_draw_line_4x4);
DECLARE_TEST(test_draw_line_3x3);
DECLARE_TEST(test_draw_line_2x2);
DECLARE_TEST(test_draw_line_1x1);
DECLARE_TEST(test_draw_line_0x0);

DECLARE_TEST(test_draw_line_4x4_x2);
DECLARE_TEST(test_draw_line_3x3_x2);
DECLARE_TEST(test_draw_line_2x2_x2);
DECLARE_TEST(test_draw_line_1x1_x2);
DECLARE_TEST(test_draw_line_0x0_x2);

DECLARE_TEST(test_draw_line_4x4_x3);
DECLARE_TEST(test_draw_line_3x3_x3);
DECLARE_TEST(test_draw_line_2x2_x3);
DECLARE_TEST(test_draw_line_1x1_x3);
DECLARE_TEST(test_draw_line_0x0_x3);

DECLARE_TEST(test_draw_line_top_left);
DECLARE_TEST(test_draw_line_top_left_2);
DECLARE_TEST(test_draw_line_top_right);
DECLARE_TEST(test_draw_line_top_right_2);

DECLARE_TEST(test_draw_line_bottom_left);
DECLARE_TEST(test_draw_line_bottom_left_2);
DECLARE_TEST(test_draw_line_bottom_right);
DECLARE_TEST(test_draw_line_bottom_right_2);

DECLARE_TEST(test_frame_rect_4x4);
DECLARE_TEST(test_frame_rect_3x3);
DECLARE_TEST(test_frame_rect_2x2);
DECLARE_TEST(test_frame_rect_1x1);
DECLARE_TEST(test_frame_rect_0x0);

DECLARE_TEST(test_frame_rect_4x4_x2);
DECLARE_TEST(test_frame_rect_3x3_x2);
DECLARE_TEST(test_frame_rect_2x2_x2);
DECLARE_TEST(test_frame_rect_1x1_x2);
DECLARE_TEST(test_frame_rect_0x0_x2);

DECLARE_TEST(test_frame_rect_4x4_x3);
DECLARE_TEST(test_frame_rect_3x3_x3);
DECLARE_TEST(test_frame_rect_2x2_x3);
DECLARE_TEST(test_frame_rect_1x1_x3);
DECLARE_TEST(test_frame_rect_0x0_x3);

DECLARE_TEST(test_frame_rect_4x4_x4);
DECLARE_TEST(test_frame_rect_3x3_x4);
DECLARE_TEST(test_frame_rect_2x2_x4);
DECLARE_TEST(test_frame_rect_1x1_x4);
DECLARE_TEST(test_frame_rect_0x0_x4);

DECLARE_TEST(test_fill_rect_4x4);
DECLARE_TEST(test_fill_rect_3x3);
DECLARE_TEST(test_fill_rect_2x2);
DECLARE_TEST(test_fill_rect_1x1);
DECLARE_TEST(test_fill_rect_0x0);

DECLARE_TEST(test_fill_rect_4x4_x2);
DECLARE_TEST(test_fill_rect_3x3_x2);
DECLARE_TEST(test_fill_rect_2x2_x2);
DECLARE_TEST(test_fill_rect_1x1_x2);
DECLARE_TEST(test_fill_rect_0x0_x2);

DECLARE_TEST(test_fill_rect_4x4_x3);
DECLARE_TEST(test_fill_rect_3x3_x3);
DECLARE_TEST(test_fill_rect_2x2_x3);
DECLARE_TEST(test_fill_rect_1x1_x3);
DECLARE_TEST(test_fill_rect_0x0_x3);

DECLARE_TEST(test_draw_rect_4x4);
DECLARE_TEST(test_draw_rect_3x3);
DECLARE_TEST(test_draw_rect_2x2);
DECLARE_TEST(test_draw_rect_1x1);
DECLARE_TEST(test_draw_rect_0x0);

DECLARE_TEST(test_draw_rect_4x4_x2);
DECLARE_TEST(test_draw_rect_3x3_x2);
DECLARE_TEST(test_draw_rect_2x2_x2);
DECLARE_TEST(test_draw_rect_1x1_x2);
DECLARE_TEST(test_draw_rect_0x0_x2);

DECLARE_TEST(test_draw_rect_4x4_x3);
DECLARE_TEST(test_draw_rect_3x3_x3);
DECLARE_TEST(test_draw_rect_2x2_x3);
DECLARE_TEST(test_draw_rect_1x1_x3);
DECLARE_TEST(test_draw_rect_0x0_x3);

DECLARE_TEST(test_raw_ellipse);

DECLARE_TEST(test_frame_ellipse_4x4);
DECLARE_TEST(test_frame_ellipse_3x3);
DECLARE_TEST(test_frame_ellipse_2x2);
DECLARE_TEST(test_frame_ellipse_1x1);
DECLARE_TEST(test_frame_ellipse_0x0);

DECLARE_TEST(test_frame_ellipse_4x4_x2);
DECLARE_TEST(test_frame_ellipse_3x3_x2);
DECLARE_TEST(test_frame_ellipse_2x2_x2);
DECLARE_TEST(test_frame_ellipse_1x1_x2);
DECLARE_TEST(test_frame_ellipse_0x0_x2);

DECLARE_TEST(test_frame_ellipse_4x4_x3);
DECLARE_TEST(test_frame_ellipse_3x3_x3);
DECLARE_TEST(test_frame_ellipse_2x2_x3);
DECLARE_TEST(test_frame_ellipse_1x1_x3);
DECLARE_TEST(test_frame_ellipse_0x0_x3);

DECLARE_TEST(test_fill_ellipse);

DECLARE_TEST(test_draw_ellipse_4x4);
DECLARE_TEST(test_draw_ellipse_3x3);
DECLARE_TEST(test_draw_ellipse_2x2);
DECLARE_TEST(test_draw_ellipse_1x1);
DECLARE_TEST(test_draw_ellipse_0x0);

DECLARE_TEST(test_draw_ellipse_4x4_2);
DECLARE_TEST(test_draw_ellipse_3x3_2);
DECLARE_TEST(test_draw_ellipse_2x2_2);
DECLARE_TEST(test_draw_ellipse_1x1_2);
DECLARE_TEST(test_draw_ellipse_0x0_2);

DECLARE_TEST(test_draw_ellipse_4x4_3);
DECLARE_TEST(test_draw_ellipse_3x3_3);
DECLARE_TEST(test_draw_ellipse_2x2_3);
DECLARE_TEST(test_draw_ellipse_1x1_3);
DECLARE_TEST(test_draw_ellipse_0x0_3);

DECLARE_TEST(test_frame_round_rectangle);
DECLARE_TEST(test_fill_round_rectangle);
DECLARE_TEST(test_draw_round_rectangle);
DECLARE_TEST(test_frame_round_rectangle_2);
DECLARE_TEST(test_fill_round_rectangle_2);
DECLARE_TEST(test_draw_round_rectangle_2);

DECLARE_TEST(test_frame_arc_4x4);
DECLARE_TEST(test_frame_arc_3x3);
DECLARE_TEST(test_frame_arc_2x2);
DECLARE_TEST(test_frame_arc_1x1);
DECLARE_TEST(test_frame_arc_0x0);

DECLARE_TEST(test_fill_arc_4x4);
DECLARE_TEST(test_fill_arc_3x3);
DECLARE_TEST(test_fill_arc_2x2);
DECLARE_TEST(test_fill_arc_1x1);
DECLARE_TEST(test_fill_arc_0x0);

DECLARE_TEST(test_draw_arc_4x4);
DECLARE_TEST(test_draw_arc_3x3);
DECLARE_TEST(test_draw_arc_2x2);
DECLARE_TEST(test_draw_arc_1x1);
DECLARE_TEST(test_draw_arc_0x0);

DECLARE_TEST(test_frame_arc_45_135);
DECLARE_TEST(test_frame_arc_135_225);
DECLARE_TEST(test_frame_arc_225_315);
DECLARE_TEST(test_frame_arc_315_405);

DECLARE_TEST(test_draw_arc_45_135);
DECLARE_TEST(test_draw_arc_135_225);
DECLARE_TEST(test_draw_arc_225_315);
DECLARE_TEST(test_draw_arc_315_405);

DECLARE_TEST(test_frame_arc_45_135_2);
DECLARE_TEST(test_frame_arc_135_225_2);
DECLARE_TEST(test_frame_arc_225_315_2);
DECLARE_TEST(test_frame_arc_315_405_2);

DECLARE_TEST(test_frame_polygon);
DECLARE_TEST(test_fill_polygon);
DECLARE_TEST(test_draw_polygon);

// --------------------------------------------------------------------------
TEST_MAIN(drawer_test) {
#ifdef TEST_RAW
  RUN_TEST(test_data2colormap);
  RUN_TEST(test_raw_rect);
  RUN_TEST(test_raw_ellipse);
#endif // TEST_RAW

#ifdef TEST_RECT
  RUN_TEST(test_frame_rect_4x4);
  RUN_TEST(test_frame_rect_3x3);
  RUN_TEST(test_frame_rect_2x2);
  RUN_TEST(test_frame_rect_1x1);
  RUN_TEST(test_frame_rect_0x0);

  RUN_TEST(test_frame_rect_4x4_x2);
  RUN_TEST(test_frame_rect_3x3_x2);
  RUN_TEST(test_frame_rect_2x2_x2);
  RUN_TEST(test_frame_rect_1x1_x2);
  RUN_TEST(test_frame_rect_0x0_x2);

  RUN_TEST(test_frame_rect_4x4_x3);
  RUN_TEST(test_frame_rect_3x3_x3);
  RUN_TEST(test_frame_rect_2x2_x3);
  RUN_TEST(test_frame_rect_1x1_x3);
  RUN_TEST(test_frame_rect_0x0_x3);

  RUN_TEST(test_frame_rect_4x4_x4);
  RUN_TEST(test_frame_rect_3x3_x4);
  RUN_TEST(test_frame_rect_2x2_x4);
  RUN_TEST(test_frame_rect_1x1_x4);
  RUN_TEST(test_frame_rect_0x0_x4);

  RUN_TEST(test_fill_rect_4x4);
  RUN_TEST(test_fill_rect_3x3);
  RUN_TEST(test_fill_rect_2x2);
  RUN_TEST(test_fill_rect_1x1);
  RUN_TEST(test_fill_rect_0x0);

  RUN_TEST(test_fill_rect_4x4_x2);
  RUN_TEST(test_fill_rect_3x3_x2);
  RUN_TEST(test_fill_rect_2x2_x2);
  RUN_TEST(test_fill_rect_1x1_x2);
  RUN_TEST(test_fill_rect_0x0_x2);

  RUN_TEST(test_fill_rect_4x4_x3);
  RUN_TEST(test_fill_rect_3x3_x3);
  RUN_TEST(test_fill_rect_2x2_x3);
  RUN_TEST(test_fill_rect_1x1_x3);
  RUN_TEST(test_fill_rect_0x0_x3);

  RUN_TEST(test_draw_rect_4x4);
  RUN_TEST(test_draw_rect_3x3);
  RUN_TEST(test_draw_rect_2x2);
  RUN_TEST(test_draw_rect_1x1);
  RUN_TEST(test_draw_rect_0x0);

  RUN_TEST(test_draw_rect_4x4_x2);
  RUN_TEST(test_draw_rect_3x3_x2);
  RUN_TEST(test_draw_rect_2x2_x2);
  RUN_TEST(test_draw_rect_1x1_x2);
  RUN_TEST(test_draw_rect_0x0_x2);

  RUN_TEST(test_draw_rect_4x4_x3);
  RUN_TEST(test_draw_rect_3x3_x3);
  RUN_TEST(test_draw_rect_2x2_x3);
  RUN_TEST(test_draw_rect_1x1_x3);
  RUN_TEST(test_draw_rect_0x0_x3);
#endif //TEST_RECT

#ifdef TEST_LINE
  RUN_TEST(test_draw_line_4x4);
  RUN_TEST(test_draw_line_3x3);
  RUN_TEST(test_draw_line_2x2);
  RUN_TEST(test_draw_line_1x1);
  RUN_TEST(test_draw_line_0x0);

  RUN_TEST(test_draw_line_4x4_x2);
  RUN_TEST(test_draw_line_3x3_x2);
  RUN_TEST(test_draw_line_2x2_x2);
  RUN_TEST(test_draw_line_1x1_x2);
  RUN_TEST(test_draw_line_0x0_x2);

  RUN_TEST(test_draw_line_4x4_x3);
  RUN_TEST(test_draw_line_3x3_x3);
  RUN_TEST(test_draw_line_2x2_x3);
  RUN_TEST(test_draw_line_1x1_x3);
  RUN_TEST(test_draw_line_0x0_x3);

  RUN_TEST(test_draw_line_top_left);
  RUN_TEST(test_draw_line_top_left_2);
  RUN_TEST(test_draw_line_top_right);
  RUN_TEST(test_draw_line_top_right_2);

  RUN_TEST(test_draw_line_bottom_left);
  RUN_TEST(test_draw_line_bottom_left_2);
  RUN_TEST(test_draw_line_bottom_right);
  RUN_TEST(test_draw_line_bottom_right_2);
#endif //TEST_LINE

#ifdef TEST_ELLIPSE
  RUN_TEST(test_frame_ellipse_4x4);
  RUN_TEST(test_frame_ellipse_3x3);
  RUN_TEST(test_frame_ellipse_2x2);
  RUN_TEST(test_frame_ellipse_1x1);
  RUN_TEST(test_frame_ellipse_0x0);

  RUN_TEST(test_frame_ellipse_4x4_x2);
  RUN_TEST(test_frame_ellipse_3x3_x2);
  RUN_TEST(test_frame_ellipse_2x2_x2);
  RUN_TEST(test_frame_ellipse_1x1_x2);
  RUN_TEST(test_frame_ellipse_0x0_x2);

  RUN_TEST(test_frame_ellipse_4x4_x3);
  RUN_TEST(test_frame_ellipse_3x3_x3);
  RUN_TEST(test_frame_ellipse_2x2_x3);
  RUN_TEST(test_frame_ellipse_1x1_x3);
  RUN_TEST(test_frame_ellipse_0x0_x3);

  RUN_TEST(test_fill_ellipse);

  RUN_TEST(test_draw_ellipse_4x4);
  RUN_TEST(test_draw_ellipse_3x3);
  RUN_TEST(test_draw_ellipse_2x2);
  RUN_TEST(test_draw_ellipse_1x1);
  RUN_TEST(test_draw_ellipse_0x0);

  RUN_TEST(test_draw_ellipse_4x4_2);
  RUN_TEST(test_draw_ellipse_3x3_2);
  RUN_TEST(test_draw_ellipse_2x2_2);
  RUN_TEST(test_draw_ellipse_1x1_2);
  RUN_TEST(test_draw_ellipse_0x0_2);

  RUN_TEST(test_draw_ellipse_4x4_3);
  RUN_TEST(test_draw_ellipse_3x3_3);
  RUN_TEST(test_draw_ellipse_2x2_3);
  RUN_TEST(test_draw_ellipse_1x1_3);
  RUN_TEST(test_draw_ellipse_0x0_3);
#endif // TEST_ELLIPSE

#ifdef TEST_ROUND_RECT
  RUN_TEST(test_frame_round_rectangle);
  RUN_TEST(test_fill_round_rectangle);
  RUN_TEST(test_draw_round_rectangle);
  RUN_TEST(test_frame_round_rectangle_2);
  RUN_TEST(test_fill_round_rectangle_2);
  RUN_TEST(test_draw_round_rectangle_2);
#endif //TEST_ROUND_RECT

#ifdef TEST_ARC
  RUN_TEST(test_frame_arc_4x4);
  RUN_TEST(test_frame_arc_3x3);
  RUN_TEST(test_frame_arc_2x2);
  RUN_TEST(test_frame_arc_1x1);
  RUN_TEST(test_frame_arc_0x0);
  RUN_TEST(test_fill_arc_4x4);
  RUN_TEST(test_fill_arc_3x3);
  RUN_TEST(test_fill_arc_2x2);
  RUN_TEST(test_fill_arc_1x1);
  RUN_TEST(test_fill_arc_0x0);
  RUN_TEST(test_draw_arc_4x4);
  RUN_TEST(test_draw_arc_3x3);
  RUN_TEST(test_draw_arc_2x2);
  RUN_TEST(test_draw_arc_1x1);
  RUN_TEST(test_draw_arc_0x0);

  RUN_TEST(test_frame_arc_45_135);
  RUN_TEST(test_frame_arc_135_225);
  RUN_TEST(test_frame_arc_225_315);
  RUN_TEST(test_frame_arc_315_405);

  RUN_TEST(test_draw_arc_45_135);
  RUN_TEST(test_draw_arc_135_225);
  RUN_TEST(test_draw_arc_225_315);
  RUN_TEST(test_draw_arc_315_405);

  RUN_TEST(test_frame_arc_45_135_2);
  RUN_TEST(test_frame_arc_135_225_2);
  RUN_TEST(test_frame_arc_225_315_2);
  RUN_TEST(test_frame_arc_315_405_2);
#endif //TEST_ARC

#ifdef TEST_POLYGON
  RUN_TEST(test_frame_polygon);
  RUN_TEST(test_fill_polygon);
  RUN_TEST(test_draw_polygon);
#endif //TEST_POLYGON

} TEST_MAIN_END(drawer_test)


// --------------------------------------------------------------------------
DEFINE_TEST(test_data2colormap) {

  std::vector<uint32_t> testdata = { 0x00000000, 0x00000001, 0x00000002, 0x00000003,
                                     0x40000000, 0x30000000, 0x20000000, 0x10000000,
                                     0x01010101, 0x10101010, 0xF0F0F0F0, 0x0F0F0F0F};
  colormap result = data2colormap ((const char*)testdata.data(), 4, 16, 3);

  EXPECT_EQUAL(result, CM({ {0x00000000, 0x00000001, 0x00000002, 0x00000003},
                            {0x40000000, 0x30000000, 0x20000000, 0x10000000},
                            {0x01010101, 0x10101010, 0xF0F0F0F0, 0x0F0F0F0F}}));

}
END_TEST(test_data2colormap)

// --------------------------------------------------------------------------
template<typename T, typename D = draw::pen>
void tester (float scale, int sz, const core::point& p1, const core::point& p2,
             colormap&& expected) {
  core::global::set_scale_factor(scale);
  pixmap img(sz, sz);
  graphics g(img);
  g.clear(color::black);
  T(p1, p2)(g, D(color::blue));
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, expected);
}

// --------------------------------------------------------------------------
template<typename T, os::color B, os::color P>
void tester2 (float scale, int sz, const core::point& p1, const core::point& p2,
             colormap&& expected) {
  core::global::set_scale_factor(scale);
  pixmap img(sz, sz);
  graphics g(img);
  g.clear(color::black);
  T(p1, p2)(g, brush(B), pen(P));
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, expected);
}

// --------------------------------------------------------------------------
DEFINE_TEST(test_raw_rect) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics g(img);
  g.clear(color::black);
  pen p(color::blue);
  draw::Use<pen> up(g, p);

  core::rectangle r(core::point(1, 1), core::size(3, 3));
#ifdef X11
  XDrawRectangle(core::global::get_instance(), img, g, r.os_x(), r.os_y(), r.os_width() - 1, r.os_height() - 1);
#endif
#ifdef WIN32
  SelectObject(g, GetStockObject(NULL_BRUSH));
  Rectangle(g, r.os_x(), r.os_y(), r.os_x2(), r.os_y2());
#endif

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,R,_},
                           {_,R,_,R,_},
                           {_,R,R,R,_},
                           {_,_,_,_,_}}));
} END_TEST()

static const int degree_360 = 360 * 64;

// --------------------------------------------------------------------------
DEFINE_TEST(test_raw_ellipse) {
  [] () {
    core::global::set_scale_factor(1.0);
    pixmap img(5, 5);
    graphics g(img);
    g.clear(color::black);
    pen p(color::blue);
    draw::Use<pen> up(g, p);

    core::rectangle r(core::point(1, 1), core::size(3, 3));
  #ifdef X11
    XSetArcMode(core::global::get_instance(), g, ArcChord);
    XDrawArc(core::global::get_instance(), img, g, 1, 1, 2, 2, 0, degree_360);
  #endif
  #ifdef WIN32
    SelectObject(g, GetStockObject(NULL_BRUSH));
    Ellipse(g, r.os_x(), r.os_y(), r.os_x2(), r.os_y2());
  #endif

    auto buffer = pixmap2colormap(img);
    TEST_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,R,_,_},
                           {_,R,_,R,_},
                           {_,_,R,_,_},
                           {_,_,_,_,_}}));
  } ();
  [] () {
    core::global::set_scale_factor(1.0);
    pixmap img(7, 7);
    graphics g(img);
    g.clear(color::black);
    pen p(color::blue);
    draw::Use<pen> up(g, p);

    core::rectangle r(core::point(1, 1), core::size(5, 5));
#ifdef X11
    XSetArcMode(core::global::get_instance(), g, ArcChord);
    XDrawArc(core::global::get_instance(), img, g, 1, 1, 4, 4, 0, degree_360);
#endif
#ifdef WIN32
    SelectObject(g, GetStockObject(NULL_BRUSH));
    Ellipse(g, r.os_x(), r.os_y(), r.os_x2(), r.os_y2());
#endif

    auto buffer = pixmap2colormap(img);
    TEST_EQUAL(buffer, CM({{_,_,_,_,_,_,_},
                           {_,_,R,R,R,_,_},
                           {_,R,_,_,_,R,_},
                           {_,R,_,_,_,R,_},
                           {_,R,_,_,_,R,_},
                           {_,_,R,R,R,_,_},
                           {_,_,_,_,_,_,_}
    }));
  } ();
  [] () {
    core::global::set_scale_factor(1.0);
    pixmap img(15, 15);
    graphics g(img);
    g.clear(color::black);
    pen p(color::blue);
    draw::Use<pen> up(g, p);

#ifdef X11
    XSetArcMode(core::global::get_instance(), g, ArcChord);
    XDrawArc(core::global::get_instance(), img, g, 4, 4, 6, 6, 0, degree_360);
#endif
#ifdef WIN32
    SelectObject(g, GetStockObject(NULL_BRUSH));
    Ellipse(g, 3, 3, 12, 12);
    Ellipse(g, 4, 4, 11, 11);
    Ellipse(g, 5, 5, 10, 10);
#endif

    auto buffer = pixmap2colormap(img);
    TEST_EQUAL(buffer, CM({
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
      {_,_,_,_,_,_,R,R,R,_,_,_,_,_,_},
      {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
      {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
      {_,_,_,R,R,R,_,_,_,R,R,R,_,_,_},
      {_,_,_,R,R,R,_,_,_,R,R,R,_,_,_},
      {_,_,_,R,R,R,_,_,_,R,R,R,_,_,_},
      {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
      {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
      {_,_,_,_,_,_,R,R,R,_,_,_,_,_,_},
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    }));
  } ();
  [] () {
    core::global::set_scale_factor(3.0);
    pixmap img(15, 15);
    graphics g(img);
    g.clear(color::black);

#ifdef X11
    pen p(color::blue);
    draw::Use<pen> up(g, p);
    core::rectangle r(core::point(1, 1), core::size(5, 5));
    XDrawRectangle(core::global::get_instance(), img, g, r.os_x(), r.os_y(), r.os_width() - 1, r.os_height() - 1);
#endif
#ifdef WIN32
    auto hPen = CreatePen(PS_SOLID, 3, color::blue);
    auto hOldPen = SelectObject(g, hPen);
    auto hBrush = CreateSolidBrush(color::black);
    auto hOldBrush = SelectObject(g, hBrush);
    //SetBkMode(g, TRANSPARENT);
    //auto hOldBrush = SelectObject(g, GetStockObject(NULL_BRUSH));
    POINT org = {0, 0};
    GetBrushOrgEx(g, &org);
    Ellipse(g, 4, 4, 11, 11);
    SelectObject(g, hOldBrush);
    //DeleteObject(hBrush);
    SelectObject(g, hOldPen);
    DeleteObject(hPen);
#endif

    auto buffer = pixmap2colormap(img);
    TEST_EQUAL(buffer, CM({
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
      {_,_,_,_,_,_,R,R,R,_,_,_,_,_,_},
      {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
      {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
      {_,_,_,R,R,R,R,_,R,R,R,R,_,_,_},
      {_,_,_,R,R,R,_,_,_,R,R,R,_,_,_},
      {_,_,_,R,R,R,R,_,R,R,R,R,_,_,_},
      {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
      {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
      {_,_,_,_,_,_,R,R,R,_,_,_,_,_,_},
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
      {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    }));
  } ();
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_4x4) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,R,R},
                           {_,R,_,_,R},
                           {_,R,_,_,R},
                           {_,R,R,R,R}}));
}
END_TEST(test_frame_rect_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_3x3) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,R,_},
                           {_,R,_,R,_},
                           {_,R,R,R,_},
                           {_,_,_,_,_}}));
}
END_TEST(test_frame_rect_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_2x2) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,_,_},
                           {_,R,R,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));
}
END_TEST(test_frame_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_1x1) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));
}
END_TEST(test_frame_rect_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_0x0) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));
}
END_TEST(test_frame_rect_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_4x4_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,_,_,_,_,R,R},
                            {_,_,R,R,_,_,_,_,R,R},
                            {_,_,R,R,_,_,_,_,R,R},
                            {_,_,R,R,_,_,_,_,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                          }));
}
END_TEST(test_frame_rect_4x4_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_3x3_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,R,R,_,_,R,R,_,_},
                            {_,_,R,R,_,_,R,R,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_3x3_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_2x2_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_2x2_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_1x1_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,_,_,_,_,_,_},
                            {_,_,R,R,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_1x1_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_0x0_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_0x0_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_4x4_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R},
                            {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R},
                            {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R},
                            {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R},
                            {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R},
                            {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                          }));
}
END_TEST(test_frame_rect_4x4_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_3x3_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,_,_,_,R,R,R,_,_,_},
                            {_,_,_,R,R,R,_,_,_,R,R,R,_,_,_},
                            {_,_,_,R,R,R,_,_,_,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_3x3_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_2x2_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_2x2_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_1x1_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_1x1_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_0x0_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_0x0_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_4x4_x4) {
  core::global::set_scale_factor(4.0);
  pixmap img(20, 20);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,R,R,R,R},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,R,R,R,R},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,R,R,R,R},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,R,R,R,R},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,R,R,R,R},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,R,R,R,R},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,R,R,R,R},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,R,R,R,R},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R,R},
                          }));
}
END_TEST(test_frame_rect_4x4_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_3x3_x4) {
  core::global::set_scale_factor(4.0);
  pixmap img(20, 20);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
                            {_,_,_,_,R,R,R,R,_,_,_,_,R,R,R,R,_,_,_,_},
                            {_,_,_,_,R,R,R,R,_,_,_,_,R,R,R,R,_,_,_,_},
                            {_,_,_,_,R,R,R,R,_,_,_,_,R,R,R,R,_,_,_,_},
                            {_,_,_,_,R,R,R,R,_,_,_,_,R,R,R,R,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_3x3_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_2x2_x4) {
  core::global::set_scale_factor(4.0);
  pixmap img(20, 20);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_2x2_x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_1x1_x4) {
  core::global::set_scale_factor(4.0);
  pixmap img(20, 20);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_1x1_x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_0x0_x4) {
  core::global::set_scale_factor(4.0);
  pixmap img(20, 20);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_frame_rect_0x0_x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_4x4) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,R,R},
                           {_,R,R,R,R},
                           {_,R,R,R,R},
                           {_,R,R,R,R}}));

}
END_TEST(test_fill_rect_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_3x3) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,R,_},
                           {_,R,R,R,_},
                           {_,R,R,R,_},
                           {_,_,_,_,_}}));

}
END_TEST(test_fill_rect_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_2x2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,_,_},
                           {_,R,R,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));

}
END_TEST(test_fill_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_1x1) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));

}
END_TEST(test_fill_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_0x0) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));

}
END_TEST(test_fill_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_4x4_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                          }));
}
END_TEST(test_fill_rect_4x4_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_3x3_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_fill_rect_3x3_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_2x2_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_fill_rect_2x2_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_1x1_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,_,_,_,_,_,_},
                            {_,_,R,R,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_fill_rect_1x1_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_0x0_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_fill_rect_0x0_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_4x4_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                          }));
}
END_TEST(test_fill_rect_4x4_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_3x3_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_fill_rect_3x3_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_2x2_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_fill_rect_2x2_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_1x1_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_fill_rect_1x1_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_0x0_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_fill_rect_0x0_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_4x4) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,R,R},
                           {_,R,B,B,R},
                           {_,R,B,B,R},
                           {_,R,R,R,R}}));
}
END_TEST(test_draw_rect_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_3x3) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,R,_},
                           {_,R,B,R,_},
                           {_,R,R,R,_},
                           {_,_,_,_,_}}));
}
END_TEST(test_draw_rect_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_2x2) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,_,_},
                           {_,R,R,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));
}
END_TEST(test_draw_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_1x1) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));
}
END_TEST(test_draw_rect_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_0x0) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));
}
END_TEST(test_draw_rect_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_4x4_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,B,B,B,B,R,R},
                            {_,_,R,R,B,B,B,B,R,R},
                            {_,_,R,R,B,B,B,B,R,R},
                            {_,_,R,R,B,B,B,B,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                            {_,_,R,R,R,R,R,R,R,R},
                          }));
}
END_TEST(test_draw_rect_4x4_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_3x3_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,R,R,B,B,R,R,_,_},
                            {_,_,R,R,B,B,R,R,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,R,R,R,R,R,R,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_draw_rect_3x3_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_2x2_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,R,R,R,R,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_draw_rect_2x2_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_1x1_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,R,R,_,_,_,_,_,_},
                            {_,_,R,R,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_draw_rect_1x1_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_0x0_x2) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_draw_rect_0x0_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_4x4_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R},
                            {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R},
                            {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R},
                            {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R},
                            {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R},
                            {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                            {_,_,_,R,R,R,R,R,R,R,R,R,R,R,R},
                          }));
}
END_TEST(test_draw_rect_4x4_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_3x3_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,B,B,B,R,R,R,_,_,_},
                            {_,_,_,R,R,R,B,B,B,R,R,R,_,_,_},
                            {_,_,_,R,R,R,B,B,B,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_draw_rect_3x3_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_2x2_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,R,R,R,R,R,R,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_draw_rect_2x2_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_1x1_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
                            {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_draw_rect_1x1_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_0x0_x3) {
  core::global::set_scale_factor(3.0);
  pixmap img(15, 15);
  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::red, color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                            {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
                          }));
}
END_TEST(test_draw_rect_0x0_x3)

// --------------------------------------------------------------------------
void test_line (float scale, int sz, const core::point& p1, const core::point& p2, colormap&& expected) {
  tester<draw::line>(scale, sz, p1, p2, std::move(expected));
}

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_4x4) {
  test_line(1.0, 5, {1, 1}, {4, 4}, CM({{_,_,_,_,_},
                                        {_,R,_,_,_},
                                        {_,_,R,_,_},
                                        {_,_,_,R,_},
                                        {_,_,_,_,R}}));
}
END_TEST(test_draw_line_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_3x3) {
  test_line(1.0, 5, {1, 1}, {3, 3}, CM({{_,_,_,_,_},
                                        {_,R,_,_,_},
                                        {_,_,R,_,_},
                                        {_,_,_,R,_},
                                        {_,_,_,_,_}}));
}
END_TEST(test_draw_line_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_2x2) {
  test_line(1.0, 5, {1, 1}, {2, 2}, CM({{_,_,_,_,_},
                                        {_,R,_,_,_},
                                        {_,_,R,_,_},
                                        {_,_,_,_,_},
                                        {_,_,_,_,_}}));
}
END_TEST(test_draw_line_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_1x1) {
  test_line(1.0, 5, {1, 1}, {1, 1}, CM({{_,_,_,_,_},
                                        {_,R,_,_,_},
                                        {_,_,_,_,_},
                                        {_,_,_,_,_},
                                        {_,_,_,_,_}}));
}
END_TEST(test_draw_line_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_0x0) {
  test_line(1.0, 5, {1, 1}, {0, 0}, CM({{R,_,_,_,_},
                                        {_,R,_,_,_},
                                        {_,_,_,_,_},
                                        {_,_,_,_,_},
                                        {_,_,_,_,_}}));
}
END_TEST(test_draw_line_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_4x4_x2) {
  test_line(2.0, 10, {1, 1}, {4, 4}, CM({{_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,R,_,_,_,_,_,_},
                                         {_,_,R,R,R,_,_,_,_,_},
                                         {_,_,_,R,R,R,_,_,_,_},
                                         {_,_,_,_,R,R,R,_,_,_},
                                         {_,_,_,_,_,R,R,R,_,_},
                                         {_,_,_,_,_,_,R,R,R,_},
                                         {_,_,_,_,_,_,_,R,R,R},
                                         {_,_,_,_,_,_,_,_,R,R},
                                        }));
}
END_TEST(test_draw_line_4x4_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_3x3_x2) {
  test_line(2.0, 10, {1, 1}, {3, 3}, CM({{_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,R,_,_,_,_,_,_},
                                         {_,_,R,R,R,_,_,_,_,_},
                                         {_,_,_,R,R,R,_,_,_,_},
                                         {_,_,_,_,R,R,R,_,_,_},
                                         {_,_,_,_,_,R,R,R,_,_},
                                         {_,_,_,_,_,_,R,R,R,_},
                                         {_,_,_,_,_,_,_,R,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                        }));
}
END_TEST(test_draw_line_3x3_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_2x2_x2) {
  test_line(2.0, 10, {1, 1}, {2, 2}, CM({{_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,R,_,_,_,_,_,_},
                                         {_,_,R,R,R,_,_,_,_,_},
                                         {_,_,_,R,R,R,_,_,_,_},
                                         {_,_,_,_,R,R,R,_,_,_},
                                         {_,_,_,_,_,R,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                        }));
}
END_TEST(test_draw_line_2x2_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_1x1_x2) {
  test_line(2.0, 10, {1, 1}, {1, 1}, CM({{_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,R,R,_,_,_,_,_,_},
                                         {_,_,R,R,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                        }));
}
END_TEST(test_draw_line_1x1_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_0x0_x2) {
  test_line(2.0, 10, {1, 1}, {0, 0}, CM({{_,R,_,_,_,_,_,_,_,_},
                                         {R,R,R,_,_,_,_,_,_,_},
                                         {_,R,R,R,_,_,_,_,_,_},
                                         {_,_,R,R,R,_,_,_,_,_},
                                         {_,_,_,R,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                         {_,_,_,_,_,_,_,_,_,_},
                                        }));
}
END_TEST(test_draw_line_0x0_x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_4x4_x3) {
  test_line(3.0, 15, {1, 1}, {4, 4}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,_,_,_,_,_,_,_,_,_,_},
    {_,_,R,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,_,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,_,R,R,R,R,_,_},
    {_,_,_,_,_,_,_,_,_,_,R,R,R,R,_},
    {_,_,_,_,_,_,_,_,_,_,_,R,R,R,R},
    {_,_,_,_,_,_,_,_,_,_,_,_,R,R,R},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,_,_,R,R,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,R,_,_},
    {_,_,_,_,_,_,_,_,_,R,R,R,R,R,_},
    {_,_,_,_,_,_,_,_,_,_,R,R,R,R,R},
    {_,_,_,_,_,_,_,_,_,_,_,R,R,R,R},
    {_,_,_,_,_,_,_,_,_,_,_,_,R,R,R},
#endif // WIN32
  }));
}
END_TEST(test_draw_line_4x4_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_3x3_x3) {
  test_line(3.0, 15, {1, 1}, {3, 3}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,_,_,_,_,_,_,_,_,_,_},
    {_,_,R,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,_,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,R,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,_,_,R,R,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_}
#endif // WIN32
  }));
}
END_TEST(test_draw_line_3x3_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_2x2_x3) {
  test_line(3.0, 15, {1, 1}, {2, 2}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,_,_,_,_,_,_,_,_,_,_},
    {_,_,R,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,R,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#endif // WIN32
                                        }));
}
END_TEST(test_draw_line_2x2_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_1x1_x3) {
  test_line(3.0, 15, {1, 1}, {1, 1}, CM({
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
  }));
}
END_TEST(test_draw_line_1x1_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_0x0_x3) {
  test_line(3.0, 15, {1, 1}, {0, 0}, CM({
#ifdef WIN32
    {R,R,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {R,R,R,_,_,_,_,_,_,_,_,_,_,_,_},
    {R,R,R,R,_,_,_,_,_,_,_,_,_,_,_},
    {_,R,R,R,R,_,_,_,_,_,_,_,_,_,_},
    {_,_,R,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,R,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {R,R,R,_,_,_,_,_,_,_,_,_,_,_,_},
    {R,R,R,R,_,_,_,_,_,_,_,_,_,_,_},
    {R,R,R,R,R,_,_,_,_,_,_,_,_,_,_},
    {_,R,R,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,R,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#endif // WIN32
  }));
}
END_TEST(test_draw_line_0x0_x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_left) {
  test_line(1.0, 5, {2, 2}, {1, 1}, CM({{_,_,_,_,_},
                                        {_,R,_,_,_},
                                        {_,_,R,_,_},
                                        {_,_,_,_,_},
                                        {_,_,_,_,_}}));
}
END_TEST(test_draw_line_top_left)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_left_2) {
  test_line(1.0, 5, {2, 2}, {0, 0}, CM({{R,_,_,_,_},
                                        {_,R,_,_,_},
                                        {_,_,R,_,_},
                                        {_,_,_,_,_},
                                        {_,_,_,_,_}}));
}
END_TEST(test_draw_line_top_left_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_right) {
  test_line(1.0, 5, {2, 2}, {3, 1}, CM({{_,_,_,_,_},
                                        {_,_,_,R,_},
                                        {_,_,R,_,_},
                                        {_,_,_,_,_},
                                        {_,_,_,_,_}}));
}
END_TEST(test_draw_line_top_right)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_right_2) {
  test_line(1.0, 5, {2, 2}, {4, 0}, CM({{_,_,_,_,R},
                                        {_,_,_,R,_},
                                        {_,_,R,_,_},
                                        {_,_,_,_,_},
                                        {_,_,_,_,_}}));
}
END_TEST(test_draw_line_top_right_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_left) {
  test_line(1.0, 5, {2, 2}, {1, 3}, CM({{_,_,_,_,_},
                                        {_,_,_,_,_},
                                        {_,_,R,_,_},
                                        {_,R,_,_,_},
                                        {_,_,_,_,_}}));
}
END_TEST(test_draw_line_bottom_left)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_left_2) {
  test_line(1.0, 5, {2, 2}, {0, 4}, CM({{_,_,_,_,_},
                                        {_,_,_,_,_},
                                        {_,_,R,_,_},
                                        {_,R,_,_,_},
                                        {R,_,_,_,_}}));
}
END_TEST(test_draw_line_bottom_left_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_right) {
  test_line(1.0, 5, {2, 2}, {3, 3}, CM({
    {_,_,_,_,_},
    {_,_,_,_,_},
    {_,_,R,_,_},
    {_,_,_,R,_},
    {_,_,_,_,_}}));
}
END_TEST(test_draw_line_bottom_right)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_right_2) {
  test_line(1.0, 5, {2, 2}, {4, 4}, CM({
    {_,_,_,_,_},
    {_,_,_,_,_},
    {_,_,R,_,_},
    {_,_,_,R,_},
    {_,_,_,_,R}}));
}
END_TEST(test_draw_line_bottom_right_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_4x4) {
  tester<draw::ellipse>(1.0F, 5, {0, 0}, {4, 4}, CM({
    {_,R,R,R,_},
    {R,_,_,_,R},
    {R,_,_,_,R},
    {R,_,_,_,R},
    {_,R,R,R,_}}));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_3x3) {
  tester<draw::ellipse>(1.0F, 5, {0, 0}, {3, 3}, CM({
    {_,R,R,_,_},
    {R,_,_,R,_},
    {R,_,_,R,_},
    {_,R,R,_,_},
    {_,_,_,_,_}
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_2x2) {
  tester<draw::ellipse>(1.0F, 5, {0, 0}, {2, 2}, CM({
#ifdef WIN32
    {_,R,_,_,_},
    {R,_,R,_,_},
    {_,R,_,_,_},
    {_,_,_,_,_},
    {_,_,_,_,_}
#else
    {R,R,R,_,_},
    {R,_,R,_,_},
    {R,R,R,_,_},
    {_,_,_,_,_},
    {_,_,_,_,_},
#endif
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_1x1) {
  tester<draw::ellipse>(1.0F, 5, {0, 0}, {1, 1}, CM({
    {R,R,_,_,_},
    {R,R,_,_,_},
    {_,_,_,_,_},
    {_,_,_,_,_},
    {_,_,_,_,_}}));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_0x0) {
  tester<draw::ellipse>(1.0F, 5, {0, 0}, {0, 0}, CM({
    {R,_,_,_,_},
    {_,_,_,_,_},
    {_,_,_,_,_},
    {_,_,_,_,_},
    {_,_,_,_,_}}));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_4x4_x2) {
  tester<draw::ellipse>(2.0F, 14, {1, 1}, {5, 5}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,_,_,_,_,R,R,_,_,_},
    {_,_,R,R,_,_,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,_,_,R,R,_,_},
    {_,_,_,R,R,_,_,_,_,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,R,R,R,_,_,_,_,R,R,R,_,_},
    {_,_,R,R,_,_,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,_,_,R,R,_,_},
    {_,_,R,R,R,_,_,_,_,R,R,R,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#endif // WIN32
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_3x3_x2) {
  tester<draw::ellipse>(2.0F, 12, {1, 1}, {4, 4}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,R,R,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,R,R,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,R,R,R,R,R,R,R,R,_,_},
    {_,_,R,R,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,R,R,_,_},
    {_,_,R,R,_,_,_,_,R,R,_,_},
    {_,_,R,R,R,R,R,R,R,R,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
#endif // WIN32
                                                     }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_2x2_x2) {
  tester<draw::ellipse>(2.0F, 10, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,_,_,_},
    {_,_,R,R,R,R,R,R,_,_},
    {_,_,R,R,_,_,R,R,_,_},
    {_,_,R,R,_,_,R,R,_,_},
    {_,_,R,R,R,R,R,R,_,_},
    {_,_,_,R,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_}
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_1x1_x2) {
  tester<draw::ellipse>(2.0F, 8, {1, 1}, {2, 2}, CM({
    {_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_},
    {_,_,R,R,R,R,_,_},
    {_,_,R,R,R,R,_,_},
    {_,_,R,R,R,R,_,_},
    {_,_,R,R,R,R,_,_},
    {_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_},
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_0x0_x2) {
  tester<draw::ellipse>(2.0F, 6, {1, 1}, {1, 1}, CM({
    {_,_,_,_,_,_},
    {_,_,_,_,_,_},
    {_,_,R,R,_,_},
    {_,_,R,R,_,_},
    {_,_,_,_,_,_},
    {_,_,_,_,_,_}
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_4x4_x3) {
  tester<draw::ellipse>(3.0F, 21, {1, 1}, {5, 5}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,_,_,_,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,_,_,_,_,_,_,_,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,R,R,R,R,_,_,_,_,_,_,_,_,_,R,R,R,R,_,_},
    {_,_,R,R,R,_,_,_,_,_,_,_,_,_,_,_,R,R,R,_,_},
    {_,_,R,R,R,_,_,_,_,_,_,_,_,_,_,_,R,R,R,_,_},
    {_,_,R,R,R,_,_,_,_,_,_,_,_,_,_,_,R,R,R,_,_},
    {_,_,_,R,R,_,_,_,_,_,_,_,_,_,_,_,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,_,_,_,_,_,_,_,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,_,_,_,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,_,_,_,R,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,_,_,_,_,_,_,_,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,_,_,_,_,_,_,_,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,_,_,_,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#endif // WIN32
   }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_3x3_x3) {
  tester<draw::ellipse>(3.0F, 18, {1, 1}, {4, 4}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,R,_,_,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,R,R,R,_,_,_,_,_,_,_,_,R,R,R,_,_},
    {_,_,R,R,R,_,_,_,_,_,_,_,_,R,R,R,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,R,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,_,_,_,_,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,_,_,_,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,_,_,_,_,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,_,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#endif // WIN32
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_2x2_x3) {
  tester<draw::ellipse>(3.0F, 15, {1, 1}, {3, 3}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,_,_,R,R,R,_,_,_},
    {_,_,R,R,R,R,_,_,_,_,R,R,R,_,_},
    {_,_,R,R,R,_,_,_,_,_,R,R,R,_,_},
    {_,_,_,R,R,_,_,_,_,_,R,R,R,_,_},
    {_,_,_,R,R,R,R,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,_,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#endif // WIN32
   }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_1x1_x3) {
  tester<draw::ellipse>(3.0F, 12, {1, 1}, {2, 2}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,_,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,R,R,R,R,R,R,R,R,_,_},
    {_,_,R,R,R,_,_,R,R,R,_,_},
    {_,_,R,R,R,_,_,R,R,R,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
#endif // WIN32
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_0x0_x3) {
  tester<draw::ellipse>(3.0F, 9, {1, 1}, {1, 1}, CM({
    {_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_}
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_ellipse) {
  tester<draw::ellipse, draw::brush>(1.0F, 5, {0, 0}, {4, 4}, CM({
    {_,R,R,R,_},
    {R,R,R,R,R},
    {R,R,R,R,R},
    {R,R,R,R,R},
    {_,R,R,R,_}}));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_4x4) {
  tester2<draw::ellipse, color::red, color::blue>(1.0F, 5, {0, 0}, {4, 4}, CM({
    {_,R,R,R,_},
    {R,B,B,B,R},
    {R,B,B,B,R},
    {R,B,B,B,R},
    {_,R,R,R,_}
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_3x3) {
  tester2<draw::ellipse, color::red, color::blue>(1.0F, 5, {0, 0}, {3, 3}, CM({
    {_,R,R,_,_},
    {R,B,B,R,_},
    {R,B,B,R,_},
    {_,R,R,_,_},
    {_,_,_,_,_}
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_2x2) {
  tester2<draw::ellipse, color::red, color::blue>(1.0F, 5, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_},
    {_,R,R,R,_},
    {_,R,B,R,_},
    {_,R,R,R,_},
    {_,_,_,_,_}
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_1x1) {
  tester2<draw::ellipse, color::red, color::blue>(1.0F, 5, {1, 1}, {2, 2}, CM({
    {_,_,_,_,_},
    {_,R,R,_,_},
    {_,R,R,_,_},
    {_,_,_,_,_},
    {_,_,_,_,_}
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_0x0) {
  tester2<draw::ellipse, color::red, color::blue>(1.0F, 5, {1, 1}, {1, 1}, CM({
    {_,_,_,_,_},
    {_,R,_,_,_},
    {_,_,_,_,_},
    {_,_,_,_,_},
    {_,_,_,_,_}
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_4x4_2) {
  tester2<draw::ellipse, color::red, color::blue>(2.0F, 14, {1, 1}, {5, 5}, CM({
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,R,R,R,B,B,B,B,R,R,R,_,_},
    {_,_,R,R,B,B,B,B,B,B,R,R,_,_},
    {_,_,R,R,B,B,B,B,B,B,R,R,_,_},
    {_,_,R,R,B,B,B,B,B,B,R,R,_,_},
    {_,_,R,R,B,B,B,B,B,B,R,R,_,_},
    {_,_,R,R,R,B,B,B,B,R,R,R,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_},
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_3x3_2) {
  tester2<draw::ellipse, color::red, color::blue>(2.0F, 12, {1, 1}, {4, 4}, CM({
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,R,R,R,R,R,R,R,R,_,_},
    {_,_,R,R,B,B,B,B,R,R,_,_},
    {_,_,R,R,B,B,B,B,R,R,_,_},
    {_,_,R,R,B,B,B,B,R,R,_,_},
    {_,_,R,R,B,B,B,B,R,R,_,_},
    {_,_,R,R,R,R,R,R,R,R,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_2x2_2) {
  tester2<draw::ellipse, color::red, color::blue>(2.0F, 10, {1, 1}, {3, 3}, CM({
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,R,_,_,_},
    {_,_,R,R,R,R,R,R,_,_},
    {_,_,R,R,B,B,R,R,_,_},
    {_,_,R,R,B,B,R,R,_,_},
    {_,_,R,R,R,R,R,R,_,_},
    {_,_,_,R,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_}
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_1x1_2) {
  tester2<draw::ellipse, color::red, color::blue>(2.0F, 8, {1, 1}, {2, 2}, CM({
    {_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_},
    {_,_,R,R,R,R,_,_},
    {_,_,R,R,R,R,_,_},
    {_,_,R,R,R,R,_,_},
    {_,_,R,R,R,R,_,_},
    {_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_},
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_0x0_2) {
  tester2<draw::ellipse, color::red, color::blue>(2.0F, 6, {1, 1}, {1, 1}, CM({
    {_,_,_,_,_,_},
    {_,_,_,_,_,_},
    {_,_,R,R,_,_},
    {_,_,R,R,_,_},
    {_,_,_,_,_,_},
    {_,_,_,_,_,_},
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_4x4_3) {
  tester2<draw::ellipse, color::red, color::blue>(3.0F, 21, {1, 1}, {5, 5}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,B,B,B,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,B,B,B,B,B,B,B,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,B,B,B,B,B,B,B,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,B,B,B,B,B,B,B,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,B,B,B,B,B,B,B,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,B,B,B,B,B,B,B,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,B,B,B,B,B,B,B,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,B,B,B,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,B,B,B,B,B,R,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,B,B,B,B,B,B,B,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,B,B,B,B,B,B,B,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,R,B,B,B,B,B,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,R,R,R,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,R,R,R,R,R,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#endif // WIN32
                                                                               }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_3x3_3) {
  tester2<draw::ellipse, color::red, color::blue>(3.0F, 18, {1, 1}, {4, 4}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,R,B,B,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,B,B,B,B,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,B,B,B,B,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,B,B,R,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,_,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,B,B,B,B,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,B,B,B,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,B,B,B,B,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,_,R,R,R,R,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#endif // WIN32
                                                                               }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_2x2_3) {
  tester2<draw::ellipse, color::red, color::blue>(3.0F, 15, {1, 1}, {3, 3}, CM({
#ifdef WIN32
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,B,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,B,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,_,R,R,R,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#else
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,B,B,B,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,R,R,R,R,R,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},
#endif // WIN32
                                                                               }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_1x1_3) {
  tester2<draw::ellipse, color::red, color::blue>(3.0F, 12, {1, 1}, {2, 2}, CM({
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,R,R,R,R,R,R,_,_,_},
    {_,_,_,_,R,R,R,R,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_,_,_,_},
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_0x0_3) {
  tester2<draw::ellipse, color::red, color::blue>(3.0F, 9, {1, 1}, {1, 1}, CM({
    {_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_},
    {_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_},
    {_,_,_,R,R,R,_,_,_},
    {_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_},
    {_,_,_,_,_,_,_,_,_}
  }));
} END_TEST()

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_round_rectangle) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::round_rectangle(core::rectangle(0, 0, 5, 5), core::size(2, 2)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,R,R,R,_},
                           {R,_,_,_,R},
                           {R,_,_,_,R},
                           {R,_,_,_,R},
                           {_,R,R,R,_}}));

}
END_TEST(test_frame_round_rectangle)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_round_rectangle) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::round_rectangle(core::rectangle(0, 0, 5, 5), core::size(2, 2)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,R,R,R,_},
                           {R,R,R,R,R},
                           {R,R,R,R,R},
                           {R,R,R,R,R},
                           {_,R,R,R,_}}));

}
END_TEST(test_fill_round_rectangle)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_round_rectangle) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::round_rectangle(core::rectangle(0, 0, 5, 5), core::size(2, 2)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,R,R,R,_},
                           {R,B,B,B,R},
                           {R,B,B,B,R},
                           {R,B,B,B,R},
                           {_,R,R,R,_}}));

}
END_TEST(test_draw_round_rectangle)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_round_rectangle_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).frame(draw::round_rectangle(core::rectangle(0, 0, 7, 7), core::size(3, 3)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,R,R,R,_,_},
                           {_,R,_,_,_,R,_},
                           {R,_,_,_,_,_,R},
                           {R,_,_,_,_,_,R},
                           {R,_,_,_,_,_,R},
                           {_,R,_,_,_,R,_},
                           {_,_,R,R,R,_,_}}));

}
END_TEST(test_frame_round_rectangle_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_round_rectangle_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).fill(draw::round_rectangle(core::rectangle(0, 0, 7, 7), core::size(3, 3)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,R,R,R,_,_},
                           {_,R,R,R,R,R,_},
                           {R,R,R,R,R,R,R},
                           {R,R,R,R,R,R,R},
                           {R,R,R,R,R,R,R},
                           {_,R,R,R,R,R,_},
                           {_,_,R,R,R,_,_}}));

}
END_TEST(test_fill_round_rectangle_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_round_rectangle_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).draw(draw::round_rectangle(core::rectangle(0, 0, 7, 7), core::size(3, 3)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,R,R,R,_,_},
                           {_,R,B,B,B,R,_},
                           {R,B,B,B,B,B,R},
                           {R,B,B,B,B,B,R},
                           {R,B,B,B,B,B,R},
                           {_,R,B,B,B,R,_},
                           {_,_,R,R,R,_,_}}));

}
END_TEST(test_draw_round_rectangle_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 2, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,R,R,R,_},
                           {R,_,_,_,R},
                           {R,_,_,_,R},
                           {R,_,_,_,R},
                           {_,R,R,R,_}}));

} END_TEST(test_frame_arc_4x4)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_3x3) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 1.5, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,R,R,_},
                           {_,R,_,_,R},
                           {_,R,_,_,R},
                           {_,_,R,R,_},
                          }));

} END_TEST(test_frame_arc_3x3)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 1, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,R,_},
                           {_,R,_,R,_},
                           {_,R,R,R,_},
                           {_,_,_,_,_}}));

} END_TEST(test_frame_arc_2x2)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 0.5, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,R,R,_},
                           {_,_,R,R,_},
                           {_,_,_,_,_}}));

} END_TEST(test_frame_arc_1x1)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 0, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));

} END_TEST(test_frame_arc_0x0)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::arc(core::point(2, 2), 2, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,R,R,R,_},
                           {R,R,R,R,R},
                           {R,R,R,R,R},
                           {R,R,R,R,R},
                           {_,R,R,R,_}}));

} END_TEST(test_fill_arc_4x4)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_3x3) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::arc(core::point(2, 2), 1.5, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({
                           {_,_,_,_,_},
                           {_,_,R,R,_},
                           {_,R,R,R,R},
                           {_,R,R,R,R},
                           {_,_,R,R,_},
                          }));

} END_TEST(test_fill_arc_3x3)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::arc(core::point(2, 2), 1, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,R,_},
                           {_,R,R,R,_},
                           {_,R,R,R,_},
                           {_,_,_,_,_}}));

} END_TEST(test_fill_arc_2x2)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::arc(core::point(2, 2), 0.5, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,R,R,_},
                           {_,_,R,R,_},
                           {_,_,_,_,_}}));
} END_TEST(test_fill_arc_1x1)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::arc(core::point(2, 2), 0, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));
} END_TEST(test_fill_arc_0x0)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 2, 0, 360), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,R,R,R,_},
                           {R,B,B,B,R},
                           {R,B,B,B,R},
                           {R,B,B,B,R},
                           {_,R,R,R,_}}));
} END_TEST(test_draw_arc_4x4)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_3x3) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 1.5, 0, 360), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,R,R,_},
                           {_,R,B,B,R},
                           {_,R,B,B,R},
                           {_,_,R,R,_},
                          }));
} END_TEST(test_draw_arc_3x3)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 1, 0, 360), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,R,R,R,_},
                           {_,R,B,R,_},
                           {_,R,R,R,_},
                           {_,_,_,_,_}}));
} END_TEST(test_draw_arc_2x2)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 0.5, 0, 360), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,R,R,_},
                           {_,_,R,R,_},
                           {_,_,_,_,_}}));
} END_TEST(test_draw_arc_1x1)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 0, 0, 360), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));
} END_TEST(test_draw_arc_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_45_135) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 2, 45, 135), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,R,R,R,_},
                           {_,R,_,R,_},
                           {_,_,R,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));
} END_TEST(test_frame_arc_45_135)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_135_225) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 2, 135, 225), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {R,R,_,_,_},
                           {R,_,R,_,_},
                           {R,R,_,_,_},
                           {_,_,_,_,_}}));
} END_TEST(test_frame_arc_135_225)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_225_315) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 2, 225, 315), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,R,_,_},
                           {_,R,_,R,_},
                           {_,R,R,R,_}}));
} END_TEST(test_frame_arc_225_315)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_315_405) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 2, 315, 405), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,R,R},
                           {_,_,R,_,R},
                           {_,_,_,R,R},
                           {_,_,_,_,_}}));
} END_TEST(test_frame_arc_315_405)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_45_135) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 2, 45, 135), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,R,R,R,_},
                           {_,R,B,R,_},
                           {_,_,R,_,_},
                           {_,_,_,_,_},
                           {_,_,_,_,_}}));
} END_TEST(test_draw_arc_45_135)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_135_225) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 2, 135, 225), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {R,R,_,_,_},
                           {R,B,R,_,_},
                           {R,R,_,_,_},
                           {_,_,_,_,_}}));
} END_TEST(test_draw_arc_135_225)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_225_315) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 2, 225, 315), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,_,_},
                           {_,_,R,_,_},
                           {_,R,B,R,_},
                           {_,R,R,R,_}}));
} END_TEST(test_draw_arc_225_315)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_315_405) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 2, 315, 405), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,_,R,R},
                           {_,_,R,B,R},
                           {_,_,_,R,R},
                           {_,_,_,_,_}}));
} END_TEST(test_draw_arc_315_405)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_45_135_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 3), 3, 45, 135), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,R,R,R,_},
                           {R,_,_,_,R},
                           {_,R,_,R,_},
                           {_,_,R,_,_},
                           {_,_,_,_,_}}));
} END_TEST(test_frame_arc_45_135_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_135_225_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(3, 2), 3, 135, 225), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,R,_,_,_},
                           {R,_,R,_,_},
                           {R,_,_,R,_},
                           {R,_,R,_,_},
                           {_,R,_,_,_}}));
} END_TEST(test_frame_arc_135_225_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_225_315_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 1), 3, 225, 315), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_},
                           {_,_,R,_,_},
                           {_,R,_,R,_},
                           {R,_,_,_,R},
                           {_,R,R,R,_}}));
} END_TEST(test_frame_arc_225_315_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_315_405_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(1, 2), 3, 315, 405), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,R,_},
                           {_,_,R,_,R},
                           {_,R,_,_,R},
                           {_,_,R,_,R},
                           {_,_,_,R,_}}));
} END_TEST(test_frame_arc_315_405_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_polygon) {
  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).frame(draw::polygon({{1,1}, {1, 5}, {3,3}, {5,5}, {5,1}}), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_},
                           {_,R,R,R,R,R,_},
                           {_,R,_,_,_,R,_},
                           {_,R,_,R,_,R,_},
                           {_,R,R,_,R,R,_},
                           {_,R,_,_,_,R,_},
                           {_,_,_,_,_,_,_}}));

}
END_TEST(test_frame_polygon)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_polygon) {
  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).fill(draw::polygon({{1,1}, {1, 5}, {3,3}, {5,5}, {5,1}}), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_},
                           {_,R,R,R,R,R,_},
                           {_,R,R,R,R,R,_},
                           {_,R,R,R,R,R,_},
                           {_,R,R,_,R,R,_},
                           {_,R,_,_,_,R,_},
                           {_,_,_,_,_,_,_}}));

}
END_TEST(test_fill_polygon)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_polygon) {
  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).draw(draw::polygon({{1,1}, {1, 5}, {3,3}, {5,5}, {5,1}}), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_,_,_,_,_,_,_},
                           {_,R,R,R,R,R,_},
                           {_,R,B,B,B,R,_},
                           {_,R,B,R,B,R,_},
                           {_,R,R,_,R,R,_},
                           {_,R,_,_,_,R,_},
                           {_,_,_,_,_,_,_}}));

}
END_TEST(test_draw_polygon)

// --------------------------------------------------------------------------
