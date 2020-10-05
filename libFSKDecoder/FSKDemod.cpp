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

#include "FSKDemod.h"

#include <string.h>

#include "BaudotDecoder.h"
#include "NavtexDecoder.h"
#include "SpectrogramReal.h"
#include "SpectrogramComplex.h"
#include "WindowFunc.h"

FSKDemod::FSKDemod(FSKDecoderOptions * Options) {
float MarkFreq, SpaceFreq;
int iBinMark, iBinSpace;
int BufferWidth;
int FFTSize;
int WindowFunction;

  WindowSize     = Options->WindowSize;
  IncreaseFactor = Options->IncreaseFactor;
  DecreaseFactor = Options->DecreaseFactor;
  Threshold      = Options->Threshold;
  StepWidth      = Options->SampleRate / 800;
  FFTSize        = Options->FFTSize;
  WindowFunction = Options->WindowFunction;

  switch (Options->DecoderType) {
  case TYPE_RTTY_REAL :
  case TYPE_RTTY_IQ :
    BufferWidth = 24;
    BitLength = Options->SampleRate / 50;
    CharacterLength = (BitLength * 15) / 2;
    StopLength = BitLength + BitLength / 2;
    if (FFTSize == 0)
      FFTSize = (Options->SampleRate * 256) / 8000;
    if (WindowSize == 0)
      WindowSize = BitLength;
    if (WindowFunction == -1)
      Options->WindowFunction = 6;
    SamplesBufferSize = (BufferWidth * 15 * BitLength) / 2 + WindowSize;
    SyncReservePoints = (BitLength * 3) / StepWidth;
    break;
  case TYPE_NAVTEX_REAL :
  case TYPE_NAVTEX_IQ :
    BufferWidth = 48;
    BitLength = Options->SampleRate / 100;
    CharacterLength = BitLength * 7;
    StopLength = 0;
    if (FFTSize == 0)
      FFTSize = (Options->SampleRate * 256) / 8000;
    if (WindowSize == 0)
      WindowSize = BitLength;
    if (WindowFunction == -1)
      Options->WindowFunction = 4;
    SamplesBufferSize = BufferWidth * CharacterLength + WindowSize;
    SyncReservePoints = (BitLength / 2) / StepWidth;
    break;
  }
  // Write back changed values to the caller.
  Options->WindowSize     = WindowSize;
  Options->FFTSize        = FFTSize;
  Options->WindowFunction = WindowFunction;
  
  AvPreLength = CharacterLength / StepWidth;
  
  SamplesBuffer = new short [SamplesBufferSize];
  nSamplesBuffer = 0;

    
  MarkSpaceBufferSize = SamplesBufferSize / StepWidth + 2 * AvPreLength;
  nMarkSpaceBuffer = 0;
  MarkBuffer    = new float [MarkSpaceBufferSize];
  SpaceBuffer   = new float [MarkSpaceBufferSize];
  AvMarkBuffer  = new float [MarkSpaceBufferSize];
  AvSpaceBuffer = new float [MarkSpaceBufferSize];
  DecisionBufferSize = MarkSpaceBufferSize - 2 * AvPreLength;
  DecisionBuffer = new int [DecisionBufferSize];

  switch (Options->DecoderType) {
  case TYPE_RTTY_REAL :
  case TYPE_NAVTEX_REAL :
    spectrogram = new SpectrogramReal();
    Channels = 1;
    break;
  case TYPE_RTTY_IQ :
  case TYPE_NAVTEX_IQ :
    spectrogram = new SpectrogramComplex();
    Channels = 2;
    break;
  }
  
  switch (Options->DecoderType) {
  case TYPE_RTTY_REAL :
  case TYPE_RTTY_IQ :
    decoder = new BaudotDecoder();
    break;
  case TYPE_NAVTEX_REAL :
  case TYPE_NAVTEX_IQ :
    decoder = new NavtexDecoder();
    break;
  }
  
  MarkFreq  = float(Options->CenterFreq) + (float(Options->Shift) / 2.0);
  iBinMark  = (int)((MarkFreq  < 0.0 ? FFTSize : 0) + (MarkFreq * FFTSize) / Options->SampleRate + 0.5);
  SpaceFreq = float(Options->CenterFreq) - (float(Options->Shift) / 2.0);
  iBinSpace = (int)((SpaceFreq < 0.0 ? FFTSize : 0) + (SpaceFreq * FFTSize) / Options->SampleRate + 0.5);
 
  spectrogram->init(WindowSize, Options->WindowFunction, FFTSize, iBinMark, iBinSpace, Options->xchgIQ);
  decoder->init(CharacterLength / StepWidth, BitLength / StepWidth, StopLength / StepWidth);
  StepWidth *= Channels;
 
  if (Options->verbose > 0) {
    switch (Options->DecoderType) {
    case TYPE_RTTY_REAL :   printf("  Decoder type     = rtty\n"); break;
    case TYPE_RTTY_IQ :     printf("  Decoder type     = rtty-iq\n"); break;
    case TYPE_NAVTEX_REAL : printf("  Decoder type     = navtex\n"); break;
    case TYPE_NAVTEX_IQ :   printf("  Decoder type     = navtex-iq\n"); break;
    }
    printf("  Sample rate      = %6i\n", Options->SampleRate);
    printf("  Channels         = %6i\n", Channels);
    printf("  Center frequency = %6i\n", Options->CenterFreq);
    printf("  Shift            = %6i\n", Options->Shift);
    printf("  Window size      = %6i\n", WindowSize);
    printf("  FFT size         = %6i\n", FFTSize);
    printf("  Window function  = %6i %s\n", Options->WindowFunction, WindowFuncName(Options->WindowFunction));
    printf("  Bin mark         = %6i\n", iBinMark);
    printf("  Bin space        = %6i\n", iBinSpace);
    if (Options->verbose > 1) {
      printf("  BitLength        = %6i\n", BitLength);
      printf("  CharacterLength  = %6i\n", CharacterLength);
      printf("  StopLength       = %6i\n", StopLength);
      printf("  IncreaseFactor   = %1.5f\n", IncreaseFactor);
      printf("  DecreaseFactor   = %1.5f\n", DecreaseFactor);
    }
    printf("\n");
  }
}

