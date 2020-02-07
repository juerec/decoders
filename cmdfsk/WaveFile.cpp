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

#include "WaveFile.h"

#include <string.h>
#include <errno.h>

WaveFile::WaveFile(bool verbose)
        : isVerbose(verbose) {
  f             = NULL;
  Subchunk2Size  = 0;
  BitsPerSample  = 0;
  Channels       = 0;
  SampleRate     = 0;
  formatHeaderLength = 0;
  FileMode       = ' ';
  HeaderSize     = 0;
}

WaveFile::~WaveFile() {
  close();
}

#ifdef WAVEFILE_WRITE

inline char cfi(int i) {   // cast from int
  return (char) (i & 0x000000FF);
}

inline char cfi(int i, int shift) {   // cast from int
  return (char) ((i >> shift) & 0x000000FF);
}


bool WaveFile::create(const char * pFilename, int SampleRate, int Channels, int bitsPerSample) {
int   ByteRate = SampleRate * Channels * bitsPerSample/8;
int   BlockAlign = Channels * bitsPerSample/8;
const char Chunk1[12] = { 'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E' };
const char Subchunk1[24]    = {'f', 'm', 't', ' ',
     /* Subchunk1Size  */       16, 0, 0, 0,
     /* AudioFormat PCM */      1, 0,
     /* Channels */             cfi(Channels), 0,
     /* Sample rate */          cfi(SampleRate), cfi(SampleRate, 8), cfi(SampleRate, 16), cfi(SampleRate, 24),
     /* ByteRate */             cfi(ByteRate), cfi(ByteRate, 8),  cfi(ByteRate, 16), cfi(ByteRate, 24),
     /* BlockAlign */           cfi(BlockAlign), cfi(BlockAlign, 8),
     /* BitsPerSample */        cfi(bitsPerSample), cfi(bitsPerSample, 8) };
const char Subchunk2[8] = { 'd', 'a', 't', 'a', 0, 0, 0, 0 };
int ret;

//  fprintf(stderr, "WaveFile::create(<pFilename>, SampleRate = %i, Channels = %i, bitsPerSample = %i)\n", SampleRate, Channels, bitsPerSample);
//  fprintf(stderr, "  ByteRate = %i   BlockAlign = %i\n", ByteRate, BlockAlign);

  WaveFile::BitsPerSample = bitsPerSample;
  WaveFile::Channels = Channels;
  WaveFile::SampleRate = SampleRate;
  f = fopen(pFilename, "w");
  if (f == NULL) {
    if (isVerbose)
      fprintf(stderr, "Error creating file \"%s\" - %s\n", pFilename, strerror(errno));
    return false;
  }
  FileMode = 'w';
  ret = fwrite(Chunk1, sizeof(char), sizeof(Chunk1), f);
  if (ret != sizeof(Chunk1)) {
    fclose(f);
    f = NULL;
    return false;
  }
  ret = fwrite(Subchunk1, sizeof(char), sizeof(Subchunk1), f);
  if (ret != sizeof(Subchunk1)) {
    fclose(f);
    f = NULL;
    return false;
  }
  ret = fwrite(Subchunk2, sizeof(char), sizeof(Subchunk2), f);
  if (ret != sizeof(Subchunk2)) {
    fclose(f);
    f = NULL;
    return false;
  }
  return true;
}

bool WaveFile::writeSamples(void * Samples, int nSamples) {
unsigned int BytesPerSample = BitsPerSample >> 3;

  int ret;
  ret = fwrite(Samples, BytesPerSample, nSamples, f);
//fprintf(stderr, "IN: WaveFile::writeSamples(nSamples = %i)  ret = %i\n", nSamples, ret);
  if (ret <= 0) {
    fprintf(stderr, "Error in WaveFile::writeSamples() %i    %s\n", ret, strerror(errno));
  } else {
    if (ret < nSamples)
      fprintf(stderr, "Error in WaveFile::writeSamples() ret = %i   nSamples = %i   %s\n", ret, nSamples, strerror(errno));
  }
  if (ret > 0)
    Subchunk2Size += ret * BytesPerSample;
  return ret == nSamples;
}
#endif // WAVEFILE_WRITE

void WaveFile::close() {
unsigned long val;
//  fprintf(stderr, "WaveFile::close()   FileMode = %c    Subchunk2Size = %li   f %c= NULL\n", FileMode, Subchunk2Size, f == NULL ? '=' : '!');
  if (f != NULL) {
    switch (FileMode) {
    case 'w' :
      val = Subchunk2Size + 36;
      fseek(f, 4, SEEK_SET);
      // fwrite((void *)&val, 1, sizeof(unsigned long), f);
      fwrite((void *)&val, 1, 4, f);
      val = Subchunk2Size;
      fseek(f, 40, SEEK_SET);
      // fwrite((void *)&val, 1, sizeof(unsigned long), f);
      fwrite((void *)&val, 1, 4, f);
    case 'r' :
      fclose(f);
    default :
      f = NULL;
      FileMode = ' ';
      SampleRate = 0;
      Subchunk2Size = 0;
      Channels = 0;
    }
  }
}

