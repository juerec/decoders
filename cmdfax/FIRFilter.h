/****************************************************************************
**
**  This file is a part of the program "CMDFAX".
**
**  Copyright © 2020 Jürgen Reche
**
** CMDFSK is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** CMDFSK is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef FIRFILTER_H
#define FIRFILTER_H

#include "Real.h"

class FIRFilter {
  
public :

  enum FILTER_TYPE {
    FLT_NO_FILTER = 100,
    FLT_DEFAULT,
    FLT_FIRLP17,  // LP 425
    FLT_FIRLP33,  // LP 450
    FLT_FIRLP41,  // LP 425
    FLT_FIRLP75,  // LP 425
  };

  FILTER_TYPE FilterType;
  
  FIRFilter();
  FIRFilter(FILTER_TYPE FilterType);
  ~FIRFilter();
  void init(FILTER_TYPE FilterType);
  void processSamples(REAL * InBuffer, int nInBuffer, REAL * OutBuffer);
  
  double * History;
  int      HistPos;
  double * Taps;
  int      nTaps;
  
  static double TapsDummy[4];
  static double TapsFLT_FIRLP17[17];
  static double TapsFLT_FIRLP33[33];
  static double TapsFLT_FIRLP41[41];
  static double TapsFLT_FIRLP75[75];
};
 
#endif // FIRFILTER_H
