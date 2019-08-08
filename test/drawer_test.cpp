
#include <iomanip>

#include <gui/draw/bitmap.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <testlib/testlib.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;

// --------------------------------------------------------------------------
typedef std::vector<uint32_t> colorline;
typedef std::vector<colorline> colormap;
colormap pixmap2colormap (const pixmap&);
std::ostream& operator<< (std::ostream&, const colormap&);

inline colormap MM(std::initializer_list<colorline> i) {
  return colormap(i);
}

// --------------------------------------------------------------------------

typedef std::vector<std::string> pixmap_str;
pixmap_str pixmap2string (const pixmap&);
pixmap_str mk_str(std::initializer_list<std::string>);
std::ostream& operator<< (std::ostream&, const pixmap_str&);

// --------------------------------------------------------------------------
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
constexpr os::color _ = color::rgba_gray<0, 255>::value;
constexpr os::color X = color::rgba<0, 0, 255, 255>::value;
constexpr os::color O = color::rgba<255, 0, 0, 255>::value;

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_4x4) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::rectangle(core::point(1, 1), core::size(4, 4)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, X, X},
                           {_, X, _, _, X},
                           {_, X, _, _, X},
                           {_, X, X, X, X}}));

}
END_TEST(test_frame_rect_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_3x3) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::rectangle(core::point(1, 1), core::size(3, 3)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, X, _},
                           {_, X, _, X, _},
                           {_, X, X, X, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_frame_rect_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_2x2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::rectangle(core::point(1, 1), core::size(2, 2)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, _, _},
                           {_, X, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_frame_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_1x1) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::rectangle(core::point(1, 1), core::size(1, 1)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_frame_rect_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_rect_0x0) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::rectangle(core::point(1, 1), core::size(0, 0)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_frame_rect_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_4x4) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::rectangle(core::point(1, 1), core::size(4, 4)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, X, X},
                           {_, X, X, X, X},
                           {_, X, X, X, X},
                           {_, X, X, X, X}}));

}
END_TEST(test_fill_rect_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_3x3) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::rectangle(core::point(1, 1), core::size(3, 3)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, X, _},
                           {_, X, X, X, _},
                           {_, X, X, X, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_fill_rect_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_2x2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::rectangle(core::point(1, 1), core::size(2, 2)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, _, _},
                           {_, X, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_fill_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_1x1) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::rectangle(core::point(1, 1), core::size(1, 1)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_fill_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_rect_0x0) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::rectangle(core::point(1, 1), core::size(0, 0)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
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

  graphics(img).clear(_).draw(draw::rectangle(core::point(1, 1), core::size(4, 4)), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, X, X},
                           {_, X, O, O, X},
                           {_, X, O, O, X},
                           {_, X, X, X, X}}));

}
END_TEST(test_draw_rect_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_3x3) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::rectangle(core::point(1, 1), core::size(3, 3)), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, X, _},
                           {_, X, O, X, _},
                           {_, X, X, X, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_rect_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_2x2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::rectangle(core::point(1, 1), core::size(2, 2)), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, _, _},
                           {_, X, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_rect_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_1x1) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::rectangle(core::point(1, 1), core::size(1, 1)), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_rect_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_rect_0x0) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::rectangle(core::point(1, 1), core::size(0, 0)), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
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

  graphics(img).clear(_).frame(draw::line(core::point(1, 1), core::point(4, 4)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, _, _, _},
                           {_, _, X, _, _},
                           {_, _, _, X, _},
                           {_, _, _, _, X}}));

}
END_TEST(test_draw_line_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_3x3) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(1, 1), core::point(3, 3)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, _, _, _},
                           {_, _, X, _, _},
                           {_, _, _, X, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_2x2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(1, 1), core::point(2, 2)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, _, _, _},
                           {_, _, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_1x1) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(1, 1), core::point(1, 1)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_0x0) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(1, 1), core::point(0, 0)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{X, _, _, _, _},
                           {_, X, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_left) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(2, 2), core::point(1, 1)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, _, _, _},
                           {_, _, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_top_left)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_left_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(2, 2), core::point(0, 0)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{X, _, _, _, _},
                           {_, X, _, _, _},
                           {_, _, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_top_left_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_right) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(2, 2), core::point(3, 1)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, X, _},
                           {_, _, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_top_right)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_top_right_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(2, 2), core::point(4, 0)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, X},
                           {_, _, _, X, _},
                           {_, _, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_top_right_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_left) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(2, 2), core::point(1, 3)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, X, _, _},
                           {_, X, _, _, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_bottom_left)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_left_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(2, 2), core::point(0, 4)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, X, _, _},
                           {_, X, _, _, _},
                           {X, _, _, _, _}}));

}
END_TEST(test_draw_line_bottom_left_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_right) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(2, 2), core::point(3, 3)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, X, _, _},
                           {_, _, _, X, _},
                           {_, _, _, _, _}}));

}
END_TEST(test_draw_line_bottom_right)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_line_bottom_right_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::line(core::point(2, 2), core::point(4, 4)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, X, _, _},
                           {_, _, _, X, _},
                           {_, _, _, _, X}}));

}
END_TEST(test_draw_line_bottom_right_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::ellipse(core::point(0, 0), core::point(4, 4)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {X, _, _, _, X},
                           {X, _, _, _, X},
                           {X, _, _, _, X},
                           {_, X, X, X, _}}));
} END_TEST(test_frame_ellipse_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_3x3) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::ellipse(core::point(0, 0), core::point(3, 3)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, _, _},
                           {X, _, _, X, _},
                           {X, _, _, X, _},
                           {_, X, X, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_ellipse_3x3)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::ellipse(core::point(0, 0), core::point(2, 2)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{X, X, X, _, _},
                           {X, _, X, _, _},
                           {X, X, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_ellipse_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::ellipse(core::point(0, 0), core::point(1, 1)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{X, X, _, _, _},
                           {X, X, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_ellipse_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_ellipse_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::ellipse(core::point(0, 0), core::point(0, 0)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_ellipse_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_ellipse) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::ellipse(core::point(0, 0), core::point(4, 4)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {X, X, X, X, X},
                           {X, X, X, X, X},
                           {X, X, X, X, X},
                           {_, X, X, X, _}}));
} END_TEST(test_fill_ellipse)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::ellipse(core::point(0, 0), core::point(4, 4)), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {X, O, O, O, X},
                           {X, O, O, O, X},
                           {X, O, O, O, X},
                           {_, X, X, X, _}}));
} END_TEST(test_draw_ellipse_4x4)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::ellipse(core::point(0, 0), core::point(2, 2)), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{X, X, X, _, _},
                           {X, O, X, _, _},
                           {X, X, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_ellipse_2x2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::ellipse(core::point(0, 0), core::point(1, 1)), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{X, X, _, _, _},
                           {X, X, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_ellipse_1x1)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_ellipse_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::ellipse(core::point(0, 0), core::point(0, 0)), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_ellipse_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_round_rectangle) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::round_rectangle(core::rectangle(0, 0, 5, 5), core::size(2, 2)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {X, _, _, _, X},
                           {X, _, _, _, X},
                           {X, _, _, _, X},
                           {_, X, X, X, _}}));

}
END_TEST(test_frame_round_rectangle)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_round_rectangle) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::round_rectangle(core::rectangle(0, 0, 5, 5), core::size(2, 2)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {X, X, X, X, X},
                           {X, X, X, X, X},
                           {X, X, X, X, X},
                           {_, X, X, X, _}}));

}
END_TEST(test_fill_round_rectangle)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_round_rectangle) {

  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::round_rectangle(core::rectangle(0, 0, 5, 5), core::size(2, 2)), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {X, O, O, O, X},
                           {X, O, O, O, X},
                           {X, O, O, O, X},
                           {_, X, X, X, _}}));

}
END_TEST(test_draw_round_rectangle)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_round_rectangle_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(_).frame(draw::round_rectangle(core::rectangle(0, 0, 7, 7), core::size(3, 3)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, X, X, X, _, _},
                           {_, X, _, _, _, X, _},
                           {X, _, _, _, _, _, X},
                           {X, _, _, _, _, _, X},
                           {X, _, _, _, _, _, X},
                           {_, X, _, _, _, X, _},
                           {_, _, X, X, X, _, _}}));

}
END_TEST(test_frame_round_rectangle_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_round_rectangle_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(_).fill(draw::round_rectangle(core::rectangle(0, 0, 7, 7), core::size(3, 3)), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, X, X, X, _, _},
                           {_, X, X, X, X, X, _},
                           {X, X, X, X, X, X, X},
                           {X, X, X, X, X, X, X},
                           {X, X, X, X, X, X, X},
                           {_, X, X, X, X, X, _},
                           {_, _, X, X, X, _, _}}));

}
END_TEST(test_fill_round_rectangle_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_round_rectangle_2) {

  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(_).draw(draw::round_rectangle(core::rectangle(0, 0, 7, 7), core::size(3, 3)), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, X, X, X, _, _},
                           {_, X, O, O, O, X, _},
                           {X, O, O, O, O, O, X},
                           {X, O, O, O, O, O, X},
                           {X, O, O, O, O, O, X},
                           {_, X, O, O, O, X, _},
                           {_, _, X, X, X, _, _}}));

}
END_TEST(test_draw_round_rectangle_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(2, 2), 2, 0, 360), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {X, _, _, _, X},
                           {X, _, _, _, X},
                           {X, _, _, _, X},
                           {_, X, X, X, _}}));

} END_TEST(test_frame_arc_4x4)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_3x3) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(2, 2), 1.5, 0, 360), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, _, _},
                           {X, _, _, X, _},
                           {X, _, _, X, _},
                           {_, X, X, _, _},
                           {_, _, _, _, _}}));

} END_TEST(test_frame_arc_3x3)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(2, 2), 1, 0, 360), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, X, _},
                           {_, X, _, X, _},
                           {_, X, X, X, _},
                           {_, _, _, _, _}}));

} END_TEST(test_frame_arc_2x2)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(2, 2), 0.5, 0, 360), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, _, _},
                           {_, X, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

} END_TEST(test_frame_arc_1x1)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(2, 2), 0, 0, 360), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));

} END_TEST(test_frame_arc_0x0)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::arc(core::point(2, 2), 2, 0, 360), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {X, X, X, X, X},
                           {X, X, X, X, X},
                           {X, X, X, X, X},
                           {_, X, X, X, _}}));

} END_TEST(test_fill_arc_4x4)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_3x3) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::arc(core::point(2, 2), 1.5, 0, 360), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, _, _},
                           {X, X, X, X, _},
                           {X, X, X, X, _},
                           {_, X, X, _, _},
                           {_, _, _, _, _}}));

} END_TEST(test_fill_arc_3x3)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::arc(core::point(2, 2), 1, 0, 360), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, X, _},
                           {_, X, X, X, _},
                           {_, X, X, X, _},
                           {_, _, _, _, _}}));

} END_TEST(test_fill_arc_2x2)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::arc(core::point(2, 2), 0.5, 0, 360), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, _, _},
                           {_, X, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_fill_arc_1x1)
// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_arc_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).fill(draw::arc(core::point(2, 2), 0, 0, 360), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_fill_arc_0x0)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_4x4) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::arc(core::point(2, 2), 2, 0, 360), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {X, O, O, O, X},
                           {X, O, O, O, X},
                           {X, O, O, O, X},
                           {_, X, X, X, _}}));
} END_TEST(test_draw_arc_4x4)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_3x3) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::arc(core::point(2, 2), 1.5, 0, 360), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, _, _},
                           {X, O, O, X, _},
                           {X, O, O, X, _},
                           {_, X, X, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_3x3)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_2x2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::arc(core::point(2, 2), 1, 0, 360), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, X, _},
                           {_, X, O, X, _},
                           {_, X, X, X, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_2x2)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_1x1) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::arc(core::point(2, 2), 0.5, 0, 360), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, X, X, _, _},
                           {_, X, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_1x1)
// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_0x0) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::arc(core::point(2, 2), 0, 0, 360), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_0x0)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_45_135) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(2, 2), 2, 45, 135), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {_, X, _, X, _},
                           {_, _, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_arc_45_135)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_135_225) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(2, 2), 2, 135, 225), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {X, X, _, _, _},
                           {X, _, X, _, _},
                           {X, X, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_arc_135_225)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_225_315) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(2, 2), 2, 225, 315), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, X, _, _},
                           {_, X, _, X, _},
                           {_, X, X, X, _}}));
} END_TEST(test_frame_arc_225_315)
// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_315_405) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(2, 2), 2, 315, 405), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, X, X},
                           {_, _, X, _, X},
                           {_, _, _, X, X},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_arc_315_405)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_45_135) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::arc(core::point(2, 2), 2, 45, 135), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {_, X, O, X, _},
                           {_, _, X, _, _},
                           {_, _, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_45_135)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_135_225) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::arc(core::point(2, 2), 2, 135, 225), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {X, X, _, _, _},
                           {X, O, X, _, _},
                           {X, X, _, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_135_225)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_225_315) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::arc(core::point(2, 2), 2, 225, 315), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, _, _},
                           {_, _, X, _, _},
                           {_, X, O, X, _},
                           {_, X, X, X, _}}));
} END_TEST(test_draw_arc_225_315)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_arc_315_405) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).draw(draw::arc(core::point(2, 2), 2, 315, 405), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, _, X, X},
                           {_, _, X, O, X},
                           {_, _, _, X, X},
                           {_, _, _, _, _}}));
} END_TEST(test_draw_arc_315_405)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_45_135_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(2, 3), 3, 45, 135), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, X, X, _},
                           {X, _, _, _, X},
                           {_, X, _, X, _},
                           {_, _, X, _, _},
                           {_, _, _, _, _}}));
} END_TEST(test_frame_arc_45_135_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_135_225_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(3, 2), 3, 135, 225), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, X, _, _, _},
                           {X, _, X, _, _},
                           {X, _, _, X, _},
                           {X, _, X, _, _},
                           {_, X, _, _, _}}));
} END_TEST(test_frame_arc_135_225_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_225_315_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(2, 1), 3, 225, 315), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _},
                           {_, _, X, _, _},
                           {_, X, _, X, _},
                           {X, _, _, _, X},
                           {_, X, X, X, _}}));
} END_TEST(test_frame_arc_225_315_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_arc_315_405_2) {
  core::global::set_scale_factor(1.0);

  pixmap img(5, 5);

  graphics(img).clear(_).frame(draw::arc(core::point(1, 2), 3, 315, 405), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, X, _},
                           {_, _, X, _, X},
                           {_, X, _, _, X},
                           {_, _, X, _, X},
                           {_, _, _, X, _}}));
} END_TEST(test_frame_arc_315_405_2)

