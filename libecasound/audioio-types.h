#ifndef INCLUDED_AUDIOIO_TYPES_H
#define INCLUDED_AUDIOIO_TYPES_H

#include "audioio.h"

/**
 * A lower level interface for audio I/O objects. Derived classes 
 * must implement routines for reading and/or writing buffers of raw data.
 */
class AUDIO_IO_BUFFERED : public AUDIO_IO {

 public:

  virtual void read_buffer(SAMPLE_BUFFER* sbuf);
  virtual void write_buffer(SAMPLE_BUFFER* sbuf);

  virtual void buffersize(long int samples, long int sample_rate);
  virtual long int buffersize(void) const { return(buffersize_rep); }

  /**
   * Low-level routine for reading samples. Number of read sample
   * frames is returned. This must be implemented by all subclasses.
   */
  virtual long int read_samples(void* target_buffer, long int sample_frames) = 0;

  /**
   * Low-level routine for writing samples. This must be implemented 
   * by all subclasses.
   */
  virtual void write_samples(void* target_buffer, long int sample_frames) = 0;

  virtual ~AUDIO_IO_BUFFERED(void);
  AUDIO_IO_BUFFERED(void);

 protected:

  void reserve_buffer_space(long int bytes);

 private:

  long int buffersize_rep;
  long int target_srate_rep;
  long int target_samples_rep;
  unsigned char* iobuf_uchar_repp;  // buffer for raw-I/O
  size_t iobuf_size_rep;
};

/**
 * Virtual base class for real-time devices.
 *
 * A realtime device...
 *
 * - is disabled after device is opened
 *
 * - is enabled with start()
 *
 * - once enabled, will handle I/O at a constant speed
 *   based on the sample format paremeters
 *
 * - is disabled with stop()
 *
 * @author Kai Vehmanen
 */
class AUDIO_IO_DEVICE : public AUDIO_IO_BUFFERED {
 
 public:

  /**
   * Prepare device for processing. After this call, device is 
   * ready for input/output (buffer can be pre-filled).
   *
   * require:
   *  is_running() != true 
   *
   * ensure:
   *  (io_mode() == si_read && readable() == true) || writable()
   */
  virtual void prepare(void) { toggle_prepared_state(true); }

  /**
   * Start prosessing sample data. Underruns will occur if the 
   * calling program can't handle data at the speed of the 
   * source device. Write_buffer() calls are blocked if necessary.
   *
   * require:
   *  is_running() != true
   *  is_prepared() == true 
   *
   * ensure:
   *  is_running() == true
   */
  virtual void start(void) { toggle_running_state(true); }

  /**
   * Stop processing. Doesn't usually concern non-realtime devices.
   * I/O is not allowed after this call. This should be used when 
   * audio object is not going to be used for a while.
   *
   * require:
   *  is_running() == true
   * 
   * ensure:
   *  is_running() != true
   *  is_prepared() != true
   *  readable() == false
   *  writable() == false
   */
  virtual void stop(void) { toggle_running_state(false); toggle_prepared_state(false); }

  /**
   * Estimed processing latency in samples.
   */
  virtual long int latency(void) const { return(0); }

  /**
   * Whether device has been started?
   */
  virtual bool is_running(void) const { return(is_running_rep); }

  /**
   * Whether device has been prepared for processing?
   */
  virtual bool is_prepared(void) const { return(is_prepared_rep); }

  virtual long position_in_samples(void) const = 0;
  
  /**
   * Seeking is impossible with realtime devices.
   */
  virtual void seek_position(void) { }

  /**
   * Whether to ignore possible under- and overrun 
   * situations. If enabled, device should try to
   * recover from these situations, ie. keep on 
   * running. If disabled, processing should be aborted
   * if an xrun occurs. Should be set before opening 
   * the device. Defaults to 'true'.
   */
  virtual void toggle_ignore_xruns(bool v) { ignore_xruns_rep = v; }

  /** 
   * Whether the use of internal buffering is limited. 
   * If disabled, the device should use minimal amount 
   * of internal buffering. The recommended size is 
   * two or three fragments, each buffersize() sample frames
   * in size. Otherwise the device can use all its
   * internal buffering. This toggle is meant for controlling
   * the latency caused by the device. Defaults to 'true'.
   */
  virtual void toggle_max_buffers(bool v) { max_buffers_rep = v; }
  
  virtual bool ignore_xruns(void) const { return(ignore_xruns_rep); }
  virtual bool max_buffers(void) const { return(max_buffers_rep); }
  virtual bool supports_seeking(void) const { return(false); }
  virtual bool finished(void) const { return(is_open() == false); }

  virtual string status(void) const;

  AUDIO_IO_DEVICE(void);
  virtual ~AUDIO_IO_DEVICE(void);

 protected:
  
  void toggle_running_state(bool v) { is_running_rep = v; }
  void toggle_prepared_state(bool v) { is_prepared_rep = v; }

 private:

  bool is_running_rep;
  bool is_prepared_rep;
  bool ignore_xruns_rep;
  bool max_buffers_rep;
};

#endif
