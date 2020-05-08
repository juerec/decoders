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

#include "CmdFax.h"

#include <stdlib.h>
#include <stdio.h>

#include "FaxImage.h"

#define ReducedSampleRate   8000


extern FaxImage faxImage;

CmdFax::CmdFax()
      : wav(true) {
  FaxState        = FAX_IDLE;
  pOptions        = NULL;
  pPhaseTest      = NULL;
  SampleRate = 0;
  CenterFrequency = 0;
  Shift           = 0;
  isIQSource      = false;
  InBuffer = NULL;
  InBufferSize    = 0;
  SamplesPerLine  = 0.0;
  ImageStart      = 0;
}

CmdFax::~CmdFax() {

  if (pPhaseTest != NULL)
    delete pPhaseTest;
  if (InBuffer != NULL)
    delete [] InBuffer;
}


bool CmdFax::init(Options * pOptions) {

  this->pOptions = pOptions;
  if (!wav.open(pOptions->InputFilename))
    return false;
  SampleRate = wav.sampleRate();
  if (SampleRate < 8000 || wav.bitsPerSample() != 16 || (wav.channels() != 1 && wav.channels() != 2)) {
    fprintf(stderr, "Not supported sample parameter %i/s - %i bits - %i channel(s)\n", wav.sampleRate(), wav.bitsPerSample(), wav.channels());
    wav.close();
    return false;
  }
  file_format     = pOptions->file_format;      // Bitmap 1 | 8 | 32 Bit
  CenterFrequency = pOptions->CenterFrequency;
  Shift           = pOptions->Shift;
  isIQSource      = wav.channels() == 2;
  
  if (pOptions->verbose >= 2)
    printf("Sample Rate = %i   Channels = %i   CenterFrequency = %i   Shift = %i\n", SampleRate, wav.channels(), CenterFrequency, Shift);
  
  if (isIQSource)
    pPhaseTest = new PhaseTestComplex();
  else
    pPhaseTest = new PhaseTestReal();
  
  pPhaseTest->init(SampleRate, CenterFrequency);

  InBufferSize = (SampleRate >> 1) * wav.channels(); //  1/2 sec.
  InBuffer = new short[InBufferSize];

  return true;
}


int CmdFax::PhasingPosition(int * BlackWhiteErrBuffer, int nBlackWhiteErrBuffer, int iStart, int nLines, int nCheckLength, int nLineLength, int nWitheLenght) {
int nCheckLengthX2 = nCheckLength * 2;
// int * SumBuffer;
int iStartPos;
int iEndPos;
int iMaxPos;
int iLine;
int iPos;
int err;
int pos;
int minErr;
int minErrPos;

  iStartPos = iStart - nCheckLength;
  if (iStartPos < 0)
    iStartPos = 0;
  iEndPos   = iStart + nCheckLength;
  
  iMaxPos = iEndPos + nLineLength * nLines + nWitheLenght;
  if (nBlackWhiteErrBuffer < iMaxPos) {
    fprintf(stderr, "cmdfax - internal error: iMaxPos invalid\n");
    exit(0);
  }
  
  minErr    = nCheckLengthX2;
  minErrPos = 0;

  // SumBuffer = new int [nCheckLengthX2];
  for (iPos = iStartPos; iPos < iEndPos; iPos++) {
    err = 0;
    pos = iPos;
    for (iLine = 0; iLine < nLines; iLine++) {
      
      if (pos + nWitheLenght >= nBlackWhiteErrBuffer) {
        fprintf(stderr, "cmdfax internal error: PhasingPosition(* BlackWhiteErrBuffer, nBlackWhiteErrBuffer %i, iStart %i, nLines %i, nCheckLength %i, nLineLength %i, nWitheLenght %i) \n",
                       nBlackWhiteErrBuffer, iStart, nLines, nCheckLength, nLineLength, nWitheLenght);
        break;
      }
      err += BlackWhiteErrBuffer[pos] - BlackWhiteErrBuffer[pos + nWitheLenght];
      pos += nLineLength;
    }
    
    if (minErr > err) {
      minErr = err;
      minErrPos = iPos;
    }
  }
  return minErrPos;
}


