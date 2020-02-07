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

#include "Spectrogram.h"
#include "WindowFunc.h"

#include <math.h>



Spectrogram::Spectrogram() {
  Window    = NULL;
  OutBuffer = NULL;
  plan      = NULL;
  iBinMin   = 0;
  iBinMax   = 0;
  xchg      = false;
  lg        = false;
}


Spectrogram::~Spectrogram() {
  cleanUp();
}


void Spectrogram::cleanUp() {
  if (Window != NULL) {
    delete [] Window;
    Window = NULL;
  }
  if (OutBuffer != NULL) {
    fftwf_free(OutBuffer);
    OutBuffer = NULL;
  }
  if (plan != NULL) {
    fftwf_destroy_plan(plan);
    plan = NULL;
  }
}


void Spectrogram::init(unsigned int nWindowSize, unsigned int iWindowFunc, unsigned int nFFTSize, int iBinMin, int iBinMax, bool xchg, bool lg) {

  cleanUp();

  this->nWindowSize = nWindowSize;
  this->nFFTSize    = nFFTSize;
  if (nFFTSize < nWindowSize)
    this->nFFTSize = nWindowSize;
  this->iBinMin     = iBinMin;
  this->iBinMax     = iBinMax;
  this->xchg        = xchg;
  this-> lg         = lg;

  Window = new float [nWindowSize];
  for (unsigned int i = 0; i < nWindowSize; i++)
    Window[i] = 1.0f;
  WindowFunc(iWindowFunc, nWindowSize, Window);
  
  // OutBuffer = fftwf_alloc_complex(nFFTSize + 1);
  OutBuffer = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) * nFFTSize);
  
  initFFT();
}

float Spectrogram::power(fftwf_complex z) {

//  float f = creal(z) * creal(z) + cimag(z) * cimag(z);
  float f = z[0] * z[0] + z[1] * z[1];
  if (f <= 1e-8f)
    f = 1e-8f;
//    return -160.0f;
  if (lg)
    return log10(sqrt(f));
  else      
    return sqrt(f);
}


void Spectrogram::calculatePowerPair(float * pMark, float * pSpace) {

  *pMark  = power(OutBuffer[iBinMin]);
  *pSpace = power(OutBuffer[iBinMax]);
}


void Spectrogram::calculatePowerRange(float * pPower) {
int   iBin, iPower;

  iPower = 0;
  // Freq-Min is negative, Freq-Max positive
  if (iBinMax < iBinMin) {
    for (iBin = iBinMin; iBin < nFFTSize; iBin++)
      pPower[iPower++] = power(OutBuffer[iBin]);
    for (iBin = 0; iBin <= iBinMax; iBin++)
      pPower[iPower++] = power(OutBuffer[iBin]);
  } else {
    for (iBin = iBinMin; iBin <= iBinMax; iBin++)
      pPower[iPower++] = power(OutBuffer[iBin]);
  }
}

void Spectrogram::calculatePair(float * pMark, float * pSpace, short * Samples) {
  calculateFFT(Samples);
  calculatePowerPair(pMark, pSpace);
}

void Spectrogram::calculateRange(float * pPower, short * Samples) {
  calculateFFT(Samples);
  calculatePowerRange(pPower);
}
