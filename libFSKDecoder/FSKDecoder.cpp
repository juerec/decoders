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

#include "FSKDecoder.h"
#include "FSKDemod.h"

#include <string.h>

/* Set any default values which does not crash the library */
extern "C"
void fskDecoderInitOptions(FSKDecoderOptions * options) {
  options->Shift          =  450;
  options->CenterFreq     = 1000;
  options->WindowSize     =    0; // if not overwritten by commandline, FSKDemod set a default value
  options->FFTSize        =    0; // if not overwritten by commandline, FSKDemod set a default value
  options->WindowFunction =   -1; // if not overwritten by commandline, FSKDemod set a default value
  options->IncreaseFactor = 0.4f;
  options->DecreaseFactor = 0.9988f;
  options->Threshold      = 0.05f;
  options->SampleRate     = 8000;
  options->xchgIQ         = false;
  options->DecoderType    = TYPE_INVALID;
  options->verbose        = 0;
}

/* Create a new instance of decoder */
extern "C"
FSKDecoderHandle fskDecoderInit(FSKDecoderOptions * options) {
FSKDecoderOptions opt = *options;

  /* opt.Type valid ? */
  switch (opt.DecoderType) {
  case TYPE_RTTY_REAL :
  case TYPE_NAVTEX_REAL :
  case TYPE_RTTY_IQ :
  case TYPE_NAVTEX_IQ :
    break;
  case TYPE_INVALID :
  default :
    return NULL;
  }
  
  if (opt.SampleRate % 800 != 0)
    return NULL;

  return (FSKDecoderHandle) new FSKDemod(&opt);
}

extern "C"
void fskDecoderProcessSamples(FSKDecoderHandle handle, short * Samples, int nSamples) {
  ((FSKDemod*)handle)->newSamples(Samples, nSamples);
}

extern "C"
FSKDecodedText * fskDecoderOutput(FSKDecoderHandle handle) {
  return ((FSKDemod*)handle)->Output();
}

extern "C"
void fskDecoderFreeOutput(FSKDecodedText * pOutput) {
  delete [] pOutput->Text;
  delete pOutput;  
}

extern "C"
void fskDecoderCleanUp(FSKDecoderHandle handle) {
  delete (FSKDemod*)handle;
}

extern "C"
FSKDecoderType fskDecoderTypeFromString(const char * type) {

  if (type != NULL) {
    if (!strcmp(type, "rtty"))
      return TYPE_RTTY_REAL;
    if (!strcmp(type, "rtty-iq") || !strcmp(type, "rttyiq"))
      return TYPE_RTTY_IQ;
    if (!strcmp(type, "navtex"))
      return TYPE_NAVTEX_REAL;
    if (!strcmp(type, "navtex-iq") || !strcmp(type, "navtexiq"))
      return TYPE_NAVTEX_IQ;
  }
  return TYPE_INVALID;  
}
