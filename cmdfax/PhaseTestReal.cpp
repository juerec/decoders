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

#include "PhaseTestReal.h"


PhaseTestReal::PhaseTestReal() {

  fft_cfg    = NULL;
  FFTInput   = NULL;
}


PhaseTestReal::~PhaseTestReal() {

  if (fft_cfg != NULL)
    free(fft_cfg);
  if (FFTInput != NULL)
    delete [] FFTInput;
}


void PhaseTestReal::init(int sampleRate, int CenterFrequency) {

  PhaseTest::init(sampleRate, CenterFrequency);

  fft_cfg  = kiss_fftr_alloc(nFFT, false, 0, 0);
  FFTInput = new kiss_fft_scalar[nFFT];
}


bool PhaseTestReal::runTestStart(short * Samples, int * pDeviation) {
 
  runFFT(Samples);
  return checkStart(pDeviation);
}


bool PhaseTestReal::runTestStop(short * Samples) {
  
  runFFT(Samples);
  return checkStop();
}


void PhaseTestReal::runFFT(short * Samples) {
int i;

  for (i = 0; i < nFFT; i++) {
    FFTInput[i] = ((kiss_fft_scalar)*Samples++) * Window[i];
  }
  kiss_fftr(fft_cfg, FFTInput, FFTOutput);
}
