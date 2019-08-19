
#include <iomanip>

#include <gui/draw/bitmap.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/io/pnm.h>
#include <testlib/testlib.h>

namespace image_data {
#include <gui/ctrl/res/file_icon.h>
#include <gui/ctrl/res/close_folder_icon.h>
#include <gui/ctrl/res/open_folder_icon.h>
} // namespace image_data

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
DECLARE_TEST(test_native_impl);
DECLARE_TEST(test_bitmap_get_data);
DECLARE_TEST(test_bitmap_get_image);
DECLARE_TEST(test_bitmap_get_image_inv);
DECLARE_TEST(test_bitmap_get_image_mask);
DECLARE_TEST(test_copy_bitmap);
DECLARE_TEST(test_copy_pixmap);
DECLARE_TEST(test_masked_from_pixmap);
DECLARE_TEST(test_masked_bitmap);
DECLARE_TEST(test_file_icon);
DECLARE_TEST(test_file_icon_selected);
DECLARE_TEST(test_close_folder_icon);
DECLARE_TEST(test_close_folder_icon_selected);
DECLARE_TEST(test_open_folder_icon);
DECLARE_TEST(test_open_folder_icon_selected);

// --------------------------------------------------------------------------
TEST_MAIN(icon_test)
  RUN_TEST(test_native_impl);
  RUN_TEST(test_bitmap_get_data);
  RUN_TEST(test_bitmap_get_image);
  RUN_TEST(test_bitmap_get_image_inv);
  RUN_TEST(test_bitmap_get_image_mask);
  RUN_TEST(test_copy_bitmap);
  RUN_TEST(test_copy_pixmap);
  RUN_TEST(test_masked_from_pixmap);
  RUN_TEST(test_masked_bitmap);
  RUN_TEST(test_file_icon);
//  RUN_TEST(test_file_icon_selected);
//  RUN_TEST(test_close_folder_icon);
//  RUN_TEST(test_close_folder_icon_selected);
//  RUN_TEST(test_open_folder_icon);
//  RUN_TEST(test_open_folder_icon_selected);
TEST_MAIN_END(icon_test)

using namespace gui;

template<typename T, size_t N>
inline std::string make_string (T(&t)[N]) {
  return std::string((const char*)t, N);
}

template<typename T, size_t N>
inline std::size_t size_of_array (T(&t)[N]) {
  return N;
}

static const unsigned char expected_file_icon_bits[] = {
  0b00000000, 0b00000000, 0b00000000, 0x0,
  0b00000000, 0b00000000, 0b00000000, 0x0,
  0b00000000, 0b00000000, 0b00000000, 0x0,
  0b00011111, 0b10000000, 0b00000000, 0x0,
  0b00010000, 0b11000000, 0b00000000, 0x0,
  0b00010000, 0b10100000, 0b00000000, 0x0,
  0b00010000, 0b11110000, 0b00000000, 0x0,
  0b00010000, 0b00010000, 0b00000000, 0x0,
  0b00010000, 0b00010000, 0b00000000, 0x0,
  0b00010000, 0b00010000, 0b00000000, 0x0,
  0b00010000, 0b00010000, 0b00000000, 0x0,
  0b00010000, 0b00010000, 0b00000000, 0x0,
  0b00010000, 0b00010000, 0b00000000, 0x0,
  0b00010000, 0b00010000, 0b00000000, 0x0,
  0b00010000, 0b00010000, 0b00000000, 0x0,
  0b00011111, 0b11110000, 0b00000000, 0x0,
  0b00000000, 0b00000000, 0b00000000, 0x0,
  0b00000000, 0b00000000, 0b00000000, 0x0,
  0b00000000, 0b00000000, 0b00000000, 0x0,
  0b00000000, 0b00000000, 0b00000000, 0x0
};

bool expected_bit_at (int x, int y) {
  const unsigned char by = expected_file_icon_bits[y * 4 + x / 8];
  const unsigned char msk = 0x01 << (x % 8);
  const bool bi = by & msk;
  return bi;
}

bool expected_bit_at_inv (int x, int y) {
  const unsigned char by = expected_file_icon_bits[y * 4 + x / 8];
  const unsigned char msk = 0x01 << (7 - x % 8);
  const bool bi = by & msk;
  return bi;
}

