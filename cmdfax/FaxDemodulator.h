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

#ifndef FAXDEMODULATOR_H
#define FAXDEMODULATOR_H

#include "Real.h"

#include "FIRFilter.h"
#include "ReSampler.h"
#include "Oscillator.h"
#include "CmdFaxLookUpTable.h"

class FaxDemodulator {

public :
  FaxDemodulator();
  ~FaxDemodulator();

  void init(int SampleRate, int centerFrequency, int Shift, int BufferSize, int filterType);
  int  processSamples(int * demod, short* audio, int nFrames, bool isIQ);

protected :

  int    SampleRate;
  int    CenterFrequency;
  int    Shift_2;
  int    InBufferSize;
  FIRFilter::FILTER_TYPE    FilterType;
  bool   isReSample;
  bool   isFilter;

  // the real buffers
  REAL * ReSampleBuffer;
  REAL * ISplitBuffer;
  REAL * QSplitBuffer;
  REAL * IFIROutBuffer;
  REAL * QFIROutBuffer;
  REAL   rate_shift2;
  
  // alias for Buffers
  // not allocatet one, pointed to one of the buffers above, depending on filtering or not
  REAL * IDemodInBuffer;
  REAL * QDemodInBuffer;
  
  FIRFilter * IFilter;
  FIRFilter * QFilter;
  
  Oscillator          oscillator;
  ReSampler           reSampler;
  LookUpTable<REAL>   arcSine;
  REAL                IAudioOld;
  REAL                QAudioOld;

};

#endif /* FAXDEMODULATOR_H */
