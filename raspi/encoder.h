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
 * @brief     C++ API: Raspi encoder interface.
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <sstream>
#include <string>

// --------------------------------------------------------------------------
//
// Library includes
//

// --------------------------------------------------------------------------
#include "camera.h"

namespace gui {

  namespace raspi {

//    namespace encoder {
//      // --------------------------------------------------------------------------

//      struct source {
//        core::port input_port;
//      };

//      struct target {
//        core::port output_port;
//      };

//      struct camera : public source {
//        camera () {}
//      };

//      struct file : public target {
//        file () {}

//        void connect (source&);
//      };

//      struct buffer : public target {
//        buffer() {}

//        void connect (source&);
//      };

//      struct encoder : public target, source {
//        encoder() {}

//        void connect (source&);
//      };

//      // --------------------------------------------------------------------------

//    } // namespace encoder

    namespace camera {

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      class raspi_encoder {
      public:
        using image_data = std::vector<uint8_t>;

        const image_data& get_data () const;

        void clear_data ();

        core::port& get_source_output_port ();
        const core::port& get_source_output_port () const;

        core::pool& get_buffer_pool ();
        const core::pool& get_buffer_pool () const;

      protected:
        raspi_encoder (core::port source_output_port);
        ~raspi_encoder ();

        using buffer_type = std::vector<uint8_t>;

        void capture (uint32_t timeout);

        static void callback_dispatcher (MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);
        void handle_callback (core::port&, core::buffer&);

        void add_data (const core::buffer&);

        core::semaphore m_complete_semaphore;

        buffer_type m_buffer;
        core::pool m_buffer_pool;

        core::port m_source_output_port;

        void init ();
      };

      // --------------------------------------------------------------------------
      class raspi_raw_encoder : public raspi_encoder {
      public:
        using super = raspi_encoder;
        using OutEncoding = raspi_camera::OutEncoding;

        raspi_raw_encoder (core::port source_output_port, OutEncoding encoding = OutEncoding::OPAC);
        ~raspi_raw_encoder ();

        void capture (uint32_t timeout = 1000);
        void enable ();
        void disable ();

        core::port get_output_port () const;
        core::port get_input_port () const;

        core::four_cc get_encoding () const;
        void set_encoding (core::four_cc enc);

      protected:
        void init (MMAL_FOURCC_T encoding);

      };

      // --------------------------------------------------------------------------
      class raspi_image_encoder : public raspi_encoder {
      public:
        using super = raspi_encoder;

        enum class InEncoding : MMAL_FOURCC_T {
          RGB16 = MMAL_ENCODING_RGB16,
          RGB24 = MMAL_ENCODING_RGB24,
          RGBA  = MMAL_ENCODING_RGBA,
          BGRA  = MMAL_ENCODING_BGRA,
          I420  = MMAL_ENCODING_I420,
          I422  = MMAL_ENCODING_I422,
          NV12  = MMAL_ENCODING_NV12,
          YUYV  = MMAL_ENCODING_YUYV,
          YVYU  = MMAL_ENCODING_YVYU,
          VYUY  = MMAL_ENCODING_VYUY,
          OPAC  = MMAL_ENCODING_OPAQUE,
        };

        enum class OutEncoding : MMAL_FOURCC_T {
          JPEG  = MMAL_ENCODING_JPEG,
          GIF   = MMAL_ENCODING_GIF,
          PNG   = MMAL_ENCODING_PNG,
          BMP   = MMAL_ENCODING_BMP,
          PPM   = MMAL_ENCODING_PPM,
          TGA   = MMAL_ENCODING_TGA,
          OPAC  = MMAL_ENCODING_OPAQUE,
        };

        raspi_image_encoder (core::port source_output_port, OutEncoding encoding);
        ~raspi_image_encoder ();

        void capture (uint32_t timeout = 1000);
        void enable ();
        void disable ();

        core::port get_output_port () const;
        core::port get_input_port () const;

        const core::connection& get_encoder_connection () const;
        core::connection& get_encoder_connection ();

        OutEncoding get_encoding () const;
        void set_encoding (OutEncoding enc);

      protected:
        raspi_image_encoder (core::port source_output_port);

        core::component m_encoder;
        core::connection m_encoder_connection;

        MMAL_FOURCC_T m_encoding;

        void init (MMAL_FOURCC_T encoding);
      };

      // --------------------------------------------------------------------------
      class raspi_resizer : public raspi_image_encoder {
      public:
        using super = raspi_image_encoder;

