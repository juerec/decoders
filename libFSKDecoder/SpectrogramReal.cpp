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

#include "SpectrogramReal.h"

SpectrogramReal::SpectrogramReal()
               : Spectrogram() {
  InBuffer = NULL;
} // ============================================ SpectrogramReal::SpectrogramReal()

SpectrogramReal::~SpectrogramReal() {
  if (InBuffer != NULL)
    fftwf_free(InBuffer);
} // ============================================ SpectrogramReal::~SpectrogramReal()
  
void SpectrogramReal::initFFT() {

  InBuffer = (float *)fftwf_malloc(nFFTSize * sizeof(float));
  plan = fftwf_plan_dft_r2c_1d(nFFTSize, InBuffer, OutBuffer, FFTW_MEASURE);  
} // ============================================ SpectrogramReal::init()

void SpectrogramReal::calculateFFT(short * Samples) {
int i;

  for (i = 0; i < nWindowSize; i++)
    InBuffer[i] = float(Samples[i]) * Window[i];
  for (; i < nFFTSize; i++)
    InBuffer[i] = 0.0f;

  fftwf_execute(plan);
} // ============================================ SpectrogramReal::init()