// --------------------------------------------------------------------------
DEFINE_TEST(test_native_impl) {
#ifdef X11
  os::instance display = core::global::get_instance();
  os::x11::screen screen = core::global::x11::get_screen();
  os::drawable drawable = DefaultRootWindow(display);
  int depth = core::global::get_device_depth();

  XGCValues values;
  values.foreground = XWhitePixel(display, screen);
  values.background = XBlackPixel(display, screen);

  Pixmap mask = XCreateBitmapFromData(display, drawable, (const char*)expected_file_icon_bits, 32, 20);

  XImage* im = XGetImage(display, mask, 0, 0, 32, 20, AllPlanes, ZPixmap);

  for (int y = 0; y < 20; ++y) {
    const uint8_t* line = (const uint8_t*)(im->data + im->bytes_per_line * y);
    for (int x = 0; x < im->bytes_per_line; ++x) {
      const uint8_t expected = expected_file_icon_bits[y * 4 + x];
      const uint8_t test = line[x];
      EXPECT_EQUAL(test, expected, " at x:", x, ", y:", y);
    }
  }
  for (int y = 0; y < 20; ++y) {
    for (int x = 0; x < 20; ++x) {
      const bool expected = expected_bit_at(x, y);
      const uint32_t test = XGetPixel(im, x, y);
      EXPECT_EQUAL(test, expected, " at x:", x, ", y:", y);
    }
  }

  XDestroyImage(im);

  Pixmap img = XCreatePixmapFromBitmapData(display, drawable, (char*)expected_file_icon_bits, 32, 20, 0xff0000, 0x00ff00, depth);

  im = XGetImage(display, img, 0, 0, 32, 20, AllPlanes, ZPixmap);

  for (int y = 0; y < 20; ++y) {
    const uint32_t* line = (const uint32_t*)(im->data + im->bytes_per_line * y);
    for (int x = 0; x < 20; ++x) {
      const uint32_t expected = expected_bit_at(x, y) ? 0xff0000 : 0x00ff00;
      const uint32_t test = line[x];
      EXPECT_EQUAL(test, expected, " at x:", x, ", y:", y);
    }
  }

  XDestroyImage(im);

  Pixmap mem = XCreatePixmap(display, drawable, 20, 20, depth);
  os::graphics gc = XCreateGC(display, mem, GCForeground|GCBackground, &values);

  XSetForeground(display, gc, 0x0000ff);
  XFillRectangle(display, mem, gc, 0, 0, 20, 20);

  XSetClipMask(display, gc, mask);
  XSetClipOrigin(display, gc, 0, 0);

  XCopyArea(display, img, mem, gc, 0, 0, 20, 20, 0, 0);

  XFreeGC(display, gc);
  XFreePixmap(display, img);
  XFreePixmap(display, mask);

  im = XGetImage(display, mem, 0, 0, 20, 20, AllPlanes, ZPixmap);
  XFreePixmap(display, mem);

  for (int y = 0; y < 20; ++y) {
    const uint32_t* line = (const uint32_t*)(im->data + im->bytes_per_line * y);
    for (int x = 0; x < 20; ++x) {
      const uint32_t expected = expected_bit_at(x, y) ? 0xff0000 : 0x0000ff;
      const uint32_t test = line[x] & 0xffffff;
      EXPECT_EQUAL(test, expected, " at x:", x, ", y:", y);
    }
  }

  XDestroyImage(im);
#endif // X11
} END_TEST(test_native_impl)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bitmap_get_data) {
  core::global::set_scale_factor(1.0);

  draw::bwmap mask;
  std::istringstream in(make_string(image_data::file_icon_bits));
  io::load_pnm(in, mask);

  auto data = mask.get_data();
  for (int y = 0; y < 20; ++y) {
    auto row = data.row(y);
    for (int x = 0; x < 20; ++x) {
      const pixel::bw_pixel expected = expected_bit_at_inv(x, y) ? pixel::bw_pixel::white : pixel::bw_pixel::black;
      const pixel::bw_pixel test = row[x];
      EXPECT_EQUAL(test, expected, " at x:", x, ", y:", y);
    }
  }

} END_TEST(test_bitmap_get_data)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bitmap_get_image) {
  core::global::set_scale_factor(1.0);

  draw::bwmap mask;
  std::istringstream in(make_string(image_data::file_icon_bits));
  io::load_pnm(in, mask);

  draw::pixmap pix(mask);
  draw::rgbmap img = pix.get<PixelFormat::RGB>();

  auto data = img.get_data();
  for (int y = 0; y < 20; ++y) {
    auto row = data.row(y);
    for (int x = 0; x < 20; ++x) {
      const pixel::rgb_pixel expected = expected_bit_at_inv(x, y) ? pixel::color<pixel::rgb_pixel>::white
                                                                  : pixel::color<pixel::rgb_pixel>::black;
      const pixel::rgb_pixel test = row[x];
      EXPECT_EQUAL(test, expected, " at x:", x, ", y:", y);
    }
  }

} END_TEST(test_bitmap_get_image)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bitmap_get_image_inv) {
  core::global::set_scale_factor(1.0);

  draw::bwmap mask;
  std::istringstream in(make_string(image_data::file_icon_bits));
  io::load_pnm(in, mask);

  draw::pixmap pix(mask);
  pix.invert();
  draw::rgbmap img = pix.get<PixelFormat::RGB>();

  auto data = img.get_data();
  for (int y = 0; y < 20; ++y) {
    auto row = data.row(y);
    for (int x = 0; x < 20; ++x) {
      const pixel::rgb_pixel expected = expected_bit_at_inv(x, y) ? pixel::color<pixel::rgb_pixel>::black
                                                                  : pixel::color<pixel::rgb_pixel>::white;
      const pixel::rgb_pixel test = row[x];
      EXPECT_EQUAL(test, expected, " at x:", x, ", y:", y);
    }
  }

} END_TEST(test_bitmap_get_image_inv)