bool CmdFax::Phasing(int * PhasingBuffer, int PhasingBufferCount) {
static const int nLineLength   = ReducedSampleRate / 2;     //  4000     Length of a phasing line
static const int checkLength   = ReducedSampleRate / 160;   //    50     checked length of one color
static const int checkLengthX2 = checkLength * 2;           //   100     checked length, sum of both colors
static const int whiteLength   = ReducedSampleRate / 40;    //   200     Length of the white part in a phasing line
static const int BlackLevel    = 127;
int           * BlackWhiteErrBuffer;
int             BlackWhiteErrBufferPos;
int             nBlack;
int             nWhite;
int             PhasingBufferPos;
int             i;
int           * ErrCountBuffer;
int             nErrCountBuffer = nLineLength;
int             nErrors;
int             nLines;
int             iLine;
  
  BlackWhiteErrBuffer = new int [PhasingBufferCount];
  BlackWhiteErrBufferPos = 0;
  nBlack = 0;
  nWhite = 0;
  PhasingBufferPos = 0;
  
  for (i = 0; i < checkLength; i++) {
    if (PhasingBuffer[PhasingBufferPos++] > BlackLevel)
      nBlack++;
  }
  // 50 ... 99 => count white pixels
  for (i = 0; i < checkLength; i++) {
    if (PhasingBuffer[PhasingBufferPos++] <= BlackLevel)
      nWhite++;
  }
  
  //  error count = nBlack + nWhite;  
  BlackWhiteErrBuffer[BlackWhiteErrBufferPos++] = nBlack + nWhite;
  

  // Fehlerrate für jede folgende Position im PhasingBuffer bestimmen  
  // determine error rate for each position in phasing buffer
  while (PhasingBufferPos < PhasingBufferCount) {
    
    nBlack -= PhasingBuffer[PhasingBufferPos - checkLengthX2] > BlackLevel;
    nBlack += PhasingBuffer[PhasingBufferPos - checkLength] > BlackLevel;
    
    nWhite -= PhasingBuffer[PhasingBufferPos - checkLength] <= BlackLevel;
    nWhite += PhasingBuffer[PhasingBufferPos] <= BlackLevel;
    
    BlackWhiteErrBuffer[BlackWhiteErrBufferPos++] = nBlack + nWhite;
    
    PhasingBufferPos++;
  }

  /*  Im Abstand der idealen Länge einer Phasing-Line die Fehlerrate aufsummieren  */
  /*  Sum error rate in ideal distance of phasing line */
  /*  Buffer for sum of  error rates  */
  ErrCountBuffer = new int [nErrCountBuffer];
  
  nLines = (PhasingBufferCount - checkLengthX2) / nLineLength;
  
  for (i = 0; i < nErrCountBuffer; i++) {
    nErrors =  0;
    BlackWhiteErrBufferPos = i;
    for (iLine = 0; iLine < nLines; iLine++) {
      nErrors += BlackWhiteErrBuffer[BlackWhiteErrBufferPos];
      BlackWhiteErrBufferPos += nLineLength;
    }
    ErrCountBuffer[i] = nErrors;
  }
  
  
  /*  Position von Minimum u. Maximum suchen => minErrPos, maxErrPos  */
  /*  Search position of minimum and maximum */
  /*  Maximum liegt whiteLength (200) Samples hinter dem Minimum  */
  /*  The maximum lies whiteLength (200) Samples behind minimum */
  /*  totalErrPos ist Summer der Fehler im Abstand von whiteLength (200) Samples  */
  /*  totalErrPos is sum of errors in a distance of whiteLength (200) Samples  */
  /*  totalErrPos = Fehler[i] + (MAX_FEHLER - Fehler[i + whiteLength]       */
  
  int totalMaxErr = checkLengthX2 * nLines;
  int totalErr = 10000;
  int totalErrPos = 0;
  int err;
  
  for (i = 0; i < nErrCountBuffer; i++) {
    /*  Summe aus Fehler beim B->W  und W->B  */
    /*  Sum of errors by B->W  und W->B  */
    err = ErrCountBuffer[i];
    err += totalMaxErr - ErrCountBuffer[(i + whiteLength) % nErrCountBuffer];
    if (totalErr > err) {
      totalErr = err;
      totalErrPos = i;
    }
  }
  
  // Die Lage den s/w bzw. w/s- Kanten der Phasing-Lines ist nun bekannt, als Durchschnitt über alle Phasing-Lines
  // Nun für jede einzelne Phasing-Line die Position suchen, in der Nähe (-checkLength/2 ... checkLength/2) der zu erwartenden Position
  // The position of s/W and w/s edges in phasing lines is now known, as average over all phasing lines
  // Now, look for every phasing line the position, near by (-checkLength/2 ... checkLength/2) the expected position
  int multipleCheckResult[65];
  
  for (i = 0; i < nLines - 11; i++) {
    multipleCheckResult[i] = PhasingPosition(BlackWhiteErrBuffer, PhasingBufferCount, totalErrPos + i * nLineLength, 10, checkLength, nLineLength, whiteLength);
    if (pOptions->verbose >= 3)
      printf("multipleCheckResult[%2i] = %6i     %4i\n", i, multipleCheckResult[i], multipleCheckResult[i] % 4000);
  }
  
  double delta[nLines - 21];
  int    SumDifferences = 0;
  for (i = 0; i < nLines - 21; i++) {
    SumDifferences += multipleCheckResult[i + 10] - multipleCheckResult[i];
    delta[i] = double(multipleCheckResult[i + 10] - multipleCheckResult[i]) * 0.1;
  }
  
  if (pOptions->verbose >= 3)
    for (i = 0; i < nLines - 21; i++)
      printf("delta[%2i] = %8.1f\n", i, delta[i]);
  
  SamplesPerLine = double(SumDifferences) / double((nLines - 21) * 10) + double(pOptions->SlantCorr) * 0.01;
  
  int Offset = multipleCheckResult[nLines - 11 - 1] + ((int)SamplesPerLine) * 10;
  ImageStart = Offset + checkLength + (whiteLength >> 1) + (int)SamplesPerLine;
  if (pOptions->verbose >= 2) {
    printf("ImageStart = %i\n", ImageStart);
    double d = fmod(double(ImageStart), SamplesPerLine);
    ImageStart = (int)d;
  }

  delete [] BlackWhiteErrBuffer;
  delete [] ErrCountBuffer;
  
  return true;
}  


