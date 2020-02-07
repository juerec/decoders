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

#ifndef FSKDECODER_H
#define FSKDECODER_H

#include "FSKDecoderOptions.h"
#include "FSKDecodedText.h"

#ifdef __cplusplus 
extern "C" {
#endif

struct FSKDecoder;
typedef struct FSKDecoder * FSKDecoderHandle;

void             fskDecoderInitOptions(FSKDecoderOptions * options);
FSKDecoderHandle fskDecoderInit(FSKDecoderOptions * options);
void             fskDecoderProcessSamples(FSKDecoderHandle handle, short * Samples, int nSamples);
FSKDecodedText * fskDecoderOutput(FSKDecoderHandle handle);
void             fskDecoderFreeOutput(FSKDecodedText * pOutput);
void             fskDecoderCleanUp(FSKDecoderHandle handle);
FSKDecoderType   fskDecoderTypeFromString(const char * type);

#ifdef __cplusplus 
}
#endif

#endif // FSKDECODER_H
