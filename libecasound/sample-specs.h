#ifndef INCLUDED_SAMPLE_SPECS_H
#define INCLUDED_SAMPLE_SPECS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/**
 * Sample value defaults and constants.
 */
namespace SAMPLE_SPECS {
  typedef float sample_type; // should be a floating-point value!

  static const long int sample_rate_default = 44100;
  static const int channel_count_default = 2;

  static const sample_type silent_value = 0.0f;     // do not change!
  static const sample_type max_amplitude = 1.0f;
  static const sample_type impl_max_value = silent_value + max_amplitude;
  static const sample_type impl_min_value = silent_value - max_amplitude;

  static const int ch_left = 0;
  static const int ch_right = 1;

#ifdef WORDS_BIGENDIAN
  static const bool is_system_littleendian = false;
#else
  static const bool is_system_littleendian = true;
#endif

  static const sample_type s16_to_st_constant = (32768.0 / max_amplitude); // 2^15
  static const sample_type s24_to_st_constant = (8388608.0 / max_amplitude); // 2^23
  static const sample_type s32_to_st_constant = (2147483648.0 / max_amplitude);  // 2^31
  static const sample_type u8_to_st_delta = 128;
  static const sample_type u8_to_st_constant = (max_amplitude / 128);
}

#endif
