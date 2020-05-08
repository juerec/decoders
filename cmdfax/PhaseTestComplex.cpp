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

#include "PhaseTestComplex.h"


PhaseTestComplex::PhaseTestComplex() {

  fft_cfg    = NULL;
  FFTInput   = NULL;
}


PhaseTestComplex::~PhaseTestComplex() {

  if (fft_cfg != NULL)
    free(fft_cfg);
  if (FFTInput != NULL)
    delete [] FFTInput;
}


void PhaseTestComplex::init(int sampleRate, int CenterFrequency) {

  PhaseTest::init(sampleRate, CenterFrequency);

  fft_cfg  = kiss_fft_alloc(nFFT, false, 0, 0);
  FFTInput = new kiss_fft_cpx[nFFT];
}


bool PhaseTestComplex::runTestStart(short * Samples, int * pDeviation) {
 
  runFFT(Samples);
  return checkStart(pDeviation);
}


bool PhaseTestComplex::runTestStop(short * Samples) {
  
  runFFT(Samples);
  return checkStop();
}


void PhaseTestComplex::runFFT(short * Samples) {
int i;

  for (i = 0; i < nFFT; i++) {
    FFTInput[i].r = ((kiss_fft_scalar)*Samples++) * Window[i];
    FFTInput[i].i = ((kiss_fft_scalar)*Samples++) * Window[i];
  }

  kiss_fft(fft_cfg, FFTInput, FFTOutput);
}