// --------------------------------------------------------------------------
DEFINE_TEST(test_bitmap_get_image_mask) {
  core::global::set_scale_factor(1.0);

  draw::bwmap bw;
  std::istringstream in(make_string(image_data::file_icon_bits));
  io::load_pnm(in, bw);

  draw::bitmap mask(bw);
  draw::pixmap pix(bw);

  draw::pixmap mem(20, 20);
  {
    os::instance display = core::global::get_instance();

    draw::graphics gc(mem);
    gc.clear(color::gray);
#ifdef X11
    pix.invert();

    XSetClipMask(display, gc, mask);
    XSetClipOrigin(display, gc, 0, 0);

    XCopyArea(display, pix, mem, gc, 0, 0, 20, 20, 0, 0);

    XSetClipMask(display, gc, None);
#endif // X11
#ifdef WIN32
    mask.invert();
    HDC mask_dc = CreateCompatibleDC(gc);
    SelectObject(mask_dc, mask.get_id());
    //HDC img_dc = CreateCompatibleDC(gc);
    //SelectObject(img_dc, pix.get_id());

    BitBlt(gc, 0, 0, 20, 20, mask_dc, 0, 0, SRCAND);
    //BitBlt(gc, 0, 0, 20, 20, img_dc, 0, 0, SRCPAINT);
    //DeleteDC(img_dc);
    DeleteDC(mask_dc);
#endif // WIN32

  }

  draw::rgbmap img = mem.get<PixelFormat::RGB>();
  auto data = img.get_data();

  auto gray = pixel::rgb_pixel::build(color::gray);

  for (uint32_t y = 0; y < 20; ++y) {
    auto row = data.row(y);
    for (uint32_t x = 0; x < 20; ++x) {
      const pixel::rgb_pixel expected = expected_bit_at_inv(x, y) ? pixel::color<pixel::rgb_pixel>::black
                                                                  : gray;
      const pixel::rgb_pixel test = row[x];
      EXPECT_EQUAL(test, expected, " at x = ", x, ", y = ", y);
    }
  }

} END_TEST(test_bitmap_get_image_mask)

// --------------------------------------------------------------------------
DEFINE_TEST(test_copy_bitmap) {
  core::global::set_scale_factor(1.0);

  draw::bwmap bw;
  std::istringstream in(make_string(image_data::file_icon_bits));
  io::load_pnm(in, bw);

  draw::bitmap bm0(bw);
  draw::bitmap bm1(bm0);

  draw::bwmap bw0 = bm0.get();
  draw::bwmap bw1 = bm1.get();

  auto data0 = bw0.get_data();
  auto data1 = bw1.get_data();

  for (uint32_t y = 0; y < 20; ++y) {
    auto row0 = data0.row(y);
    auto row1 = data1.row(y);
    for (uint32_t x = 0; x < 20; ++x) {
      const auto pix0 = row0[x];
      const auto pix1 = row1[x];
      EXPECT_EQUAL(pix1, pix0, " at x = ", x, ", y = ", y);
    }
  }

} END_TEST(test_copy_bitmap)

