
#include <iomanip>

#include <gui/draw/bitmap.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <testlib/image_test_lib.h>
#include <testlib/testlib.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;
using namespace testing;

// --------------------------------------------------------------------------
DECLARE_TEST(test_data2colormap);
DECLARE_TEST(test_draw_line_4x4);
DECLARE_TEST(test_draw_line_3x3);
DECLARE_TEST(test_draw_line_2x2);
DECLARE_TEST(test_draw_line_1x1);
DECLARE_TEST(test_draw_line_0x0);
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
DECLARE_TEST(test_fill_rect_4x4);
DECLARE_TEST(test_fill_rect_3x3);
DECLARE_TEST(test_fill_rect_2x2);
DECLARE_TEST(test_fill_rect_1x1);
DECLARE_TEST(test_fill_rect_0x0);
DECLARE_TEST(test_draw_rect_4x4);
DECLARE_TEST(test_draw_rect_3x3);
DECLARE_TEST(test_draw_rect_2x2);
DECLARE_TEST(test_draw_rect_1x1);
DECLARE_TEST(test_draw_rect_0x0);

DECLARE_TEST(test_frame_rect_4x4_scaled);
DECLARE_TEST(test_frame_rect_3x3_scaled);
DECLARE_TEST(test_frame_rect_2x2_scaled);
DECLARE_TEST(test_frame_rect_1x1_scaled);
DECLARE_TEST(test_frame_rect_0x0_scaled);

DECLARE_TEST(test_frame_ellipse_4x4);
DECLARE_TEST(test_frame_ellipse_3x3);
DECLARE_TEST(test_frame_ellipse_2x2);
DECLARE_TEST(test_frame_ellipse_1x1);
DECLARE_TEST(test_frame_ellipse_0x0);
DECLARE_TEST(test_fill_ellipse);
DECLARE_TEST(test_draw_ellipse_4x4);
DECLARE_TEST(test_draw_ellipse_2x2);
DECLARE_TEST(test_draw_ellipse_1x1);
DECLARE_TEST(test_draw_ellipse_0x0);

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

#define TEST_LINE
#define TEST_RECT
#define TEST_ELLIPSE
#define TEST_ROUND_RECT
#define TEST_ARC
#define TEST_POLYGON