// --------------------------------------------------------------------------
DEFINE_TEST(test_frame_polygon) {
  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(_).frame(draw::polygon({{1,1}, {1, 5}, {3,3}, {5,5}, {5,1}}), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _, _, _},
                           {_, X, X, X, X, X, _},
                           {_, X, _, _, _, X, _},
                           {_, X, _, X, _, X, _},
                           {_, X, X, _, X, X, _},
                           {_, X, _, _, _, X, _},
                           {_, _, _, _, _, _, _}}));

}
END_TEST(test_frame_polygon)

// --------------------------------------------------------------------------
DEFINE_TEST(test_fill_polygon) {
  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(_).fill(draw::polygon({{1,1}, {1, 5}, {3,3}, {5,5}, {5,1}}), X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _, _, _},
                           {_, X, X, X, X, X, _},
                           {_, X, X, X, X, X, _},
                           {_, X, X, X, X, X, _},
                           {_, X, X, _, X, X, _},
                           {_, X, _, _, _, X, _},
                           {_, _, _, _, _, _, _}}));

}
END_TEST(test_fill_polygon)

// --------------------------------------------------------------------------
DEFINE_TEST(test_draw_polygon) {
  core::global::set_scale_factor(1.0);

  pixmap img(7, 7);

  graphics(img).clear(_).draw(draw::polygon({{1,1}, {1, 5}, {3,3}, {5,5}, {5,1}}), O, X);

  auto buffer = pixmap2colormap(img);

  EXPECT_EQUAL(buffer, MM({{_, _, _, _, _, _, _},
                           {_, X, X, X, X, X, _},
                           {_, X, O, O, O, X, _},
                           {_, X, O, X, O, X, _},
                           {_, X, X, _, X, X, _},
                           {_, X, _, _, _, X, _},
                           {_, _, _, _, _, _, _}}));

}
END_TEST(test_draw_polygon)