// --------------------------------------------------------------------------
DEFINE_TEST(test_copy_pixmap) {
  core::global::set_scale_factor(1.0);

  draw::bwmap bw;
  std::istringstream in(make_string(image_data::file_icon_bits));
  io::load_pnm(in, bw);

  draw::pixmap pix0(bw);
  draw::pixmap pix1(pix0);

  draw::rgbamap map0 = pix0.get<PixelFormat::RGBA>();
  draw::rgbamap map1 = pix1.get<PixelFormat::RGBA>();

  auto data0 = map0.get_data();
  auto data1 = map1.get_data();

  for (uint32_t y = 0; y < 20; ++y) {
    auto row0 = data0.row(y);
    auto row1 = data1.row(y);
    for (uint32_t x = 0; x < 20; ++x) {
      const auto pix0 = row0[x];
      const auto pix1 = row1[x];
      EXPECT_EQUAL(pix1, pix0, " at x = ", x, ", y = ", y);
    }
  }

} END_TEST(test_copy_pixmap)

// --------------------------------------------------------------------------
DEFINE_TEST(test_masked_from_pixmap) {
  core::global::set_scale_factor(1.0);

  draw::pixmap pix(5, 5);
  draw::graphics(pix).clear(color::black).draw(draw::rectangle(core::rectangle(1, 1, 3, 3)), color::red, color::blue);

  os::color expected_color[5][5] = {
    {color::black, color::black, color::black, color::black, color::black},
    {color::black, color::blue,  color::blue,  color::blue,  color::black},
    {color::black, color::blue,  color::red,   color::blue,  color::black},
    {color::black, color::blue,  color::blue,  color::blue,  color::black},
    {color::black, color::black, color::black, color::black, color::black}
  };

  {
    auto img = pix.get<PixelFormat::RGB>();
    auto data = img.get_data();
    for (uint32_t y = 0; y < 5; ++y) {
      auto row = data.row(y);
      for (uint32_t x = 0; x < 5; ++x) {
        const pixel::rgb_pixel expected = pixel::rgb_pixel::build(expected_color[x][y]);
        const pixel::rgb_pixel test = row[x];
        EXPECT_EQUAL(test, expected, " at x = ", x, ", y = ", y);
      }
    }
  }

  pixel::bw_pixel expected_bw[5][5] = {
    {pixel::bw_pixel::black, pixel::bw_pixel::black, pixel::bw_pixel::black, pixel::bw_pixel::black, pixel::bw_pixel::black},
    {pixel::bw_pixel::black, pixel::bw_pixel::white, pixel::bw_pixel::white, pixel::bw_pixel::white, pixel::bw_pixel::black},
    {pixel::bw_pixel::black, pixel::bw_pixel::white, pixel::bw_pixel::white, pixel::bw_pixel::white, pixel::bw_pixel::black},
    {pixel::bw_pixel::black, pixel::bw_pixel::white, pixel::bw_pixel::white, pixel::bw_pixel::white, pixel::bw_pixel::black},
    {pixel::bw_pixel::black, pixel::bw_pixel::black, pixel::bw_pixel::black, pixel::bw_pixel::black, pixel::bw_pixel::black}
  };

  draw::masked_bitmap icon(pix);

  {
    auto img = icon.mask.get();
    auto data = img.get_data();
    for (uint32_t y = 0; y < 5; ++y) {
      auto row = data.row(y);
      for (uint32_t x = 0; x < 5; ++x) {
        const pixel::bw_pixel expected = expected_bw[x][y];// == pixel::bw_pixel::black ? pixel::bw_pixel::white : pixel::bw_pixel::black;
        const pixel::bw_pixel test = row[x];
        EXPECT_EQUAL(test, expected, " at x = ", x, ", y = ", y);
      }
    }
  }

} END_TEST(test_masked_from_pixmap)

