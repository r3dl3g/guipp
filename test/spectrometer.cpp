

#include <gui/win/attach_layout.h>
#include <gui/win/border_layout.h>
#include <gui/win/adaption_layout.h>
#include <gui/win/layout_container.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/drop_down.h>
#include <gui/ctrl/std_dialogs.h>
#include <gui/draw/graphics.h>
#include <gui/draw/bitmap.h>
#include <gui/io/wavelength_to_rgb.h>

#ifdef BUILD_FOR_ARM
#include "raspi/encoder.h"
#else
#define MMAL_ENCODING_RGB24_SLICE 0
#define VCOS_ALIGN_DOWN(p,n) (((ptrdiff_t)(p)) & ~((n)-1))
#define VCOS_ALIGN_UP(p,n) VCOS_ALIGN_DOWN((ptrdiff_t)(p)+(n)-1,(n))
namespace gui { namespace raspi { namespace camera {
  struct raspi_camera {
    raspi_camera ()
      : cr{0, 0, 1, 1}
      , sz{3280, 2456}
      , g{1, 1}
      , ss(10000)
      , iso(100)
    {}

    struct crop {
      float x, y, width, height;
    };

    struct awb_gains {
      float r_gain, b_gain;
    };

    struct size {
      int width, height;
    };

    crop get_crop () const { return cr; }
    void set_crop (const crop& c) { cr = c; }
    crop get_abs_crop () const { return cr; }
    void set_abs_crop (const crop& c) { cr = c; }

    size get_size () const { return sz; }
    void set_size (const size& c) { sz = c; }

    int get_shutter_speed () const { return ss; }
    void set_shutter_speed (int s) { ss = s; }
    int get_iso () const { return iso; }
    void set_iso (int i) { iso = i; }

    int get_still_output_port () const {
      return 0;
    }

    int get_pixel_per_line () const {
      return 0;
    }

  private:
    crop cr;
    size sz;
    awb_gains g;
    int ss;
    int iso;
  };

  struct raspi_raw_encoder {
    raspi_raw_encoder(int, int) {}

    void capture (int) {}

    using image_data = std::basic_string<uint8_t>;

    image_data get_data () { return {}; }
    void clear_data () {}
  };
}}}
#endif // BUILD_FOR_ARM

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;
using namespace gui::layout;
using namespace gui::win;
using namespace gui::ctrl;
using namespace gui::raspi::camera;

const os::color nero = color::rgb<64,66,68>::value;
const os::color silver = color::rgb<0xC3,0xC6,0xC7>::value;

template<os::color background = nero>
class image_view : public client_control<background> {
public:
  image_view () {
    this->on_paint(draw::paint([&] (const graphics& graph) {
      if (image) {
        graph.copy_from(image, core::point::zero);
      } else {
        graph.fill(draw::rectangle(this->client_area()), background);
      }
    }));
  }

  pixmap image;
};

template<typename T>
class value_block : public group_window<layout::vertical_adaption<>> {
public:
  using super = layout_container<layout::vertical_adaption<>>;
  using fnkt = std::function<void(T)>;

  value_block ()
    : low_step(1)
    , high_step(20)
  {
    on_create(basepp::bind_method(this, &value_block::onCreated));
  }

  value_block (const std::string& l, text_source&& src)
    : label(l)
    , value(std::move(src))
    , low_step(1)
    , high_step(20)
  {
    on_create(basepp::bind_method(this, &value_block::onCreated));
  }

  void set_label (const std::string& l) {
    label.set_text(l);
  }

  void set_value (text_source&& src) {
    value.set_text(std::move(src));
  }

  void set_steps (T low, T high) {
    low_step = low;
    high_step = high;
  }

  void set_handler (fnkt f) {
    up_button.on_clicked([&, f] () { f(low_step); });
    fast_up_button.on_clicked([&, f] () { f(this->high_step); });
    down_button.on_clicked([&, f] () { f(-low_step); });
    fast_down_button.on_clicked([&, f] () { f(-high_step); });
  }