        enum class InEncoding : MMAL_FOURCC_T {
          RGBA        = MMAL_ENCODING_RGBA,
          RGBA_SLICE  = MMAL_ENCODING_RGBA_SLICE,
          BGRA        = MMAL_ENCODING_BGRA,
          BGRA_SLICE  = MMAL_ENCODING_BGRA_SLICE,
          RGB16       = MMAL_ENCODING_RGB16,
          RGB16_SLICE = MMAL_ENCODING_RGB16_SLICE,
          I420        = MMAL_ENCODING_I420,
          I420_SLICE  = MMAL_ENCODING_I420_SLICE,
          OPAC        = MMAL_ENCODING_OPAQUE,
        };

        enum class OutEncoding : MMAL_FOURCC_T {
          RGBA        = MMAL_ENCODING_RGBA,
          RGBA_SLICE  = MMAL_ENCODING_RGBA_SLICE,
          BGRA        = MMAL_ENCODING_BGRA,
          BGRA_SLICE  = MMAL_ENCODING_BGRA_SLICE,
          RGB16       = MMAL_ENCODING_RGB16,
          RGB16_SLICE = MMAL_ENCODING_RGB16_SLICE,
          I420        = MMAL_ENCODING_I420,
          I420_SLICE  = MMAL_ENCODING_I420_SLICE,
          OPAC        = MMAL_ENCODING_OPAQUE,
        };

        raspi_resizer (core::port source_output_port, OutEncoding encoding);
        ~raspi_resizer ();

        OutEncoding get_encoding () const;
      };

      // --------------------------------------------------------------------------
      class raspi_isp : public raspi_image_encoder {
      public:
        using super = raspi_image_encoder;

        enum class InEncoding : MMAL_FOURCC_T {
          BAYER_SBGGR8       = MMAL_ENCODING_BAYER_SBGGR8,
          BAYER_SBGGR10DPCM8 = MMAL_ENCODING_BAYER_SBGGR10DPCM8,
          BAYER_SBGGR10P     = MMAL_ENCODING_BAYER_SBGGR10P,
          BAYER_SBGGR12P     = MMAL_ENCODING_BAYER_SBGGR12P,
          YUYV               = MMAL_ENCODING_YUYV,
          YVYU               = MMAL_ENCODING_YVYU,
          VYUY               = MMAL_ENCODING_VYUY,
          UYVY               = MMAL_ENCODING_UYVY,
          I420               = MMAL_ENCODING_I420,
          YV12               = MMAL_ENCODING_YV12,
          I422               = MMAL_ENCODING_I422,
          RGB24              = MMAL_ENCODING_RGB24,
          BGR24              = MMAL_ENCODING_BGR24,
          RGBA               = MMAL_ENCODING_RGBA,
          BGRA               = MMAL_ENCODING_BGRA,
          RGB16              = MMAL_ENCODING_RGB16,
          YUVUV128           = MMAL_ENCODING_YUVUV128,
          NV12               = MMAL_ENCODING_NV12,
          NV21               = MMAL_ENCODING_NV21,
          OPAC               = MMAL_ENCODING_OPAQUE,
        };

        enum class OutEncoding : MMAL_FOURCC_T {
          YUYV               = MMAL_ENCODING_YUYV,
          YVYU               = MMAL_ENCODING_YVYU,
          VYUY               = MMAL_ENCODING_VYUY,
          UYVY               = MMAL_ENCODING_UYVY,
          I420               = MMAL_ENCODING_I420,
          YV12               = MMAL_ENCODING_YV12,
          I422               = MMAL_ENCODING_I422,
          RGB24              = MMAL_ENCODING_RGB24,
          BGR24              = MMAL_ENCODING_BGR24,
          RGBA               = MMAL_ENCODING_RGBA,
          BGRA               = MMAL_ENCODING_BGRA,
          RGB16              = MMAL_ENCODING_RGB16,
          YUVUV128           = MMAL_ENCODING_YUVUV128,
          NV12               = MMAL_ENCODING_NV12,
          NV21               = MMAL_ENCODING_NV21,
          OPAC               = MMAL_ENCODING_OPAQUE,
        };

        raspi_isp (core::port source_output_port, OutEncoding encoding);
        ~raspi_isp ();

        OutEncoding get_encoding () const;
      };

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------

    } // namespace camera

  } // namespace raspi

} // namespace gui

