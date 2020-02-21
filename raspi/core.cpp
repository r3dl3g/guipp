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
      port::~port () {
      }

      MMAL_STATUS_T port::enable (MMAL_PORT_BH_CB_T cb) {
        disable();
        if (data->is_enabled) {
          return MMAL_ENOSPC;
        }
        LogTrace << "enable port";
        return mmal_port_enable(data, cb);
      }

      MMAL_STATUS_T port::disable () {
        if (data->is_enabled) {
          LogTrace << "disanble port";
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

      MMAL_STATUS_T port::connect (port& rhs) {
        LogTrace << "port::connect";
        return mmal_port_connect(data, rhs.data);
      }

      MMAL_STATUS_T port::capture () {
        LogTrace << "port::capture()";
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
      MMAL_STATUS_T port::set_crop (const MMAL_RECT_T& c) {
//        uint32_t w_up = VCOS_ALIGN_UP(c.width, 32);
//        uint32_t h_up = VCOS_ALIGN_UP(c.height, 16);
//        auto fmt = get_format();
//        fmt.video.crop.x = c.x;
//        fmt.video.crop.y = c.y;
//        fmt.video.crop.width = c.width;
//        fmt.video.crop.height = c.height;
//        fmt.video.width = w_up;
//        fmt.video.height = h_up;
//        set_format(fmt);
//        return MMAL_SUCCESS;

        int32_t x = VCOS_ALIGN_DOWN(c.x, 32);
        int32_t y = VCOS_ALIGN_DOWN(c.y, 16);
        int32_t w = VCOS_ALIGN_UP(c.width, 32);
        int32_t h = VCOS_ALIGN_UP(c.height, 16);
        MMAL_PARAMETER_CROP_T crop = {{MMAL_PARAMETER_CROP, sizeof(MMAL_PARAMETER_CROP_T)}, {x, y, w, h}};
        return set(crop.hdr);
      }

      MMAL_RECT_T port::get_crop () const {
//        return get_format().video.crop;

        MMAL_PARAMETER_CROP_T cr = {{MMAL_PARAMETER_CROP, sizeof(MMAL_PARAMETER_CROP_T)}};
        if (get(cr.hdr) == MMAL_SUCCESS) {
          return cr.rect;
        }
        return {};
      }

      // --------------------------------------------------------------------------
      MMAL_PARAMETER_RESIZE_T port::get_resize () const {
        MMAL_PARAMETER_RESIZE_T resize = {{MMAL_PARAMETER_RESIZE_PARAMS, sizeof(MMAL_PARAMETER_RESIZE_T)}, MMAL_RESIZE_DUMMY, 0};
        get(resize.hdr);
        return resize;
      }

      MMAL_STATUS_T port::set_resize (MMAL_PARAMETER_RESIZE_T resize) {
        resize.hdr = {MMAL_PARAMETER_RESIZE_PARAMS, sizeof(MMAL_PARAMETER_RESIZE_T)};
        return set(resize.hdr);
      }

      // --------------------------------------------------------------------------
      connection::connection ()
        : data(nullptr)
      {}

      MMAL_STATUS_T connection::connect (port& from, port& to, uint32_t flags) {
        LogTrace << "connection::connection";
        // 1. find matching formats
//        MMAL_STATUS_T status = MMAL_SUCCESS;

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

        MMAL_STATUS_T status = mmal_connection_create(&data, from.data, to.data, flags);
        if (MMAL_SUCCESS == status) {
          return enable();
        }
        return status;
      }

      connection::connection (connection&& rhs)
        : data(std::move(rhs.data))
      {
        rhs.detach();
      }

      connection::connection (const connection& rhs)
        : data(rhs.data)
      {
        acquire();
      }

      void connection::operator= (connection&& rhs) {
        destroy();
        data = std::move(rhs.data);
        rhs.detach();
      }

      void connection::operator= (const connection& rhs) {
        release();
        data = rhs.data;
        acquire();
      }

      connection::~connection () {
        destroy();
      }

      MMAL_STATUS_T connection::enable () {
        if (data && !data->is_enabled) {
          LogTrace << "Enable connection";
          return mmal_connection_enable(data);
        }
        return MMAL_SUCCESS;
      }

      MMAL_STATUS_T connection::disable () {
        if (data && data->is_enabled) {
          LogTrace << "Disable connection";
          return mmal_connection_disable(data);
        }
        return MMAL_SUCCESS;
      }

      MMAL_STATUS_T connection::acquire () {
        if (data) {
          LogTrace << "Acquire connection";
          mmal_connection_acquire(data);
        }
        return MMAL_SUCCESS;
      }

      MMAL_STATUS_T connection::release () {
        if (data) {
          LogTrace << "Release connection";
          MMAL_STATUS_T r = mmal_connection_release(data);
          data = nullptr;
          return r;
        }
        return MMAL_SUCCESS;
      }

      MMAL_STATUS_T connection::destroy () {
        if (data) {
          LogTrace << "Destroy connection";
          MMAL_STATUS_T r = mmal_connection_destroy(data);
          data = nullptr;
          return r;
        }
        return MMAL_SUCCESS;
      }

      void connection::detach () {
        LogTrace << "Detach connection";
        data = nullptr;
      }

      // --------------------------------------------------------------------------

    } // namespace core

  } // raspi

} // gui
