/**
 * @copyright (c) 2018-2019 Ing. Buero Rothfuss
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


namespace gui {

  namespace raspi {

    namespace camera {

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      class raspi_camera {
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
          I420 =  MMAL_ENCODING_I420,
          NV12 =  MMAL_ENCODING_NV12,
          I422 =  MMAL_ENCODING_I422,
          YUYV =  MMAL_ENCODING_YUYV,
          YVYU =  MMAL_ENCODING_YVYU,
          VYUY =  MMAL_ENCODING_VYUY,
          UYVY =  MMAL_ENCODING_UYVY,
          BGR24 = MMAL_ENCODING_BGR24,
          BGRA =  MMAL_ENCODING_BGRA,
          RGB16 = MMAL_ENCODING_RGB16,
          YV12 =  MMAL_ENCODING_YV12,
          NV21 =  MMAL_ENCODING_NV21,
          RGB24 = MMAL_ENCODING_RGB24,
          RGBA =  MMAL_ENCODING_RGBA
        };

        raspi_camera (int num = 0);
        ~raspi_camera ();

        void set_defaults (int speed, int iso = 100);
        void show_current (std::ostream& out) const;

        void set_sensor_mode (SensorModeV1 mode);
        void set_sensor_mode (SensorModeV2 mode);
        void set_sensor_mode (SensorMode mode);
        SensorMode get_sensor_mode () const;

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

        void set_shutter_speed (int speed);
        int get_shutter_speed () const;

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

        void set_awb_mode (MMAL_PARAM_AWBMODE_T awb_mode);
        MMAL_PARAM_AWBMODE_T get_awb_mode () const;

        void set_video_stabilisation (bool vstabilisation);
        bool get_video_stabilisation () const;

        void set_exposure_compensation (int exp_comp);
        int get_exposure_compensation () const;

        void set_burst_mode (bool);
        bool get_burst_mode () const;

        void set_raw_mode (bool);
        bool get_raw_mode () const;

        struct awb_gains {
          float r_gain;
          float b_gain;
        };

        void set_awb_gains (const awb_gains&);
        awb_gains get_awb_gains () const;

        void set_rotation (int rotation);
        int get_rotation () const;

        void set_flip (MMAL_PARAM_MIRROR_T mode);
        MMAL_PARAM_MIRROR_T get_flip () const;

        struct crop {
          crop (float x_ = 0.0F, float y_ = 0.0F, float w_ = 1.0F, float h_ = 1.0F)
            : x(x_) , y(y_) , width(w_) , height(h_)
          {}

          float x;
          float y;
          float width;
          float height;
        };

        void set_crop (const crop&);
        crop get_crop () const;

        void set_abs_crop (const MMAL_RECT_T&);
        MMAL_RECT_T get_abs_crop () const;

        void set_stats_pass (bool stats_pass);
        bool get_stats_pass () const;

        void set_image_fx (MMAL_PARAM_IMAGEFX_T imageFX);
        MMAL_PARAM_IMAGEFX_T get_image_fx () const;

        struct color_fx {
          bool enable;
          uint32_t u;
          uint32_t v;
        };

        void set_color_fx (const color_fx&);
        color_fx get_color_fx () const;

        void set_drc (MMAL_PARAMETER_DRC_STRENGTH_T drc);
        MMAL_PARAMETER_DRC_STRENGTH_T get_drc () const;

        struct stereo_mode {
          MMAL_STEREOSCOPIC_MODE_T mode;
          bool decimate;
          bool swap_eyes;
        };

        void set_stereo_mode (const stereo_mode&);
        stereo_mode get_stereo_mode () const;

        struct size {
          uint32_t width;
          uint32_t height;
        };

        void set_size (const size&);
        size get_size () const;

        uint32_t get_pixel_per_line () const;

        MMAL_PARAMETER_CAMERA_CONFIG_T get_camera_config () const;
        void set_camera_config (MMAL_PARAMETER_CAMERA_CONFIG_T config);

        MMAL_PARAMETER_CAMERA_INFO_CAMERA_T get_camera_info (int num = -1) const;
        MMAL_PARAMETER_CAMERA_INFO_T get_config () const;

#ifdef CAMERA_RX_CONFIG_AVAILABLE
        MMAL_PARAMETER_CAMERA_RX_CONFIG_T get_camera_rx_config () const;
        void set_camera_rx_config (MMAL_PARAMETER_CAMERA_RX_CONFIG_T config);
#endif // CAMERA_RX_CONFIG_AVAILABLE

#ifdef RESIZE_AVAILABLE
        MMAL_PARAMETER_RESIZE_T get_resize () const;
        void set_resize (MMAL_PARAMETER_RESIZE_T config);
#endif // RESIZE_AVAILABLE

        void capture ();

        void enable ();
        void disable ();

        core::port get_still_output_port () const;

      protected:

        void init (int num);
        void fini ();

      private:
        void set_rational (uint32_t id, float v, float min, float max);
        void set_rational (uint32_t id, float v);
        float get_rational (uint32_t id) const;

        template<typename T>
        void set_mode (T mode);

        template<typename T>
        T get_mode () const;

        void set_camera_num (int32_t num);
        int32_t get_camera_num () const;

        core::component m_camera;
//        port m_camera_still_port;

        size m_sensor_size;
        size m_current_size;

      };

      // --------------------------------------------------------------------------
      std::ostream& operator<< (std::ostream&, const raspi_camera&);
      std::ostream& operator<< (std::ostream&, const raspi_camera::awb_gains&);
      std::ostream& operator<< (std::ostream&, const raspi_camera::crop&);
      std::ostream& operator<< (std::ostream&, const raspi_camera::color_fx&);
      std::ostream& operator<< (std::ostream&, const raspi_camera::stereo_mode&);
      std::ostream& operator<< (std::ostream&, const raspi_camera::size&);

      std::istream& operator>> (std::istream&, raspi_camera::awb_gains&);
      std::istream& operator>> (std::istream&, raspi_camera::crop&);
      std::istream& operator>> (std::istream&, raspi_camera::size&);

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------

    } // namespace camera

  } // namespace raspi

} // namespace gui

namespace std {
  std::ostream& operator<< (std::ostream&, const gui::raspi::core::four_cc&);
  std::istream& operator>> (std::istream&, gui::raspi::core::four_cc&);
  // --------------------------------------------------------------------------
  ostream& operator<< (ostream&, const MMAL_RECT_T&);
  ostream& operator<< (ostream&, const MMAL_PARAMETER_CAMERA_CONFIG_T&);
  ostream& operator<< (ostream&, const MMAL_PARAMETER_CAMERA_INFO_CAMERA_T&);
  ostream& operator<< (ostream&, const MMAL_PARAMETER_CAMERA_RX_CONFIG_T&);
  ostream& operator<< (ostream&, const MMAL_PARAMETER_RESIZE_T&);
  ostream& operator<< (ostream&, const MMAL_VIDEO_FORMAT_T&);
  ostream& operator<< (ostream&, const MMAL_RATIONAL_T&);

  ostream& operator<< (ostream&, const MMAL_RESIZEMODE_T);
}