FSKDemod::~FSKDemod() {
  delete [] SamplesBuffer;
  delete [] MarkBuffer;
  delete [] SpaceBuffer;
  delete [] AvMarkBuffer;
  delete [] AvSpaceBuffer;
  delete [] DecisionBuffer;
  delete spectrogram;
  delete decoder;
}

int FSKDemod::newSamples(short * Samples, int nSamples) {
int iSamplePos = 0;
int nUsedSamples;
  
  while (iSamplePos < nSamples) {
    while (nSamplesBuffer < SamplesBufferSize && iSamplePos < nSamples) {
      SamplesBuffer[nSamplesBuffer] = float(Samples[iSamplePos]);
      nSamplesBuffer++;
      iSamplePos++;
    }
    if (nSamplesBuffer == SamplesBufferSize) {
      nUsedSamples = calculate(SamplesBuffer, SamplesBufferSize);
      nSamplesBuffer = SamplesBufferSize - nUsedSamples;
      memmove(SamplesBuffer, &SamplesBuffer[nUsedSamples], nSamplesBuffer * sizeof(short));
    }
  }
  return iSamplePos;
}

FSKDecodedText * FSKDemod::Output() {
  return decoder->Output();
}

int FSKDemod::calculate(short * Samples, int nSamples) {
int iSamplePos = 0;
int iPoint, nUsedPoints;
int EndPos = nSamples - WindowSize * Channels;

  while (iSamplePos < EndPos) {
    while (nMarkSpaceBuffer < MarkSpaceBufferSize && iSamplePos < EndPos) {
      spectrogram->calculatePair(&MarkBuffer[nMarkSpaceBuffer], &SpaceBuffer[nMarkSpaceBuffer], Samples + iSamplePos);
      iSamplePos += StepWidth;
      nMarkSpaceBuffer++;
    }
    if (nMarkSpaceBuffer == MarkSpaceBufferSize) {
      calcAnalysis(MarkBuffer, SpaceBuffer, MarkSpaceBufferSize);
      nUsedPoints = decoder->decode(DecisionBuffer, DecisionBufferSize);
      nMarkSpaceBuffer = MarkSpaceBufferSize - (nUsedPoints - SyncReservePoints);
      memmove(MarkBuffer,  &MarkBuffer[MarkSpaceBufferSize - nMarkSpaceBuffer], nMarkSpaceBuffer * sizeof(float));
      memmove(SpaceBuffer, &SpaceBuffer[MarkSpaceBufferSize - nMarkSpaceBuffer], nMarkSpaceBuffer * sizeof(float));
    }
  }
  return iSamplePos;
}