// --------------------------------------------------------------------------
pixmap_str data2string (const char* data, const int bytes_per_pixel, const int bytes_per_line, const int height) {
  pixmap_str result;
  for (int y = 0; y < height; ++y) {
    std::ostringstream buffer;
    //    buffer << y << ':';
    int y_offset = y * bytes_per_line;
    for (int x = 0; x < bytes_per_line; x += bytes_per_pixel) {
      if (x > 0) {
        buffer << ' ';
      }
      for (int j = 0; j < bytes_per_pixel; ++j) {
        buffer << std::setw(2) << std::setfill('0') << std::hex << (unsigned short)(unsigned char)data[y_offset + x + j];
      }
    }
    result.emplace_back(buffer.str());
  }
  return result;
}

pixmap_str pixmap2string (const pixmap& img) {
#ifdef X11
  core::size sz = img.size();
  XImage* xim = XGetImage(core::global::get_instance(), img.get_id(), 0, 0, sz.width(), sz.height(), AllPlanes, ZPixmap);
  auto str = data2string(xim->data, xim->bits_per_pixel / 8, xim->bytes_per_line, xim->height);
  XDestroyImage(xim);
#endif // X11
#ifdef WIN32
  BITMAP bmi;
  GetObject(img.get_id(), sizeof (BITMAP), &bmi);
  blob data;
  data.resize(bmi.bmHeight * bmi.bmWidthBytes);
  GetBitmapBits(img.get_id(), (LONG)data.size(), data.data());
  auto str = data2string((const char*)data.data(), bmi.bmBitsPixel / 8, bmi.bmWidthBytes, bmi.bmHeight);
#endif // WIN32
  return str;
}

