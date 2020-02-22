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
#include <algorithm>

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
#include "encoder.h"

#include <interface/mmal/util/mmal_util.h>
#include <interface/mmal/util/mmal_util_params.h>
#include <interface/mmal/util/mmal_default_components.h>

// --------------------------------------------------------------------------
//
// Source:
//

#define MMAL_COMPONENT_DEFAULT_NULL_SINK "vc.null_sink"
#define MMAL_COMPONENT_DEFAULT_RESIZER   "vc.ril.resize"
#define MMAL_COMPONENT_DEFAULT_ISP       "vc.ril.isp"
#define MMAL_COMPONENT_RAW_CAMERA        "vc.ril.rawcam"

namespace gui {

  namespace raspi {

    namespace camera {

      // --------------------------------------------------------------------------
      raspi_encoder::raspi_encoder (core::port source_output_port)
        : m_source_output_port(source_output_port)
      {
        init();
      }

      // --------------------------------------------------------------------------
      raspi_encoder::~raspi_encoder () {
        m_buffer_pool.destroy();
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::init () {
        LogTrace << "Create RaspiStill-sem";
        m_complete_semaphore.create("RaspiStill-sem", 0);
      }

      // --------------------------------------------------------------------------
      core::port& raspi_encoder::get_source_output_port () {
        return m_source_output_port;
      }

      // --------------------------------------------------------------------------
      const core::port& raspi_encoder::get_source_output_port () const {
        return m_source_output_port;
      }

      // --------------------------------------------------------------------------
      core::pool& raspi_encoder::get_buffer_pool () {
        return m_buffer_pool;
      }

      // --------------------------------------------------------------------------
      const core::pool& raspi_encoder::get_buffer_pool () const {
        return m_buffer_pool;
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::capture (uint32_t timeout) {
        LogTrace << "raspi_encoder::capture(" << timeout << ")";
        clear_data();
        m_source_output_port.capture();
        m_complete_semaphore.wait(timeout);
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::add_data (const core::buffer& buf) {
        m_buffer.insert(m_buffer.end(), buf.begin(), buf.end());
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::clear_data () {
        buffer_type temp;
        m_buffer.swap(temp);
      }

      // --------------------------------------------------------------------------
      auto raspi_encoder::get_data () const -> const image_data& {
        return m_buffer;
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::callback_dispatcher (MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
        LogTrace << "handle_callback cmd:" << buffer->cmd << ", length:" << buffer->length << ", port:" << port->name;
        raspi_encoder* encoder = reinterpret_cast<raspi_encoder*>(port->userdata);
        if (encoder && (0 == buffer->cmd)) {
          core::port p(port);
          core::buffer b(buffer);
          encoder->handle_callback(p, b);
        }
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::handle_callback (core::port& port, core::buffer& buf) {
        bool complete = false;
        {
          try {
            buf.lock();
            if (buf.get_length()) {
              add_data(buf);
            }
            if (buf.get_flags() & (MMAL_BUFFER_HEADER_FLAG_FRAME_END | MMAL_BUFFER_HEADER_FLAG_TRANSMISSION_FAILED)) {
              complete = true;
            }
          } catch (std::exception& ex) {
            LogError << ex;
          } catch (...) {
            LogError << "Unknown exception in raspi_encoder::handle_callback";
          }
          buf.unlock();
          buf.release();
        }

        if (port.is_enabled() && m_buffer_pool.is_valid()) {
          core::buffer new_buffer = m_buffer_pool.get_buffer();
          if (new_buffer.is_valid()) {
            port.send_buffer(new_buffer);
          } else {
            LogWarng << "Could not aquire new buffer in raspi_encoder::handle_callback";
          }
        }

        if (complete) {
          m_complete_semaphore.post();
        }
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_raw_encoder::raspi_raw_encoder (core::port source_output_port, OutEncoding encoding)
        : super(source_output_port)
      {
        init(static_cast<MMAL_FOURCC_T>(encoding));
      }

      // --------------------------------------------------------------------------
      raspi_raw_encoder::~raspi_raw_encoder () {
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::init (MMAL_FOURCC_T encoding) {
        auto output_port = get_output_port();
        auto min = output_port.get_min_buffer_size();
        auto sz = output_port.get_buffer_size();
        if (sz.size < min.size) {
          sz.size = min.size;
        }
        if (sz.num < min.num) {
          sz.num = min.num;
        }
        output_port.set_buffer_size(sz);
        output_port.set_encoding(encoding);
        output_port.commit_format_change();

        m_buffer_pool.create(output_port);
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::enable () {
        LogTrace << "raspi_raw_encoder::enable()";
        auto output_port = get_output_port();

        output_port.set_user_data(this);
        output_port.enable(raspi_encoder::callback_dispatcher);

        const auto num = m_buffer_pool.queue_length();
        for (auto i = 0; i < num; i++ ) {
          core::buffer new_buffer = m_buffer_pool.get_buffer();
          if (new_buffer.is_valid()) {
            output_port.send_buffer(new_buffer);
          }
        }
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::disable () {
        LogTrace << "raspi_raw_encoder::disable()";
        auto output_port = get_output_port();
        output_port.flush();
        output_port.disable();
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::capture (uint32_t timeout) {
        LogTrace << "raspi_raw_encoder::capture(" << timeout << ")";
        super::capture(timeout);
      }

      core::port raspi_raw_encoder::get_output_port () const {
        return super::get_source_output_port();
      }

      core::port raspi_raw_encoder::get_input_port () const {
        return super::get_source_output_port();
      }

      // --------------------------------------------------------------------------
      core::four_cc raspi_raw_encoder::get_encoding () const {
        return super::get_source_output_port().get_encoding();
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::set_encoding (core::four_cc enc) {
        super::get_source_output_port().set_encoding(enc);
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_image_encoder::raspi_image_encoder (core::port source_output_port, OutEncoding encoding)
        : super(source_output_port)
      {
        m_encoder.create(MMAL_COMPONENT_DEFAULT_IMAGE_ENCODER);
        init(static_cast<MMAL_FOURCC_T>(encoding));
      }

      // --------------------------------------------------------------------------
      raspi_image_encoder::raspi_image_encoder (core::port source_output_port)
        : super(source_output_port)
      {}

      // --------------------------------------------------------------------------
      raspi_image_encoder::~raspi_image_encoder () {
        LogTrace << "m_encoder.destroy()" << logging::flush();
        m_encoder.destroy();
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::init (MMAL_FOURCC_T encoding) {
        LogTrace << "init raspi_image_encoder for " << encoding;
        m_encoding = encoding;

        if ( !m_encoder.num_input_ports() || !m_encoder.num_output_ports()) {
          throw std::invalid_argument(ostreamfmt("Encoder has no input/output ports (in:" << m_encoder.num_input_ports() << ", out:" << m_encoder.num_output_ports() << ")"));
        }

      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::enable () {
        LogTrace << "Enable raspi_image_encoder";

        core::port input_port = get_input_port();
        core::port output_port = get_output_port();

        output_port.copy_format_from(input_port);
        output_port.set_encoding(m_encoding);

        auto sz = output_port.get_recommended_buffer_size();
        sz |= output_port.get_min_buffer_size();
        output_port.set_buffer_size(sz);

        output_port.commit_format_change();

        output_port.set_uint32(MMAL_PARAMETER_JPEG_Q_FACTOR, 85);
        output_port.set_uint32(MMAL_PARAMETER_JPEG_RESTART_INTERVAL, 0);
        output_port.set_bool(MMAL_PARAMETER_EXIF_DISABLE, true);

        MMAL_PARAMETER_THUMBNAIL_CONFIG_T param_thumb = {{MMAL_PARAMETER_THUMBNAIL_CONFIGURATION, sizeof(MMAL_PARAMETER_THUMBNAIL_CONFIG_T)}, 0, 0, 0, 0};
        m_encoder.control_port().set(param_thumb.hdr);

        m_encoder.enable();
        m_buffer_pool.create(output_port);

        m_encoder_connection.connect(m_source_output_port, input_port,
                                     MMAL_CONNECTION_FLAG_TUNNELLING | MMAL_CONNECTION_FLAG_ALLOCATION_ON_INPUT);

        if (!m_encoder_connection.is_enabled()) {
          throw std::invalid_argument("Encoder connection port is no enabled!");
        }

        LogTrace << "Register dispatcher";
        output_port.set_user_data(this);
        output_port.enable(raspi_encoder::callback_dispatcher);

        const auto num = m_buffer_pool.queue_length();
        for (auto i = 0; i < num; i++ ) {
          core::buffer new_buffer = m_buffer_pool.get_buffer();
          if (new_buffer.is_valid()) {
            LogTrace << "Send buffer no " << i;
            output_port.send_buffer(new_buffer);
          }
        }
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::disable () {
        LogTrace << "raspi_image_encoder::disable()";

        LogTrace << "get_output_port().disable()" << logging::flush();
        get_output_port().disable();
        LogTrace << "m_encoder_connection.destroy()" << logging::flush();
        m_encoder_connection.destroy();
        LogTrace << "m_encoder.disable()" << logging::flush();
        m_encoder.disable();

//        get_input_port().disable();
//        get_output_port().disable();
//        get_encoder_connection().disable();
//        get_source_output_port().disable();
//        m_encoder.disable();
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::capture (uint32_t timeout) {
        LogTrace << "raspi_image_encoder::capture(" << timeout << ")";
        super::capture(timeout);
      }

      // --------------------------------------------------------------------------
      core::port raspi_image_encoder::get_output_port () const {
        return m_encoder.output_port(0);
      }

      // --------------------------------------------------------------------------
      core::port raspi_image_encoder::get_input_port () const {
        return m_encoder.input_port(0);
      }

      // --------------------------------------------------------------------------
      core::connection& raspi_image_encoder::get_encoder_connection () {
        return m_encoder_connection;
      }

      // --------------------------------------------------------------------------
      const core::connection& raspi_image_encoder::get_encoder_connection () const {
        return m_encoder_connection;
      }

      // --------------------------------------------------------------------------
      auto raspi_image_encoder::get_encoding () const -> OutEncoding {
        return OutEncoding((MMAL_FOURCC_T)get_output_port().get_encoding());
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::set_encoding (OutEncoding enc) {
        m_encoding = static_cast<MMAL_FOURCC_T>(enc);
        get_output_port().set_encoding(m_encoding);
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_resizer::raspi_resizer (core::port source_output_port, OutEncoding encoding)
        : super(source_output_port)
      {
        m_encoder.create(MMAL_COMPONENT_DEFAULT_RESIZER);
        init(static_cast<MMAL_FOURCC_T>(encoding));
      }

      raspi_resizer::~raspi_resizer () {
      }

      // --------------------------------------------------------------------------
      auto raspi_resizer::get_encoding () const -> OutEncoding {
        return OutEncoding((MMAL_FOURCC_T)get_output_port().get_encoding());
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_isp::raspi_isp (core::port source_output_port, OutEncoding encoding)
        : super(source_output_port)
      {
        m_encoder.create(MMAL_COMPONENT_DEFAULT_ISP);
        init(static_cast<MMAL_FOURCC_T>(encoding));
      }

      raspi_isp::~raspi_isp () {
      }

      // --------------------------------------------------------------------------
      auto raspi_isp::get_encoding () const -> OutEncoding {
        return OutEncoding((MMAL_FOURCC_T)get_output_port().get_encoding());
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------

    } // camera

  } // raspi

} // gui