  void set_handler (T low, T high, fnkt f) {
    set_steps(low, high);
    set_handler(f);
  }

  void refresh () {
    value.redraw();
  }

  void onCreated (window*, const core::rectangle&) {
    label.create(*this);
    up_view.create(*this);
    up_button.create(up_view, "+");
    fast_up_button.create(up_view, "++");
    value.create(*this);
    down_view.create(*this);
    down_button.create(down_view, "-");
    fast_down_button.create(down_view, "--");
    set_children_visible();
  }

private:
  group_window<layout::horizontal_adaption<0>> up_view;
  group_window<layout::horizontal_adaption<0>> down_view;

  ctrl::text_button up_button;
  ctrl::text_button fast_up_button;

  ctrl::text_button down_button;
  ctrl::text_button fast_down_button;

  ctrl::label_center label;
  ctrl::label_center value;

  T low_step;
  T high_step;
};

// --------------------------------------------------------------------------
class spectrometer : public layout_main_window<gui::layout::border_layout<>, float, float, float, float> {
public:
  using super = layout_main_window<gui::layout::border_layout<>, float, float, float, float>;
  spectrometer ();

  void onCreated (window*, const core::rectangle&);
  void quit ();

  void capture ();
  void display ();
  void calc_spectrum ();
  void save_image ();

  void save_settings ();
  void load_settings ();

  void set_crop (const raspi_camera::crop& c);
  void change_crop_x (float v);
  void change_crop_y (float v);
  void change_crop_w (float v);
  void change_crop_h (float v);

private:
  group_window<layout::vertical_adaption<>> right_button_view;
  ctrl::text_button capture_button;
  ctrl::text_button save_button;
  ctrl::text_button clear_button;

  group_window<layout::vertical_adaption<>> left_button_view;
  ctrl::text_button fullview_button;
  ctrl::text_button halfview_button;
  ctrl::text_button quarterview_button;

  ctrl::drop_down_list<std::string> encoding_down;

  value_block<float> x_pos;
  value_block<float> y_pos;
  value_block<float> w_pos;
  value_block<float> h_pos;

  value_block<int> ss;
  value_block<int> iso;

  value_block<int> extra;
  int extra_bytes;

  group_window<layout::horizontal_adaption<1, 0>> values_view;

  image_view<color::black> capture_view;
  image_view<> spectrum_view;

  raspi_camera camera;
  raspi_raw_encoder encoder;

  raspi_raw_encoder::image_data data;
};