pixmap_str mk_str(std::initializer_list<string> i) {
  return pixmap_str(i);
}

std::ostream& operator<< (std::ostream& o, const pixmap_str& v) {
  for (const auto& i : v) {
    o << i << std::endl;
  }
  return o;
}
// --------------------------------------------------------------------------
colormap data2colormap (const char* data, const int bytes_per_pixel, const int bytes_per_line, const int height) {
  colormap result;
  for (int y = 0; y < height; ++y) {
    colorline line;
    line.reserve(bytes_per_line / bytes_per_pixel);
    int y_offset = y * bytes_per_line;
    for (int x = 0; x < bytes_per_line; x += bytes_per_pixel) {
      line.emplace_back(*reinterpret_cast<const uint32_t*>(data + y_offset + x));
    }
    result.emplace_back(line);
  }
  return result;
}

colormap pixmap2colormap (const pixmap& img) {
#ifdef X11
  core::size sz = img.size();
  XImage* xim = XGetImage(core::global::get_instance(), img.get_id(), 0, 0, sz.width(), sz.height(), AllPlanes, ZPixmap);
  auto result = data2colormap(xim->data, xim->bits_per_pixel / 8, xim->bytes_per_line, xim->height);
  XDestroyImage(xim);
#endif // X11
#ifdef WIN32
  BITMAP bmi;
  GetObject(img.get_id(), sizeof (BITMAP), &bmi);
  blob data;
  data.resize(bmi.bmHeight * bmi.bmWidthBytes);
  GetBitmapBits(img.get_id(), (LONG)data.size(), data.data());
  auto result = data2colormap((const char*)data.data(), bmi.bmBitsPixel / 8, bmi.bmWidthBytes, bmi.bmHeight);
#endif // WIN32
  return result;
}

std::ostream& operator<< (std::ostream& out, const colormap& m) {
  out << '[';
  bool first_line = true;
  for(const colorline& line: m) {
    if (first_line) {
      first_line = false;
    } else {
      out << ",";
    }
    out << '[';
    bool first = true;
    for(const uint32_t v: line) {
      if (first) {
        first = false;
      } else {
        out << ",";
      }
      switch (v) {
        case _: out << "_";break;
        case X: out << "X";break;
        case O: out << "O";break;
        default:
          out << std::setw(8) << std::setfill('0') << std::hex << v;
          break;
      }
    }
    out << ']';
  }
  out << ']';
  return out;
}
// --------------------------------------------------------------------------
