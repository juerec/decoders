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

#include "FaxDemodulator.h"
#include <cmath>


FaxDemodulator::FaxDemodulator()
                 : arcSine(256) {
  
  for(size_t i=0; i<arcSine.size(); i++)
    arcSine[i]=std::asin(2.0*i/arcSine.size()-1.0)/2.0/M_PI;

  isReSample     = false;
  ReSampleBuffer = NULL;
  ISplitBuffer   = NULL;
  QSplitBuffer   = NULL;
  IFIROutBuffer  = NULL;
  QFIROutBuffer  = NULL;
  IDemodInBuffer = NULL;
  QDemodInBuffer = NULL;
  InBufferSize   = 0;
  IFilter        = NULL;
  QFilter        = NULL;
  IAudioOld      = 0.0;
  QAudioOld      = 0.0;
  FilterType     = FIRFilter::FLT_NO_FILTER;
};


FaxDemodulator::~FaxDemodulator() {
  
  if (ReSampleBuffer != NULL)
    delete [] ReSampleBuffer;    
  if (ISplitBuffer != NULL)
    delete [] ISplitBuffer;
  if (QSplitBuffer != NULL)
    delete [] QSplitBuffer;
  if (IFIROutBuffer != NULL)
    delete [] IFIROutBuffer;
  if (QFIROutBuffer != NULL)
    delete [] QFIROutBuffer;
}


void FaxDemodulator::init(int sampleRate, int centerFrequency, int shift, int BufferSize, int filterType) {
FIRFilter::FILTER_TYPE  fType;

  SampleRate      = sampleRate;
  CenterFrequency = centerFrequency;
  Shift_2         = shift >> 1;
  InBufferSize    = BufferSize;
  
  switch (filterType) {
  default :
  case 0 : fType = FIRFilter::FLT_NO_FILTER; break;
  case 1 : fType = FIRFilter::FLT_FIRLP17;   break;
  case 2 : fType = FIRFilter::FLT_FIRLP33;   break;
  case 3 : fType = FIRFilter::FLT_FIRLP41;   break;
  case 4 : fType = FIRFilter::FLT_FIRLP75;   break;
  }
  
  if (FilterType != fType) {
    if (IFilter != NULL) {
      delete IFilter;
      IFilter = NULL;
    }
    if (QFilter != NULL) {
      delete QFilter;
      QFilter = NULL;
    }
    FilterType = fType;
    if (fType != FIRFilter::FLT_NO_FILTER) {
      IFilter = new FIRFilter(fType);
      QFilter = new FIRFilter(fType);
    }
  }
  isFilter = FilterType != FIRFilter::FLT_NO_FILTER;

  oscillator.init(CenterFrequency, SampleRate);
  
  isReSample = false;
  if (SampleRate != 8000) {
                //oldSampleRate, newSampleRate, Channels, maxInputFrames
    reSampler.init(SampleRate,     8000,               2, BufferSize);
    isReSample = true;
  }

  IAudioOld = 0.0;
  QAudioOld = 0.0;
  
  if (ReSampleBuffer != NULL) {
    delete [] ReSampleBuffer;
    ReSampleBuffer = NULL;
  }

  if (ISplitBuffer != NULL) {
    delete [] ISplitBuffer;
    ISplitBuffer = NULL;
  }
  if (QSplitBuffer != NULL) {
    delete [] QSplitBuffer;
    QSplitBuffer = NULL;
  }
  
  if (IFIROutBuffer != NULL) {
    delete [] IFIROutBuffer;
    IFIROutBuffer = NULL;
  }
  if (QFIROutBuffer != NULL) {
    delete [] QFIROutBuffer;
    QFIROutBuffer = NULL;
  }

  
  ISplitBuffer = new REAL[InBufferSize];
  QSplitBuffer = new REAL[InBufferSize];

  if (isReSample)
    ReSampleBuffer = new REAL[InBufferSize * 2];

  if (isFilter) {
    IFIROutBuffer = new REAL[InBufferSize];
    QFIROutBuffer = new REAL[InBufferSize];
    IDemodInBuffer = IFIROutBuffer;
    QDemodInBuffer = QFIROutBuffer;
  } else {
    IDemodInBuffer = ISplitBuffer;
    QDemodInBuffer = QSplitBuffer;
  }
  
  rate_shift2 = REAL(8000.0) / REAL(Shift_2) * -1.0;
}

/************************************************/
/* FaxDemodulator::processSamples()             */
/*                                              */
/* int * demod          out                     */
/* short* audio         in audio                */
/* int nSamples         count of input samples  */
/*                                              */
/*                                              */
/*                                              */
/************************************************/

int FaxDemodulator::processSamples(int * demod, short* audio, int nSamples, bool isIQ) {
int   iSample, iSample2, iFrame, nReasampledFrames;
REAL  Sample;
REAL  Imul, Qmul;
REAL  IAudio, QAudio;
REAL  x, y, abs;

  // Komplexs Signal erstellen mit Mittenfrequenz = 0
  // Ausgabe nach ReSampleBuffer IQIQIQIQ...
  if (isIQ) {
    for (iFrame = 0, iSample = 0, iSample2 = 0; iSample < nSamples; iFrame++) {

      oscillator.nextSample(&Imul, &Qmul);
      IAudio = REAL(audio[iSample++]);
      QAudio = REAL(audio[iSample++]);

      if (isReSample) {
        ReSampleBuffer[iSample2++] = IAudio * Imul - QAudio * Qmul;
        ReSampleBuffer[iSample2++] = IAudio * Qmul + QAudio * Imul;
      } else {
        ISplitBuffer[iFrame] = IAudio * Imul - QAudio * Qmul;
        QSplitBuffer[iFrame] = IAudio * Qmul + QAudio * Imul;
      }
    }
  } else {
    for (iFrame = 0, iSample2 = 0; iFrame < nSamples; iFrame++) {

      oscillator.nextSample(&Imul, &Qmul);
      Sample = REAL(audio[iFrame]);

      if (isReSample) {
        ReSampleBuffer[iSample2++] = Sample * Imul;
        ReSampleBuffer[iSample2++] = Sample * Qmul;
      } else {
        ISplitBuffer[iFrame] = Sample * Imul;
        QSplitBuffer[iFrame] = Sample * Qmul;
      }
    }
  }
  

  if (isReSample) {
    reSampler.resample(ISplitBuffer, QSplitBuffer, &nReasampledFrames, ReSampleBuffer, iFrame);
  } else {
    nReasampledFrames = iFrame;
  }

  if (isFilter) {
    IFilter->processSamples(ISplitBuffer, nReasampledFrames, IFIROutBuffer);
    QFilter->processSamples(QSplitBuffer, nReasampledFrames, QFIROutBuffer);
  }

  for(iFrame = 0; iFrame < nReasampledFrames; iFrame++) {
    
    IAudio = IDemodInBuffer[iFrame];
    QAudio = QDemodInBuffer[iFrame];
    
    abs = sqrt(IAudio * IAudio + QAudio * QAudio);
    
    IAudio /= abs;
    QAudio /= abs;
    
    y = QAudioOld * IAudio - IAudioOld * QAudio;
    y = (y + 1.0) * 0.5 * arcSine.size();
    
    x = rate_shift2 * arcSine[static_cast<size_t>(y)];
    
    if (x < -1.0) {
      x=-1.0;
    } else {
      if (x > 1.0)
        x = 1.0;
    }
    demod[iFrame] = static_cast<int>((x / 2.0 + 0.5) * 255.0);
    IAudioOld = IAudio;
    QAudioOld = QAudio;
  }
  return nReasampledFrames;
}  
