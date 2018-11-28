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
#include <gui/core/color.h>

// --------------------------------------------------------------------------
#include <interface/mmal/mmal.h>
#include <interface/mmal/util/mmal_connection.h>

namespace gui {

  namespace raspi {

    namespace camera {

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      class raspi_camera {
      public:
        raspi_camera (int num = 0);
        ~raspi_camera ();

        void set_saturation (float saturation);
        float get_saturation ();

        void set_sharpness (float sharpness);
        float get_sharpness ();

        void set_contrast (float contrast);
        float get_contrast ();

        void set_brightness (float brightness);
        float get_brightness ();

        void set_iso (uint32_t iso);
        uint32_t get_iso ();

        void set_shutter_speed (int speed);
        int get_shutter_speed ();

        void set_analog_gain (float gain);
        float get_analog_gain ();

        void set_digital_gain (float gain);
        float get_digital_gain ();

        void set_metering_mode (MMAL_PARAM_EXPOSUREMETERINGMODE_T mode);
        MMAL_PARAM_EXPOSUREMETERINGMODE_T get_metering_mode ();

        void set_exposure_mode (MMAL_PARAM_EXPOSUREMODE_T mode);
        MMAL_PARAM_EXPOSUREMODE_T get_exposure_mode ();

        void set_flicker_avoid_mode (MMAL_PARAM_FLICKERAVOID_T mode);
        MMAL_PARAM_FLICKERAVOID_T get_flicker_avoid_mode ();

        void set_awb_mode (MMAL_PARAM_AWBMODE_T awb_mode);
        MMAL_PARAM_AWBMODE_T get_awb_mode ();

        void set_video_stabilisation (bool vstabilisation);
        bool get_video_stabilisation ();

        void set_exposure_compensation (int exp_comp);
        int get_exposure_compensation ();

        void set_awb_gains (float r_gain, float b_gain);
        void get_awb_gains (float& r_gain, float& b_gain);

        void set_rotation (int rotation);
        int get_rotation ();

        void set_flip (MMAL_PARAM_MIRROR_T mode);
        MMAL_PARAM_MIRROR_T get_flip ();

        void set_crop (float x, float y, float w, float h);
        void get_crop (float& x, float& y, float& w, float& h);

        void set_stats_pass (bool stats_pass);
        bool get_stats_pass ();

        void set_image_fx (MMAL_PARAM_IMAGEFX_T imageFX);
        MMAL_PARAM_IMAGEFX_T get_image_fx ();

        void set_color_fx (bool enable, uint32_t u, uint32_t v);
        void get_color_fx (bool& enable, uint32_t& u, uint32_t& v);

        void set_drc (MMAL_PARAMETER_DRC_STRENGTH_T drc);
        MMAL_PARAMETER_DRC_STRENGTH_T get_drc ();

        void set_stereo_mode (MMAL_STEREOSCOPIC_MODE_T mode, bool decimate = false, bool swap_eyes = false);
        void get_stereo_mode (MMAL_STEREOSCOPIC_MODE_T& mode, bool& decimate, bool& swap_eyes);

        void set_size (float w, float h);
        void get_size (int& w, int& h);

        MMAL_PARAMETER_CAMERA_CONFIG_T get_camera_config ();
        void set_camera_config (MMAL_PARAMETER_CAMERA_CONFIG_T config);

        MMAL_PARAMETER_CAMERA_INFO_CAMERA_T get_camera_info (int num);
        MMAL_PARAMETER_CAMERA_INFO_T get_config ();

      private:
        void set_rational (uint32_t id, float v, float min, float max);
        void set_rational (uint32_t id, float v);
        float get_rational (uint32_t id);

        template<typename T>
        void set_mode (T mode);

        template<typename T>
        T get_mode ();

        void set_camera_num (int32_t num);
        int32_t get_camera_num ();

        MMAL_COMPONENT_T* m_camera;
        MMAL_COMPONENT_T* m_encoder;
        MMAL_CONNECTION_T* m_encoder_connection;
        MMAL_POOL_T* m_encoder_pool;
        MMAL_PORT_T* m_camera_still_port;
        MMAL_PORT_T* m_encoder_input_port;
        MMAL_PORT_T* m_encoder_output_port;
      };

    } // namespace camera

  } // namespace raspi

} // namespace gui

