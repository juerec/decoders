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

#ifndef RESAMPLER_H
#define RESAMPLER_H

#include <samplerate.h>


class ReSampler {

public :
  
  ReSampler();
  ~ReSampler();

  bool init(int oldSampleRate, int newSampleRate, int Channels, int maxInputFrames);

  int resample(float * pIOutBuffer, float * pQOutBuffer, int * pnFramesGen, float * InBuffer, int nFramesInBuffer);

protected :

  int         Channels;
  SRC_STATE * src_state;
  SRC_DATA    src_data;
  float     * inBuffer;
  float     * outBuffer;
  float       f1_MaxShort;

};

#endif // RESAMPLER_H
