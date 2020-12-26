
#include <gui/draw/datamap.h>
#include <gui/io/pnm.h>
#include <testlib/image_test_lib.h>
#include <testlib/testlib.h>


// --------------------------------------------------------------------------
enum stretch {
  stretch_1 = 0,
  stretch_1_6 = 1,
  stretch_2 = 2,
  stretch_3 = 3,
  stretch_0_8 = 4,
  stretch_0_6 = 5
};

std::ostream& operator<< (std::ostream& out, const stretch& s) {
  switch (s) {
    case stretch_1: out << "1"; break;
    case stretch_1_6: out << "1.6"; break;
    case stretch_2: out << "2"; break;
    case stretch_3: out << "3"; break;
    case stretch_0_8: out << "0.8"; break;
    case stretch_0_6: out << "0.6"; break;
  }
  return out;
}

double stretcht_factor(stretch st) {
  static const double factors[] = {1.0, 1.6, 2.0, 3.0, 0.8, 0.6};
  return factors[static_cast<int>(st)];
}

// --------------------------------------------------------------------------
stretch stretch_f = stretch_1;

// --------------------------------------------------------------------------
using namespace testing;

colormap expected_nearest[] = {
  CM({{_, _, _, _, _},
      {_, W, W, W, _},
      {_, W, _, W, _},
      {_, W, W, W, _},
      {_, _, _, _, _}})
  , CM({{_, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _},
        {_, _, W, W, W, W, _, _},
        {_, _, W, _, _, W, _, _},
        {_, _, W, _, _, W, _, _},
        {_, _, W, W, W, W, _, _},
        {_, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _}})
  , CM({{_, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _},
        {_, _, W, W, W, W, W, W, _, _},
        {_, _, W, W, W, W, W, W, _, _},
        {_, _, W, W, _, _, W, W, _, _},
        {_, _, W, W, _, _, W, W, _, _},
        {_, _, W, W, W, W, W, W, _, _},
        {_, _, W, W, W, W, W, W, _, _},
        {_, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _}})
  , CM({{_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, W, W, W, _, _, _, W, W, W, _, _, _},
        {_, _, _, W, W, W, _, _, _, W, W, W, _, _, _},
        {_, _, _, W, W, W, _, _, _, W, W, W, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, W, W, W, W, W, W, W, W, W, _, _, _},
        {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _}})
  , CM({{_, _, _, _},
        {_, W, W, _},
        {_, W, W, _},
        {_, _, _, _}})
  , CM({{_, _, _},
        {_, _, _},
        {_, _, _}})
};

// --------------------------------------------------------------------------
void test_bw () {
  using namespace gui;
  using namespace gui::draw;

  bwmap img = colormap2datamap<pixel_format_t::BW>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::BW);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  bwmap stretched(expected_size);
  stretched.stretch_from(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 1);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::BW);

  auto buffer = datamap2colormap(stretched);
  EXPECT_EQUAL(buffer, expected_nearest[stretch_f]);
}

// --------------------------------------------------------------------------
void test_gray () {
  using namespace gui;
  using namespace gui::draw;

  graymap img = colormap2datamap<pixel_format_t::GRAY>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 8);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::GRAY);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  graymap stretched(expected_size);
  stretched.stretch_from(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 8);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::GRAY);

  auto buffer = datamap2colormap(stretched);
  EXPECT_EQUAL(buffer, expected_nearest[stretch_f]);
}

// --------------------------------------------------------------------------
void test_rgb () {
  using namespace gui;
  using namespace gui::draw;

  rgbmap img = colormap2datamap<pixel_format_t::RGB>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::RGB);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  rgbmap stretched(expected_size);
  stretched.stretch_from(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 24);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::RGB);

  auto buffer = datamap2colormap(stretched);
  EXPECT_EQUAL(buffer, expected_nearest[stretch_f]);
}

