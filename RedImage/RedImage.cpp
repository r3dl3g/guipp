
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/menu.h>
#include <base/string_util.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;
using namespace gui::layout;
using namespace gui::win;
using namespace gui::ctrl;

// --------------------------------------------------------------------------
class ImageView : public control {
public:
  typedef control super;
  typedef no_erase_window_class<ImageView> clazz;

  ImageView ();

  void create (const win::container& parent,
               const core::rectangle& place = core::rectangle::def);

  void paint (const draw::graphics& graph);
  void resized (const core::size&);

  draw::pixmap image;
  cv::Mat source;
};

// --------------------------------------------------------------------------
void ImageView::create (const win::container& parent,
                        const core::rectangle& place) {
  super::create(clazz::get(), parent, place);
  on_size(basepp::bind_method(this, &ImageView::resized));
}

// --------------------------------------------------------------------------
void ImageView::paint (const draw::graphics& graph) {
  gui::core::rectangle place = client_area();
  if (image.is_valid()) {
    graph.copy_from(image);
  }
}

// --------------------------------------------------------------------------
void ImageView::resized (const core::size& size) {
  core::native_size nsize = core::global::scale(size);

  cv::Size sz((cv::Size::value_type)nsize.width(), (cv::Size::value_type)nsize.height());
  cv::Mat outImage = cv::Mat(sz, source.type());
  cv::resize(source, outImage, sz, 0, 0, cv::INTER_LINEAR);

  if (CV_8UC3 == source.type()) {
    bitmap_info info(nsize, PixelFormat::RGB);
    const auto data_size = info.bytes_per_line * info.height;

    typedef const_image_data<PixelFormat::RGB> img_data;
    img_data::raw_type data(outImage.data, data_size);

    image = datamap<PixelFormat::RGB>(img_data(data, info));
  } else if (CV_8UC4 == source.type()) {
    bitmap_info info(nsize, PixelFormat::RGBA);
    const auto data_size = info.bytes_per_line * info.height;

    typedef const_image_data<PixelFormat::RGBA> img_data;
    img_data::raw_type data(outImage.data, data_size);

    image = datamap<PixelFormat::RGBA>(img_data(data, info));
  } else if (CV_8UC1 == source.type()) {
    bitmap_info info(nsize, PixelFormat::GRAY);
    const auto data_size = info.bytes_per_line * info.height;

    typedef const_image_data<PixelFormat::GRAY> img_data;
    img_data::raw_type data(outImage.data, data_size);

    image = datamap<PixelFormat::GRAY>(img_data(data, info));
  }

  invalidate();
}

// --------------------------------------------------------------------------
ImageView::ImageView () {
  on_paint(draw::paint(this, &ImageView::paint));
}

// --------------------------------------------------------------------------
class RedImage : public layout_main_window<gui::layout::border_layout<>, float, float, float, float> {
public:
  typedef layout_main_window<gui::layout::border_layout<>, float, float, float, float> super;
  RedImage ();

  void onCreated (window*, const core::rectangle&);
  void open ();
  void quit ();
  void show_all ();
  void show_red ();
  void show_green ();
  void show_blue ();

private:
  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu edit_sub_menu;
  popup_menu help_sub_menu;

  ImageView image_view;
  cv::Mat raw_image;
};

// --------------------------------------------------------------------------
RedImage::RedImage ()
  : super(20, 0, 0, 100)
{
  on_create(basepp::bind_method(this, &RedImage::onCreated));

  on_destroy([&]() {
    LogDebug << *this << " Destroyed!";
    win::quit_main_loop();
  });

  on_close(basepp::bind_method(this, &RedImage::quit));

}

// --------------------------------------------------------------------------
void RedImage::onCreated (win::window*, const core::rectangle&) {
  menu.data.add_entries({
    main_menu_entry("File", 'F', [&]() {
      file_sub_menu.popup(menu);
    }),
    main_menu_entry("Edit", 'E', [&]() {
      edit_sub_menu.popup(menu);
    }),
    main_menu_entry("Window", 'W', [&]() {
    }, menu_state::disabled),
    main_menu_entry("Help", 'H', [&]() {
      help_sub_menu.popup(menu);
    })
  });

  file_sub_menu.data.add_entries({
    menu_entry("Open", 'o', basepp::bind_method(this, &RedImage::open), hot_key('O', state::control), false),
    menu_entry("Exit", 'x', basepp::bind_method(this, &RedImage::quit), hot_key(keys::f4, state::alt), true)
  });

  edit_sub_menu.data.add_entries({
    menu_entry("All", 'a', basepp::bind_method(this, &RedImage::show_all), hot_key('A', state::control), false),
    menu_entry("Red", 'r', basepp::bind_method(this, &RedImage::show_red), hot_key('R', state::control), false),
    menu_entry("Green", 'g', basepp::bind_method(this, &RedImage::show_green), hot_key('G', state::control), false),
    menu_entry("Blue", 'b', basepp::bind_method(this, &RedImage::show_blue), hot_key('B', state::control), false),
  });

  help_sub_menu.data.add_entry(
    menu_entry("About", 'A', [&]() {
      message_dialog::show(*this, "About RedImage", "RedImage Version 0.0.1", "Ok");
    })
  );

  menu.create(*this);

  image_view.create(*this);

  get_layout().set_top(&menu);
  get_layout().set_center(&image_view);
  set_children_visible();
}

//-----------------------------------------------------------------------------
void RedImage::show_all () {
  image_view.source = raw_image;
  image_view.resized(image_view.client_size());
}
//-----------------------------------------------------------------------------
void RedImage::show_red () {
  cv::Mat channel;
  cv::extractChannel(raw_image, channel, 0);
  image_view.source = channel;
  image_view.resized(image_view.client_size());
}
//-----------------------------------------------------------------------------
void RedImage::show_green () {
  cv::Mat channel;
  cv::extractChannel(raw_image, channel, 1);
  image_view.source = channel;
  image_view.resized(image_view.client_size());
}
//-----------------------------------------------------------------------------
void RedImage::show_blue () {
  cv::Mat channel;
  cv::extractChannel(raw_image, channel, 2);
  image_view.source = channel;
  image_view.resized(image_view.client_size());
}

//-----------------------------------------------------------------------------
void RedImage::quit () {
  yes_no_dialog::ask(*this, "Question!", "Do you realy want to exit?", "Yes", "No", [&] (bool yes) {
    if (yes) {
      win::quit_main_loop();
    } else {
      take_focus();
    }
  });
}

//-----------------------------------------------------------------------------
void RedImage::open () {
  file_open_dialog::show(*this, "Open File", "Open", "Cancel", [&] (const sys_fs::path& file) {
    if (sys_fs::exists(file)) {
      raw_image = cv::imread(file.string(), cv::IMREAD_COLOR);
      show_all();
    }
  });
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  RedImage main;

  main.create({50, 50, 800, 600});
  main.set_title("RedImage");
  main.set_visible();

  return run_main_loop();
}
