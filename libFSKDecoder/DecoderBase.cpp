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

#include "DecoderBase.h"
#include <string.h>

DecoderBase::DecoderBase() {
  maxOutBuffer = 128;
  output = new FSKDecodedText;
  output->BufferSize = maxOutBuffer;
  output->TextLength = 0;
  output->Text = new char [maxOutBuffer];
  memset(output->Text, 0, maxOutBuffer);
}

DecoderBase::~DecoderBase() {
  delete [] output->Text;
  delete output;
}

FSKDecodedText * DecoderBase::Output() {
FSKDecodedText * dtNew;
FSKDecodedText * dtOld;

  dtNew = new FSKDecodedText;
  dtNew->BufferSize = maxOutBuffer;
  dtNew->BufferSize = maxOutBuffer;
  dtNew->TextLength = 0;
  dtNew->Text = new char [maxOutBuffer];
  memset(dtNew->Text, 0, maxOutBuffer);

  dtOld = output;
  output = dtNew;
  return dtOld;
}

void DecoderBase::outputChar(char c) {

  if (output->TextLength >= output->BufferSize -1) {
    maxOutBuffer += 128;
    char * S = new char [maxOutBuffer];
    memset(S, 0, maxOutBuffer);
    strcpy(S, output->Text);
    delete [] output->Text;
    output->Text = S;
    output->BufferSize = maxOutBuffer;
  }
  output->Text[output->TextLength] = c;
  output->TextLength++;
}