// --------------------------------------------------------------------------// --------------------------------------------------------------------------
void test_rgba () {
  using namespace gui;
  using namespace gui::draw;

  rgbamap img = colormap2datamap<pixel_format_t::RGBA>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::RGBA);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  rgbamap stretched(expected_size);
  stretched.stretch_from(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 24);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::RGBA);

  auto buffer = datamap2colormap(stretched);
  EXPECT_EQUAL(buffer, expected_nearest[stretch_f]);
}

// --------------------------------------------------------------------------
graysmap expected_bilinear[] = {
  GM({{0, 0,   0,   0,   0},
      {0, 255, 255, 255, 0},
      {0, 255, 0,   255, 0},
      {0, 255, 255, 255, 0},
      {0, 0,   0,   0,   0}})
  , GM({{0,   0,   0,   0,   0,   0,   0, 0},
        {0,  82, 145, 145, 145, 145,  82, 0},
        {0, 144, 248, 227, 227, 248, 144, 0},
        {0, 144, 227, 123, 123, 227, 144, 0},
        {0, 144, 227, 123, 123, 227, 144, 0},
        {0, 144, 248, 227, 227, 248, 144, 0},
        {0,  82, 145, 145, 145, 145,  82, 0},
        {0,   0,   0,   0,   0,   0,   0, 0}})
//  , GM({{0,   0,   0,   0,   0,   0,   0,   0},
//        {0, 121, 177, 177, 177, 177, 121,   0},
//        {0, 177, 255, 250, 250, 255, 177,   0},
//        {0, 177, 250, 192, 192, 250, 177,   0},
//        {0, 177, 250, 192, 192, 250, 177,   0},
//        {0, 177, 255, 250, 250, 255, 177,   0},
//        {0, 121, 177, 177, 177, 177, 121,   0},
//        {0,   0,   0,   0,   0,   0,   0,   0}})
  , GM({{0,   0,   0,   0,   0,   0,   0,   0,   0, 0},
        {0,  50, 100, 112, 112, 112, 112, 100,  50, 0},
        {0, 100, 200, 225, 225, 225, 225, 200, 100, 0},
        {0, 112, 225, 225, 187, 187, 225, 225, 112, 0},
        {0, 112, 225, 188,  99,  99, 187, 225, 112, 0},
        {0, 112, 225, 188,  99,  99, 187, 225, 112, 0},
        {0, 112, 225, 225, 187, 187, 225, 225, 112, 0},
        {0, 100, 200, 225, 225, 225, 225, 200, 100, 0},
        {0,  50, 100, 112, 112, 112, 112, 100,  50, 0},
        {0,   0,   0,   0,   0,   0,   0,   0,   0, 0}})
//  , GM({{0,   0,   0,   0,   0,   0,   0,   0,   0, 0},
//        {0,  71, 120, 137, 137, 137, 137, 120,  71, 0},
//        {0, 120, 198, 225, 225, 225, 225, 198, 120, 0},
//        {0, 137, 225, 248, 233, 233, 248, 225, 137, 0},
//        {0, 137, 225, 233, 177, 177, 233, 225, 137, 0},
//        {0, 137, 225, 233, 177, 177, 233, 225, 137, 0},
//        {0, 137, 225, 248, 233, 233, 248, 225, 137, 0},
//        {0, 120, 198, 225, 225, 225, 225, 198, 120, 0},
//        {0,  71, 120, 137, 137, 137, 137, 120,  71, 0},
//        {0,   0,   0,   0,   0,   0,   0,   0,   0, 0}})
  ,  GM({{0, 0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0, 0},
         {0, 20,  41,  62,  72,  72,  72,  72,  72,  72,  72,  62,  41, 20, 0},
         {0, 41,  82, 124, 145, 145, 145, 145, 145, 145, 145, 124,  82, 41, 0},
         {0, 61, 124, 186, 217, 217, 217, 218, 217, 217, 217, 186, 124, 61, 0},
         {0, 71, 144, 217, 248, 237, 227, 218, 227, 237, 248, 217, 144, 71, 0},
         {0, 71, 144, 217, 238, 207, 175, 145, 175, 207, 238, 217, 144, 71, 0},
         {0, 71, 144, 217, 227, 175, 123,  72, 123, 175, 227, 217, 144, 71, 0},
         {0, 72, 145, 218, 218, 145, 72,    0,  72, 145, 218, 218, 145, 72, 0},
         {0, 71, 144, 217, 227, 175, 123,  72, 123, 175, 227, 217, 144, 71, 0},
         {0, 71, 144, 217, 238, 207, 175, 145, 175, 207, 238, 217, 144, 71, 0},
         {0, 71, 144, 217, 248, 237, 227, 218, 227, 237, 248, 217, 144, 71, 0},
         {0, 61, 124, 186, 217, 217, 217, 218, 217, 217, 217, 186, 124, 61, 0},
         {0, 41,  82, 124, 145, 145, 145, 145, 145, 145, 145, 124,  82, 41, 0},
         {0, 20,  41,  62,  72,  72,  72,  72,  72,  72,  72,  62,  41, 20, 0},
         {0, 0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0, 0}})
//  , GM({{0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0},
//        {0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0},
//        {0, 0,  94, 130, 156, 156, 156, 156, 156, 156, 156, 130,  94, 0, 0},
//        {0, 0, 130, 178, 213, 213, 213, 213, 213, 213, 213, 178, 130, 0, 0},
//        {0, 0, 156, 213, 255, 255, 255, 255, 255, 255, 255, 213, 156, 0, 0},
//        {0, 0, 156, 213, 255, 242, 228, 213, 228, 242, 255, 213, 156, 0, 0},
//        {0, 0, 156, 213, 255, 228, 197, 156, 197, 228, 255, 213, 156, 0, 0},
//        {0, 0, 156, 213, 255, 213, 156,   0, 156, 213, 255, 213, 156, 0, 0},
//        {0, 0, 156, 213, 255, 228, 197, 156, 197, 228, 255, 213, 156, 0, 0},
//        {0, 0, 156, 213, 255, 242, 228, 213, 228, 242, 255, 213, 156, 0, 0},
//        {0, 0, 156, 213, 255, 255, 255, 255, 255, 255, 255, 213, 156, 0, 0},
//        {0, 0, 130, 178, 213, 213, 213, 213, 213, 213, 213, 178, 130, 0, 0},
//        {0, 0,  94, 130, 156, 156, 156, 156, 156, 156, 156, 130,  94, 0, 0},
//        {0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0},
//        {0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0}})
  , GM({{0,   0,   0, 0},
        {0, 225, 225, 0},
        {0, 225, 225, 0},
        {0,   0,   0, 0}})
//  , GM({{34,  99,  99, 34},
//        {99, 239, 239, 99},
//        {99, 239, 239, 99},
//        {34,  99,  99, 34}})
  , GM({{0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}})
//  , GM({{94, 156, 94},
//        {156, 0, 156},
//        {94, 156, 94}})
};

