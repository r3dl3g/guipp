/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    standard lib
 *
 * Customer   -
 *
 * @brief     C++ API: basic typedefs
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <vector>
#include <string>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>

#ifdef WIN32
# include <gui/core/win32defs.h>
#endif // WIN32
#ifdef X11
# include <gui/core/x11defs.h>
#endif

#if !defined(WIN32) && !defined(X11)
# pragma error "Unknown target system"
#endif

namespace gui {

  // --------------------------------------------------------------------------
  typedef std::vector<byte> blob;

  // --------------------------------------------------------------------------
  template<byte bit, bit_order O = os::bitmap_bit_order>
  struct system_bit_mask {};

  template<byte bit>
  struct system_bit_mask<bit, bit_order::lsb> : lsb_bit_mask<bit> {};

  template<byte bit>
  struct system_bit_mask<bit, bit_order::msb> : msb_bit_mask<bit> {};

  // --------------------------------------------------------------------------
  struct system_bw_bits : public bw_bits<os::bitmap_bit_order> {
    static constexpr byte mask[8] = {
      system_bit_mask<0>::value,
      system_bit_mask<1>::value,
      system_bit_mask<2>::value,
      system_bit_mask<3>::value,
      system_bit_mask<4>::value,
      system_bit_mask<5>::value,
      system_bit_mask<6>::value,
      system_bit_mask<7>::value
    };
    static constexpr byte shift[8] = {
      system_bit_mask<0>::shift,
      system_bit_mask<1>::shift,
      system_bit_mask<2>::shift,
      system_bit_mask<3>::shift,
      system_bit_mask<4>::shift,
      system_bit_mask<5>::shift,
      system_bit_mask<6>::shift,
      system_bit_mask<7>::shift
    };
  };

  // --------------------------------------------------------------------------
  inline bool get_bit (byte value, byte bit) {
    return (value & system_bw_bits::mask[bit]) >> system_bw_bits::shift[bit] != 0;
  }

  inline void set_bit (byte& value, byte bit, bool b) {
    value = b ? value | system_bw_bits::mask[bit] : value & ~system_bw_bits::mask[bit];
  }

  // --------------------------------------------------------------------------
  template<typename T>
  struct bit_wrapper {
    inline bit_wrapper (byte& value, byte bit)
      : value(value)
      , bit(bit)
    {}

    inline operator T () const {
      return static_cast<T>(get_bit(value, bit));
    }

    inline T operator= (T b) {
      set_bit(value, bit, static_cast<bool>(b));
      return b;
    }

  private:
    byte& value;
    const byte bit;
  };

  // --------------------------------------------------------------------------
  template<typename T>
  struct bit_wrapper<T const> {
    inline bit_wrapper (byte value, byte bit)
      : value(value)
      , bit(bit)
    {}

    inline operator T () const {
      return static_cast<T>(get_bit(value, bit));
    }

  private:
    const byte value;
    const byte bit;
  };

  // --------------------------------------------------------------------------
  enum class BPP : byte {
    Undefined = 0,
    BW = 1,
    GRAY = 8,
    RGB = 24,
    RGBA = 32
  };

  // --------------------------------------------------------------------------
  enum class orientation : bool {
    vertical,
    horizontal
  };

  constexpr orientation operator! (orientation o) {
    return (o == orientation::vertical) ? orientation::horizontal : orientation::vertical;
  }

  // --------------------------------------------------------------------------
  enum class origin : byte {
    start,
    center,
    end
  };

  // --------------------------------------------------------------------------
  enum class alignment : byte {
    left,
    hcenter,
    right,
    top,
    vcenter,
    bottom
  };


  // --------------------------------------------------------------------------
  template<alignment>
  struct alignment_orientation {};

  template<>
  struct alignment_orientation<alignment::left> {
    static constexpr orientation value = orientation::horizontal;
  };

  template<>
  struct alignment_orientation<alignment::hcenter> {
    static constexpr orientation value = orientation::horizontal;
  };

  template<>
  struct alignment_orientation<alignment::right> {
    static constexpr orientation value = orientation::horizontal;
  };

  template<>
  struct alignment_orientation<alignment::top> {
    static constexpr orientation value = orientation::vertical;
  };

  template<>
  struct alignment_orientation<alignment::vcenter> {
    static constexpr orientation value = orientation::vertical;
  };

  template<>
  struct alignment_orientation<alignment::bottom> {
    static constexpr orientation value = orientation::vertical;
  };

  // --------------------------------------------------------------------------
  template<alignment>
  struct alignment_origin {};

  template<>
  struct alignment_origin<alignment::left> {
    static constexpr origin value = origin::start;
  };

  template<>
  struct alignment_origin<alignment::hcenter> {
    static constexpr origin value = origin::center;
  };

  template<>
  struct alignment_origin<alignment::right> {
    static constexpr origin value = origin::end;
  };

  template<>
  struct alignment_origin<alignment::top> {
    static constexpr origin value = origin::start;
  };

  template<>
  struct alignment_origin<alignment::vcenter> {
    static constexpr origin value = origin::center;
  };

  template<>
  struct alignment_origin<alignment::bottom> {
    static constexpr origin value = origin::end;
  };

  // --------------------------------------------------------------------------
  namespace core {

    namespace global {

      void init (os::instance instance);
      os::instance get_instance ();

      void sync ();

      BPP get_device_bits_per_pixel ();
      int get_device_depth ();

      os::key_state get_key_state ();

    }

  } // core

} //gui

// --------------------------------------------------------------------------
//
// this is the main function that has to be defined in every application
//
// --------------------------------------------------------------------------
extern int gui_main (const std::vector<std::string>& args);

// --------------------------------------------------------------------------