void FSKDemod::calcAnalysis(float * MarkBuffer, float *  SpaceBuffer, int MarkSpaceBufferSize) {
float   fSpace, fMark;
int     i;
float * SpacePtr;
float * MarkPtr;
float * AvSpacePtr;
float * AvMarkPtr;
bool    bSpace, bMark;
float   max = 0.0f;
float   Space, Mark;
float   AvDec = DecreaseFactor;
float   AvInc = IncreaseFactor;

  for (i = 0; i < MarkSpaceBufferSize; i++) {
    if (max < SpaceBuffer[i])
      max = SpaceBuffer[i];
    if (max < MarkBuffer[i])
      max = MarkBuffer[i];
  }
  if (max <= 0.0f)
    max = 0.000001f;
  else
    max = 1 / max;
  
  SpacePtr = SpaceBuffer + (MarkSpaceBufferSize - 1);
  MarkPtr  = MarkBuffer + (MarkSpaceBufferSize - 1);

  fSpace = *SpacePtr * max;
  fMark  = *MarkPtr * max;
  AvSpaceBuffer[MarkSpaceBufferSize - 1] = fSpace;
  AvMarkBuffer[MarkSpaceBufferSize - 1]  = fMark;

  for (i = MarkSpaceBufferSize - 2; i >= 0; i--) {
    SpacePtr--;
    fSpace *= AvDec;
    Space = *SpacePtr * max;
    if (fSpace < Space)
      fSpace += (Space - fSpace) * AvInc;
    AvSpaceBuffer[i] = fSpace;

    MarkPtr--;
    fMark *= AvDec;
    Mark = *MarkPtr * max;
    if (fMark < Mark)
      fMark += (Mark - fMark) * AvInc;
    AvMarkBuffer[i] = fMark;
  }

  SpacePtr = SpaceBuffer;
  MarkPtr  = MarkBuffer;
  fSpace   = *SpacePtr * max;
  fMark    = *MarkPtr * max;
  
  for (i = 1; i < MarkSpaceBufferSize; i++) {
    SpacePtr++;
    fSpace *= AvDec;
    Space = *SpacePtr * max;
    if (fSpace < Space)
      fSpace += (Space - fSpace) * AvInc;
    AvSpaceBuffer[i] += fSpace;
    AvSpaceBuffer[i] *= 0.5;

    MarkPtr++;
    fMark *= AvDec;
    Mark = *MarkPtr * max;
    if (fMark < Mark)
      fMark += (Mark - fMark) * AvInc;
    AvMarkBuffer[i] += fMark;
    AvMarkBuffer[i] *= 0.5;
  }

  SpacePtr   = SpaceBuffer   + AvPreLength;
  MarkPtr    = MarkBuffer    + AvPreLength;
  AvSpacePtr = AvSpaceBuffer + AvPreLength;
  AvMarkPtr  = AvMarkBuffer  + AvPreLength;

  for (i = 0; i < DecisionBufferSize; i++) {

    bSpace = *AvSpacePtr - Threshold > *SpacePtr * max;
    bMark  = *AvMarkPtr  - Threshold > *MarkPtr * max;
    if (bSpace != bMark)
      DecisionBuffer[i] = bMark;
    else {
      if (*AvSpacePtr > *AvMarkPtr)
        DecisionBuffer[i] = !bSpace;
      else
        DecisionBuffer[i] = bMark;
    }
    SpacePtr++;
    MarkPtr++;
    AvSpacePtr++;
    AvMarkPtr++;
  }
}
