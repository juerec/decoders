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

#include "PhaseTest.h"

#include "kiss_fft.h"
#include "Options.h"


PhaseTest::PhaseTest() {

  FFTOutput       = NULL;
  LineBuffer      = NULL;
  Window          = NULL;
  nFFT            = 1024;
  CenterFrequency = 1500;
}

PhaseTest::~PhaseTest() {

  if (FFTOutput != NULL)
    delete [] FFTOutput;
  if (LineBuffer != NULL)
    delete [] LineBuffer;
  if (Window != NULL)
    delete [] Window;
}

int PhaseTest::BinNr(int Frequency) {
 
  int iBin = (int)roundf((float(Frequency) / float(SampleRate)) * nFFT);
  if (iBin < 0)
    iBin += nFFT;
  return iBin;
}

void PhaseTest::init(int sampleRate, int centerFrequency) {

  SampleRate = sampleRate;
  CenterFrequency = centerFrequency;
  nFFT = SampleRate / 8;

// Depends on Complex/Real
  FFTOutput  = new kiss_fft_cpx[nFFT + 1];
  LineBuffer = new kiss_fft_scalar[nFFT + 1];
  Window     = new kiss_fft_scalar[nFFT];
  // Hann
  for (int i = 0; i < nFFT; i++)
    Window[i] = 0.50 - 0.50 * cos(2 * M_PI * i / (nFFT - 1));
  
  BinsStart[0] = BinNr(CenterFrequency - 600);
  BinsStart[1] = BinNr(CenterFrequency - 300);
  BinsStart[2] = BinNr(CenterFrequency);
  BinsStart[3] = BinNr(CenterFrequency + 300);
  BinsStart[4] = BinNr(CenterFrequency + 600);
  
  BinsEnd[0] = BinNr(CenterFrequency - 900);
  BinsEnd[1] = BinNr(CenterFrequency - 450);
  BinsEnd[2] = BinNr(CenterFrequency);
  BinsEnd[3] = BinNr(CenterFrequency + 450);
  BinsEnd[4] = BinNr(CenterFrequency + 900);
  BinStart = BinNr(CenterFrequency - 1000);
  BinEnd   = BinNr(CenterFrequency + 1000);
  
  DeltaStart = BinNr(CenterFrequency - 120) - BinNr(CenterFrequency);
  DeltaEnd   = BinNr(CenterFrequency + 120) - BinNr(CenterFrequency);
  Delta = DeltaEnd - DeltaStart + 1;
  
}

bool PhaseTest::checkStart(int * pDeviation) {
float  av = 0.0;
int    i;
int    nAv = 0;
int    sum[Delta];
int    maxSum;
int    maxPos;
int    nStart;
bool   ret;

  for (i = BinStart; i <= BinEnd; i++) {
    LineBuffer[i] = sqrt( FFTOutput[i].r * FFTOutput[i].r + FFTOutput[i].i * FFTOutput[i].i);
    av += LineBuffer[i];
    nAv++;
  }
  av /= nAv;
  nStart = 4;
  switch (Options::pOptions->StartThreshold) {
  case 1 :
    break;
  case 2 :
    av *= 1.5;
    break;
  case 3 :
    av *= 2.5;
    nStart = 5;
  }

  for (i = 0; i < Delta; i++) {
    sum[i] = LineBuffer[ BinsStart[0] + DeltaStart + i ] +
             LineBuffer[ BinsStart[1] + DeltaStart + i ] +
             LineBuffer[ BinsStart[2] + DeltaStart + i ] +
             LineBuffer[ BinsStart[3] + DeltaStart + i ] +
             LineBuffer[ BinsStart[4] + DeltaStart + i ];
  } 
  
  maxSum = 0;
  maxPos = 0;
  for (i = 0; i < Delta; i++)
    if (maxSum < sum[i]) {
      maxSum = sum[i];
      maxPos = i;
    }
    
  ret =  ((LineBuffer[BinsStart[0] + DeltaStart + maxPos] > av) +
          (LineBuffer[BinsStart[1] + DeltaStart + maxPos] > av) +
          (LineBuffer[BinsStart[2] + DeltaStart + maxPos] > av) +
          (LineBuffer[BinsStart[3] + DeltaStart + maxPos] > av) +
          (LineBuffer[BinsStart[4] + DeltaStart + maxPos] > av)) >= nStart;
          
  if (ret) {
    *pDeviation = maxPos + DeltaStart;
  }
  return ret;
}


// A lightweight variant of runTestStart(). No worry about Deviations, it's already known.
bool PhaseTest::checkStop() {
float  av = 0.0;
int    i;
int    nAv = 0;

  for (i = BinStart; i <= BinEnd; i++) {
    LineBuffer[i] = sqrt( FFTOutput[i].r * FFTOutput[i].r + FFTOutput[i].i * FFTOutput[i].i);
    av += LineBuffer[i];
    nAv++;
  }

  for (i = 0; i < 5; i++) {
    av -= LineBuffer[BinsEnd[i] - 1];
    av -= LineBuffer[BinsEnd[i]];
    av -= LineBuffer[BinsEnd[i] + 1];
  }
  nAv -= 15;

  av /= nAv;

  switch (Options::pOptions->StopThreshold) {
  case 1 :
    break;
  case 2 :
    av *= 1.5;
    break;
  case 3 :
    av *= 2.5;
  }

  return (int(LineBuffer[BinsEnd[0]] / av) +
          int(LineBuffer[BinsEnd[1]] / av) +
          int(LineBuffer[BinsEnd[2]] / av) +
          int(LineBuffer[BinsEnd[3]] / av) +
          int(LineBuffer[BinsEnd[4]] / av) > 8) &&
          int(LineBuffer[BinsEnd[1]] / av) > 2.0 &&
          int(LineBuffer[BinsEnd[2]] / av) > 2.0 &&
          int(LineBuffer[BinsEnd[3]] / av) > 2.0;
}


void PhaseTest::setDeviaion(int Deviation) {
  BinsEnd[0] = BinNr(CenterFrequency - 900) + Deviation;
  BinsEnd[1] = BinNr(CenterFrequency - 450) + Deviation;
  BinsEnd[2] = BinNr(CenterFrequency) + Deviation;
  BinsEnd[3] = BinNr(CenterFrequency + 450) + Deviation;
  BinsEnd[4] = BinNr(CenterFrequency + 900) + Deviation;
}

int PhaseTest::deltaF(int nBins) {
  float f = roundf((float(SampleRate) / float(nFFT)) * float(nBins));
  return (int)f;
}