graysmap graysmap2bwsmap (graysmap m) {
  for (grayline& y : m) {
    for (uint8_t& x : y) {
      x = (x < 128) ? 0 : 255;
    }
  }
  return m;
}

// --------------------------------------------------------------------------
void test_bw_bilinear () {
  using namespace gui;
  using namespace gui::draw;

  bwmap img = colormap2datamap<pixel_format_t::BW>(expected_nearest[0]);

//  io::ofpnm<io::PNM::P1>(ostreamfmt("mono_" << stretch_f)) << img;

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::BW);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  bwmap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bilinear>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 1);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::BW);

  auto buffer = datamap2graysmap(graymap(stretched));
  EXPECT_EQUAL(buffer, graysmap2bwsmap(expected_bilinear[stretch_f]));
}

// --------------------------------------------------------------------------
void test_gray_bilinear () {
  using namespace gui;
  using namespace gui::draw;

  graymap img = colormap2datamap<pixel_format_t::GRAY>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 8);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::GRAY);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  graymap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bilinear>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 8);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::GRAY);

  io::ofpnm<io::PNM::P2>(ostreamfmt("gray_" << stretch_f)) << stretched;

  auto buffer = datamap2graysmap(stretched);
  EXPECT_EQUAL(buffer, expected_bilinear[stretch_f]);
}