spectrometer::spectrometer ()
  : super(160, 160, 0, 0)
  , encoder(camera.get_still_output_port(), MMAL_ENCODING_RGB24_SLICE)
  , extra_bytes(0)
{
  on_create(basepp::bind_method(this, &spectrometer::onCreated));
  on_destroy(&gui::win::quit_main_loop);
  on_close(basepp::bind_method(this, &spectrometer::quit));

  //  camera.set_crop({0.25,0.25,0.5,0.5});
    camera.set_iso(100);
    camera.set_shutter_speed(50000);
  //  auto sz = camera.get_size();

  //  MMAL_PARAMETER_CROP_T crop = {{MMAL_PARAMETER_CROP, sizeof(MMAL_PARAMETER_CROP_T)}};
  //  encoder.get_output_port().get(crop.hdr);
  //  crop.rect.height = sz.height / 2;
  //  crop.rect.width = sz.width;
  //  encoder.get_output_port().set(crop.hdr);

  capture_button.set_text("Capture");
  save_button.set_text("Save");
  clear_button.set_text("Clear");

  fullview_button.set_text("1/1");
  halfview_button.set_text("1/2");
  quarterview_button.set_text("1/4");

  capture_button.on_clicked(basepp::bind_method(this, &spectrometer::capture));
  save_button.on_clicked(basepp::bind_method(this, &spectrometer::save_image));
  clear_button.on_clicked([&] () {
    extra_bytes = 0;
    extra.refresh();
  });
  fullview_button.on_clicked([&] () { set_crop({0,0,1,1}); });
  halfview_button.on_clicked([&] () { set_crop({0.25F, 0.25F, 0.5F, 0.5F}); });
  quarterview_button.on_clicked([&] () { set_crop({0.375F, 0.375F, 0.25F, 0.25F}); });

  x_pos.set_handler(0.01F, 0.05F, basepp::bind_method(this, &spectrometer::change_crop_x));
  x_pos.set_label("X");
  x_pos.set_value([&] () { return ostreamfmt(camera.get_abs_crop().x); });

  y_pos.set_handler(0.01F, 0.05F, basepp::bind_method(this, &spectrometer::change_crop_y));
  y_pos.set_label("Y");
  y_pos.set_value([&] () { return ostreamfmt(camera.get_abs_crop().y); });

  w_pos.set_handler(0.01F, 0.05F, basepp::bind_method(this, &spectrometer::change_crop_w));
  w_pos.set_label("W");
  w_pos.set_value([&] () { return ostreamfmt(camera.get_abs_crop().width); });

  h_pos.set_handler(0.01F, 0.05F, basepp::bind_method(this, &spectrometer::change_crop_h));
  h_pos.set_label("H");
  h_pos.set_value([&] () { return ostreamfmt(camera.get_abs_crop().height); });

  ss.set_handler([&] (int step) {
    auto current = camera.get_shutter_speed();
    if (step > 0.0F) {
      camera.set_shutter_speed(current * step);
    } else {
      camera.set_shutter_speed(current / -step);
    }
    ss.refresh();
//    capture();
  });
  ss.set_steps(1.1F, 2.0F);
  ss.set_label("Shutter");
  ss.set_value([&] () { return ostreamfmt(camera.get_shutter_speed()); });

  iso.set_handler([&] (int step) {
    auto current = camera.get_iso();
    if (step > 0.0F) {
      camera.set_iso(current * step);
    } else {
      camera.set_iso(std::max<uint32_t>(current / -step, 25));
    }
    iso.refresh();
//    capture();
  });
  iso.set_steps(2.0F, 4.0F);
  iso.set_label("ISO");
  iso.set_value([&] () { return ostreamfmt(camera.get_iso()); });

  extra.set_handler([&] (int step) {
    extra_bytes = std::max(extra_bytes + step, 0);
    extra.refresh();
    display();
  });
  extra.set_steps(1, 10);
  extra.set_label("Extra");
  extra.set_value([&] () { return ostreamfmt(extra_bytes); });

}

void spectrometer::set_crop (const raspi_camera::crop& c) {
  camera.set_crop(c);
  auto sz = camera.get_size();
  extra_bytes = VCOS_ALIGN_UP(sz.width, 32) - sz.width;
  extra.refresh();
  x_pos.refresh();
  y_pos.refresh();
  w_pos.refresh();
  h_pos.refresh();
}

void spectrometer::change_crop_x (float v) {
  auto crop = camera.get_crop();
  crop.x = std::min(std::max(crop.x + v, 0.0F), 1.0F - crop.width);
  camera.set_crop(crop);
  x_pos.refresh();
//    capture();
}

void spectrometer::change_crop_y (float v) {
  auto crop = camera.get_crop();
  crop.y = std::min(std::max(crop.y + v, 0.0F), 1.0F - crop.height);
  camera.set_crop(crop);
  y_pos.refresh();
//    capture();
}

void spectrometer::change_crop_w (float v) {
  auto crop = camera.get_crop();
  crop.width = std::min(std::max(crop.width + v, 0.0F), 1.0F - crop.x);
  camera.set_crop(crop);
  w_pos.refresh();
//    capture();
}

void spectrometer::change_crop_h (float v) {
  auto crop = camera.get_crop();
  crop.height = std::min(std::max(crop.height + v, 0.0F), 1.0F - crop.y);
  camera.set_crop(crop);
  h_pos.refresh();
//    capture();
}