void CmdFax::Separator() {
int  col;
int  currRow = mCurrRow;
int  pixel = 0;
int  width = faxImage.width();
int  y;

  // fill withe pixels until end of line
  col = mLastCol;
  switch (file_format) {
  case 1 :
    pixel = 1;
    break;
  case 8 :
    pixel = 255;
    break;
  case 32 :
    pixel = 255;
  }
  for (col = mLastCol; col < width; col++) 
    switch (file_format) {
    case 1 :
      faxImage.set_pixel_1(col, currRow, pixel);
      break;
    case 8 :
      faxImage.set_pixel_8(col, currRow, pixel);
      break;
    case 32 :
      faxImage.set_pixel_32(col, currRow, 0, 0, 0, pixel);
    }
  currRow++;

  // draw 19 lines, alternate b - w    
  for (y = 1; y < 20; y++) {
    switch (file_format) {
    case 1 :
      pixel = y < 10 || y & 1;
      for (col = 0; col < width; col++)
       faxImage.set_pixel_1(col, currRow, pixel);
      break;
    case 8 :
      pixel = (y < 10 || y & 1) ? 255 : 0;
      for (col = 0; col < width; col++)
        faxImage.set_pixel_8(col, currRow, pixel);
      break;
    case 32 :
      pixel = (y < 10 || y & 1) ? 255 : 0;
      for (col = 0; col < width; col++)
        faxImage.set_pixel_32(col, currRow, 0, 0, 0, pixel);
    }
    currRow++;
  }
  mCurrRow = currRow;
  mLineSample = 0;
  mPixel = 0;
  mPixelSamples = 0;
  mLastCol = 0;
}


