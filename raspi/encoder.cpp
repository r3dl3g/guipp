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
#include <base/string_util.h>
#include <base/ostreamfmt.h>
#include <base/logger.h>

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
#     define check_mmal_status(A) check_mmal_status_(A, #A)

      void check_mmal_status_ (MMAL_STATUS_T status, const char* info);

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_encoder::raspi_encoder (core::port source_output_port)
        : m_source_output_port(source_output_port)
      {
        init();
      }

      // --------------------------------------------------------------------------
      raspi_encoder::~raspi_encoder () {
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::init () {
        m_complete_semaphore.create("RaspiStill-sem", 0);
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::enable () {
        //m_source_output_port.enable();
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::disable () {
        //m_source_output_port.disable();
      }

      // --------------------------------------------------------------------------
      core::port raspi_encoder::get_output_port () const {
        return m_source_output_port;
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::capture (uint32_t timeout) {
        clear_data();
        m_source_output_port.capture();
        m_complete_semaphore.wait(timeout);
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::add_data (const core::buffer& buf) {
        m_buffer.write(buf.get_data(), buf.get_length());
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::clear_data () {
        buffer_type temp;
        m_buffer.swap(temp);
      }

      // --------------------------------------------------------------------------
      auto raspi_encoder::get_data () const -> image_data {
        return m_buffer.str();
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::callback_dispatcher (MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
//        LogTrace << "handle_callback cmd:" << buffer->cmd << ", length:" << buffer->length;
        raspi_encoder* encoder = (raspi_encoder*)(port->userdata);
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
          buf.destroy();
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
      raspi_raw_encoder::raspi_raw_encoder (core::port source_output_port, MMAL_FOURCC_T encoding)
        : super(source_output_port)
      {
        init(encoding);
      }

      // --------------------------------------------------------------------------
      raspi_raw_encoder::~raspi_raw_encoder () {
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::init (MMAL_FOURCC_T encoding) {
        auto output_port = get_output_port();
        auto format = output_port.get_format();
        format.encoding = encoding;
        auto min = output_port.get_min_buffer_size();
        auto sz = output_port.get_buffer_size();
        if (sz.size < min.size) {
          sz.size = min.size;
        }
        if (sz.num < min.num) {
          sz.num = min.num;
        }
        output_port.set_buffer_size(sz);
        output_port.set_format(format);

        m_buffer_pool = output_port.create_buffer_pool();
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::enable () {
        super::enable();
        auto output_port = get_output_port();

        output_port.set_user_data(this);
        check_mmal_status(output_port.enable(raspi_encoder::callback_dispatcher));

        int num = m_buffer_pool.queue_length();
        for (int i = 0; i < num; i++ ) {
          core::buffer new_buffer = m_buffer_pool.get_buffer();
          if (new_buffer.is_valid()) {
            output_port.send_buffer(new_buffer);
          }
        }
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::disable () {
        auto output_port = get_output_port();
        output_port.flush();
        output_port.disable();
        super::disable();
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::capture (uint32_t timeout) {
        enable();
        super::capture(timeout);
        disable();
      }

      core::port raspi_raw_encoder::get_output_port () {
        return super::get_output_port();
      }

      core::port raspi_raw_encoder::get_input_port () {
        return super::get_output_port();
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_image_encoder::raspi_image_encoder (core::port source_output_port, OutEncoding encoding)
        : super(source_output_port)
      {
        check_mmal_status(m_encoder.create(MMAL_COMPONENT_DEFAULT_IMAGE_ENCODER));
        init(static_cast<MMAL_FOURCC_T>(encoding));
      }

      // --------------------------------------------------------------------------
      raspi_image_encoder::raspi_image_encoder (core::port source_output_port)
        : super(source_output_port)
      {}

      // --------------------------------------------------------------------------
      raspi_image_encoder::~raspi_image_encoder () {
        m_encoder.destroy();
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::init (MMAL_FOURCC_T encoding) {

        if ( !m_encoder.num_input_ports() || !m_encoder.num_output_ports()) {
          throw std::invalid_argument(ostreamfmt("Encoder has no input/output ports (in:" << m_encoder.num_input_ports() << ", out:" << m_encoder.num_output_ports() << ")"));
        }

        m_encoder_input_port = m_encoder.input_port(0);
        m_encoder_output_port = m_encoder.output_port(0);

        m_encoding = encoding;
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::enable () {
        m_encoder_output_port.copy_format_from(m_encoder_input_port);

        m_encoder_input_port.set_encoding(MMAL_ENCODING_OPAQUE);

        auto format = m_encoder_output_port.get_format();
        format.encoding = m_encoding;
        auto min = m_encoder_output_port.get_min_buffer_size();
        auto sz = m_encoder_output_port.get_recommended_buffer_size();
        if (sz.size < min.size) {
          sz.size = min.size;
        }
        min.num = std::max(min.num, 2U);
        if (sz.num < min.num) {
          sz.num = min.num;
        }
        m_encoder_output_port.set_buffer_size(sz);
        m_encoder_output_port.set_format(format);

        m_encoder.enable();
        m_encoder_connection = m_source_output_port.connect_in_port(m_encoder_input_port,
                                                                    MMAL_CONNECTION_FLAG_TUNNELLING | MMAL_CONNECTION_FLAG_ALLOCATION_ON_INPUT);
        check_mmal_status(m_encoder_connection.enable());

        super::enable();

        m_buffer_pool = m_encoder_output_port.create_buffer_pool();

        if (!m_encoder_connection.is_enabled()) {
          throw std::invalid_argument("Encoder connection port is no enabled!");
        }
        m_encoder_output_port.set_user_data(this);
        check_mmal_status(m_encoder_output_port.enable(raspi_encoder::callback_dispatcher));

        int num = m_buffer_pool.queue_length();
        for (int i = 0; i < num; i++ ) {
          core::buffer new_buffer = m_buffer_pool.get_buffer();
          if (new_buffer.is_valid()) {
            m_encoder_output_port.send_buffer(new_buffer);
          }
        }
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::disable () {
        check_mmal_status(m_encoder_output_port.flush());
        check_mmal_status(m_encoder_output_port.disable());
        m_encoder_connection.disable();
        m_encoder_connection.destroy();
        super::disable();
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::capture (uint32_t timeout) {
        enable();
        super::capture(timeout);
        disable();
      }

      // --------------------------------------------------------------------------
      core::port& raspi_image_encoder::get_output_port () {
        return m_encoder_output_port;
      }

      // --------------------------------------------------------------------------
      core::port& raspi_image_encoder::get_input_port () {
        return m_encoder_input_port;
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_resizer::raspi_resizer (core::port source_output_port, OutEncoding encoding)
        : super(source_output_port)
      {
        check_mmal_status(m_encoder.create(MMAL_COMPONENT_DEFAULT_RESIZER));
        init(static_cast<MMAL_FOURCC_T>(encoding));
      }

      raspi_resizer::~raspi_resizer () {
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_isp::raspi_isp (core::port source_output_port, OutEncoding encoding)
        : super(source_output_port)
      {
        check_mmal_status(m_encoder.create(MMAL_COMPONENT_DEFAULT_ISP));
        init(static_cast<MMAL_FOURCC_T>(encoding));
      }

      raspi_isp::~raspi_isp () {
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------

    } // camera

  } // raspi

} // gui