// --------------------------------------------------------------------------
DEFINE_TEST(test_masked_bitmap) {
  core::global::set_scale_factor(1.0);

  draw::pixmap pix(5, 5);
  draw::graphics(pix).clear(color::black).draw(draw::rectangle(core::rectangle(1, 1, 3, 3)), color::red, color::blue);

  draw::masked_bitmap icon(pix);

  draw::pixmap mem(5, 5);
  draw::graphics g(mem);
  g.clear(color::gray).copy_from(icon);

  os::color expected_color[5][5] = {
    {color::gray, color::gray, color::gray, color::gray, color::gray},
    {color::gray, color::blue, color::blue, color::blue, color::gray},
    {color::gray, color::blue, color::red,  color::blue, color::gray},
    {color::gray, color::blue, color::blue, color::blue, color::gray},
    {color::gray, color::gray, color::gray, color::gray, color::gray}
  };

  for (uint32_t y = 0; y < 5; ++y) {
    for (uint32_t x = 0; x < 5; ++x) {
      const os::color expected = expected_color[x][y];
      const os::color test = g.get_pixel({x, y});
      EXPECT_EQUAL(test, expected, " at x = ", x, ", y = ", y);
    }
  }

} END_TEST(test_masked_bitmap)

// --------------------------------------------------------------------------
DEFINE_TEST(test_file_icon) {

  core::global::set_scale_factor(1.0);

  draw::bwmap mask;
  std::istringstream in(make_string(image_data::file_icon_bits));
  io::load_pnm(in, mask);

  EXPECT_TRUE(mask.is_valid());
  EXPECT_EQUAL(mask.size(), core::size(20, 20));
  EXPECT_EQUAL(mask.depth(), 1);
  EXPECT_EQUAL(mask.pixel_format(), PixelFormat::BW);
  EXPECT_EQUAL(mask.get_info(), draw::bitmap_info(20, 20, 4, PixelFormat::BW));

  auto bw_data = mask.get_data();

  {
    auto data = mask.get_data();
    auto raw = data.raw_data();

    for (int i = 0; i < size_of_array(expected_file_icon_bits); ++i) {
#ifdef WIN32
      EXPECT_EQUAL((uint8_t)raw[i], (uint8_t)expected_file_icon_bits[i], " at i = ", i);
#else
      EXPECT_EQUAL((uint8_t)raw[i], (uint8_t)basepp::reverse_bit_order(expected_file_icon_bits[i]), " at i = ", i);
#endif // WIN32
    }

    for (uint32_t y = 0; y < 20; ++y) {
      auto row = data.row(y);
      for (uint32_t x = 0; x < 20; ++x) {
        const bool expected = expected_bit_at_inv(x, y);
        const pixel::bw_pixel test = row[x];
        EXPECT_EQUAL((bool)test, expected, " at x = ", x, ", y = ", y);
      }
    }
  }

  draw::bitmap m(mask);
  draw::pixmap img(mask);

  {
    draw::graphics g(img);
    for (uint32_t y = 0; y < 20; ++y) {
      for (uint32_t x = 0; x < 20; ++x) {
        const os::color  expected = expected_bit_at_inv(x, y) ? color::white : color::black;
        const os::color test = g.get_pixel({x, y});
        EXPECT_EQUAL(test, expected, " at x = ", x, ", y = ", y);
      }
    }
  }

  draw::masked_bitmap icon(img, m);

  draw::pixmap mem(20, 20);
  {
    draw::graphics g(mem);
    g.clear(color::gray);
    g.copy_from(icon);

    for (uint32_t y = 0; y < 20; ++y) {
      for (uint32_t x = 0; x < 20; ++x) {
        os::color expected = expected_bit_at_inv(x, y) ? color::white : color::gray;
        os::color test = g.get_pixel({x, y});
        EXPECT_EQUAL(test, expected, " at x = ", x, ", y = ", y);
      }
    }
  }
}
END_TEST(test_file_icon)

// --------------------------------------------------------------------------
DEFINE_TEST(test_file_icon_selected) {} END_TEST(test_file_icon_selected)
// --------------------------------------------------------------------------
DEFINE_TEST(test_close_folder_icon) {} END_TEST(test_close_folder_icon)
// --------------------------------------------------------------------------
DEFINE_TEST(test_close_folder_icon_selected) {} END_TEST(test_close_folder_icon_selected)
// --------------------------------------------------------------------------
DEFINE_TEST(test_open_folder_icon) {} END_TEST(test_open_folder_icon)
// --------------------------------------------------------------------------
DEFINE_TEST(test_open_folder_icon_selected) {} END_TEST(test_open_folder_icon_selected)

// --------------------------------------------------------------------------

