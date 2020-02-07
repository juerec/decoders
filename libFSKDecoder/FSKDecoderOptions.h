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

#ifndef FSKDECODEROPTIONS_H
#define FSKDECODEROPTIONS_H

#ifdef __cplusplus 
extern "C" {
#endif

enum enFSKDecoderType {
  TYPE_INVALID,
  TYPE_RTTY_REAL,
  TYPE_RTTY_IQ,
  TYPE_NAVTEX_REAL,
  TYPE_NAVTEX_IQ
};

typedef enum enFSKDecoderType FSKDecoderType;

typedef struct tagFSKDecoderOptions {

  int            Shift;
  int            CenterFreq;
  int            WindowSize;     // real samples used
  int            FFTSize;        // >= WindowSize. If FFTSize > WindowSize, difference is zero padded
  int            WindowFunction;
  float          IncreaseFactor;
  float          DecreaseFactor;
  float          Threshold;
  int            SampleRate;
  bool           xchgIQ;
  FSKDecoderType DecoderType;
  int            verbose;
} FSKDecoderOptions;


#ifdef __cplusplus 
}
#endif

#endif /* FSKDECODEROPTIONS_H  */
