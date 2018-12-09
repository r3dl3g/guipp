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

namespace gui {

  namespace raspi {

    namespace camera {
#     define check_mmal_status(A) check_mmal_status_(A, #A)

      void check_mmal_status_ (MMAL_STATUS_T status, const char* info);

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_encoder::raspi_encoder (core::port camera)
        : m_camera(camera)
      {
        init();
      }

      // --------------------------------------------------------------------------
      raspi_encoder::~raspi_encoder () {
        fini();
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::init () {
        VCOS_STATUS_T vcos_status = vcos_semaphore_create(&m_complete_semaphore, "RaspiStill-sem", 0);
        vcos_assert(vcos_status == VCOS_SUCCESS);
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::fini () {
        vcos_semaphore_delete(&m_complete_semaphore);
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::enable () {
        //m_camera.enable();
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::disable () {
        //m_camera.disable();
      }

      // --------------------------------------------------------------------------
      core::port raspi_encoder::get_camera_still_port () const {
        return m_camera;
      }

      // --------------------------------------------------------------------------
      void raspi_encoder::capture (uint32_t timeout) {
        clear_data();
        m_camera.capture();
        vcos_semaphore_wait_timeout(&m_complete_semaphore, timeout);
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
      void raspi_encoder::handle_callback(core::port& port, core::buffer& buf) {
        bool complete = false;
        {
          buf.lock();
          if (buf.get_length()) {
            add_data(buf);
          }
          complete = (buf.get_flags() & (MMAL_BUFFER_HEADER_FLAG_FRAME_END | MMAL_BUFFER_HEADER_FLAG_TRANSMISSION_FAILED));
          buf.unlock();
        }

        if (port.is_enabled() && m_buffer_pool.is_valid()) {
          core::buffer new_buffer = m_buffer_pool.get_buffer();
          if (new_buffer.is_valid()) {
            port.send_buffer(new_buffer);
          }
        }

        if (complete) {
          vcos_semaphore_post(&m_complete_semaphore);
        }
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_raw_encoder::raspi_raw_encoder (core::port camera, MMAL_FOURCC_T encoding)
        : super(camera)
      {
        init(encoding);
      }

      // --------------------------------------------------------------------------
      raspi_raw_encoder::~raspi_raw_encoder () {
        fini();
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::init (MMAL_FOURCC_T encoding) {
        auto camera_still_port = get_camera_still_port();
        auto format = camera_still_port.get_format();
        format.encoding = encoding;
        auto min = camera_still_port.get_buffer_min();
        auto sz = camera_still_port.get_buffer();
        if (sz.size < min.size) {
          sz.size = min.size;
        }
        if (sz.num < min.num) {
          sz.num = min.num;
        }
        camera_still_port.set_buffer(sz);
        camera_still_port.set_format(format);

        m_buffer_pool = camera_still_port.create_buffer_pool();
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::fini () {
        m_buffer_pool.destroy();
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::enable () {
        super::enable();
        auto camera_still_port = get_camera_still_port();

        camera_still_port.set_user_data(this);
        check_mmal_status(camera_still_port.enable(raspi_encoder::callback_dispatcher));

        int num = m_buffer_pool.queue_length();
        for (int i = 0; i < num; i++ ) {
          core::buffer new_buffer = m_buffer_pool.get_buffer();
          if (new_buffer.is_valid()) {
            camera_still_port.send_buffer(new_buffer);
          }
        }
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::disable () {
        auto camera_still_port = get_camera_still_port();
        camera_still_port.flush();
        camera_still_port.disable();
        super::disable();
      }

      // --------------------------------------------------------------------------
      void raspi_raw_encoder::capture (uint32_t timeout) {
        enable();
        super::capture(timeout);
        disable();
      }

      // --------------------------------------------------------------------------
      core::port raspi_raw_encoder::get_output_port () {
        return get_camera_still_port();
      }

      // --------------------------------------------------------------------------
      // --------------------------------------------------------------------------
      raspi_image_encoder::raspi_image_encoder (core::port camera, MMAL_FOURCC_T encoding)
        : super(camera)
      {
        init(encoding);
      }

      // --------------------------------------------------------------------------
      raspi_image_encoder::~raspi_image_encoder () {
        fini();
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::init (MMAL_FOURCC_T encoding) {

        check_mmal_status(m_encoder.create(MMAL_COMPONENT_DEFAULT_IMAGE_ENCODER));
        if ( !m_encoder.num_input_ports() || !m_encoder.num_output_ports()) {
          throw std::invalid_argument(ostreamfmt("Encoder has no input/output ports (in:" << m_encoder.num_input_ports() << ", out:" << m_encoder.num_output_ports() << ")"));
        }

        m_encoder_input_port = m_encoder.input_port(0);
        m_encoder_output_port = m_encoder.output_port(0);
        m_encoder_output_port.copy_format_from(m_encoder_input_port);

        auto format = m_encoder_output_port.get_format();
        format.encoding = encoding;
        auto min = m_encoder_output_port.get_buffer_min();
        auto sz = m_encoder_output_port.get_buffer_recommended();
        if (sz.size < min.size) {
          sz.size = min.size;
        }
        if (sz.num < min.num) {
          sz.num = min.num;
        }
        m_encoder_output_port.set_buffer(sz);
        m_encoder_output_port.set_format(format);

        m_encoder.enable();

        m_buffer_pool = m_encoder_output_port.create_buffer_pool();

        m_encoder_connection = get_camera_still_port().connect_in_port(m_encoder_input_port,
                                                                       MMAL_CONNECTION_FLAG_TUNNELLING | MMAL_CONNECTION_FLAG_ALLOCATION_ON_INPUT);
        check_mmal_status(m_encoder_connection.enable());
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::fini () {
        m_buffer_pool.destroy();
        m_encoder_connection.disable();
        m_encoder_connection.destroy();
        m_encoder.destroy();
      }

      // --------------------------------------------------------------------------
      void raspi_image_encoder::enable () {
        super::enable();
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
      // --------------------------------------------------------------------------

    } // camera

  } // raspi

} // gui