#ifdef WAVEFILE_READ
bool WaveFile::open(const char * pFilename) {
char arr[8];
// int  ret;
short format;
int   AuxiLength;

  if (FileMode != ' ')
    return false;
  f = fopen(pFilename, "r");
  if (f == NULL) {
    if (isVerbose)
      fprintf(stderr, "Error opening input file \"%s\" - %s\n", pFilename, strerror(errno));
    return false;
  }
  FileMode = 'r';
  while (1) {
    if (fread(arr, 1, 8, f) != 8)
      break;
    if (arr[0] != 'R' || arr[1] != 'I' || arr[2] != 'F' || arr[3] != 'F')
      break;
    if (fread(arr, 1, 4, f) != 4)
      break;
    if (arr[0] != 'W' || arr[1] != 'A' || arr[2] != 'V' || arr[3] != 'E')
      break;
    if (fread(arr, 1, 4, f) != 4)
      break;
    if (arr[0] != 'f' || arr[1] != 'm' || arr[2] != 't' || arr[3] != ' ')
      break;
    if (fread(&formatHeaderLength, 4, 1, f) != 1)
      break;
    if (fread(&format, 1, 2, f) != 2)
      break;
    if (format != 1)
      break;
    if (fread(&Channels, 1, 2, f) != 2)
      break;
    if (fread(&SampleRate, 1, 4, f) != 4)
      break;
    if (fread(&BytesPerSecond, 1, 4, f) != 4)
      break;
    if (fread(&BlockAlign, 1, 2, f) != 2)
      break;
    if (fread(&BitsPerSample, 1, 2, f) != 2)
      break;
    if (fseek(f, formatHeaderLength + 20, SEEK_SET) != 0)
      break;
    if (fread(arr, 1, 4, f) != 4)
      break;
    if (arr[0] == 'a' && arr[1] == 'u' && arr[2] == 'x' && arr[3] == 'i') {
      if (fread(&AuxiLength, 1, 4, f) != 4)
        break;
      if (fseek(f, AuxiLength, SEEK_CUR) != 0)
        break;
      if (fread(arr, 1, 4, f) != 4)
        break;
    }
    if (arr[0] != 'd' || arr[1] != 'a' || arr[2] != 't' || arr[3] != 'a')
      break;
    if (fread(&Subchunk2Size, 1, 4, f) != 4)
      break;
    HeaderSize = ftell(f);
    return true;
  }
  fclose(f);
  f = NULL;
  FileMode = ' ';
  return false;
}

int  WaveFile::sampleRate() {
  return SampleRate;
}

int  WaveFile::bitsPerSample() {
  return BitsPerSample;
}

unsigned int WaveFile::channels() {
  return Channels;
}

int  WaveFile::byteRate() {
  return SampleRate * Channels * (BitsPerSample >> 3);
}

int WaveFile::readSamples(void * Samples, size_t maxSamples) {
  if (f == NULL || SampleRate == 0)
    return -1;
  return fread(Samples, BitsPerSample >> 3, maxSamples, f);
}

int WaveFile::eof() {
  if (f == NULL || SampleRate == 0 || FileMode != 'r')
    return -1;
  return feof(f);
}


#ifdef WAVEFILE_PLAYTIME

long WaveFile::playTimeMilliSec() {
long long ll;

  if (f == NULL || SampleRate == 0 || FileMode != 'r')
    return -1;
  ll = Subchunk2Size / ((BitsPerSample >> 3) * Channels);
  ll *= 1000;
  return (long)(ll / SampleRate);
}

long WaveFile::playTimeSec() {
long long ll;

  if (f == NULL || SampleRate == 0 || FileMode != 'r')
    return -1;
  ll = Subchunk2Size / ((BitsPerSample >> 3) * Channels);
  return (long)(ll / SampleRate);
}

#endif

#ifdef WAVEFILE_EXTRA

long   WaveFile::sampleCount() {
  return Subchunk2Size / (BitsPerSample >> 3);
}