void CmdFax::Image(int * Buffer, int BufferCount) {
int  col;
int  currRow = mCurrRow;
int  line_sample = mLineSample;
int  pixel = mPixel;
int  pixelSamples = mPixelSamples;
int  lastCol = mLastCol;
int  i;


  for (i = 0; i < BufferCount; i++) {
    line_sample++;
    if (line_sample > line_end_sample) {
      currRow++;
      last_line_start = line_sample;
      line_end_sample_d += SamplesPerLine;
      line_end_sample   = (int)(line_end_sample_d + 0.4999);
      pixel = 0;
      pixelSamples = 0;
    }
    col = (int)((line_sample - last_line_start) * line_pixels_div_line_samples);

    if (col == lastCol) {
      pixel+=Buffer[i];
      pixelSamples++;
    } else {
      if (pixelSamples > 0) {
        pixel /= pixelSamples;
        switch (file_format) {
        case 1 :
          faxImage.set_pixel_1(lastCol, currRow, pixel > threshold);
          break;
        case 8 :
          faxImage.set_pixel_8(lastCol, currRow, pixel);
          break;
        case 32 :
          faxImage.set_pixel_32(lastCol, currRow, 0, 0, 0, 255 - pixel);
        }
      }
      lastCol = col;
      pixel   = Buffer[i];
      pixelSamples = 1;
    }
  }
  mLineSample = line_sample;
  mCurrRow = currRow;
  mPixel = pixel;
  mPixelSamples = pixelSamples;
  mLastCol = lastCol;
}


