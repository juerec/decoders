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

#ifndef NAVTEXDECODER_H
#define NAVTEXDECODER_H

#include "DecoderBase.h"

class NavtexDecoder : public DecoderBase {
  
  enum Mode {
      LETTERS,
      FIGURES
  };

public :
  NavtexDecoder();
  ~NavtexDecoder();
  
  virtual void init(int CharacterLen, int BitLen, int StopLen);
  virtual int decode(int * Points, int nPoints);
  
protected :
  int   CharacterLen;
  int   BitLen;
  static const unsigned char NavtexValidChars[128];
  static const unsigned char NavtexLetters[128];
  static const unsigned char NavtexFigures[128];
  bool   isSync;
  int    CharCounter;
  int    StartMark;
  int    EndMark[16];
  int    EndMarkPos;
  int    Fec[5];
  Mode   NavtexMode;

  int  runDecoder(int * Input, int nInput, int * startPos);
  int  findSyncronisation(int * Input, int nInput);
  int  findBitSync(int * Input, int nInput);
  int  validateChar(int Char) { return NavtexValidChars[Char]; }
  int  readChar(int * Input);
  int  decodeChar(int Char);
  static int decodeChar(int Char, Mode LetFig) { if (LetFig == LETTERS) return NavtexLetters[Char]; else return NavtexFigures[Char]; }
  void   outputChar(int Char);
  int    SyncCharBitError(int * Input);
  int    isSyncCharChain(int * Input, int nInput);
  
  void  printPoints(int * Points);

};

#endif // NAVTEXDECODER_H