size_t WaveFile::readBytesAtMilliSec(void * Samples, size_t maxBytes, long milliSecPos, long * newFilePos, int * newMilliSec) {
long pos;
//int  milliSec;
size_t ret;

  if (f == NULL || SampleRate == 0 || FileMode != 'r')
    return -1;

  maxBytes -= maxBytes % BlockAlign;
  pos       = (milliSecPos * BlockAlign * SampleRate) / 1000 + 24 + formatHeaderLength;

  // EOF ?
  if (pos >= (long)(Subchunk2Size + 24 + formatHeaderLength))
    return 0;

  ret = fseek(f, pos, SEEK_SET);
  if (ret != 0) {
    fprintf(stderr, " WaveFile::readSamples() ==> fseek(f, %li, SEEK_SET): %s\n", pos, strerror(errno));
    return -1;
  }
  ret =  fread(Samples, 1, maxBytes, f);
  if (long(ret) < 0) {
    fprintf(stderr, " WaveFile::readSamples() ==> fread(Samples, 1, %li, f): %s\n", pos, strerror(errno));
    return -1;
  }
  *newFilePos = ftell(f);
  *newMilliSec = ((*newFilePos - (24 + formatHeaderLength)) * 1000) / (BlockAlign * SampleRate);
  return ret;
}

size_t WaveFile::readBytesAtFilePos(void * Samples, size_t maxBytes, long bytePos, long * newFilePos, int * newMilliSec) {
int ret;

  maxBytes -= maxBytes % BlockAlign;
  ret = readBytes(Samples, maxBytes, bytePos, newFilePos);
  if (ret > 0)
    *newMilliSec = ((*newFilePos - (24 + formatHeaderLength)) * 1000) / (BlockAlign * SampleRate);
  return ret;
}

long WaveFile::readBytes(void * Samples, size_t maxBytes, long bytePos, long * newPos) {
long pos;
int  ret;

  if (f == NULL || SampleRate == 0 || FileMode != 'r')
    return -1;

  if (Subchunk2Size <= bytePos)
    return -1;

  pos =  20 + formatHeaderLength + bytePos;
  ret = fseek(f, pos, SEEK_SET);
  if (ret != 0) {
    fprintf(stderr, " WaveFile::readSamples() ==> fseek(f, %li, SEEK_SET): %s\n", pos, strerror(errno));
    return -1;
  }
  ret =  fread(Samples, 1, maxBytes, f);
  if (ret <= 0) {
    fprintf(stderr, " WaveFile::readSamples() ==> fread(Samples, 1, %li, f): %s\n", pos, strerror(errno));
    return -1;
  }
  *newPos = ftell(f);
  return ret;
}

int WaveFile::readAtSamplePos(void * Samples, size_t maxSamples, int atSamplePos, int * newSamplePos) {
int ret;

  if (f == NULL)
    return -1;

  ret = fseek(f, atSamplePos * (BitsPerSample >> 3), SEEK_SET);
  if (ret != 0) {
    fprintf(stderr, " WaveFile::readAtSamplePos() ==> fseek(f, %i, SEEK_SET): %s\n", atSamplePos * (BitsPerSample >> 3), strerror(errno));
    return -1;
  }
  ret = fread(Samples, BitsPerSample >> 3, maxSamples, f);
  if (ret <= 0) {
    fprintf(stderr, " WaveFile::readAtSamplePos() ==> fread(Samples, %i, %li, f): %s\n", BitsPerSample >> 3, maxSamples, strerror(errno));
    return -1;
  }
  if (newSamplePos != NULL)
    *newSamplePos = ftell(f);
  return ret;
}

int WaveFile::seekToSec(int sec) {
  if (f == NULL || SampleRate == 0 || FileMode != 'r')
    return -1;

  int pos = 20 + formatHeaderLength + BytesPerSecond * sec;
  int ret = fseek(f, pos, SEEK_SET);
  if (ret != 0) {
    fprintf(stderr, " WaveFile::readSamples() ==> fseek(f, %i, SEEK_SET): %s\n", pos, strerror(errno));
    return -1;
  }
  return pos;
}

int WaveFile::seekToSample(int Sample) {
  if (f == NULL || SampleRate == 0 || FileMode != 'r')
    return -1;

  int pos = 20 + formatHeaderLength + (BitsPerSample >> 3) * Sample;
  int ret = fseek(f, pos, SEEK_SET);
  if (ret != 0) {
    fprintf(stderr, " WaveFile::seekToSample(%i) ==> fseek(f, %i, SEEK_SET): %s\n", Sample, pos, strerror(errno));
    return -1;
  }
  return pos;
}

long WaveFile::samplePos() {
  return (ftell(f) - (20 + formatHeaderLength)) / (BitsPerSample >> 3);
}

#endif // WAVEFILE_EXTRA
#endif // WAVEFILE_READ
