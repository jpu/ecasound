// ------------------------------------------------------------------------
// audioio-resample.cpp: A proxy class that resamples the child 
//                       object's data.
// Copyright (C) 2002-2004,2008 Kai Vehmanen
//
// Attributes:
//     eca-style-version: 3
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
// ------------------------------------------------------------------------

#include <cstdlib> /* atoi() */
#include <cmath>   /* ceil(), floor() */
#include <iostream>

#include <kvu_dbc.h>
#include <kvu_numtostr.h>

#include "audioio-resample.h"
#include "eca-logger.h"
#include "eca-object-factory.h"
#include "samplebuffer.h"

/**
 * Constructor.
 */
AUDIO_IO_RESAMPLE::AUDIO_IO_RESAMPLE (void) 
  : sbuf_rep(buffersize(), 0)
{
  init_rep = false;
  quality_rep = 50;
}

/**
 * Destructor.
 */
AUDIO_IO_RESAMPLE::~AUDIO_IO_RESAMPLE (void)
{
}

AUDIO_IO_RESAMPLE* AUDIO_IO_RESAMPLE::clone(void) const
{
  AUDIO_IO_RESAMPLE* target = new AUDIO_IO_RESAMPLE();
  for(int n = 0; n < number_of_params(); n++) {
    target->set_parameter(n + 1, get_parameter(n + 1));
  }
  return target;
}

void AUDIO_IO_RESAMPLE::open(void) throw(AUDIO_IO::SETUP_ERROR&)
{
  ECA_LOG_MSG(ECA_LOGGER::user_objects, 
	      "open " + 
	      child_params_as_string(1 + AUDIO_IO_RESAMPLE::child_parameter_offset, &params_rep) + ".");  

  if (init_rep != true) {
    AUDIO_IO* tmp =
      ECA_OBJECT_FACTORY::create_audio_object(
        child_params_as_string(1 + AUDIO_IO_RESAMPLE::child_parameter_offset, &params_rep));

    /* FIXME: add check for real-time devices, resample does _not_
     *        work with them (rt API not proxied properly)
     */
    
    DBC_CHECK(tmp != 0);
    if (tmp != 0) {
      set_child(tmp);
    }

    int numparams = child()->number_of_params();
    for(int n = 0; n < numparams; n++) {
      child()->set_parameter(n + 1, get_parameter(n + 1 + AUDIO_IO_RESAMPLE::child_parameter_offset));
      if (child()->variable_params())
	numparams = child()->number_of_params();
    }

    init_rep = true; /* must be set after dyn. parameters */
  }

  if (child_srate_rep == 0) {
    /* query the sampling rate from child object */
    child()->set_io_mode(io_mode());
    child()->open();
    child_srate_rep = child()->samples_per_second();
    child()->close();
  }

  psfactor_rep = 1.0f;
  if (io_mode() == AUDIO_IO::io_read) {
    psfactor_rep = static_cast<float>(samples_per_second()) / child_srate_rep;
    child_buffersize_rep = static_cast<long int>(std::floor(buffersize() * (1.0f / psfactor_rep)));
  }
  else {
    throw(SETUP_ERROR(SETUP_ERROR::io_mode, "AUDIOIO-RESAMPLE: 'io_write' and 'io_readwrite' modes are not supported."));
  }

  ECA_LOG_MSG(ECA_LOGGER::user_objects, 
	      "pre-open(); psfactor=" + kvu_numtostr(psfactor_rep) +
	      ", child_srate=" + kvu_numtostr(child_srate_rep) +
	      ", srate=" + kvu_numtostr(samples_per_second()) +
	      ", bsize=" + kvu_numtostr(buffersize()) +
	      ", c-bsize=" + kvu_numtostr(child_buffersize_rep) + 
	      ", child=" + child()->label() + ".");

  /* note, we don't use pre_child_open() as 
   * we want to set srate differently */
  child()->set_buffersize(child_buffersize_rep);
  child()->set_io_mode(io_mode());
  child()->set_audio_format(audio_format());
  child()->set_samples_per_second(child_srate_rep);

  child()->open();

  ECA_LOG_MSG(ECA_LOGGER::user_objects, 
	      "post-open(); child=" + child()->label() + ".");

  /* same for the post processing */ 
  SAMPLE_SPECS::sample_rate_t orig_srate = samples_per_second();
  if (child()->locked_audio_format() == true) {
    set_audio_format(child()->audio_format());
    set_samples_per_second(orig_srate);
  }

  sbuf_rep.length_in_samples(buffersize());
  sbuf_rep.number_of_channels(channels());
  sbuf_rep.resample_init_memory(child_srate_rep, samples_per_second());
  sbuf_rep.resample_set_quality(quality_rep);
    
  set_label(child()->label());
  set_length_in_samples(child()->length_in_samples());

  AUDIO_IO_PROXY::open();
}

