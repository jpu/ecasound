// ------------------------------------------------------------------------
// eca-test-repository.cpp: A testing subsystem implemented as a
//                          singleton class.
// Copyright (C) 2002,2003 Kai Vehmanen
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

#include "kvu_locks.h"
#include "kvu_numtostr.h"

#include "eca-logger.h"
#include "eca-test-repository.h"

using namespace std;

ECA_TEST_REPOSITORY* ECA_TEST_REPOSITORY::interface_impl_repp = 0;
pthread_mutex_t ECA_TEST_REPOSITORY::lock_rep = PTHREAD_MUTEX_INITIALIZER;

ECA_TEST_REPOSITORY& ECA_TEST_REPOSITORY::instance(void)
{
  //
  // Note! Below we use the Double-Checked Locking Pattern
  //       to protect against concurrent access

  if (interface_impl_repp == 0) {
    KVU_GUARD_LOCK guard(&ECA_TEST_REPOSITORY::lock_rep);
    if (interface_impl_repp == 0) {
      interface_impl_repp = new ECA_TEST_REPOSITORY();
    }
  }
  return(*interface_impl_repp);
}

/**
 * List of unit tests defined in libecasound.
 */

#include "eca-audio-time_test.h"
#include "eca-control_test.h"
#include "eca-session_test.h"
#include "eca-object-factory_test.h"
#include "eca-sample-conversion_test.h"

/** 
 * Class constructor.
 *
 * Registers known tests in libecasound to 
 * the repository.
 */
ECA_TEST_REPOSITORY::ECA_TEST_REPOSITORY(void)
{
  test_cases_rep.push_back(new ECA_AUDIO_TIME_TEST());
  test_cases_rep.push_back(new ECA_SESSION_TEST());
  test_cases_rep.push_back(new ECA_CONTROL_TEST());
  test_cases_rep.push_back(new ECA_OBJECT_FACTORY_TEST());
  test_cases_rep.push_back(new ECA_SAMPLE_CONVERSION_TEST());
}

/** 
 * Class destructor.
 */
ECA_TEST_REPOSITORY::~ECA_TEST_REPOSITORY(void)
{
}

void ECA_TEST_REPOSITORY::do_run(void)
{
  list<ECA_TEST_CASE*>::const_iterator p = test_cases_rep.begin();
  int n = 0;
  while(p != test_cases_rep.end()) {
    ECA_LOG_MSG(ECA_LOGGER::user_objects, "Running test case '" + (*p)->name() + "'.");
    (*p)->run();
    if ((*p)->success() != true) {
      string errormsg = (string("Test case ") + (*p)->name() + string(" FAILED."));
      ECA_LOG_MSG(ECA_LOGGER::user_objects, errormsg);
      ECA_TEST_FAILURE(kvu_numtostr(++n) + ". " + errormsg);
      const list<string>& failures = (*p)->failures();
      list<string>::const_iterator q = failures.begin();
      int m = 1;
      while(q != failures.end()) {
	ECA_TEST_FAILURE(kvu_numtostr(n) + "." + kvu_numtostr(m++) + ". " + *q);
	++q;
      }
    }
    else {
      ECA_LOG_MSG(ECA_LOGGER::user_objects, "Test case '" + (*p)->name() + "' passed.");
    }
    ++p;
  }
}
