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
#include "core.h"

// --------------------------------------------------------------------------
//
// Source:
//

namespace gui {

  namespace raspi {

    namespace core {

      void port::size_num::operator |= (const port::size_num& rhs) {
        size = std::max(size, rhs.size);
        num = std::max(num, rhs.num);
      }

      // --------------------------------------------------------------------------
      MMAL_STATUS_T port::enable (MMAL_PORT_BH_CB_T cb) {
        disable();
        if (data->is_enabled) {
          return MMAL_ENOSPC;
        }
        return mmal_port_enable(data, cb);
      }

      MMAL_STATUS_T port::disable () {
        if (data->is_enabled) {
          return mmal_port_disable(data);
        }
        return MMAL_SUCCESS;
      }

      MMAL_STATUS_T port::set_bool (uint32_t id, bool v) {
        MMAL_PARAMETER_BOOLEAN_T t = {{id, sizeof(MMAL_PARAMETER_BOOLEAN_T)}, v ? MMAL_TRUE : MMAL_FALSE};
        return set(t.hdr);
      }

      bool port::get_bool (uint32_t id) {
        MMAL_PARAMETER_BOOLEAN_T t = {{id, sizeof(MMAL_PARAMETER_BOOLEAN_T)}};
        get(t.hdr);
        return t.enable != MMAL_FALSE;
      }

      void port::set_format (const MMAL_ES_SPECIFIC_FORMAT_T& f) {
        *(data->format->es) = f;
      }

      void port::set_encoding (four_cc f) {
        data->format->encoding = f.type.uint32;
        if (MMAL_ENCODING_OPAQUE == data->format->encoding) {
          data->format->encoding_variant = MMAL_ENCODING_I420;
        }
      }

      void port::set_buffer_size (size_num sz) {
        data->buffer_num = sz.num;
        data->buffer_size = sz.size;
      }

      connection port::connect_in_port (port& in, uint32_t flags) {
        // 1. find matching formats
        MMAL_STATUS_T status = MMAL_SUCCESS;

//        copy_format_from(in);

//        four_cc in_enc = in.get_encoding();

////        std::vector<four_cc> in_encodings = in.get_supported_encodings();
//        std::vector<four_cc> out_encodings = get_supported_encodings();
//        auto i = std::find(out_encodings.begin(), out_encodings.end(), in_enc);
//        if (i != out_encodings.end()) {
//          set_encoding(*i);
//        } else {
//          in.set_encoding(MMAL_ENCODING_OPAQUE);
//          set_encoding(MMAL_ENCODING_OPAQUE);
//        }

//        auto buf_sz = get_buffer_size();
//        buf_sz |= in.get_buffer_size();
//        set_buffer_size(buf_sz);
//        in.set_buffer_size(buf_sz);

//        status = commit_format_change();
//        status = in.commit_format_change();

        MMAL_CONNECTION_T* con = nullptr;
        status = mmal_connection_create(&con, data, in.data, flags);
        return connection(con);
      }

      MMAL_STATUS_T port::connect (port& rhs) {
        return mmal_port_connect(data, rhs.data);
      }

      MMAL_STATUS_T port::capture () {
        return set_bool(MMAL_PARAMETER_CAPTURE, true);
      }

      std::vector<four_cc> port::get_supported_encodings () const {
        std::vector<four_cc> encodings;

        std::vector<char*> buffer;
        buffer.resize(sizeof(MMAL_PARAMETER_ENCODING_T));
        MMAL_PARAMETER_ENCODING_T* t = (MMAL_PARAMETER_ENCODING_T*)buffer.data();
        t->hdr = {MMAL_PARAMETER_SUPPORTED_ENCODINGS, buffer.size()};
        MMAL_STATUS_T stat = get(t->hdr);
        while (stat == MMAL_ENOSPC) {
          auto sz = t->hdr.size + sizeof(MMAL_PARAMETER_HEADER_T);
          buffer.resize(sz);
          t = (MMAL_PARAMETER_ENCODING_T*)buffer.data();
          t->hdr = {MMAL_PARAMETER_SUPPORTED_ENCODINGS, buffer.size()};
          stat = get(t->hdr);
        }
        if (MMAL_SUCCESS == stat) {
          auto count = (t->hdr.size - sizeof(MMAL_PARAMETER_HEADER_T)) / sizeof(MMAL_FOURCC_T);
          for (int i = 0; (i < count) && t->encoding[i]; ++i) {
            encodings.emplace_back(t->encoding[i]);
          }
        }

        encodings.emplace_back(MMAL_ENCODING_OPAQUE);
        return encodings;
      }

    // --------------------------------------------------------------------------
    // --------------------------------------------------------------------------

    } // namespace core

  } // raspi

} // gui
