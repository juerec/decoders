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

#ifndef DECODERIO_H
#define DECODERIO_H

#include "WaveFile.h"
#include "ASoundCapture.h"

class DecoderIO {
  
  enum INPUT_SOURCE {
    INPUT_NONE,  
    INPUT_WAVEFILE,
    INPUT_SOUND
  };
 
 
public :
  DecoderIO();
  ~DecoderIO();
 
  int  init(const char * DeviceName, bool isSoundDevice, bool isIQ, int * pSampleRate);
  int  read(short * SampleBuffer, int SampleBufferSize);
  bool eof();
  void close();


protected :
  INPUT_SOURCE InputSource;
  WaveFile       Wave;
  ASoundCapture  Sound;
  
};

#endif // DECODERIO_H