int CmdFax::run() {
int   ret;
int   iTryStart = 0;
int   iTryStop  = 0;
int   CheckStart[10];
int   Deviations[10];
int   Deviation = 0;
int   DeviationSum;
int   CheckStop[10];
int   i, n;
int * PhasingBuffer;
int   PhasingBufferCount = 0;
int   PhasingBufferSize = ReducedSampleRate * 32;  // 64 lines
int   line_pixels;
int   nSamples8000 = 0;
int   nExpectedSamples8000 = 4000;
int   nFaxe = 0;
int   SamplesRead = 0;
float FaxPos = 0.0;
float Limit  = 0.0;

  mCurrRow = 0;
  faxImage.reset((int)(576 * M_PI), 0, file_format);
  
  PhasingBuffer = new int [PhasingBufferSize];
  PhasingBufferCount = 0;

  FaxState = IMAGE_END;

  while (!wav.eof()) {
    ret = wav.readSamples(InBuffer, InBufferSize);
    if (ret != InBufferSize)
      break;
    SamplesRead += ret;
    FaxPos += 0.5;      // 0.5 seconds

    switch (FaxState) {
    case FAX_IDLE :
      // fprintf(stderr, "FaxState = FAX_IDLE\n");
      CheckStart[iTryStart] = pPhaseTest->runTestStart(InBuffer, &Deviations[iTryStart]);
      iTryStart++;
      if (iTryStart == 10)
        iTryStart = 0;
      n = 0;
      for (i = 0; i < 10; i++)
        n += CheckStart[i];
      if (n >= 7) {
        // Correction for real Frequency
        DeviationSum = 0;
        for (i = 0; i < 10; i++) 
          if (CheckStart[i]) {
            DeviationSum += Deviations[i];
            if (pOptions->verbose >= 2)
              printf("deviation[%i] = %i\n", i, Deviations[i]);
          }
        Deviation = (int)roundf(float(DeviationSum) / float(n));
        if (pOptions->verbose >= 2)
          printf("deviation set to %i\n", Deviation);
        // Correct the Bins to check for in search for fax end mark
        pPhaseTest->setDeviaion(Deviation);
        FaxState = PHASING_START;
      }
      break;

    case PHASING_START :
      demodulator.init(SampleRate, CenterFrequency + Deviation, Shift, InBufferSize, pOptions->filterType);
      nSamples8000 = demodulator.processSamples(PhasingBuffer, InBuffer, InBufferSize, isIQSource);
      PhasingBufferCount = nSamples8000;
      FaxState = PHASING;
      break;
    case PHASING :
      if (PhasingBufferSize >= PhasingBufferCount + nExpectedSamples8000) {
        nSamples8000 = demodulator.processSamples(PhasingBuffer + PhasingBufferCount, InBuffer, InBufferSize, isIQSource);
        PhasingBufferCount += nSamples8000;
        break;
      }
      if (!Phasing(PhasingBuffer, PhasingBufferCount)) {
        FaxState = FAX_IDLE;
        break;
      }
      FaxState = IMAGE_INIT;
      // NO break
    case IMAGE_INIT :
      // der Rest aus dem PhasingBuffer wird verarbeitet
      // process the phasing buffer until it is empty
      mLineSample = 0;
      line_end_sample_d = 0.0;
      line_end_sample = -1;
      line_pixels = (int)(M_PI * 576 /*fax_ioc*/ + 0.4999);
      line_pixels_div_line_samples = line_pixels / SamplesPerLine;
      memmove(PhasingBuffer, PhasingBuffer + ImageStart, (PhasingBufferSize - ImageStart) * sizeof(int));
      FaxPos = 0.0;
      if (nFaxe)
        Separator();
      Limit = (float)pOptions->Limit(nFaxe);
      Image(PhasingBuffer, PhasingBufferSize - ImageStart);
      FaxState = IMAGE;
      nFaxe++;
      break;

    case IMAGE :
      CheckStop[iTryStop] = pPhaseTest->runTestStop(InBuffer);
      iTryStop++;
      if (iTryStop == 10)
        iTryStop = 0;
      n = 0;
      for (i = 0; i < 10; i++)
        n += CheckStop[i];
      if (n >= 7) {
        FaxState = IMAGE_END;
        break;
      }
      if (Limit > 0.0 && FaxPos >= Limit) {
        FaxState = IMAGE_END;
        fprintf(stderr, "FaxState = IMAGE_END       %6i  %3i:%02i    By Limit!\n", SamplesRead / SampleRate / wav.channels(), SamplesRead / SampleRate / wav.channels() / 60, (SamplesRead / SampleRate / wav.channels()) % 60);
        break;
      }
      nSamples8000 = demodulator.processSamples(PhasingBuffer, InBuffer, InBufferSize, isIQSource);
      Image(PhasingBuffer, nSamples8000);
      break;
    case IMAGE_END :
      for (i = 0; i < 10; i++) {
        CheckStart[i] = 0;
        CheckStop[i] = 0;
        Deviations[i] = 0;
      }
      iTryStart = 0;
      iTryStop = 0;
      FaxState = FAX_IDLE;
    }
  }
  wav.close();

  if (pOptions->verbose > 0) {
    switch (FaxState) {
    case FAX_IDLE :
    case IMAGE_END :
      if (nFaxe)
        printf("%i Faxes found\n", nFaxe);
      else
        printf("No fax found.\n");
      break;
    case PHASING_START :
      printf("Warning: FaxState = PHASING_START\n");
      break;
    case PHASING :
      printf("Warning: FaxState = PHASING\n");
      break;
    case IMAGE_INIT :
      printf("Warning: FaxState = IMAGE_INIT\n");
      break;
    case IMAGE :
      printf("Warning: FaxState = IMAGE\n");
      break;
    }
  }
  delete [] PhasingBuffer;

  return 0;
}
