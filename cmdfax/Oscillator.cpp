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

#include "Oscillator.h"

#include <math.h>
#include <stdio.h>

Oscillator::Oscillator() {
  RealTable   = NULL;
  ImagTable   = NULL;
  TableLength = 0;
  TablePos    = 0;
}


Oscillator::~Oscillator() {
  if (RealTable != NULL)
    delete RealTable;
  if (ImagTable != NULL)
    delete ImagTable;
}


void Oscillator::init(int Frequency, int SampleRate) {
int i;
REAL rFrequency = REAL(Frequency);
REAL rSampleRate = REAL(SampleRate);

  if (RealTable != NULL)
    delete RealTable;
  if (ImagTable != NULL)
    delete ImagTable;
  
  TableLength = SampleRate;
  TablePos    = 0;
  
  RealTable   = new REAL[TableLength];
  ImagTable   = new REAL[TableLength];
  
  if (Frequency > 0) {
    for (i = 0; i < TableLength; i++)
      RealTable[i] = sin(2 * M_PI * rFrequency * i / rSampleRate);
    for (i = 0; i < TableLength; i++)
      ImagTable[i] = cos(2 * M_PI * rFrequency * i / rSampleRate);
  } else {
    for (i = 0; i < TableLength; i++)
      RealTable[i] = cos(2 * M_PI * rFrequency * i / rSampleRate);
    for (i = 0; i < TableLength; i++)
      ImagTable[i] = sin(2 * M_PI * rFrequency * i / rSampleRate);
  }
}


void Oscillator::nextSample(REAL * real, REAL * imag) {

  *real = RealTable[TablePos];
  *imag = ImagTable[TablePos];
  TablePos++;
  if (TablePos >= TableLength)
    TablePos = 0;
}
