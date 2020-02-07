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

#include "SpectrogramComplex.h"

SpectrogramComplex::SpectrogramComplex()
               : Spectrogram() {
  InBuffer = NULL;
} // ========================================== SpectrogramComplex::SpectrogramComplex()

SpectrogramComplex::~SpectrogramComplex() {
  if (InBuffer != NULL) {
    fftwf_free(InBuffer);
    InBuffer = NULL;
  }
} // ========================================== SpectrogramComplex::~SpectrogramComplex()
  
void SpectrogramComplex::initFFT() {
  
  InBuffer = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) * nFFTSize);
  plan = fftwf_plan_dft_1d(nFFTSize, InBuffer, OutBuffer, FFTW_FORWARD, FFTW_MEASURE);
} // ========================================== SpectrogramComplex::init()

void SpectrogramComplex::calculateFFT(short * Samples) {
unsigned int i;

  if (!xchg) {
    for (i = 0; i < nWindowSize; i++) {
//      InBuffer[i] = (float(Samples[0]) + float(Samples[1]) * I) * Window[i];
      InBuffer[i][0] = float(*Samples) * Window[i];
      Samples++;
      InBuffer[i][1] = float(*Samples) * Window[i];
      Samples++;
    }
  } else {
    for (i = 0; i < nWindowSize; i++) {
      // InBuffer[i] = (float(Samples[1]) + float(Samples[0]) * I) * Window[i];
      InBuffer[i][1] = float(*Samples) * Window[i];
      Samples++;
      InBuffer[i][0] = float(*Samples) * Window[i];
      Samples++;
    }
  }
  for (; i < nFFTSize; i++) {
    InBuffer[i][0] = 0.0f;
    InBuffer[i][1] = 0.0f;
  }
  fftwf_execute(plan);
} // ========================================== SpectrogramComplex::calculateFFT()
