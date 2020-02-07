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

#include "NavtexDecoder.h"

#include <string.h>

const unsigned char NavtexDecoder::NavtexValidChars[128] = {
      //  0       1       2       3         4       5       6       7           8      9       A       B         C       D       E       F
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80', '\x80',    '\x80', '\x80', '\x80', '\x80',   '\x8F', '\x80', '\x80', '\x0F',
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80', '\x17',    '\x80', '\x80', '\x80', '\x1B',   '\x80', '\x1D', '\x1E', '\x80',
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80', '\x27',    '\x80', '\x80', '\x80', '\x2B',   '\x80', '\x2D', '\x2E', '\x80',
      '\x80', '\x80', '\x80', '\x33',   '\x80', '\x35', '\x36', '\x80',    '\x80', '\x39', '\x3A', '\x80',   '\x3C', '\x80', '\x80', '\x80',
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80', '\x47',    '\x80', '\x80', '\x80', '\x4B',   '\x80', '\x4D', '\x4E', '\x80',
      '\x80', '\x80', '\x80', '\x53',   '\x80', '\x55', '\x56', '\x80',    '\x80', '\x59', '\x5A', '\x80',   '\x5C', '\x80', '\x80', '\x80',
      '\x80', '\x80', '\x80', '\x63',   '\x80', '\x65', '\x66', '\x80',    '\x80', '\x69', '\x6A', '\x80',   '\x6C', '\x80', '\x80', '\x80',
      '\x80', '\x71', '\x72', '\x80',   '\x74', '\x80', '\x80', '\x80',    '\x78', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80', '\x80' };

const unsigned char NavtexDecoder::NavtexLetters[128] = {
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80', '\x80',    '\x80', '\x80', '\x80', '\x80',   '\x8F', '\x80', '\x80', '\x8F',
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80',    'J',    '\x80', '\x80', '\x80',    'F',   '\x80',    'C',    'K', '\x80',
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80',    'W',    '\x80', '\x80', '\x80',    'Y',   '\x80',    'P',    'Q', '\x80',
      '\x80', '\x80', '\x80', '\xB3',   '\x80',    'G', '\xB6', '\x80',    '\x80',    'M',    'X', '\x80',      'V', '\x80', '\x80', '\x80',
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80',    'A',    '\x80', '\x80', '\x80',    'S',   '\x80',    'I',    'U', '\x80',
      '\x80', '\x80', '\x80',    'D',   '\x80',    'R',    'E', '\x80',    '\x80',    'N', '\xDA', '\x80',      ' ', '\x80', '\x80', '\x80',
      '\x80', '\x80', '\x80',    'Z',   '\x80',    'L', '\xE6', '\x80',    '\x80',    'H',   ' ',  '\x80',     '\n', '\x80', '\x80', '\x80',
      '\x80',    'O',    'B', '\x80',      'T', '\x80', '\x80', '\x80',      '\r', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80', '\x80' };

const unsigned char NavtexDecoder::NavtexFigures[128] = {
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80', '\x80',    '\x80', '\x80', '\x80', '\x80',   '\x8F', '\x80', '\x80', '\x8F',
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80',   '\\',    '\x80', '\x80', '\x80',    '!',   '\x80',    ':',    '(', '\x80',
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80',    '2',    '\x80', '\x80', '\x80',    '6',   '\x80',    '0',    '1', '\x80',
      '\x80', '\x80', '\x80', '\xB3',   '\x80',    '&', '\xB6', '\x80',    '\x80',    '.',    '/', '\x80',      '=', '\x80', '\x80', '\x80',
      '\x80', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80',    '-',    '\x80', '\x80', '\x80',    '"',   '\x80',    '8',    '7', '\x80',
      '\x80', '\x80', '\x80',    '$',   '\x80',    '4',    '3', '\x80',    '\x80',    ',', '\xDA', '\x80',      ' ', '\x80', '\x80', '\x80',
      '\x80', '\x80', '\x80',    '+',   '\x80',    ')', '\xE6', '\x80',    '\x80',    '#',    ' ', '\x80',     '\n', '\x80', '\x80', '\x80',
      '\x80',    '9',    '?', '\x80',      '5', '\x80', '\x80', '\x80',      '\r', '\x80', '\x80', '\x80',   '\x80', '\x80', '\x80', '\x80' };

NavtexDecoder::NavtexDecoder()
             : DecoderBase() {
  CharCounter   = -1;
  StartMark     = 0;
  NavtexMode    = LETTERS;
  for (int i = 0; i < 5; i++)
    Fec[i] = 0x80;
  isSync = false;
  for (int i = 0; i < 16; i++)
    EndMark[i] = ' ';
  EndMarkPos = 0;
} // ========================================== NavtexDecoder::NavtexDecoder()

NavtexDecoder::~NavtexDecoder() {
} // ========================================== NavtexDecoder::~NavtexDecoder()

  
void NavtexDecoder::init(int CharacterLen, int BitLen, int) {
  this->CharacterLen = CharacterLen;
  this->BitLen       = BitLen;
} // ========================================== NavtexDecoder::init()

int NavtexDecoder::findBitSync(int * Input, int nInput) {
int iPos, iBit, iPoint, iInput, Sum, ErrSum, nBits;
int halfBitLen = BitLen >> 1;
int BitError[BitLen];
int minErr = nInput;
int minErrPos = 0;

  nBits = nInput / BitLen - 2;
  if (nBits > 56)
    nBits = 56;
  if (nBits < 7)
    return -1;

  for (iPos = 0; iPos < BitLen; iPos++) {
    ErrSum = 0;
    iInput = iPos;

    for (iBit = 0; iBit < nBits; iBit++) {
      Sum = 0;
      for (iPoint = 0; iPoint < BitLen; iPoint++)
        Sum += Input[iInput++];
      if (Sum > halfBitLen)
        ErrSum += BitLen - Sum;
      else
        ErrSum += Sum;
    }
    BitError[iPos] = ErrSum;
  }

  int maxErr = 0;
  int maxErrPos = 0;
  int xx;

  for (iPos = 0; iPos < BitLen; iPos++) {
    if (minErr > BitError[iPos]) {
      minErr = BitError[iPos];
      minErrPos = iPos;
    }
    if (maxErr < BitError[iPos]) {
      maxErr = BitError[iPos];
      maxErrPos = iPos;
    }
  }

//  fprintf(stderr, " .... BitError:");
//  fprintf(stderr, "  ==>  %4i %4i\n", minErrPos, maxErrPos);
//  for (int xx = 0; xx < BitLen; xx++)
//    fprintf(stderr, "%5i", BitError[xx]);
  if (BitLen >= 8) {
    if (minErrPos < (BitLen >> 2) || minErrPos >= halfBitLen + (BitLen >> 2)) {
      int maxErrPos2 = maxErrPos;
      for (xx = maxErrPos + 1; xx < BitLen && BitError[xx] == maxErr; xx++)
        maxErrPos2++;
      xx = ((maxErrPos + maxErrPos2) >> 1);
      if (xx >= halfBitLen)
        minErrPos = xx - halfBitLen;
      else
        minErrPos = xx + halfBitLen;
    } else {
      int minErrPos2 = minErrPos;
      for (xx = minErrPos + 1; xx < BitLen && BitError[xx] == minErr; xx++)
        minErrPos2++;
      minErrPos = ((minErrPos + minErrPos2) >> 1);
    }
  }

  return minErrPos;
} // ========================================== NavtexDecoder::findBitSync()

int NavtexDecoder::readChar(int * Input) {
int iPoint = BitLen >> 1;
int Char   = 0;

  for (int i = 0; i < 7; i++) {
    Char += (Input[iPoint] == 0) << i;
    iPoint += BitLen;
  }
  return Char;
} // ========================================== NavtexDecoder::readChar()

int NavtexDecoder::decodeChar(int Char) {

  if (NavtexMode == LETTERS)
    Char = NavtexLetters[Char];
  else
    Char = NavtexFigures[Char];
  if (Char >= 0x80)
    switch (Char) {
    case 0xB6 :
      NavtexMode = FIGURES;
      break;
    case 0xDA :
      NavtexMode = LETTERS;
      break;
    case 0x80 :  // illegal Character
    case 0x8F :  // ALPHA
    case 0xB3 :  // BETA
    case 0xE6 :  // REP
      break;
    }
  if (Char == 0x8F) {
    EndMark[EndMarkPos] = Char;
    EndMarkPos++;
    if (EndMarkPos >= 16)
      EndMarkPos = 0;
    DecoderBase::outputChar('@');
  } 
  int nEndMark = 0;
  for (int i = 0; i < 16; i++)
    if (EndMark[i] == 0x8F)
      nEndMark++;
  if (nEndMark >= 10) {
    for (int i = 0; i < 16; i++)
      EndMark[i] = ' ';
    EndMarkPos = 0;
    isSync = false;
    CharCounter = -1;
    for (int i = 0; i < 16; i++) {
      DecoderBase::outputChar('-');
      DecoderBase::outputChar('=');
    }
    DecoderBase::outputChar('\n');
  }
  return Char;
} // ========================================== NavtexDecoder::decodeChar()

void NavtexDecoder::outputChar(int Char) {

    switch (Char) {
    case 0x80 :
      Char = '_';
      break;
    case 0x8F :
      Char = '@';
      break;
    case 0xB3 :
      Char = '^';
      break;
    case 0xE6 :
      Char = '|';
      break;
    }
    if (Char != '\r')
      DecoderBase::outputChar((char)Char);
} // ========================================== NavtexDecoder::outputChar()

int NavtexDecoder::SyncCharBitError(int * Input) {
int err = 0;
int Offset = BitLen >> 1;

  err += Input[Offset] == 0;
  Offset += BitLen;
  err += Input[Offset] != 0;
  Offset += BitLen;
  err += Input[Offset] != 0;
  Offset += BitLen;
  err += Input[Offset] == 0;
  Offset += BitLen;
  err += Input[Offset] == 0;
  Offset += BitLen;
  err += Input[Offset] != 0;
  Offset += BitLen;
  err += Input[Offset] != 0;
  Offset += BitLen;
  err += Input[Offset] != 0;
  Offset += BitLen;
  err += Input[Offset] != 0;
  Offset += BitLen;
  err += Input[Offset] != 0;
  Offset += BitLen;
  err += Input[Offset] != 0;
  Offset += BitLen;
  err += Input[Offset] == 0;
  Offset += BitLen;
  err += Input[Offset] == 0;
  Offset += BitLen;
  err += Input[Offset] == 0;
  return err;
} // ========================================== NavtexDecoder::SyncCharBitError()

int NavtexDecoder::isSyncCharChain(int * Input, int nInput) {
int nSyncChars = (nInput / CharacterLen) >> 1;   
int iSyncChar= 0;
int ErrCnt = 0;

  if (nSyncChars > 5)
    nSyncChars = 5;
  
  for (iSyncChar = 0; iSyncChar < nSyncChars; iSyncChar++) {
    ErrCnt += SyncCharBitError(Input) != 0;
    Input += (CharacterLen << 1);
  }
  return ErrCnt <= nSyncChars >> 1;
}

// returns the pos of the first sync char pair
// if not found -1
int NavtexDecoder::findSyncronisation(int * Input, int nInput) {
int SyncCharLength = CharacterLen << 1;
int ErrArray[SyncCharLength];
int nSyncChars = nInput / SyncCharLength - 2;
int iPos, Sum;
int TwoBits = BitLen << 1;
int FourBits = BitLen << 2;
int SixBits  = TwoBits + FourBits;
int minErr = nInput;
int minErrPos = 0;
int iSyncChar, iPoint, iInput, n, nHits;

  if (nSyncChars < 2)
    return -1;

#ifdef SYNC_ALL_POINTS


  for (iPos = 0; iPos < SyncCharLength; iPos++) {
  /*  0001111         1100110
      <==         <==
      in time order: 1111000 0110011

      4x 0
      2x 1
      2x 0
      6x 1      */
    iInput = iPos;
    Sum = 0;
    for (iSyncChar = 0; iSyncChar < nSyncChars; iSyncChar++) {
      for (iPoint = 0; iPoint < FourBits; iPoint++) {
        // printf("%c", Input[iInput] != 0 ? '1' : '0');
        if (Input[iInput++] == 0)
          Sum++;
      }
      for (iPoint = 0; iPoint < TwoBits; iPoint++) {
        // printf("%c", Input[iInput] != 0 ? '1' : '0');
        if (Input[iInput++] != 0)
          Sum++;
      }
      for (iPoint = 0; iPoint < TwoBits; iPoint++) {
        // printf("%c", Input[iInput] != 0 ? '1' : '0');
        if (Input[iInput++] == 0)
          Sum++;
      }
      for (iPoint = 0; iPoint < SixBits; iPoint++) {
        // printf("%c", Input[iInput] != 0 ? '1' : '0');
        if (Input[iInput++] != 0)
          Sum++;
      }
    }
    ErrArray[iPos] = Sum;
    // printf("iInput = %4i Sum = %4i\n", iInput, Sum);
  }
  for (iPos = 0; iPos < SyncCharLength; iPos++) {
    if (minErr > ErrArray[iPos]) {
      minErr = ErrArray[iPos];
      minErrPos = iPos;
    }
  }

  // correct the 11 bit offset from search algorithm
  n = minErrPos - 11 * BitLen;
  if (n < BitLen)
    n += SyncCharLength;

  // we have found thes best fitting position, but really found sync chracters?
  iPos = n;
/*  
  for (int ip = 0; ip < SyncCharLength; ip++) {
    printf("%3i: ", ip);
    for (int ic = 0; ic < (nInput / CharacterLen) - 2; ic++)
      printf(" %02X", readChar(Input + ic * CharacterLen + ip));
    printf("\n");
    printf("%3i: ", ip);
    for (int ic = 0; ic < (nInput / SyncCharLength) - 2; ic++)
      printf("%6i", SyncCharBitError(Input + (ic * SyncCharLength + ip)));
    printf("\n");
  }
*/
#endif // SYNC_ALL_POINTS

  for (int ip = 0; ip < SyncCharLength; ip++) {
    int err = 0;
    for (int ic = 0; ic < (nInput / SyncCharLength) - 2; ic++)
      err += SyncCharBitError(Input + (ic * SyncCharLength + ip));
    ErrArray[ip] = err;
  }
  minErr = 999999;
  for (iPos = 0; iPos < SyncCharLength; iPos++) {
    if (minErr > ErrArray[iPos]) {
      minErr = ErrArray[iPos];
      minErrPos = iPos;
    }
  }

  int xx = minErrPos;
  while (ErrArray[xx] == minErr)
    xx++;
  iPos = (minErrPos + xx) >> 1;
  
  nHits = 0;
  int ret = -1;
  nSyncChars = (nInput - iPos) / SyncCharLength;
  iSyncChar = 0;

  while (iSyncChar < nSyncChars && SyncCharBitError(Input + iPos) > 0) {
    iSyncChar++;
    iPos += SyncCharLength;
  }
 
  if (iSyncChar == nSyncChars)
    return -1;
  
  int lastSyncCharPos = iPos;
  while (iSyncChar < nSyncChars && isSyncCharChain(Input + iPos, nInput - iPos)) {
    nHits++;
    iSyncChar++;
    lastSyncCharPos = iPos;
    iPos += SyncCharLength;
  }
  
  if (nHits > 5)
    return iPos;
  return -1;
} // ========================================== NavtexDecoder::findSyncronisation()

// returns the actual used count of points:  = *nIntOutput * CharacterLen + *startPos
int NavtexDecoder::runDecoder(int * Input, int nInput, int * startPos) {
int StartPos = *startPos;
int nChars;
int Char, iChar, iPos;
int BitSync;

  StartPos = *startPos;

  BitSync = findBitSync(Input, nInput);

  if (BitSync >= 0)
    StartPos = BitSync;
  
  nChars  = ((nInput - BitLen * 2 - StartPos) / CharacterLen) & 0xFE;
  iPos = StartPos;
  for (iChar = 0; iChar < nChars; iChar++) {
    Char = validateChar(readChar(Input + iPos));
    if (!(iChar & 1)) {
      if (CharCounter >= 0)
        Fec[CharCounter % 5] = Char;
    } else {
      if (CharCounter > 4) {
        int CharFec = Fec[CharCounter % 5];
        if (CharFec == 0x80)
          CharFec = Char;
        if (CharFec == 0x80)
          Char = '_';
        else
          Char = decodeChar(CharFec);
        if (Char <= 0x80)
          outputChar(Char);
      }
    }
    if (CharCounter != -1)
      CharCounter++;
    iPos += CharacterLen;
  }
  *startPos = StartPos;
  return iPos;
} // ========================================== NavtexDecoder::runDecoder()


void  NavtexDecoder::printPoints(int * Points) {
int  c1 = readChar(Points);
int  c2 = readChar(Points + 56);
int  c1c = c1 >= 0x80 ? 0x80 : NavtexValidChars[c1];
int  c2c = c2 >= 0x80 ? 0x80 : NavtexValidChars[c2];


  for (int x = 0; x < 112; x++)
    fprintf(stderr, "%c", (x & 7) == 0 ? (Points[x] ? 'I' : '*') : (Points[x] ? '!' : '.'));
  fprintf(stderr, " - %02X.%02X   %c %c \\n\n", c1, c2, c1c >= 0x80 ? '^' : NavtexLetters[c1c], c2c >= 0x80 ? '^' : NavtexLetters[c2c]);
}


int NavtexDecoder::decode(int * Points, int nPoints) {
int x, ret = -1;
int halfBitLen = BitLen >> 1;
int StartPos = halfBitLen;
int Offset = 0;
int xx;


  if (!isSync) {
    x = findSyncronisation(Points, nPoints);
    if (x < 0)
      return  nPoints - (CharacterLen << 2);
    DecoderBase::outputChar('\n');
    for (int i = 0; i < 16; i++) {
      DecoderBase::outputChar('-');
      DecoderBase::outputChar('.');
    }
    DecoderBase::outputChar('\n');
    isSync = true;
    CharCounter = 0;
    Offset = x - halfBitLen;
    if (Offset < 0)
      Offset = 0;
  }
  ret = Offset + runDecoder(Points + Offset, nPoints - Offset, &StartPos);
  return ret;
} // ========================================== NavtexDecoder::dekodieren()
