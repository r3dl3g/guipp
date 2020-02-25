/**
 * @copyright (c) 2018-2020 Ing. Buero Rothfuss
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

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>
#include <fstream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "core.h"


namespace raspi {

  namespace camera {

    // --------------------------------------------------------------------------
    class still {
    public:
      typedef uint32_t SensorMode;

      enum class SensorModeV1 : SensorMode {
        SM_1920x1080_16_9_video_partial_fov = 1,
        SM_2592x1944_4_3_video_still_low_fps,
        SM_2592x1944_4_3_video_still_high_fps,
        SM_1296x972_4_3_video_2x2,
        SM_1296x730_16_9_video_2x2,
        SM_640x480_4_3_video_4x4_low_fps,
        SM_640x480_4_3_video_4x4_high_fps
      };

      enum class SensorModeV2 : SensorMode {
        SM_1920x1080_16_9_video_partial_fov = 1,
        SM_3280x2464_4_3_video_still_low_fps,
        SM_3280x2464_4_3_video_still_high_fps,
        SM_1640x1232_4_3_video_2x2,
        SM_1640x922_16_9_video_2x2,
        SM_1280x720_16_9_video_partial_fov_2x2,
        SM_640x480_4_3_video_partial_fov_2x2
      };

      enum class OutEncoding : MMAL_FOURCC_T {
        OPAC =  MMAL_ENCODING_OPAQUE,
        I420 =  MMAL_ENCODING_I420_SLICE,
        NV12 =  MMAL_ENCODING_NV12,
        I422 =  MMAL_ENCODING_I422_SLICE,
        YUYV =  MMAL_ENCODING_YUYV,
        YVYU =  MMAL_ENCODING_YVYU,
        VYUY =  MMAL_ENCODING_VYUY,
        UYVY =  MMAL_ENCODING_UYVY,
        BGR24 = MMAL_ENCODING_BGR24_SLICE,
        BGRA =  MMAL_ENCODING_BGRA_SLICE,
        RGB16 = MMAL_ENCODING_RGB16_SLICE,
        YV12 =  MMAL_ENCODING_YV12,
        NV21 =  MMAL_ENCODING_NV21,
        RGB24 = MMAL_ENCODING_RGB24_SLICE,
        RGBA =  MMAL_ENCODING_RGBA_SLICE
      };

      enum class AWBMode : uint8_t {
         Off          = MMAL_PARAM_AWBMODE_OFF,
         Auto         = MMAL_PARAM_AWBMODE_AUTO,
         Sunlight     = MMAL_PARAM_AWBMODE_SUNLIGHT,
         Cloudy       = MMAL_PARAM_AWBMODE_CLOUDY,
         Shade        = MMAL_PARAM_AWBMODE_SHADE,
         Tungsten     = MMAL_PARAM_AWBMODE_TUNGSTEN,
         Fluorescent  = MMAL_PARAM_AWBMODE_FLUORESCENT,
         Incandescent = MMAL_PARAM_AWBMODE_INCANDESCENT,
         Flash        = MMAL_PARAM_AWBMODE_FLASH,
         Horizon      = MMAL_PARAM_AWBMODE_HORIZON,
         Greyworld    = MMAL_PARAM_AWBMODE_GREYWORLD
      };

      struct size {
        uint32_t width;
        uint32_t height;
      };

      struct awb_gains {
        float r_gain;
        float b_gain;
      };

      struct color_fx {
        bool enable;
        uint32_t u;
        uint32_t v;
      };

      struct stereo_mode {
        MMAL_STEREOSCOPIC_MODE_T mode;
        bool decimate;
        bool swap_eyes;
      };

      still (int num = 0);
      ~still ();

      void set_defaults (uint32_t speed, uint32_t iso = 100);
      void show_current (std::ostream& out) const;

      void set_sensor_mode (SensorModeV1 mode);
      void set_sensor_mode (SensorModeV2 mode);
      void set_sensor_mode (SensorMode mode);
      SensorMode get_sensor_mode () const;

      void set_resolution (const size&);
      size get_resolution () const;

      void set_saturation (float saturation);
      float get_saturation () const;

      void set_sharpness (float sharpness);
      float get_sharpness () const;

      void set_contrast (float contrast);
      float get_contrast () const;

      void set_brightness (float brightness);
      float get_brightness () const;

      void set_iso (uint32_t iso);
      uint32_t get_iso () const;

      uint32_t get_min_iso () const;

      void set_shutter_speed (uint32_t speed);
      uint32_t get_shutter_speed () const;

      void set_analog_gain (float gain);
      float get_analog_gain () const;

      void set_digital_gain (float gain);
      float get_digital_gain () const;

      void set_metering_mode (MMAL_PARAM_EXPOSUREMETERINGMODE_T mode);
      MMAL_PARAM_EXPOSUREMETERINGMODE_T get_metering_mode () const;

      void set_exposure_mode (MMAL_PARAM_EXPOSUREMODE_T mode);
      MMAL_PARAM_EXPOSUREMODE_T get_exposure_mode () const;

      void set_flicker_avoid_mode (MMAL_PARAM_FLICKERAVOID_T mode);
      MMAL_PARAM_FLICKERAVOID_T get_flicker_avoid_mode () const;

      void set_awb_mode (AWBMode awb_mode);
      AWBMode get_awb_mode () const;

      void set_video_stabilisation (bool vstabilisation);
      bool get_video_stabilisation () const;

      void set_exposure_compensation (int exp_comp);
      int get_exposure_compensation () const;

      void set_burst_mode (bool);
      bool get_burst_mode () const;

      void set_raw_mode (bool);
      bool get_raw_mode () const;

      void set_awb_gains (const awb_gains&);
      awb_gains get_awb_gains () const;

      void set_rotation (int rotation);
      int get_rotation () const;

      void set_flip (MMAL_PARAM_MIRROR_T mode);
      MMAL_PARAM_MIRROR_T get_flip () const;

      void set_input_crop (const core::crop&);
      core::crop get_input_crop () const;

      void set_abs_crop (const MMAL_RECT_T&);
      MMAL_RECT_T get_abs_crop () const;

      void set_stats_pass (bool stats_pass);
      bool get_stats_pass () const;

      void set_image_fx (MMAL_PARAM_IMAGEFX_T imageFX);
      MMAL_PARAM_IMAGEFX_T get_image_fx () const;

      void set_color_fx (const color_fx&);
      color_fx get_color_fx () const;

      void set_drc (MMAL_PARAMETER_DRC_STRENGTH_T drc);
      MMAL_PARAMETER_DRC_STRENGTH_T get_drc () const;

      void set_stereo_mode (const stereo_mode&);
      stereo_mode get_stereo_mode () const;

      void set_size (const size&);
      size get_size () const;

      uint32_t get_pixel_per_line () const;

      MMAL_PARAMETER_CAMERA_CONFIG_T get_camera_config () const;
      void set_camera_config (const MMAL_PARAMETER_CAMERA_CONFIG_T& config);
      void config_camera (const size& max_still, const size& max_preview);

      MMAL_PARAMETER_CAMERA_INFO_CAMERA_T get_camera_info () const;

      static MMAL_PARAMETER_CAMERA_INFO_CAMERA_T get_camera_info (int num);
      static MMAL_PARAMETER_CAMERA_INFO_T get_config ();

      size get_sensor_size () const;

#ifdef CAMERA_RX_CONFIG_AVAILABLE
      MMAL_PARAMETER_CAMERA_RX_CONFIG_T get_camera_rx_config () const;
      void set_camera_rx_config (MMAL_PARAMETER_CAMERA_RX_CONFIG_T config);
#endif // CAMERA_RX_CONFIG_AVAILABLE

      void capture ();

      void enable ();
      void disable ();

      core::port get_still_output_port () const;
      core::port get_control_port () const;

    protected:

      void init (int num);
      void fini ();

    private:
      void set_rational (uint32_t id, float v, float min, float max, int den = 100);
      void set_rational (uint32_t id, float v, int den = 100);
      float get_rational (uint32_t id) const;

      template<typename T>
      void set_mode (T mode);

      template<typename T>
      T get_mode () const;

      void set_camera_num (int32_t num);
      int32_t get_camera_num () const;

      core::component m_camera;
      core::component m_null_preview;
      core::connection m_preview_connection;

      size m_sensor_size;
      size m_current_size;

      int32_t m_camera_num;

      MMAL_PARAMETER_CAMERA_CONFIG_T m_camera_config;
    };

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream&, const still&);
    std::ostream& operator<< (std::ostream&, const still::awb_gains&);
    std::ostream& operator<< (std::ostream&, const still::color_fx&);
    std::ostream& operator<< (std::ostream&, const still::stereo_mode&);
    std::ostream& operator<< (std::ostream&, const still::size&);
    std::ostream& operator<< (std::ostream&, const still::AWBMode&);

    std::istream& operator>> (std::istream&, still::awb_gains&);
    std::istream& operator>> (std::istream&, still::size&);
    std::istream& operator>> (std::istream&, still::AWBMode&);

    // --------------------------------------------------------------------------
    static std::array<still::AWBMode, 11> AWBModes = {
      still::AWBMode::Off,
      still::AWBMode::Auto,
      still::AWBMode::Sunlight,
      still::AWBMode::Cloudy,
      still::AWBMode::Shade,
      still::AWBMode::Tungsten,
      still::AWBMode::Fluorescent,
      still::AWBMode::Incandescent,
      still::AWBMode::Flash,
      still::AWBMode::Horizon,
      still::AWBMode::Greyworld
    };

    // --------------------------------------------------------------------------

  } // namespace camera

} // namespace raspi

// --------------------------------------------------------------------------
namespace std {
  ostream& operator<< (ostream&, const MMAL_PARAMETER_CAMERA_CONFIG_T&);
  ostream& operator<< (ostream&, const MMAL_PARAMETER_CAMERA_INFO_CAMERA_T&);
  ostream& operator<< (ostream&, const MMAL_PARAMETER_CAMERA_RX_CONFIG_T&);
  ostream& operator<< (ostream&, const MMAL_PARAMETER_RESIZE_T&);
  ostream& operator<< (ostream&, const MMAL_VIDEO_FORMAT_T&);
  ostream& operator<< (ostream&, const MMAL_RESIZEMODE_T);
}
// --------------------------------------------------------------------------
