// ------------------------------------------------------------------------
// global_preset.cpp: Global effect preset
// Copyright (C) 2000 Kai Vehmanen (kaiv@wakkanet.fi)
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

#include <kvutils.h>

#include "resource-file.h"
#include "eca-resources.h"
#include "eca-debug.h"
#include "eca-error.h"
#include "global-preset.h"

GLOBAL_PRESET::GLOBAL_PRESET(const string& preset_name) 
  : preset_name_rep(preset_name) {
  ECA_RESOURCES ecarc;
  ecadebug->msg(ECA_DEBUG::system_objects,"(global-preset) Opening sc-preset file.");
  string filename =
    ecarc.resource("resource-directory") + "/" + ecarc.resource("resource-file-effect-presets");

  RESOURCE_FILE rc (filename);
  string raw = rc.resource(preset_name);
  if (raw != "") {
    parse(raw);
    set_name(preset_name);
  }
  else {
    set_name("empty");
    throw(ECA_ERROR("GLOBAL_PRESET", "requested preset was not found from " + filename + "."));
  }
}
