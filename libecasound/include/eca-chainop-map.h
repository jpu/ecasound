#ifndef _ECA_CHAIN_OPERATOR_MAP_H
#define _ECA_CHAIN_OPERATOR_MAP_H

#include <string>
#include <map>

#include "eca-chainop.h"

/**
 * Dynamic register for chain operators and their id-strings
 *
 * @author Kai Vehmanen
 */
class ECA_CHAIN_OPERATOR_MAP {

 public:

  static map<string, DYNAMIC_OBJECT*> object_map;
  static map<string, string> object_prefix_map;

  /**
   * Register a new effect.
   */
  static void register_object(const string& id_string, DYNAMIC_OBJECT* object);

  /**
   * Register default chain operators
   */
  static void register_default_objects(void);
};

#endif
