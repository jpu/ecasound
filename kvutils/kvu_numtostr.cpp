// ------------------------------------------------------------------------
// kvu_numtostr.cpp: Routines for converting string objects to numbers. 
// Copyright (C) 1999 Kai Vehmanen (kaiv@wakkanet.fi)
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

#include <cstdio>
#include <string>

#include "kvu_numtostr.h"

string kvu_numtostr (unsigned char c) { return kvu_numtostr((int)c); }
string kvu_numtostr (signed char c) {  return kvu_numtostr((int)c); }
string kvu_numtostr (bool b) { return kvu_numtostr((int)b); }
string kvu_numtostr(unsigned short i) { return kvu_numtostr((int)i); }

string kvu_numtostr (float n, int flo_prec) { return kvu_numtostr(static_cast<double>(n), flo_prec); }

string kvu_numtostr (int n) {
  char ctmp[12];
  snprintf(ctmp, 12, "%d",n);
  ctmp[11] = 0;
  return(string(ctmp));
}

string kvu_numtostr (const void *p) {
  char ctmp[12];
  snprintf(ctmp, 12, "%p",p);
  ctmp[11] = 0;
  return(string(ctmp));
}

string kvu_numtostr (unsigned int n) {
  char ctmp[12];
  snprintf(ctmp, 12, "%u",n);
  ctmp[11] = 0;
  return(string(ctmp));
}

string kvu_numtostr (long n) {
  char ctmp[12];
  snprintf(ctmp, 12, "%ld",n);
  ctmp[11] = 0;
  return(string(ctmp));
}

string kvu_numtostr (unsigned long n) {
  char ctmp[12];
  snprintf(ctmp, 12, "%lu",n);
  ctmp[11] = 0;
  return(string(ctmp));
}


string kvu_numtostr (double n, int flo_prec) {
  char ctmp[32];
  snprintf(ctmp, 12, "%.*f",flo_prec, n);
  ctmp[31] = 0;
  return(string(ctmp));
}









