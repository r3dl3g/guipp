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

      MMAL_STATUS_T port::set_format (MMAL_ES_FORMAT_T& f) {
        *(data->format) = f;
        return commit_format_change();
      }

      MMAL_STATUS_T port::set_encoding (four_cc f) {
        data->format->encoding = f;
        if (MMAL_ENCODING_OPAQUE == data->format->encoding) {
          data->format->encoding_variant = MMAL_ENCODING_I420;
        }
        return commit_format_change();
      }

      connection port::connect_in_port (port& in, uint32_t flags) {
        // 1. find matching formats
        MMAL_STATUS_T status = MMAL_SUCCESS;

        four_cc my_enc = get_encoding();
        four_cc in_enc = get_encoding();
        std::vector<four_cc> in_encodings = in.get_supported_encodings();
        std::vector<four_cc> out_encodings = get_supported_encodings();
        auto i = std::find_first_of(out_encodings.begin(), out_encodings.end(), in_encodings.begin(), in_encodings.end());
        if (i != out_encodings.end()) {
          status = in.set_encoding(*i);
        } else {
          status = in.set_encoding(MMAL_ENCODING_OPAQUE);
        }
        copy_format_from(in);

        auto my_buf = get_buffer_size();
        auto in_buf = in.get_buffer_size();
        in_buf.num = my_buf.num = std::max(my_buf.num, in_buf.num);
        in_buf.size = my_buf.size = std::max(my_buf.size, in_buf.size);

        status = commit_format_change();
        status = in.commit_format_change();

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

        auto sz = sizeof(MMAL_PARAMETER_ENCODING_T);
        MMAL_PARAMETER_ENCODING_T t = {{MMAL_PARAMETER_SUPPORTED_ENCODINGS, sz}};
        MMAL_STATUS_T stat = get(t.hdr);
        encodings.emplace_back(MMAL_ENCODING_OPAQUE);
        if (stat == MMAL_SUCCESS) {
          encodings.emplace_back(t.encoding[0]);
        } else if (stat == MMAL_ENOSPC) {
          MMAL_PARAMETER_ENCODING_T* t2 = (MMAL_PARAMETER_ENCODING_T*)new char[t.hdr.size + sz];
          t2->hdr = {MMAL_PARAMETER_SUPPORTED_ENCODINGS, t.hdr.size + sz};
          stat = get(t2->hdr);
          if (MMAL_SUCCESS == stat) {
            auto count = t2->hdr.size / sizeof(MMAL_FOURCC_T);
            for (int i = 0; (i < count) && t2->encoding[i]; ++i) {
              encodings.emplace_back(t2->encoding[i]);
            }
          }
          delete [] (char*)t2;
        }
        return encodings;
      }

    // --------------------------------------------------------------------------
    // --------------------------------------------------------------------------

    } // namespace core

  } // raspi

} // gui