void AUDIO_IO_RESAMPLE::close(void)
{
  if (child()->is_open() == true) 
    child()->close();

  init_rep = false;

  AUDIO_IO_PROXY::close();
}

bool AUDIO_IO_RESAMPLE::finished(void) const
{
  return child()->finished();
}

string AUDIO_IO_RESAMPLE::parameter_names(void) const
{
  return string("resample,srate,") + child()->parameter_names();
}

void AUDIO_IO_RESAMPLE::set_parameter(int param, string value)
{
  ECA_LOG_MSG(ECA_LOGGER::user_objects, 
	      AUDIO_IO::parameter_set_to_string(param, value));

  /* total of n+1 params, where n is number of childobj params */
  if (param > static_cast<int>(params_rep.size())) params_rep.resize(param);

  if (param > 0) {
    params_rep[param - 1] = value;

    if (param == 1) {
      if (value == "resample-hq") {
	quality_rep = 100;
	ECA_LOG_MSG(ECA_LOGGER::user_objects, 
		    "using high-quality resampler");
      }
      else if (value == "resample-lq") {
	quality_rep = 5;
	ECA_LOG_MSG(ECA_LOGGER::user_objects, 
		    "using low-quality resampler");
      }
      else {
	quality_rep = 50;
	ECA_LOG_MSG(ECA_LOGGER::user_objects, 
		    "using default resampler");
      }
    }
    else if (param == 2) {
      if (value == "auto") {
	child_srate_rep = 0;
	ECA_LOG_MSG(ECA_LOGGER::user_objects, 
		  "resampling with automatic detection of child srate");
      }
      else {
	child_srate_rep = std::atoi(value.c_str());
	ECA_LOG_MSG(ECA_LOGGER::user_objects, 
		  "resampling w/ child srate of " + 
		  kvu_numtostr(child_srate_rep));
      }
    }
  }
  
  sbuf_rep.resample_set_quality(quality_rep);

  if (param > AUDIO_IO_RESAMPLE::child_parameter_offset 
      && init_rep == true) {
    child()->set_parameter(param - AUDIO_IO_RESAMPLE::child_parameter_offset, value);
  }
}

string AUDIO_IO_RESAMPLE::get_parameter(int param) const
{
  if (param > 0 && param < static_cast<int>(params_rep.size()) + 1) {
    if (param > AUDIO_IO_RESAMPLE::child_parameter_offset 
	&& init_rep == true) {
      params_rep[param - 1] = 
	child()->get_parameter(param - AUDIO_IO_RESAMPLE::child_parameter_offset);
    }
    return params_rep[param - 1];
  }

  return "";
}

void AUDIO_IO_RESAMPLE::set_audio_format(const ECA_AUDIO_FORMAT& f_str)
{
  AUDIO_IO::set_audio_format(f_str);
  child()->set_audio_format(f_str);
  
  /* set_audio_format() also sets the sample rate so we need to 
     reset the value back to the correct one */
  child()->set_samples_per_second(child_srate_rep);
}

void AUDIO_IO_RESAMPLE::set_samples_per_second(SAMPLE_SPECS::sample_rate_t v)
{
  AUDIO_IO::set_samples_per_second(v);
  /* the child srate is only set in open */
}

void AUDIO_IO_RESAMPLE::read_buffer(SAMPLE_BUFFER* sbuf)
{
  /* read sample buffer */
  child()->read_buffer(&sbuf_rep);
  /* resample and copy to sbuf */
  sbuf_rep.resample(child_srate_rep, samples_per_second());
  sbuf->copy(sbuf_rep);
  change_position_in_samples(sbuf->length_in_samples());
}

void AUDIO_IO_RESAMPLE::write_buffer(SAMPLE_BUFFER* sbuf)
{
  /* FIXME: not implemented */
  change_position_in_samples(sbuf->length_in_samples());
}
