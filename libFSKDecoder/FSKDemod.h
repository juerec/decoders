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

#ifndef FSKDEMOD_H
#define FSKDEMOD_H

#include "FSKDecoderOptions.h"
#include "Spectrogram.h"
#include "DecoderBase.h"
#include "FSKDecodedText.h"

class FSKDemod {

  public :
    FSKDemod(FSKDecoderOptions * opt);
    ~FSKDemod();
    int newSamples(short * Samples, int nSamples);
    FSKDecodedText * Output();

  protected :

    int  calculate(short * Samples, int nSamples);
    void calcAnalysis(float * MarkBuffer, float * SpaceBuffer, int nMarkSpaceBuffer);

    Spectrogram * spectrogram;
    DecoderBase * decoder;

    int     WindowSize;
    float   IncreaseFactor;
    float   DecreaseFactor;
    float   Threshold;
    int     StepWidth;
    int     Channels;
    
    // Puffergroessen
    int     SamplesBufferSize;
    short * SamplesBuffer;
    int     nSamplesBuffer;
    int     MarkSpaceBufferSize;
    float * MarkBuffer;
    float * SpaceBuffer;
    int     nMarkSpaceBuffer;
    int     DecisionBufferSize;
    int   * DecisionBuffer;
    float * AvMarkBuffer;
    float * AvSpaceBuffer;
    int     AvPreLength;
    int     BitLength;
    int     CharacterLength;
    int     StopLength;
    int     SyncReservePoints;
};

#endif // FSKDEMOD_H
