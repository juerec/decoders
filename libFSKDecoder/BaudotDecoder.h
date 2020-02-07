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

#ifndef BAUDOTDECODER_H
#define BAUDOTDECODER_H

#include "DecoderBase.h"

class BaudotDecoder : public DecoderBase {

  enum Mode {
    LETTERS,
    FIGURES
  };

public :
  BaudotDecoder();
  ~BaudotDecoder();

  virtual void init(int CharacterLen, int BitLen, int StopLen);
  virtual int decode(int * Points, int nPoints);

  protected :
    static const unsigned char BaudotLetters[32];
    static const unsigned char BaudotFigures[32];
    int   CharacterLen;
    int   BitLen;
    int   StopLen;
    int   halfBit;
    int   ErrLookUpLen;
    Mode  BaudotMode;
    int   countErrors(int * Points);
    int   runDecoder(int * startPos = NULL);
    int   decodeChar(int Char);
};

#endif // BAUDOTDECODER_H