void spectrometer::quit () {
  yes_no_dialog::ask(*this, "Question!", "Do you realy want to exit?", "Yes", "No", [&] (bool yes) {
    if (yes) {
      save_settings();
      set_children_visible(false);
      win::quit_main_loop();
    } else {
      take_focus();
    }
  });
}

void spectrometer::onCreated (window*, const core::rectangle&) {
  capture_view.create(*this);
  spectrum_view.create(*this);
  values_view.create(*this);

  left_button_view.create(values_view);
  fullview_button.create(left_button_view);
  halfview_button.create(left_button_view);
  quarterview_button.create(left_button_view);

  x_pos.create(values_view);
  y_pos.create(values_view);
  w_pos.create(values_view);
  h_pos.create(values_view);
  ss.create(values_view);
  iso.create(values_view);
  extra.create(values_view);

  right_button_view.create(values_view);
  capture_button.create(right_button_view);
  save_button.create(right_button_view);
  clear_button.create(right_button_view);

  auto encodings = encoder.get_output_port().get_supported_encodings();
  encoding_down.set_data([=](std::size_t i) { return ostreamfmt(four_cc(encodings[i])); }, encodings.size());
  encoding_down.create(right_button_view);
  encoding_down.set_visible_items(8);
  encoding_down.set_selection(0, event_source::logic);

  get_layout().set_center_top_bottom_left_right(&capture_view, &spectrum_view, &values_view, nullptr, nullptr);
  load_settings();
//  set_children_visible();
}

void spectrometer::display () {
  auto sz = camera.get_size();
  LogDebug << "Captured " << data.size() << " Bytes with dimensions:" << sz.width << "x" << sz.height << " ppl:" << camera.get_pixel_per_line();
  if (data.size()) {
    bitmap_info bmi(sz.width, sz.height, camera.get_pixel_per_line() * 3 + extra_bytes, PixelFormat::RGB);
    const_image_data<PixelFormat::RGB> image_data(basepp::array_wrapper<const byte>(data.c_str(), data.size()), bmi);
//  rgbmap image(capture_view.size());
//  image.stretch_from(rgbmap(image_data));
    capture_view.image = image_data;
  }
  capture_view.redraw();
}

void spectrometer::capture () {
  LogDebug << "Capture image";
  try {
    encoder.clear_data();
    capture_view.image.clear();

    encoder.capture(2000);
    data = encoder.get_data();
    display();
    calc_spectrum();
  } catch (std::exception& ex) {
    LogFatal << ex;
  }
}

void spectrometer::calc_spectrum () {
  auto sz = spectrum_view.size();
  if (spectrum_view.image.size() != sz) {
    spectrum_view.image.create(sz);
  }
  graphics g(spectrum_view.image);
  double scale = 500.0 / sz.width();
  for (int x = 0; x < sz.width(); ++x) {
    os::color rgb = io::optics::wave_length_to_rgb(330.0 + static_cast<double>(x) * scale);
    g.draw_lines({core::point(x, 0), core::point(x, sz.height())}, draw::pen(rgb));
  }
  spectrum_view.redraw();
}

void spectrometer::save_image () {
  file_save_dialog::show(*this, "Save captured data", "spectrogram.ppm", "Name:", "OK", "Cancel",
                         [&] (const sys_fs::path& path) {
    std::ofstream file(path);
    auto crop = camera.get_abs_crop();
    file << "P6" << std::endl << crop.width << " " << crop.height << std::endl << "255" << std::endl;
    file.write((const char*)data.c_str(), data.size());
  });
}

void spectrometer::save_settings () {

}

void spectrometer::load_settings () {

}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  spectrometer main;

  main.create({0, 0, 1420, 800});
  main.set_title("Raspi Spectrometer");
  main.set_visible();

  return gui::win::run_main_loop();
}