// --------------------------------------------------------------------------
TEST_MAIN(drawer_test) {
  RUN_TEST(test_data2colormap);
#ifdef TEST_LINE
  RUN_TEST(test_draw_line_4x4);
  RUN_TEST(test_draw_line_3x3);
  RUN_TEST(test_draw_line_2x2);
  RUN_TEST(test_draw_line_1x1);
  RUN_TEST(test_draw_line_0x0);
  RUN_TEST(test_draw_line_top_left);
  RUN_TEST(test_draw_line_top_left_2);
  RUN_TEST(test_draw_line_top_right);
  RUN_TEST(test_draw_line_top_right_2);
  RUN_TEST(test_draw_line_bottom_left);
  RUN_TEST(test_draw_line_bottom_left_2);
  RUN_TEST(test_draw_line_bottom_right);
  RUN_TEST(test_draw_line_bottom_right_2);
#endif //TEST_LINE

#ifdef TEST_RECT
  RUN_TEST(test_frame_rect_4x4);
  RUN_TEST(test_frame_rect_3x3);
  RUN_TEST(test_frame_rect_2x2);
  RUN_TEST(test_frame_rect_1x1);
  RUN_TEST(test_frame_rect_0x0);
  RUN_TEST(test_frame_rect_4x4_scaled);
  RUN_TEST(test_frame_rect_3x3_scaled);
  RUN_TEST(test_frame_rect_2x2_scaled);
  RUN_TEST(test_frame_rect_1x1_scaled);
  RUN_TEST(test_frame_rect_0x0_scaled);
  RUN_TEST(test_fill_rect_4x4);
  RUN_TEST(test_fill_rect_3x3);
  RUN_TEST(test_fill_rect_2x2);
  RUN_TEST(test_fill_rect_1x1);
  RUN_TEST(test_fill_rect_0x0);
  RUN_TEST(test_draw_rect_4x4);
  RUN_TEST(test_draw_rect_3x3);
  RUN_TEST(test_draw_rect_2x2);
  RUN_TEST(test_draw_rect_1x1);
  RUN_TEST(test_draw_rect_0x0);
#endif //TEST_RECT

#ifdef TEST_ELLIPSE
  RUN_TEST(test_frame_ellipse_4x4);
  RUN_TEST(test_frame_ellipse_3x3);
  RUN_TEST(test_frame_ellipse_2x2);
  RUN_TEST(test_frame_ellipse_1x1);
  RUN_TEST(test_frame_ellipse_0x0);
  RUN_TEST(test_fill_ellipse);
  RUN_TEST(test_draw_ellipse_4x4);
  RUN_TEST(test_draw_ellipse_2x2);
  RUN_TEST(test_draw_ellipse_1x1);
  RUN_TEST(test_draw_ellipse_0x0);
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
DEFINE_TEST(test_frame_rect_4x4) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, R, R},
                           {_, R, _, _, R},
                           {_, R, _, _, R},
                           {_, R, R, R, R}}));
}
END_TEST(test_frame_rect_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_3x3) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, R, _},
                           {_, R, _, R, _},
                           {_, R, R, R, _},
                           {_, _, _, _, _}}));
}
END_TEST(test_frame_rect_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_2x2) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, _, _},
                           {_, R, R, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
}
END_TEST(test_frame_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_1x1) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
}
END_TEST(test_frame_rect_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_0x0) {
  core::global::set_scale_factor(1.0);
  pixmap img(5, 5);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
}
END_TEST(test_frame_rect_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_4x4_scaled) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, R, R, R, R, R, R, R, R},
                            {_, _, R, _, _, _, _, _, _, R},
                            {_, _, R, _, _, _, _, _, _, R},
                            {_, _, R, _, _, _, _, _, _, R},
                            {_, _, R, _, _, _, _, _, _, R},
                            {_, _, R, _, _, _, _, _, _, R},
                            {_, _, R, _, _, _, _, _, _, R},
                            {_, _, R, R, R, R, R, R, R, R},
                          }));
}
END_TEST(test_frame_rect_4x4_scaled)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_3x3_scaled) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, R, R, R, R, R, R, _, _},
                            {_, _, R, _, _, _, _, R, _, _},
                            {_, _, R, _, _, _, _, R, _, _},
                            {_, _, R, _, _, _, _, R, _, _},
                            {_, _, R, _, _, _, _, R, _, _},
                            {_, _, R, R, R, R, R, R, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                          }));
}
END_TEST(test_frame_rect_3x3_scaled)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_2x2_scaled) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, R, R, R, R, _, _, _, _},
                            {_, _, R, _, _, R, _, _, _, _},
                            {_, _, R, _, _, R, _, _, _, _},
                            {_, _, R, R, R, R, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                          }));
}
END_TEST(test_frame_rect_2x2_scaled)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_1x1_scaled) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, R, R, _, _, _, _, _, _},
                            {_, _, R, R, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                          }));
}
END_TEST(test_frame_rect_1x1_scaled)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_0x0_scaled) {
  core::global::set_scale_factor(2.0);
  pixmap img(10, 10);
  graphics(img).clear(color::black).frame(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::blue);
  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                            {_, _, _, _, _, _, _, _, _, _},
                          }));
}
END_TEST(test_frame_rect_0x0_scaled)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_4x4) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, R, R},
                           {_, R, R, R, R},
                           {_, R, R, R, R},
                           {_, R, R, R, R}}));

}
END_TEST(test_fill_rect_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_3x3) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, R, _},
                           {_, R, R, R, _},
                           {_, R, R, R, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_fill_rect_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_2x2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, _, _},
                           {_, R, R, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_fill_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_1x1) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_fill_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_0x0) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_fill_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_4x4) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(4, 4)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, R, R},
                           {_, R, B, B, R},
                           {_, R, B, B, R},
                           {_, R, R, R, R}}));

}
END_TEST(test_draw_rect_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_3x3) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(3, 3)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, R, _},
                           {_, R, B, R, _},
                           {_, R, R, R, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_rect_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_2x2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(2, 2)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, _, _},
                           {_, R, R, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_1x1) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(1, 1)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_rect_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_0x0) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::rectangle(core::point(1, 1), core::size(0, 0)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_rect_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_4x4) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(1, 1), core::point(4, 4)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, _, _, _},
                           {_, _, R, _, _},
                           {_, _, _, R, _},
                           {_, _, _, _, R}}));

}
END_TEST(test_draw_line_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_3x3) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(1, 1), core::point(3, 3)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, _, _, _},
                           {_, _, R, _, _},
                           {_, _, _, R, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_2x2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(1, 1), core::point(2, 2)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, _, _, _},
                           {_, _, R, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_1x1) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(1, 1), core::point(1, 1)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_0x0) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(1, 1), core::point(0, 0)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{R, _, _, _, _},
                           {_, R, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_left) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(2, 2), core::point(1, 1)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, _, _, _},
                           {_, _, R, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_top_left)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_left_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(2, 2), core::point(0, 0)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{R, _, _, _, _},
                           {_, R, _, _, _},
                           {_, _, R, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_top_left_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_right) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(2, 2), core::point(3, 1)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, R, _},
                           {_, _, R, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_top_right)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_right_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(2, 2), core::point(4, 0)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, R},
                           {_, _, _, R, _},
                           {_, _, R, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_top_right_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_left) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(2, 2), core::point(1, 3)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, R, _, _},
                           {_, R, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_bottom_left)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_left_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(2, 2), core::point(0, 4)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, R, _, _},
                           {_, R, _, _, _},
                           {R, _, _, _, _}}));

}
END_TEST(test_draw_line_bottom_left_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_right) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(2, 2), core::point(3, 3)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, R, _, _},
                           {_, _, _, R, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_bottom_right)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_right_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::line(core::point(2, 2), core::point(4, 4)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, R, _, _},
                           {_, _, _, R, _},
                           {_, _, _, _, R}}));

}
END_TEST(test_draw_line_bottom_right_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::ellipse(core::point(0, 0), core::point(4, 4)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {R, _, _, _, R},
                           {R, _, _, _, R},
                           {R, _, _, _, R},
                           {_, R, R, R, _}}));
} END_TEST(test_frame_ellipse_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_3x3) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::ellipse(core::point(0, 0), core::point(3, 3)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, _, _},
                           {R, _, _, R, _},
                           {R, _, _, R, _},
                           {_, R, R, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_ellipse_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::ellipse(core::point(0, 0), core::point(2, 2)), color::blue);

  auto buffer = pixmap2colormap(img);
  EXPECT_EQUAL(buffer, CM({{R, R, R, _, _},
                           {R, _, R, _, _},
                           {R, R, R, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

} END_TEST(test_frame_ellipse_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::ellipse(core::point(0, 0), core::point(1, 1)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{R, R, _, _, _},
                           {R, R, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_ellipse_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::ellipse(core::point(0, 0), core::point(0, 0)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_ellipse_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_ellipse) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::ellipse(core::point(0, 0), core::point(4, 4)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {R, R, R, R, R},
                           {R, R, R, R, R},
                           {R, R, R, R, R},
                           {_, R, R, R, _}}));
} END_TEST(test_fill_ellipse)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::ellipse(core::point(0, 0), core::point(4, 4)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {R, B, B, B, R},
                           {R, B, B, B, R},
                           {R, B, B, B, R},
                           {_, R, R, R, _}}));
} END_TEST(test_draw_ellipse_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::ellipse(core::point(0, 0), core::point(2, 2)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({ {R, R, R, _, _},
                            {R, B, R, _, _},
                            {R, R, R, _, _},
                            {_, _, _, _, _},
                            {_, _, _, _, _}}));
} END_TEST(test_draw_ellipse_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::ellipse(core::point(0, 0), core::point(1, 1)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{R, R, _, _, _},
                           {R, R, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_ellipse_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::ellipse(core::point(0, 0), core::point(0, 0)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_ellipse_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_round_rectangle) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::round_rectangle(core::rectangle(0, 0, 5, 5), core::size(2, 2)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {R, _, _, _, R},
                           {R, _, _, _, R},
                           {R, _, _, _, R},
                           {_, R, R, R, _}}));

}
END_TEST(test_frame_round_rectangle)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_round_rectangle) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::round_rectangle(core::rectangle(0, 0, 5, 5), core::size(2, 2)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {R, R, R, R, R},
                           {R, R, R, R, R},
                           {R, R, R, R, R},
                           {_, R, R, R, _}}));

}
END_TEST(test_fill_round_rectangle)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_round_rectangle) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::round_rectangle(core::rectangle(0, 0, 5, 5), core::size(2, 2)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {R, B, B, B, R},
                           {R, B, B, B, R},
                           {R, B, B, B, R},
                           {_, R, R, R, _}}));

}
END_TEST(test_draw_round_rectangle)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_round_rectangle_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).frame(draw::round_rectangle(core::rectangle(0, 0, 7, 7), core::size(3, 3)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, R, R, R, _, _},
                           {_, R, _, _, _, R, _},
                           {R, _, _, _, _, _, R},
                           {R, _, _, _, _, _, R},
                           {R, _, _, _, _, _, R},
                           {_, R, _, _, _, R, _},
                           {_, _, R, R, R, _, _}}));

}
END_TEST(test_frame_round_rectangle_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_round_rectangle_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).fill(draw::round_rectangle(core::rectangle(0, 0, 7, 7), core::size(3, 3)), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, R, R, R, _, _},
                           {_, R, R, R, R, R, _},
                           {R, R, R, R, R, R, R},
                           {R, R, R, R, R, R, R},
                           {R, R, R, R, R, R, R},
                           {_, R, R, R, R, R, _},
                           {_, _, R, R, R, _, _}}));

}
END_TEST(test_fill_round_rectangle_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_round_rectangle_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).draw(draw::round_rectangle(core::rectangle(0, 0, 7, 7), core::size(3, 3)), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, R, R, R, _, _},
                           {_, R, B, B, B, R, _},
                           {R, B, B, B, B, B, R},
                           {R, B, B, B, B, B, R},
                           {R, B, B, B, B, B, R},
                           {_, R, B, B, B, R, _},
                           {_, _, R, R, R, _, _}}));

}
END_TEST(test_draw_round_rectangle_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 2, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {R, _, _, _, R},
                           {R, _, _, _, R},
                           {R, _, _, _, R},
                           {_, R, R, R, _}}));

} END_TEST(test_frame_arc_4x4)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_3x3) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 1.5, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({
                           {_, _, _, _, _},
                           {_, _, R, R, _},
                           {_, R, _, _, R},
                           {_, R, _, _, R},
                           {_, _, R, R, _},
                          }));

} END_TEST(test_frame_arc_3x3)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 1, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, R, _},
                           {_, R, _, R, _},
                           {_, R, R, R, _},
                           {_, _, _, _, _}}));

} END_TEST(test_frame_arc_2x2)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 0.5, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, R, R, _},
                           {_, _, R, R, _},
                           {_, _, _, _, _}}));

} END_TEST(test_frame_arc_1x1)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 0, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

} END_TEST(test_frame_arc_0x0)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::arc(core::point(2, 2), 2, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {R, R, R, R, R},
                           {R, R, R, R, R},
                           {R, R, R, R, R},
                           {_, R, R, R, _}}));

} END_TEST(test_fill_arc_4x4)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_3x3) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::arc(core::point(2, 2), 1.5, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({
                           {_, _, _, _, _},
                           {_, _, R, R, _},
                           {_, R, R, R, R},
                           {_, R, R, R, R},
                           {_, _, R, R, _},
                          }));

} END_TEST(test_fill_arc_3x3)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::arc(core::point(2, 2), 1, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, R, _},
                           {_, R, R, R, _},
                           {_, R, R, R, _},
                           {_, _, _, _, _}}));

} END_TEST(test_fill_arc_2x2)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::arc(core::point(2, 2), 0.5, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, R, R, _},
                           {_, _, R, R, _},
                           {_, _, _, _, _}}));
} END_TEST(test_fill_arc_1x1)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).fill(draw::arc(core::point(2, 2), 0, 0, 360), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_fill_arc_0x0)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 2, 0, 360), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {R, B, B, B, R},
                           {R, B, B, B, R},
                           {R, B, B, B, R},
                           {_, R, R, R, _}}));
} END_TEST(test_draw_arc_4x4)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_3x3) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 1.5, 0, 360), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({
                           {_, _, _, _, _},
                           {_, _, R, R, _},
                           {_, R, B, B, R},
                           {_, R, B, B, R},
                           {_, _, R, R, _},
                          }));
} END_TEST(test_draw_arc_3x3)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 1, 0, 360), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, R, R, R, _},
                           {_, R, B, R, _},
                           {_, R, R, R, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_2x2)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 0.5, 0, 360), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, R, R, _},
                           {_, _, R, R, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_1x1)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 0, 0, 360), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_45_135) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 2, 45, 135), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {_, R, _, R, _},
                           {_, _, R, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_arc_45_135)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_135_225) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 2, 135, 225), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {R, R, _, _, _},
                           {R, _, R, _, _},
                           {R, R, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_arc_135_225)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_225_315) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 2, 225, 315), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, R, _, _},
                           {_, R, _, R, _},
                           {_, R, R, R, _}}));
} END_TEST(test_frame_arc_225_315)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_315_405) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 2), 2, 315, 405), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, R, R},
                           {_, _, R, _, R},
                           {_, _, _, R, R},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_arc_315_405)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_45_135) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 2, 45, 135), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {_, R, B, R, _},
                           {_, _, R, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_45_135)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_135_225) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 2, 135, 225), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {R, R, _, _, _},
                           {R, B, R, _, _},
                           {R, R, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_135_225)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_225_315) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 2, 225, 315), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, R, _, _},
                           {_, R, B, R, _},
                           {_, R, R, R, _}}));
} END_TEST(test_draw_arc_225_315)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_315_405) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).draw(draw::arc(core::point(2, 2), 2, 315, 405), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, _, R, R},
                           {_, _, R, B, R},
                           {_, _, _, R, R},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_315_405)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_45_135_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 3), 3, 45, 135), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, R, R, _},
                           {R, _, _, _, R},
                           {_, R, _, R, _},
                           {_, _, R, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_arc_45_135_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_135_225_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(3, 2), 3, 135, 225), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, R, _, _, _},
                           {R, _, R, _, _},
                           {R, _, _, R, _},
                           {R, _, R, _, _},
                           {_, R, _, _, _}}));
} END_TEST(test_frame_arc_135_225_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_225_315_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(2, 1), 3, 225, 315), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _},
                           {_, _, R, _, _},
                           {_, R, _, R, _},
                           {R, _, _, _, R},
                           {_, R, R, R, _}}));
} END_TEST(test_frame_arc_225_315_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_315_405_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(color::black).frame(draw::arc(core::point(1, 2), 3, 315, 405), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, R, _},
                           {_, _, R, _, R},
                           {_, R, _, _, R},
                           {_, _, R, _, R},
                           {_, _, _, R, _}}));
} END_TEST(test_frame_arc_315_405_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_polygon) {
  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).frame(draw::polygon({{1,1}, {1, 5}, {3,3}, {5,5}, {5,1}}), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _, _, _},
                           {_, R, R, R, R, R, _},
                           {_, R, _, _, _, R, _},
                           {_, R, _, R, _, R, _},
                           {_, R, R, _, R, R, _},
                           {_, R, _, _, _, R, _},
                           {_, _, _, _, _, _, _}}));

}
END_TEST(test_frame_polygon)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_polygon) {
  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).fill(draw::polygon({{1,1}, {1, 5}, {3,3}, {5,5}, {5,1}}), color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _, _, _},
                           {_, R, R, R, R, R, _},
                           {_, R, R, R, R, R, _},
                           {_, R, R, R, R, R, _},
                           {_, R, R, _, R, R, _},
                           {_, R, _, _, _, R, _},
                           {_, _, _, _, _, _, _}}));

}
END_TEST(test_fill_polygon)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_polygon) {
  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(color::black).draw(draw::polygon({{1,1}, {1, 5}, {3,3}, {5,5}, {5,1}}), color::red, color::blue);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, CM({{_, _, _, _, _, _, _},
                           {_, R, R, R, R, R, _},
                           {_, R, B, B, B, R, _},
                           {_, R, B, R, B, R, _},
                           {_, R, R, _, R, R, _},
                           {_, R, _, _, _, R, _},
                           {_, _, _, _, _, _, _}}));

}
END_TEST(test_draw_polygon)

// --------------------------------------------------------------------------