// --------------------------------------------------------------------------
void test_rgb_bilinear () {
  using namespace gui;
  using namespace gui::draw;

  rgbmap img = colormap2datamap<pixel_format_t::RGB>(expected_nearest[0]);

//  io::ofpnm<io::PNM::P3>(ostreamfmt("rgb_" << stretch_f)) << img;

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::RGB);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  rgbmap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bilinear>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 24);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::RGB);

  auto buffer = datamap2graysmap(graymap(stretched));
  EXPECT_EQUAL(buffer, expected_bilinear[stretch_f]);
}

// --------------------------------------------------------------------------
void test_rgba_bilinear () {
  using namespace gui;
  using namespace gui::draw;

  rgbamap img = colormap2datamap<pixel_format_t::RGBA>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::RGBA);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  rgbamap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bilinear>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 24);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::RGBA);

  auto buffer = datamap2graysmap(graymap(stretched));
  EXPECT_EQUAL(buffer, expected_bilinear[stretch_f]);
}

// --------------------------------------------------------------------------
graysmap expected_bicubic[] = {
  GM({{0, 0,   0,   0,   0},
      {0, 255, 255, 255, 0},
      {0, 255, 0,   255, 0},
      {0, 255, 255, 255, 0},
      {0, 0,   0,   0,   0}})
  , GM({{0,  0,  0,  0,  0,  0,  0,0},
        {0, 87,159,169,169,159, 87,0},
        {0,159,255,253,253,255,159,0},
        {0,169,253, 94, 94,253,169,0},
        {0,169,253, 94, 94,253,169,0},
        {0,159,255,253,253,255,159,0},
        {0, 87,159,169,169,159, 87,0},
        {0,  0,  0,  0,  0,  0,  0,0}})
  , GM({{0,  0,  0,  0,  0,  0,  0,  0,  0,0},
        {0, 46,101,122,124,124,122,101, 46,0},
        {0,101,218,255,250,250,255,218,101,0},
        {0,122,255,255,203,203,255,255,122,0},
        {0,124,250,203, 61, 61,203,250,124,0},
        {0,124,250,203, 61, 61,203,250,124,0},
        {0,122,255,255,203,203,255,255,122,0},
        {0,101,218,255,250,250,255,218,101,0},
        {0, 46,101,122,124,124,122,101, 46,0},
        {0,  0,  0,  0,  0,  0,  0,  0,  0,0}})
  ,  GM({{0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,0},
         {0,14, 35, 55, 65, 69, 69, 69, 69, 69, 65, 55, 35,14,0},
         {0,35, 87,134,159,168,169,168,169,168,159,134, 87,35,0},
         {0,55,134,206,243,252,247,243,247,252,243,206,134,55,0},
         {0,65,159,243,255,255,253,240,253,255,255,243,159,65,0},
         {0,69,168,252,255,237,182,154,182,237,255,252,168,69,0},
         {0,69,169,247,253,182, 94, 50, 94,182,253,247,169,69,0},
         {0,69,168,243,240,154, 50,  0, 50,154,240,243,168,69,0},
         {0,69,169,247,253,182, 94, 50, 94,182,253,247,169,69,0},
         {0,69,168,252,255,237,182,154,182,237,255,252,168,69,0},
         {0,65,159,243,255,255,253,240,253,255,255,243,159,65,0},
         {0,55,134,206,243,252,247,243,247,252,243,206,134,55,0},
         {0,35, 87,134,159,168,169,168,169,168,159,134, 87,35,0},
         {0,14, 35, 55, 65, 69, 69, 69, 69, 69, 65, 55, 35,14,0},
         {0, 0,  0  ,0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,0}})
  , GM({{0,  0,  0,0},
        {0,255,255,0},
        {0,255,255,0},
        {0,  0,  0,0}})
  , GM({{0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}})
};

