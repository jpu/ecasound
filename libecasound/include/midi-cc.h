#ifndef _MIDI_CC_H
#define _MIDI_CC_H

#include <string>

#include "ctrl-source.h"

/**
 * Interface to MIDI continuous controllers
 */
class MIDI_CONTROLLER : public CONTROLLER_SOURCE {
    
  double controller, channel;
  parameter_type value_rep;

 public:

  /**
   * Initialize MIDI-controller (child-thread is spawned)
   */
  void init(parameter_type phasestep);

  string parameter_names(void) const { return("controller,channel"); }
  void set_parameter(int param, parameter_type value);
  parameter_type get_parameter(int param) const;

  string name(void) const {  return("MIDI-Controller"); }
  parameter_type value(void);
 
  MIDI_CONTROLLER* clone(void)  { return new MIDI_CONTROLLER(*this); }
  MIDI_CONTROLLER(int controller_number = 0, int midi_channel = 0);
};

#endif
