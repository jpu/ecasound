#ifndef INCLUDED_SAMPLEBUFFER_H
#define INCLUDED_SAMPLEBUFFER_H

#include <vector>

#include "eca-audio-format.h"
#include "sample-specs.h"

class SAMPLE_BUFFER_FUNCTIONS;
class SAMPLE_BUFFER_impl;

/**
 * Class representing a buffer of audio samples. The primary goal of this class is to 
 * provide a reasonably efficient implementation while still hiding the
 * the actual type information.
 */
class SAMPLE_BUFFER {

  friend class SAMPLE_BUFFER_FUNCTIONS;
  friend class SAMPLE_ITERATOR;
  friend class SAMPLE_ITERATOR_CHANNEL;
  friend class SAMPLE_ITERATOR_CHANNELS;
  friend class SAMPLE_ITERATOR_INTERLEAVED;

 public:

  /** @name Public type definitions */
  /*@{*/

  typedef int channel_size_t;
  typedef long int buf_size_t;
  typedef long int srate_size_t;

  typedef SAMPLE_SPECS::sample_type sample_type;

  /*@}*/

 public:

  /** @name Constructors/destructors */
  /*@{*/

  SAMPLE_BUFFER& operator= (const SAMPLE_BUFFER& t);
  SAMPLE_BUFFER (buf_size_t buffersize = 0, channel_size_t channels = 0, srate_size_t sample_rate = 0);
  ~SAMPLE_BUFFER(void);
  SAMPLE_BUFFER (const SAMPLE_BUFFER& x);

  /*@}*/

 public:
    
  void resample_from(srate_size_t from_srate) { resample_with_memory(from_srate, sample_rate_rep); }
  void resample_to(srate_size_t to_srate) { resample_with_memory(sample_rate_rep, to_srate); }

  /** @name Public member routines */
  /*@{*/

  void add(const SAMPLE_BUFFER& x);
  void add_with_weight(const SAMPLE_BUFFER& x, int weight);
  void copy(const SAMPLE_BUFFER& x);
  void copy_range(const SAMPLE_BUFFER& x, buf_size_t start_pos, buf_size_t end_pos, buf_size_t to_pos);
  void divide_by(sample_type dvalue);

  void limit_values(void);
  void make_silent(void);
  void make_silent_range(buf_size_t start_pos, buf_size_t end_pos);

  void copy_to_buffer(unsigned char* source,
		      buf_size_t samples,
		      ECA_AUDIO_FORMAT::Sample_format fmt,
		      channel_size_t ch,
		      buf_size_t srate);

  void copy_to_buffer_vector(unsigned char* source,
			     buf_size_t samples,
			     ECA_AUDIO_FORMAT::Sample_format fmt,
			     channel_size_t ch,
			     buf_size_t srate);

  void copy_from_buffer(unsigned char* target,
			ECA_AUDIO_FORMAT::Sample_format fmt,
			channel_size_t ch,
			buf_size_t srate);

  void copy_from_buffer_vector(unsigned char* target,
			       ECA_AUDIO_FORMAT::Sample_format fmt,
			       channel_size_t ch,
			       buf_size_t srate);

  /*@}*/
        
 public:

  /** @name Routines for observing and modifying buffer setup */
  /*@{*/

  void number_of_channels(channel_size_t len);
  inline channel_size_t number_of_channels(void) const { return(channel_count_rep); }

  void sample_rate(srate_size_t srate) { sample_rate_rep = srate; }
  inline srate_size_t sample_rate(void) const { return(sample_rate_rep); }
  void length_in_samples(buf_size_t len) { if (buffersize_rep != len) resize(len); }
  inline buf_size_t length_in_samples(void) const { return(buffersize_rep); }
  inline double length_in_seconds(void) const { return((double)buffersize_rep / sample_rate_rep); }

  /*@}*/

 private:

  void resize(buf_size_t buffersize);

 public:

  void resample_extfilter(srate_size_t from_srate, srate_size_t to_srate);
  void resample_simplefilter(srate_size_t from_srate, srate_size_t to_srate);
  void resample_nofilter(srate_size_t from_srate, srate_size_t to_srate);
  void resample_with_memory(srate_size_t from_srate, srate_size_t to_srate);

 public:

  /**
   * WARNING! Although 'buffer' is a public member, you should only 
   * use it directly for a very, very good reason. All normal 
   * input/output should be done via the SAMPLEBUFFER_ITERATORS 
   * class. Representation of 'buffer' may change at any time, 
   * and this will break all code using direct-access.
   */
  std::vector<sample_type*> buffer;

 private:

  /** 
   * @name Sample buffer data 
   *
   * Only these variables (+ sample data) are processed when copying 
   * and contructing buffer objects 
   **/
  /*@{*/

  channel_size_t channel_count_rep;
  buf_size_t buffersize_rep;
  srate_size_t sample_rate_rep;
  buf_size_t reserved_samples_rep;

  /*@}*/

  SAMPLE_BUFFER_impl* impl_repp;

};

#endif
