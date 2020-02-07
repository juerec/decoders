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

#include "BaudotDecoder.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>

const unsigned char BaudotDecoder::BaudotLetters[32] = {
    ' ',    'E',    '\n',   'A',    ' ',    'S',    'I',    'U',
    '\r',   'D',    'R',    'J',    'N',    'F',    'C',    'K',
    'T',    'Z',    'L',    'W',    'H',    'Y',    'P',    'Q',
    'O',    'B',    'G',   0x1B,    'M',    'X',    'V',    0x1F
};

const unsigned char BaudotDecoder::BaudotFigures[32] = {
    ' ',    '3',    '\n',   '-',    ' ',    '\'',   '8',    '7',
    '\r',   '$',    '4',    '|',    ',',    '!',    ':',    '(',
    '5',    '+',    ')',    '2',    '#',    '6',    '0',    '1',
    '9',    '?',    '&',   0x1B,    '.',    '/',    '=',   0x1F
};

BaudotDecoder::BaudotDecoder() 
             : DecoderBase(){
  CharacterLen   = 0;
  BitLen         = 0;
  StopLen        = 0;
  halfBit        = 0;
  BaudotMode     = LETTERS;
}

BaudotDecoder::~BaudotDecoder() {
}

void BaudotDecoder::init(int characterLen, int bitLen, int stopLen) {
  CharacterLen   = characterLen;
  BitLen         = bitLen;
  StopLen        = stopLen;
  halfBit        = BitLen >> 1;
  BaudotMode     = LETTERS;
  ErrLookUpLen   = CharacterLen + StopLen; //    -ErrLookUpStart + ErrLookUpEnd;
} // ========================================== BaudotDecoder::init()


int BaudotDecoder::countErrors(int * Points) {
int i, k;
int nErr = 0;
int iErr;

  // Stop-Bit
  for (i = 0; i < StopLen; i++) {
    if (!*Points++)
      nErr++;
  }
  // Start-Bit
  for (i = 0; i < BitLen; i++) {
    if (*Points++)
      nErr++;
  }
  // 5x Data-Bit
  for (k = 0; k < 5; k++) {
    iErr = 0; 
    for (i = 0; i < BitLen; i++) {
      if (*Points++)
        iErr++;
    }
    if (iErr <= halfBit)
      nErr += iErr;
    else
      nErr += BitLen - iErr;
  }
  // Stop-Bit
  for (i = 0; i < StopLen; i++) {
    if (!*Points++)
      nErr++;
  }
  return nErr;  
}

int BaudotDecoder::decode(int * Points, int nPoints) {
int   errors[ErrLookUpLen];
int   iPos = 0;
int   endPos = nPoints - (ErrLookUpLen + CharacterLen + BitLen);
int   i, ii, k, x;
int   minErr, minErrPos;
int   Char;

  while (iPos < endPos) {
    for (i = 0; i < ErrLookUpLen; i++)
      errors[i] = countErrors(Points + iPos + i);
    
    minErr = 2000000000;
    minErrPos = 0;
    for (i = 0; i < ErrLookUpLen; i++)
      if (minErr > errors[i]) {
        minErr = errors[i];
        minErrPos = i;
      }
    ii = minErrPos + 1;
    k = 1;
    while (ii < ErrLookUpLen && errors[ii] == minErr) {
      ii++;
      k++;
    }
    minErrPos += k >> 1;
    
    ii = iPos + minErrPos + StopLen + BitLen + halfBit;

    Char = Points[ii] +
           (Points[ii + BitLen] << 1) +
           (Points[ii + BitLen * 2] << 2) +
           (Points[ii + BitLen * 3] << 3) +
           (Points[ii + BitLen * 4] << 4);

    Char = decodeChar(Char);
    switch (Char) {
    case 0 :
      outputChar(' ');
    case 0x1F :
    case 0x1B :
    case '\r' :
      break;
    default :
      outputChar(Char);
    }
    iPos = ii + (CharacterLen / 3);
  }
  return iPos;    
}

int BaudotDecoder::decodeChar(int Char) {

  switch (Char) {
  case 0x1F:        /* letters */
    BaudotMode = LETTERS;
    return Char;
  case 0x1B:        /* figures */
    BaudotMode = FIGURES;
    return Char;
  default:
    return (BaudotMode == LETTERS) ? BaudotLetters[Char] : BaudotFigures[Char];
  }
} // ========================================== BaudotDecoder::decodeChar()

