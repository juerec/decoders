/****************************************************************************
**
**  This file is a part of the program "CMDFSK".
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

#ifndef WAVEFILE_H
#define WAVEFILE_H

#include <stdio.h>


#ifdef WAVEFILE_EXTRA
#ifndef WAVEFILE_READ
#define WAVEFILE_READ
#endif
#endif

// For compatibily
#ifndef WAVEFILE_WRITE
#ifndef WAVEFILE_READ
#define WAVEFILE_WRITE
#define WAVEFILE_READ
#endif // WAVEFILE_READ
#endif // WAVEFILE_WRITE

#ifdef WAVEFILE_EXTRA
#ifndef WAVEFILE_PLAYTIME
#define WAVEFILE_PLAYTIME
#endif
#endif

class WaveFile {

protected :

  FILE * f;
  short          BitsPerSample;
  unsigned int   Channels;
  unsigned int   SampleRate;
  unsigned int   formatHeaderLength;
  int            BytesPerSecond;
  long           Subchunk2Size;
  short          BlockAlign;
  char           FileMode;
  int            HeaderSize;
  bool           isVerbose;

public :

  WaveFile(bool verbose = false);
  ~WaveFile();

#ifdef WAVEFILE_WRITE
  bool create(const char * pFilename, int SampleRate, int Channels, int BitsPerSample);
  bool writeSamples(void * Samples, int nSamples);
#endif // WAV_PCM_WRITE

  bool   isOpen() { return f != NULL; }
  void   close();

#ifdef WAVEFILE_READ
  bool   open(const char * pFilename);
  int    sampleRate();
  int    bitsPerSample();
  unsigned int channels();
  int    byteRate();
  int    readSamples(void * Samples, size_t maxSamples);
  int    eof();
  
#ifdef WAVEFILE_PLAYTIME
  long   playTimeMilliSec();
  long   playTimeSec();
#endif // WAVEFILE_PLAYTIME

#ifdef WAVEFILE_EXTRA
  long   sampleCount();
  size_t readBytesAtMilliSec(void * Samples, size_t maxBytes, long milliSecPos, long * newFilePos, int * newMilliSec);
  size_t readBytesAtFilePos(void * Samples, size_t maxBytes, long bytePos, long * newFilePos, int * newMilliSec);
  int    readAtSamplePos(void * Samples, size_t maxSamples, int atSamplePos, int * newSamplePos);
  long   readBytes(void * Samples, size_t maxBytes, long bytePos, long * newPos);
  int    seekToSample(int Sample);
  int    seekToSec(int sec);
  long   samplePos();
#endif // WAVEFILE_EXTRA
#endif // WAVEFILE_READ
};

#endif /* WAVEFILE_H */
