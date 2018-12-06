
#include "raspi/encoder.h"
#include "base/ostreamfmt.h"
#include "base/command_line.h"


#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

template<typename T>
T parse_arg (const std::string& str, const std::string& name) {
  T t = T();
  std::istringstream(str) >> t;
  LogInfo << "Found " << name << ":'" << t << "'";
  return t;
}

// --------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
  LogDebug << "Create raspi_camera";

  using namespace gui::raspi::camera;
  raspi_camera camera;

  camera.set_defaults(1000);

  MMAL_FOURCC_T encoding = 0;
  bool raw = false;
  std::string outname = "raspicam_test";

  basepp::command_line::parser("raspicam_test V 0.1.0",
  {
    {"-e", "--encoding", "<FOURCC>", "Use <FOURCC> encoding (BMP, PNG(default), PPM, JPEG, GIF, TGA)",
      [&](const std::string& arg) {
        encoding = parse_arg<four_cc>(arg, "encoding").m_type;
      }},
    {"-r", "--raw", {}, "Use raw capture (-f: BD10, bRA8, bGA8, BGGR, RGAA, I420, S420, I422, S422, RGBA, rgba, RGB3, rgb3)",
      [&](const std::string&) {
        raw = true;
        LogInfo << "Enable raw";
      }},
    {"-ss", "--shutter", "<us>", "Set shutter speed in us",
      [&](const std::string& arg) {
        camera.set_shutter_speed(parse_arg<int>(arg, "Shutter speed"));
      }},
    {"-a", "--awb", "<R,B>", "Set AWB gains red (R) and blue (B)",
      [&](const std::string& arg) {
        camera.set_awb_mode(MMAL_PARAM_AWBMODE_OFF);
        camera.set_awb_gains(parse_arg<raspi_camera::awb_gains>(arg, "AWB gains"));
      }},
    {"-c", "--crop", "<X,Y,W,H>", "Crop image by <X,Y,W,H> in normalised coordinates [0.0-1.0]",
      [&](const std::string& arg) {
        camera.set_crop(parse_arg<raspi_camera::crop>(arg, "Crop"));
      }},
    {"-sz", "--size", "<W,H>", "Use image size <W,H>",
      [&](const std::string& arg) {
        camera.set_size(parse_arg<raspi_camera::size>(arg, "Size"));
      }},
    {"-rz", "--resize", "<W,H>", "Use image resize <W,H>",
      [&](const std::string& arg) {
        auto sz = parse_arg<raspi_camera::size>(arg, "Resize");
        camera.set_resize({{}, MMAL_RESIZE_CROP, sz.w, sz.h, 0, false, false});
      }},
    {"-i", "--iso", "<ISO>", "Set sensor ISO",
      [&](const std::string& arg) {
        camera.set_iso(parse_arg<int>(arg, "ISO"));
      }},
    {"-sh", "--sharpness", "[-1.0-1.0]", "Set image sharpness",
      [&](const std::string& arg) {
        camera.set_sharpness(parse_arg<float>(arg, "Sharpness"));
      }},
    {"-co", "--contrast", "[-1.0-1.0]", "Set image contrast",
      [&](const std::string& arg) {
        camera.set_contrast(parse_arg<float>(arg, "Contrast"));
      }},
    {"-br", "--brightness", "[0.0-1.0]", "Set image brightness",
      [&](const std::string& arg) {
        camera.set_brightness(parse_arg<float>(arg, "Brightness"));
      }},
    {"-sa", "--saturation", "[-1.0-1.0]", "Set image saturation",
      [&](const std::string& arg) {
        camera.set_saturation(parse_arg<float>(arg, "Saturation"));
      }},
    {"-o", "--out", "<filename>", "Set output file name",
      [&](const std::string& arg) {
        outname = basepp::string::trimed(arg);
        LogInfo << "Found output file name:'" << outname << "'";
      }}
  }).process(argc, argv);

  LogInfo << "Camera info: " << camera;

  if (raw) {
    if (!encoding) {
      encoding = MMAL_ENCODING_RGB24_SLICE;
    }
    raspi_raw_encoder encoder(camera, encoding);
    LogInfo << "raw capture " << four_cc(encoding);
    encoder.capture(5000);

    raspi_encoder::image_data data = encoder.get_data();

    LogInfo << "get_data size:" << data.length();

    std::ofstream file(ostreamfmt(outname << "." << four_cc(encoding)));
    file.write((const char*)data.c_str(), data.length());

  } else {
    if (!encoding) {
      encoding = MMAL_ENCODING_PNG;
    }
    raspi_image_encoder encoder(camera, encoding);
    LogInfo << "encoded capture " << four_cc(encoding);
    encoder.capture(10000);

    auto data = encoder.get_data();

    LogInfo << "get_data size:" << data.length();

    std::ofstream file(ostreamfmt(outname << "." << four_cc(encoding)));
    file.write((const char*)data.c_str(), data.length());
  }

  basepp::log::core::instance().finish();

  return 0;
}
