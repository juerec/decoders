/****************************************************************************
**
**  This file is a part of the library "libFSKDecoder".
**
**  Copyright © 2020 Jürgen Reche
**
** libFSKDecoder is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** libFSKDecoder is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <fftw3.h>

class Spectrogram {

public :

  Spectrogram();
  virtual ~Spectrogram();
  
  virtual void init(unsigned int nWindowSize, unsigned int iWindowFunc, unsigned int nFFTSize, int iBinMin, int iBinMax, bool xchg, bool lg = true);
  void calculatePair(float * OutMark, float * OutSpace, short * Samples);
  void calculateRange(float * OutBuffer, short * Samples);

protected :

  void         cleanUp();
  virtual void initFFT() = 0;
  virtual void calculateFFT(short * Samples) = 0;

  inline float power(fftwf_complex z);
  void  calculatePowerPair(float * PowerMin, float * PowerMax);
  void  calculatePowerRange(float * OutBuffer);

  unsigned int     nWindowSize;
  unsigned int     nFFTSize;
  float          * Window;
  fftwf_complex  * OutBuffer;
  fftwf_plan       plan;
  int              iBinMin;
  int              iBinMax;
  bool             xchg;
  bool             lg;
};

#endif // SPECTROGRAM_H