// --------------------------------------------------------------------------
void test_bw_bicubic () {
  using namespace gui;
  using namespace gui::draw;

  bwmap img = colormap2datamap<pixel_format_t::BW>(expected_nearest[0]);

//  io::ofpnm<io::PNM::P1>(ostreamfmt("mono_" << stretch_f)) << img;

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 1);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::BW);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  bwmap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bicubic>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 1);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::BW);

  auto buffer = datamap2graysmap(graymap(stretched));
  EXPECT_EQUAL(buffer, graysmap2bwsmap(expected_bicubic[stretch_f]));
}

// --------------------------------------------------------------------------
void test_gray_bicubic () {
  using namespace gui;
  using namespace gui::draw;

  graymap img = colormap2datamap<pixel_format_t::GRAY>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 8);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::GRAY);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  graymap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bicubic>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 8);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::GRAY);

  io::ofpnm<io::PNM::P2>(ostreamfmt("gray_bicubic_" << stretch_f)) << stretched;

  auto buffer = datamap2graysmap(stretched);
  EXPECT_EQUAL(buffer, expected_bicubic[stretch_f]);
}

// --------------------------------------------------------------------------
void test_rgb_bicubic () {
  using namespace gui;
  using namespace gui::draw;

  rgbmap img = colormap2datamap<pixel_format_t::RGB>(expected_nearest[0]);

//  io::ofpnm<io::PNM::P3>(ostreamfmt("rgb_" << stretch_f)) << img;

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::RGB);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  rgbmap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bicubic>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 24);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::RGB);

  auto buffer = datamap2graysmap(graymap(stretched));
  EXPECT_EQUAL(buffer, expected_bicubic[stretch_f]);
}

// --------------------------------------------------------------------------
void test_rgba_bicubic () {
  using namespace gui;
  using namespace gui::draw;

  rgbamap img = colormap2datamap<pixel_format_t::RGBA>(expected_nearest[0]);

  EXPECT_TRUE(img.is_valid());
  EXPECT_EQUAL(img.native_size(), core::native_size(5, 5));
  EXPECT_EQUAL(img.depth(), 24);
  EXPECT_EQUAL(img.pixel_format(), pixel_format_t::RGBA);

  auto factor = stretcht_factor(stretch_f);
  std::cout << "Stretching with factor " << factor << std::endl;

  auto expected_size = img.native_size() * factor;
  rgbamap stretched(expected_size);
  stretched.stretch_from<convert::interpolation::bicubic>(img);

  EXPECT_TRUE(stretched.is_valid());
  EXPECT_EQUAL(stretched.native_size(), expected_size);
  EXPECT_EQUAL(stretched.depth(), 24);
  EXPECT_EQUAL(stretched.pixel_format(), pixel_format_t::RGBA);

  auto buffer = datamap2graysmap(graymap(stretched));
  EXPECT_EQUAL(buffer, expected_bicubic[stretch_f]);
}

// --------------------------------------------------------------------------
void test_main (const testing::start_params& params) {
  testing::init_gui(params);
  testing::log_info("Running stretch_test");

  for (stretch_f = stretch_1; stretch_f <= stretch_0_6; stretch_f = (stretch)(stretch_f + 1)) {
    run_test(test_bw);
    run_test(test_gray);
    run_test(test_rgb);
    run_test(test_rgba);

    run_test(test_bw_bilinear);
    run_test(test_gray_bilinear);
    run_test(test_rgb_bilinear);
    run_test(test_rgba_bilinear);

    run_test(test_bw_bicubic);
    run_test(test_gray_bicubic);
    run_test(test_rgb_bicubic);
    run_test(test_rgba_bicubic);
  }
}

// --------------------------------------------------------------------------// --------------------------------------------------------------------------
