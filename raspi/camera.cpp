/**
 * @copyright (c) 2018-2018 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui raspi lib
 *
 * Customer   -
 *
 * @brief     C++ API: Raspi camera interface.
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>
#include <sstream>
#include <iomanip>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>
#include <util/ostreamfmt.h>
#include <logging/logger.h>

// --------------------------------------------------------------------------
//
// Project includes
//
#include "camera.h"

#include <interface/mmal/util/mmal_util.h>
#include <interface/mmal/util/mmal_util_params.h>
#include <interface/mmal/util/mmal_default_components.h>
#include <bcm_host.h>

// --------------------------------------------------------------------------
//
// Source:
//

namespace gui {

  namespace raspi {

    namespace camera {
#     define MMAL_CAMERA_PREVIEW_PORT 0
#     define MMAL_CAMERA_VIDEO_PORT   1
#     define MMAL_CAMERA_CAPTURE_PORT 2

      // --------------------------------------------------------------------------
      std::ostream& operator<< (std::ostream& out, const raspi_camera& cam) {
        cam.show_current(out);
        return out;
      }

      std::ostream& operator<< (std::ostream& out, const raspi_camera::awb_gains& gains) {
        out << "{red:" << gains.r_gain << ", blue:" << gains.b_gain << "}";
        return out;
      }

      std::istream& operator>> (std::istream& in, raspi_camera::awb_gains& gains) {
        char delemiter;
        in >> gains.r_gain >> delemiter >> gains.b_gain;
        return in;
      }

      std::ostream& operator<< (std::ostream& out, const raspi_camera::color_fx& v) {
        out << "{enable:" << v.enable << ", u:" << v.u << ", v:" << v.v << "}";
        return out;
      }

      std::ostream& operator<< (std::ostream& out, const raspi_camera::stereo_mode& v) {
        out << "{mode:" << v.mode << ", decimate:" << v.decimate << ", swap_eyes:" << v.swap_eyes << "}";
        return out;
      }

      std::ostream& operator<< (std::ostream& out, const raspi_camera::size& v) {
        out << v.width << ", " << v.height;
        return out;
      }

      std::istream& operator>> (std::istream& in, raspi_camera::size& sz) {
        char delemiter;
        in >> sz.width >> delemiter >> sz.height;
        return in;
      }

      namespace {
        struct AWBModeName {
          raspi_camera::AWBMode mode;
          std::string name;
        };

#       define mkAWBModeName(a) { raspi_camera::AWBMode::a, #a }

        AWBModeName awbnames[] = {
          mkAWBModeName(Off),
          mkAWBModeName(Auto),
          mkAWBModeName(Sunlight),
          mkAWBModeName(Cloudy),
          mkAWBModeName(Shade),
          mkAWBModeName(Tungsten),
          mkAWBModeName(Fluorescent),
          mkAWBModeName(Incandescent),
          mkAWBModeName(Flash),
          mkAWBModeName(Horizon),
          mkAWBModeName(Greyworld)
        };
      }

      std::ostream& operator<< (std::ostream& out, const raspi_camera::AWBMode& m) {
        if (m <= raspi_camera::AWBMode::Greyworld) {
          out << awbnames[static_cast<int>(m)].name;
        } else {
          out << "Unknown:" << static_cast<int>(m);
        }
        return out;
      }

      std::istream& operator>> (std::istream& in, raspi_camera::AWBMode& m) {
        std::string str;
        in >> str;
        if (str.size() > 0) {
          str[0] = std::toupper(str[0]);
          for (AWBModeName& an : awbnames) {
            if (an.name == str) {
              m = an.mode;
              break;
            }
          }
        }
        return in;
      }

      // --------------------------------------------------------------------------
      static void camera_control_callback (MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *b) {
        core::buffer buffer(b);
        if (buffer.get_command() == MMAL_EVENT_PARAMETER_CHANGED) {
          MMAL_EVENT_PARAMETER_CHANGED_T *param = (MMAL_EVENT_PARAMETER_CHANGED_T *)buffer.get_data();
          switch (param->hdr.id) {
            case MMAL_PARAMETER_CAMERA_SETTINGS: {
              MMAL_PARAMETER_CAMERA_SETTINGS_T *settings = (MMAL_PARAMETER_CAMERA_SETTINGS_T*)param;
              LogDebug << "camera_control_callback for port:'" << port->name
                       << "' encoding:'" << core::four_cc(port->format->encoding)
                       << "' crop:" << port->format->es->video.crop
                       << " size:" << port->format->es->video.width << " x " << port->format->es->video.width
                       << " color_space:'" << core::four_cc(port->format->es->video.color_space) << "'";
              // variant:'" << four_cc(port->format->encoding_variant) << "'";
              LogInfo << "Exposure now " << settings->exposure
                      << ", analog gain " << settings->analog_gain.num << "/" << settings->analog_gain.den
                      << ", digital gain " << settings->digital_gain.num << "/" << settings->digital_gain.den;
              LogInfo << "AWB R=" << settings->awb_red_gain.num << "/" << settings->awb_red_gain.den << ", B=" << settings->awb_blue_gain.num << "/" << settings->awb_blue_gain.den;
              break;
            }
            default:
              LogTrace << "camera_control_callback with id:" << param->hdr.id;
              break;
          }
        } else if (buffer.get_command() == MMAL_EVENT_ERROR) {
          LogError << "No data received from sensor. Check all connections, including the Sunny one on the camera board";
        } else {
          LogError << "Received unexpected camera control callback event, 0x%08x", buffer.get_command();
        }
      }

      // --------------------------------------------------------------------------
      raspi_camera::raspi_camera (int num) {
        bcm_host_init();
        init(num);
      }

      raspi_camera::~raspi_camera () {
        try {
          fini();
        } catch (std::exception& ex) {
          LogFatal << ex;
        }
      }

      // --------------------------------------------------------------------------
      void raspi_camera::init (int num) {
        m_camera.create(MMAL_COMPONENT_DEFAULT_CAMERA);
        if (!m_camera.num_output_ports()) {
            throw std::runtime_error("Camera doesn't have output ports");
        }

        set_stereo_mode(stereo_mode{.mode = MMAL_STEREOSCOPIC_MODE_NONE, .decimate = false, .swap_eyes = false});
        set_camera_num(num);
        set_sensor_mode(SensorModeV2::SM_3280x2464_4_3_video_still_low_fps);

        MMAL_PARAMETER_CAMERA_INFO_CAMERA_T info = get_camera_info();

        m_camera_config = {
          .hdr = { MMAL_PARAMETER_CAMERA_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_CONFIG_T) },
          .max_stills_w = info.max_width,
          .max_stills_h = info.max_height,
          .stills_yuv422 = 0,
          .one_shot_stills = 1,
          .max_preview_video_w = 1024,
          .max_preview_video_h = 768,
          .num_preview_video_frames = 3,
          .stills_capture_circular_buffer_height = 0,
          .fast_preview_resume = 0,
          .use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC
        };

        set_defaults(10000);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::fini () {
        disable();
        LogTrace << "m_camera.destroy()" << logging::flush();
        m_preview_connection.destroy();
        m_null_preview.destroy();
        m_camera.destroy();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::enable () {
        LogTrace << "raspi_camera::enable()";

        core::port preview_port = m_camera.preview_port();
        core::port video_port = m_camera.video_port();
        core::port still_port = m_camera.still_port();
        core::port control_port = m_camera.control_port();

        control_port.enable(camera_control_callback);

        set_camera_config(m_camera_config);

        //set_raw_mode(false);

        MMAL_ES_SPECIFIC_FORMAT_T format = preview_port.get_specific_format();

        format.video.width = VCOS_ALIGN_UP(640, 32);
        format.video.height = VCOS_ALIGN_UP(480, 16);
        format.video.crop.x = 0;
        format.video.crop.y = 0;
        format.video.crop.width = 640;
        format.video.crop.height = 480;
        format.video.frame_rate.num = 0;
        format.video.frame_rate.den = 1;

        preview_port.set_specific_format(format);
        preview_port.set_encoding(MMAL_ENCODING_OPAQUE);
        preview_port.commit_format_change();

        video_port.copy_full_format_from(preview_port);
        video_port.commit_format_change();

        video_port.set_bool(MMAL_PARAMETER_ZERO_COPY, true);

        format = still_port.get_specific_format();

        format.video.width = VCOS_ALIGN_UP(m_camera_config.max_stills_w, 32);
        format.video.height = VCOS_ALIGN_UP(m_camera_config.max_stills_h, 16);
        format.video.crop.x = 0;
        format.video.crop.y = 0;
        format.video.crop.width = m_camera_config.max_stills_w;
        format.video.crop.height = m_camera_config.max_stills_h;
        format.video.frame_rate.num = 0;
        format.video.frame_rate.den = 1;

        still_port.set_specific_format(format);
        still_port.set_encoding(MMAL_ENCODING_OPAQUE);
        still_port.commit_format_change();

        m_camera.enable();

        m_null_preview.create("vc.null_sink");
        m_null_preview.enable();

        core::port preview_input = m_null_preview.input_port(0);

        m_preview_connection.connect(preview_port, preview_input,
                                     MMAL_CONNECTION_FLAG_TUNNELLING | MMAL_CONNECTION_FLAG_ALLOCATION_ON_INPUT);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::disable () {
        LogTrace << "raspi_camera::disable()";
        if (m_null_preview.is_enabled()) {
          m_null_preview.disable();
        }
        core::port video_port = m_camera.video_port();
        if (video_port.is_enabled()) {
          video_port.disable();
        }
        if (m_camera.is_enabled()) {
          m_camera.disable();
        }
      }

      // --------------------------------------------------------------------------
      core::port raspi_camera::get_still_output_port () const {
        return m_camera.still_port();
      }

      core::port raspi_camera::get_control_port () const {
        return m_camera.control_port();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::capture () {
        LogTrace << "raspi_camera::capture()";
        m_camera.still_port().capture();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_defaults (uint32_t speed, uint32_t iso) {
        set_saturation(0);
        set_sharpness(0);
        set_contrast(0);
        set_brightness(0.5F);
        set_iso(iso);
        set_video_stabilisation(0);
        set_exposure_compensation(0);
        set_exposure_mode(MMAL_PARAM_EXPOSUREMODE_AUTO);
        set_flicker_avoid_mode(MMAL_PARAM_FLICKERAVOID_OFF);
        set_metering_mode(MMAL_PARAM_EXPOSUREMETERINGMODE_AVERAGE);
        set_awb_mode(AWBMode::Auto);
        set_awb_gains({1.0F, 1.0F});
        set_image_fx(MMAL_PARAM_IMAGEFX_NONE);
        set_color_fx({false, 128, 128});
        set_rotation(0);
        set_flip(MMAL_PARAM_MIRROR_NONE);
//        set_crop({0.0F, 0.0F, 1.0F, 1.0F});
        set_shutter_speed(speed);
        set_drc(MMAL_PARAMETER_DRC_STRENGTH_OFF);
        set_stats_pass(MMAL_FALSE);
        set_analog_gain(1.0F);
        set_digital_gain(1.0F);

//        MMAL_PARAMETER_CHANGE_EVENT_REQUEST_T change_event_request =
//           {{MMAL_PARAMETER_CHANGE_EVENT_REQUEST, sizeof(MMAL_PARAMETER_CHANGE_EVENT_REQUEST_T)},
//            MMAL_PARAMETER_CAMERA_SETTINGS, 1};

//        m_camera.control_port().set(change_event_request.hdr);

      }

      void raspi_camera::show_current (std::ostream& out) const {
        out << "saturation:" << get_saturation();
        out << ", sharpness:" << get_sharpness();
        out << ", contrast:" << get_contrast();
        out << ", brightness:" << get_brightness();
        out << ", iso:" << get_iso();
        out << ", shutter_speed:" << get_shutter_speed();
        out << ", analog_gain:" << get_analog_gain();
        out << ", digital_gain:" << get_digital_gain();
        out << ", size[w,h]:{" << get_size() << "}";

        out << ", video_stabilisation:" << get_video_stabilisation();
        out << ", exposure_compensation:" << get_exposure_compensation();
        out << ", exposure_mode:" << get_exposure_mode();
        out << ", flicker_avoid_mode:" << get_flicker_avoid_mode();
        out << ", metering_mode:" << get_metering_mode();
        out << ", awb_mode:" << get_awb_mode();
        out << ", awb_gains:" << get_awb_gains();

        out << ", stereo_mode:" << get_stereo_mode();

        out << ", image_fx:" << get_image_fx();
        out << ", color_fx:" << get_color_fx();
        out << ", flip:" << get_flip();
        out << ", crop:" << get_input_crop();
        //out << ", abs_crop:" << get_abs_crop();
        out << ", rotation:" << get_rotation();
        out << ", drc:" << get_drc();
        out << ", stats_pass:" << get_stats_pass();

        out << ", camera_config:" << get_camera_config();
        out << ", camera_info:" << get_camera_info();
//        out << ", resize:" << get_resize();
//        out << ", camera_rx_config:" << get_camera_rx_config();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_rational (uint32_t id, float v, int den) {
        m_camera.control_port().set_float(id, v, den);
      }

      void raspi_camera::set_rational (uint32_t id, float v, float min, float max, int den) {
        if ((min <= v) && (v <= max)) {
           set_rational(id, v, den);
        } else {
           throw std::invalid_argument(ostreamfmt("Invalid rational value (" << min << " <= " << v << " <= " << max));
        }
      }

      float raspi_camera::get_rational (uint32_t id) const {
        return m_camera.control_port().get_float(id);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_saturation (float saturation) {
        set_rational(MMAL_PARAMETER_SATURATION, saturation, -1.0F, 1.0F);
      }

      float raspi_camera::get_saturation () const {
        return get_rational(MMAL_PARAMETER_SATURATION);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_sharpness (float sharpness) {
        set_rational(MMAL_PARAMETER_SHARPNESS, sharpness, -1.0F, 1.0F);
      }

      float raspi_camera::get_sharpness () const {
        return get_rational(MMAL_PARAMETER_SHARPNESS);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_contrast (float contrast) {
        set_rational(MMAL_PARAMETER_CONTRAST, contrast, -1.0F, 1.0F);
      }

      float raspi_camera::get_contrast () const {
        return get_rational(MMAL_PARAMETER_CONTRAST);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_brightness (float brightness) {
        set_rational(MMAL_PARAMETER_BRIGHTNESS, brightness, 0, 1.0F);
      }

      float raspi_camera::get_brightness () const {
        return get_rational(MMAL_PARAMETER_BRIGHTNESS);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_sensor_mode (SensorModeV1 mode) {
        set_sensor_mode(static_cast<SensorMode>(mode));
      }

      void raspi_camera::set_sensor_mode (SensorModeV2 mode) {
        set_sensor_mode(static_cast<SensorMode>(mode));
      }

      void raspi_camera::set_sensor_mode (SensorMode mode) {
        m_camera.control_port().set_uint32(MMAL_PARAMETER_CAMERA_CUSTOM_SENSOR_CONFIG, mode);
      }

      auto raspi_camera::get_sensor_mode () const -> SensorMode {
        return m_camera.control_port().get_uint32(MMAL_PARAMETER_CAMERA_CUSTOM_SENSOR_CONFIG);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_resolution (const size& sz) {
        m_camera_config.max_stills_w = sz.width;
        m_camera_config.max_stills_h = sz.height;
        set_camera_config(m_camera_config);
      }

      auto raspi_camera::get_resolution () const -> size {
        return {m_camera_config.max_stills_w, m_camera_config.max_stills_h};
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_iso (uint32_t iso) {
        m_camera.control_port().set_uint32(MMAL_PARAMETER_ISO, iso);
      }

      uint32_t raspi_camera::get_iso () const {
        return m_camera.control_port().get_uint32(MMAL_PARAMETER_ISO);
      }

      uint32_t raspi_camera::get_min_iso () const {
        return m_camera.control_port().get_uint32(MMAL_PARAMETER_CAMERA_MIN_ISO);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_shutter_speed (uint32_t speed) {
        m_camera.control_port().set_uint32(MMAL_PARAMETER_SHUTTER_SPEED, speed);
      }

      uint32_t raspi_camera::get_shutter_speed () const {
        return m_camera.control_port().get_uint32(MMAL_PARAMETER_SHUTTER_SPEED);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_analog_gain (float gain) {
        set_rational(MMAL_PARAMETER_ANALOG_GAIN, gain, 65536);
      }

      float raspi_camera::get_analog_gain () const {
        return get_rational(MMAL_PARAMETER_ANALOG_GAIN);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_digital_gain (float gain) {
        set_rational(MMAL_PARAMETER_DIGITAL_GAIN, gain, 65536);
      }

      float raspi_camera::get_digital_gain () const {
        return get_rational(MMAL_PARAMETER_DIGITAL_GAIN);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_video_stabilisation (bool vstabilisation) {
        m_camera.control_port().set_bool(MMAL_PARAMETER_VIDEO_STABILISATION, vstabilisation);
      }

      bool raspi_camera::get_video_stabilisation () const {
        return m_camera.control_port().get_bool(MMAL_PARAMETER_VIDEO_STABILISATION);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_stats_pass (bool stats_pass) {
        m_camera.control_port().set_bool(MMAL_PARAMETER_CAPTURE_STATS_PASS, stats_pass);
      }

      bool raspi_camera::get_stats_pass () const {
        return m_camera.control_port().get_bool(MMAL_PARAMETER_CAPTURE_STATS_PASS);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_exposure_compensation (int exp_comp) {
        m_camera.control_port().set_uint32(MMAL_PARAMETER_EXPOSURE_COMP, exp_comp);
      }

      int raspi_camera::get_exposure_compensation () const {
        return m_camera.control_port().get_uint32(MMAL_PARAMETER_EXPOSURE_COMP);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_burst_mode (bool burst) {
        m_camera.control_port().set_bool(MMAL_PARAMETER_CAMERA_BURST_CAPTURE, burst);
      }

      bool raspi_camera::get_burst_mode () const {
        return m_camera.control_port().get_bool(MMAL_PARAMETER_CAMERA_BURST_CAPTURE);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_raw_mode (bool raw) {
        m_camera.control_port().set_bool(MMAL_PARAMETER_ENABLE_RAW_CAPTURE, raw);
      }

      bool raspi_camera::get_raw_mode () const {
        return m_camera.control_port().get_bool(MMAL_PARAMETER_ENABLE_RAW_CAPTURE);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_awb_gains (const awb_gains& gains) {
        MMAL_PARAMETER_AWB_GAINS_T param = {{MMAL_PARAMETER_CUSTOM_AWB_GAINS,sizeof(param)}, {0,0}, {0,0}};

        param.r_gain.den = param.b_gain.den = 65536;
        param.r_gain.num = (unsigned int)(gains.r_gain * 65536);
        param.b_gain.num = (unsigned int)(gains.b_gain * 65536);
        m_camera.control_port().set(param.hdr);
      }

      auto raspi_camera::get_awb_gains () const -> awb_gains {
        MMAL_PARAMETER_AWB_GAINS_T param = {{MMAL_PARAMETER_CUSTOM_AWB_GAINS,sizeof(param)}, {0,0}, {0,0}};
        m_camera.control_port().get(param.hdr);

        return awb_gains{
          .r_gain = static_cast<float>(param.r_gain.num) / static_cast<float>(param.r_gain.den),
          .b_gain = static_cast<float>(param.b_gain.num) / static_cast<float>(param.b_gain.den)
        };
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_rotation (int rotation) {
        int my_rotation = ((rotation % 360 ) / 90) * 90;
        for (int i = 0; i < 2; ++i) {
          m_camera.output_port(i).set_int32(MMAL_PARAMETER_ROTATION, my_rotation);
        }
      }

      int raspi_camera::get_rotation () const {
        return m_camera.preview_port().get_int32(MMAL_PARAMETER_ROTATION);
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_flip (MMAL_PARAM_MIRROR_T mode) {
        MMAL_PARAMETER_MIRROR_T mirror = {{MMAL_PARAMETER_MIRROR, sizeof(MMAL_PARAMETER_MIRROR_T)}, mode};
        for (int i = 0; i < 2; ++i) {
          m_camera.output_port(i).set(mirror.hdr);
        }
      }

      MMAL_PARAM_MIRROR_T raspi_camera::get_flip () const {
        return m_camera.output_port(0).get<MMAL_PARAMETER_MIRROR_T>(MMAL_PARAMETER_MIRROR);
      }

#define USE_CROP
#define USE_ES_VIDEOxx


      // --------------------------------------------------------------------------
      void raspi_camera::set_input_crop (const core::crop& c) {
#ifdef USE_CROP
        MMAL_PARAMETER_INPUT_CROP_T crop = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)},
                                            {static_cast<int32_t>(c.x * 65535.0F), static_cast<int32_t>(c.y * 65535.0F),
                                             static_cast<int32_t>(c.width * 65535.0F), static_cast<int32_t>(c.height * 65535.0F)}};
        m_camera.control_port().set(crop.hdr);
#endif // USE_CROP
#ifdef USE_SET_SIZE
        uint32_t w = static_cast<uint32_t>(c.width * m_sensor_size.width);
        uint32_t h = static_cast<uint32_t>(c.height * m_sensor_size.height);

        uint32_t w_up = VCOS_ALIGN_UP(w, 32);
        uint32_t h_up = VCOS_ALIGN_UP(h, 16);

        set_size({w_up, h_up});
#endif // USE_SET_SIZE
#ifdef USE_ES_VIDEO
        auto still = m_camera.still_port();
        auto fmt = still.get_specific_format();
        fmt.video.crop.x = static_cast<uint32_t>(c.x * m_sensor_size.width);
        fmt.video.crop.y = static_cast<uint32_t>(c.y * m_sensor_size.height);
        fmt.video.crop.width = w;
        fmt.video.crop.height = h;
        fmt.video.width = w_up;
        fmt.video.height = h_up;
        still.set_specific_format(fmt);
        still.commit_format_change();
#endif // USE_ES_VIDEO
      }

      core::crop raspi_camera::get_input_crop () const {
#ifdef USE_CROP
        MMAL_PARAMETER_INPUT_CROP_T cr = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)}};
        m_camera.control_port().get(cr.hdr);
        auto& c = cr.rect;
        return {c.x / 65535.0F, c.y / 65535.0F, c.width / 65535.0F, c.height / 65535.0F};
#endif // USE_CROP
#ifdef USE_ES_VIDEO
        MMAL_RECT_T& c = m_camera.still_port->format->es->video.crop;
        return {(float)c.x / m_sensor_size.width, (float)c.y / m_sensor_size.height,
                (float)c.width / m_sensor_size.width, (float)c.height / m_sensor_size.height};
#endif // USE_ES_VIDEO
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_abs_crop (const MMAL_RECT_T& c) {
//        m_camera.control_port().set_crop(c);
#ifdef USE_CROP
        float fx = 65535.0F / (float)m_sensor_size.width;
        float fy = 65535.0F / (float)m_sensor_size.height;
        MMAL_PARAMETER_INPUT_CROP_T crop = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)},
                                            {static_cast<int32_t>(c.x * fx), static_cast<int32_t>(c.y * fy),
                                             static_cast<int32_t>(c.width * fx), static_cast<int32_t>(c.height * fy)}};
        m_camera.control_port().set(crop.hdr);
#endif // USE_CROP
#ifdef USE_SET_SIZE
        uint32_t w_up = VCOS_ALIGN_UP(c.width, 32);
        uint32_t h_up = VCOS_ALIGN_UP(c.height, 16);

        set_size({w_up, h_up});
#endif // USE_SET_SIZE
#ifdef USE_ES_VIDEO
        auto still = m_camera.still_port();
        auto fmt = still.get_specific_format();
        fmt.video.crop.x = c.x;
        fmt.video.crop.y = c.y;
        fmt.video.crop.width = c.width;
        fmt.video.crop.height = c.height;
        fmt.video.width = w_up;
        fmt.video.height = h_up;
        still.set_specific_format(fmt);
        still.commit_format_change();
#endif // USE_ES_VIDEO

      }

      MMAL_RECT_T raspi_camera::get_abs_crop () const {
        return m_camera.control_port().get_crop();
#ifdef USE_INPUT_CROP
        float fx = (float)m_sensor_size.width / 65535.0F;
        float fy = (float)m_sensor_size.height / 65535.0F;

        MMAL_PARAMETER_INPUT_CROP_T cr = {{MMAL_PARAMETER_INPUT_CROP, sizeof(MMAL_PARAMETER_INPUT_CROP_T)}};
        m_camera.control_port().get(cr.hdr);
        auto& c = cr.rect;
        return {static_cast<int32_t>(c.x * fx), static_cast<int32_t>(c.y * fy),
                static_cast<int32_t>(c.width * fx), static_cast<int32_t>(c.height * fy)};
#endif // USE_CROP
#ifdef USE_ES_VIDEO
        return m_camera_still_port->format->es->video.crop;
#endif // USE_ES_VIDEO
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_color_fx (const color_fx& c) {
        MMAL_PARAMETER_COLOURFX_T colfx = {{MMAL_PARAMETER_COLOUR_EFFECT, sizeof(MMAL_PARAMETER_COLOURFX_T)}, c.enable, c.u, c.v};
        m_camera.control_port().set(colfx.hdr);
      }

      auto raspi_camera::get_color_fx () const -> color_fx {
        MMAL_PARAMETER_COLOURFX_T colfx = {{MMAL_PARAMETER_COLOUR_EFFECT, sizeof(MMAL_PARAMETER_COLOURFX_T)}, 0, 0, 0};
        m_camera.control_port().get(colfx.hdr);
        return color_fx{
          .enable = colfx.enable != 0,
          .u = colfx.u,
          .v = colfx.v
        };
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_stereo_mode (const stereo_mode& sm) {
        MMAL_PARAMETER_STEREOSCOPIC_MODE_T stereo = { {MMAL_PARAMETER_STEREOSCOPIC_MODE, sizeof(MMAL_PARAMETER_STEREOSCOPIC_MODE_T)},
                                    sm.mode, sm.decimate, sm.swap_eyes};
        for (int i = 0; i < 2; ++i) {
          m_camera.output_port(i).set(stereo.hdr);
        }
      }

      auto raspi_camera::get_stereo_mode () const -> stereo_mode {
        // Seems to be not imlemented yet!
//        MMAL_PARAMETER_STEREOSCOPIC_MODE_T stereo = { {MMAL_PARAMETER_STEREOSCOPIC_MODE, sizeof(MMAL_PARAMETER_STEREOSCOPIC_MODE_T)},
//                                    MMAL_STEREOSCOPIC_MODE_NONE, MMAL_FALSE, MMAL_FALSE };
//        mmal_port_parameter_get(m_camera->output[2], &stereo.hdr);
        return stereo_mode{
          .mode = MMAL_STEREOSCOPIC_MODE_NONE/*stereo.mode*/,
          .decimate = MMAL_FALSE/*stereo.decimate != 0*/,
          .swap_eyes = MMAL_FALSE/*stereo.swap_eyes != 0*/
        };
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_drc (MMAL_PARAMETER_DRC_STRENGTH_T strength) {
        MMAL_PARAMETER_DRC_T drc = {{MMAL_PARAMETER_DYNAMIC_RANGE_COMPRESSION, sizeof(MMAL_PARAMETER_DRC_T)}, strength};
        m_camera.control_port().set(drc.hdr);
      }

      MMAL_PARAMETER_DRC_STRENGTH_T raspi_camera::get_drc () const {
        MMAL_PARAMETER_DRC_T drc = {{MMAL_PARAMETER_DYNAMIC_RANGE_COMPRESSION, sizeof(MMAL_PARAMETER_DRC_T)}};
        m_camera.control_port().get(drc.hdr);
        return drc.strength;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_size (const size& sz) {
        m_current_size = sz;
//        MMAL_RECT_T& crop = m_camera_still_port->format->es->video.crop;
//        crop.width = sz.width;
//        crop.height = sz.height;
      }

      auto raspi_camera::get_size () const -> size {
        return m_current_size;
//        MMAL_RECT_T& crop = m_camera_still_port->format->es->video.crop;
//        return {(uint32_t)crop.width, (uint32_t)crop.height};
      }

      // --------------------------------------------------------------------------
      uint32_t raspi_camera::get_pixel_per_line () const {
        return VCOS_ALIGN_UP(m_current_size.width, 32);
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_CAMERA_CONFIG_T raspi_camera::get_camera_config () const {
//        MMAL_PARAMETER_CAMERA_CONFIG_T camConfig = {{MMAL_PARAMETER_CAMERA_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_CONFIG_T)}};
//        m_camera.control_port().get(camConfig.hdr);
        return m_camera_config;
      }

      void raspi_camera::set_camera_config (const MMAL_PARAMETER_CAMERA_CONFIG_T& camConfig) {
//        bool was_enabled = m_camera.control_port().is_enabled();
//        m_camera.disable();
        m_camera_config = camConfig;
        m_camera_config.hdr = {MMAL_PARAMETER_CAMERA_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_CONFIG_T)};
        m_camera.control_port().set(m_camera_config.hdr);
        m_current_size = m_sensor_size = {camConfig.max_stills_w, camConfig.max_stills_h};
//        if (was_enabled) {
//          m_camera.enable();
//        }
      }

      void raspi_camera::config_camera (const size& max_still, const size& max_preview) {
        m_camera_config.max_stills_w = max_still.width;
        m_camera_config.max_stills_h = max_still.height;
        m_camera_config.max_preview_video_w = max_preview.width;
        m_camera_config.max_preview_video_h = max_preview.height;
        set_camera_config(m_camera_config);
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_CAMERA_INFO_CAMERA_T raspi_camera::get_camera_info () const {
        return raspi_camera::get_camera_info(get_camera_num());
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_CAMERA_INFO_CAMERA_T raspi_camera::get_camera_info (int num) {
        if (num < 0) {
          num = 0;
        }
        MMAL_PARAMETER_CAMERA_INFO_T config = raspi_camera::get_config();
        if (num < config.num_cameras) {
          return config.cameras[num];
        }
        throw std::invalid_argument("camera number is invalid");
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_CAMERA_INFO_T raspi_camera::get_config () {
        MMAL_COMPONENT_T* component = nullptr;
        try {
          check_mmal_status(mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA_INFO, &component));

          MMAL_PARAMETER_CAMERA_INFO_T config = {{MMAL_PARAMETER_CAMERA_INFO, sizeof(MMAL_PARAMETER_CAMERA_INFO_T) - 4}};
          auto status = mmal_port_parameter_get(component->control, &config.hdr);
          if (status != MMAL_SUCCESS) {
            config.hdr.size = sizeof(MMAL_PARAMETER_CAMERA_INFO_T);
            status = mmal_port_parameter_get(component->control, &config.hdr);
          }
          if (status != MMAL_SUCCESS) {
            config.num_cameras = 1;
            config.cameras[0].max_width = 2592;
            config.cameras[0].max_height = 1944;
          }
          check_mmal_status(mmal_component_destroy(component));
          return config;
        } catch (std::exception& ex) {
          mmal_component_destroy(component);
          throw ex;
        }
      }

      auto raspi_camera::get_sensor_size () const -> size {
        return m_sensor_size;
      }

      // --------------------------------------------------------------------------
      // Seems to be not imlemented yet!
#ifdef CAMERA_RX_CONFIG_AVAILABLE
      MMAL_PARAMETER_CAMERA_RX_CONFIG_T raspi_camera::get_camera_rx_config () const {
        MMAL_PARAMETER_CAMERA_RX_CONFIG_T cam_rx_config = {{MMAL_PARAMETER_CAMERA_RX_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_RX_CONFIG_T)}};
        m_camera.control_port().get(cam_rx_config.hdr);
        return cam_rx_config;
      }

      void raspi_camera::set_camera_rx_config (MMAL_PARAMETER_CAMERA_RX_CONFIG_T cam_rx_config) {
        cam_rx_config.hdr = {MMAL_PARAMETER_CAMERA_RX_CONFIG, sizeof(MMAL_PARAMETER_CAMERA_RX_CONFIG_T)};
        m_camera.control_port().set(cam_rx_config.hdr);
      }
#endif // CAMERA_RX_CONFIG_AVAILABLE

#ifdef RESIZE_AVAILABLE
      // --------------------------------------------------------------------------
      MMAL_PARAMETER_RESIZE_T raspi_camera::get_resize () const {
        MMAL_PARAMETER_RESIZE_T resize = {{MMAL_PARAMETER_RESIZE_PARAMS, sizeof(MMAL_PARAMETER_RESIZE_T)}/*, MMAL_RESIZE_DUMMY, 0*/};
        m_camera.control_port().get(resize.hdr);
        return resize;
      }

      void raspi_camera::set_resize (MMAL_PARAMETER_RESIZE_T resize) {
        resize.hdr = {MMAL_PARAMETER_RESIZE_PARAMS, sizeof(MMAL_PARAMETER_RESIZE_T)};
        m_camera.control_port().set(resize.hdr);
      }
#endif // RESIZE_AVAILABLE

      // --------------------------------------------------------------------------
      template<typename T>
      struct mode {
        using type = void;
        static constexpr int id = 0;
      };

      // --------------------------------------------------------------------------
      template<>
      struct mode<MMAL_PARAM_EXPOSUREMETERINGMODE_T> {
        using type = MMAL_PARAMETER_EXPOSUREMETERINGMODE_T;
        static constexpr int id = MMAL_PARAMETER_EXP_METERING_MODE;
      };

      template<>
      struct mode<MMAL_PARAM_EXPOSUREMODE_T> {
        using type = MMAL_PARAMETER_EXPOSUREMODE_T;
        static constexpr int id = MMAL_PARAMETER_EXPOSURE_MODE;
      };

      template<>
      struct mode<MMAL_PARAM_FLICKERAVOID_T> {
        using type = MMAL_PARAMETER_FLICKERAVOID_T;
        static constexpr int id = MMAL_PARAMETER_FLICKER_AVOID;
      };

      template<>
      struct mode<MMAL_PARAM_AWBMODE_T> {
        using type = MMAL_PARAMETER_AWBMODE_T;
        static constexpr int id = MMAL_PARAMETER_AWB_MODE;
      };

      template<>
      struct mode<MMAL_PARAM_IMAGEFX_T> {
        using type = MMAL_PARAMETER_IMAGEFX_T;
        static constexpr int id = MMAL_PARAMETER_IMAGE_EFFECT;
      };

      template<>
      struct mode<int32_t> {
        using type = MMAL_PARAMETER_INT32_T;
        static constexpr int id = MMAL_PARAMETER_CAMERA_NUM;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      void raspi_camera::set_mode (T m) {
        typename mode<T>::type param = {{mode<T>::id, sizeof(param)}, m};
        m_camera.control_port().set(param.hdr);
      }

      template<typename T>
      T raspi_camera::get_mode () const {
        typename mode<T>::type param = {{mode<T>::id, sizeof(param)}};
        m_camera.control_port().get(param.hdr);
        return param.value;
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_metering_mode (MMAL_PARAM_EXPOSUREMETERINGMODE_T mode) {
        set_mode<MMAL_PARAM_EXPOSUREMETERINGMODE_T>(mode);
      }

      MMAL_PARAM_EXPOSUREMETERINGMODE_T raspi_camera::get_metering_mode () const {
        return get_mode<MMAL_PARAM_EXPOSUREMETERINGMODE_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_exposure_mode (MMAL_PARAM_EXPOSUREMODE_T mode) {
        set_mode<MMAL_PARAM_EXPOSUREMODE_T>(mode);
      }

      MMAL_PARAM_EXPOSUREMODE_T raspi_camera::get_exposure_mode () const {
        return get_mode<MMAL_PARAM_EXPOSUREMODE_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_flicker_avoid_mode (MMAL_PARAM_FLICKERAVOID_T mode) {
        set_mode<MMAL_PARAM_FLICKERAVOID_T>(mode);
      }

      MMAL_PARAM_FLICKERAVOID_T raspi_camera::get_flicker_avoid_mode () const {
        return get_mode<MMAL_PARAM_FLICKERAVOID_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_awb_mode (AWBMode awb_mode) {
        set_mode<MMAL_PARAM_AWBMODE_T>(static_cast<MMAL_PARAM_AWBMODE_T>(awb_mode));
      }

      auto raspi_camera::get_awb_mode () const -> AWBMode {
        return static_cast<AWBMode>(get_mode<MMAL_PARAM_AWBMODE_T>());
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_image_fx (MMAL_PARAM_IMAGEFX_T imageFX) {
        set_mode<MMAL_PARAM_IMAGEFX_T>(imageFX);
      }

      MMAL_PARAM_IMAGEFX_T raspi_camera::get_image_fx () const {
        return get_mode<MMAL_PARAM_IMAGEFX_T>();
      }

      // --------------------------------------------------------------------------
      void raspi_camera::set_camera_num (int32_t num) {
        m_camera_num = num;
        set_mode<int32_t>(num);
      }

      int32_t raspi_camera::get_camera_num () const {
        return m_camera_num;//get_mode<int32_t>();
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------

    } // camera

  } // raspi

} // gui

namespace std {
  std::ostream& operator<< (std::ostream& out, const gui::raspi::core::four_cc& fourcc) {
    if (fourcc.type.uint32) {
      out.write(fourcc.type.char4, 4);
    } else {
      out << "[" << fourcc.type.uint32 << "]";
    }
    return out;
  }

  std::istream& operator>> (std::istream& in, gui::raspi::core::four_cc& fourcc) {
    in >> *((char*)&fourcc.type.uint32);
    in >> *(((char*)&fourcc.type.uint32)+1);
    in >> *(((char*)&fourcc.type.uint32)+2);
    in >> *(((char*)&fourcc.type.uint32)+3);
    return in;
  }

  std::ostream& operator<< (std::ostream& out, const gui::raspi::core::crop& v) {
    out << "{x:" << v.x << ", y:" << v.y << ", w:" << v.width << ", h:" << v.height << "}";
    return out;
  }

  std::istream& operator>> (std::istream& in, gui::raspi::core::crop& v) {
    char delemiter;
    in >> v.x >> delemiter >> v.y >> delemiter >> v.width >> delemiter >> v.height;
    return in;
  }

  // --------------------------------------------------------------------------
  std::ostream& operator<< (std::ostream& out, const MMAL_RECT_T& v) {
    out << "{x:" << v.x << ", y:" << v.y << ", w:" << v.width << ", h:" << v.height << "}";
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_RATIONAL_T& v) {
    out << v.num << "/" << v.den;
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_PARAMETER_CAMERA_CONFIG_T& v) {
    out << "{w:" << v.max_stills_w << ", h:" << v.max_stills_w
        << ", stills_yuv422:" << v.stills_yuv422 << ", one_shot_stills:" << v.one_shot_stills
        << ", max_preview_video_w:" << v.max_preview_video_w << ", max_preview_video_h:" << v.max_preview_video_h
        << ", num_preview_video_frames:" << v.num_preview_video_frames << ", stills_capture_circular_buffer_height:" << v.stills_capture_circular_buffer_height
        << ", fast_preview_resume:" << v.fast_preview_resume<< ", use_stc_timestamp:" << v.use_stc_timestamp
        << "}";
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_PARAMETER_CAMERA_INFO_CAMERA_T& v) {
    out << "{name:" << v.camera_name << ", lens:" << v.lens_present
        << ", w:" << v.max_width  << ", h:" << v.max_height << "}";
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_PARAMETER_CAMERA_RX_CONFIG_T& v) {
    out << "{decode:" << v.decode << ", encode:" << v.encode
        << ", unpack:" << v.unpack << ", pack:" << v.pack
        << ", data_lanes:" << v.data_lanes << ", encode_block_length:" << v.encode_block_length
        << ", embedded_data_lines:" << v.embedded_data_lines << ", image_id:" << v.image_id
        << "}";
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_PARAMETER_RESIZE_T& v) {
    out << "{mode:" << v.mode << ", max_bytes:" << v.max_bytes
        << ", w:" << v.max_width << ", h:" << v.max_height
        << ", aspect:" << v.preserve_aspect_ratio << ", upscaling:" << v.allow_upscaling
        << "}";
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_VIDEO_FORMAT_T& v) {
    out << "{w:" << v.width << ", h:" << v.height
        << ", crop:" << v.crop<< ", framerate:" << v.frame_rate
        << ", par:" << v.par << ", fourcc:" << gui::raspi::core::four_cc(v.color_space)
        << "}";
    return out;
  }

  std::ostream& operator<< (std::ostream& out, const MMAL_RESIZEMODE_T v) {
    switch (v) {
      case MMAL_RESIZE_NONE:  out << "NONE"; break;
      case MMAL_RESIZE_CROP:  out << "CROP"; break;
      case MMAL_RESIZE_BOX:   out << "BOX"; break;
      case MMAL_RESIZE_BYTES: out << "BYTES"; break;
      case MMAL_RESIZE_DUMMY: out << "DUMMY"; break;
      default:                out << "UNKNOWN"; break;
    }
    return out;
  }

}
