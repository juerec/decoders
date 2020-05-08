/****************************************************************************
**
**  This file is a part of the program "CMDFAX".
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

#ifndef CMDFAX_H
#define CMDFAX_H

#include "Options.h"
#include "PhaseTestReal.h"
#include "PhaseTestComplex.h"
#include "WaveFile.h"
#include "FaxDemodulator.h"

class CmdFax {

  public :
    CmdFax();
    ~CmdFax();
    bool init(Options * Options);
    int run();

    enum FAXState { FAX_IDLE,
                    PHASING_START,
                    PHASING,
                    IMAGE_INIT,
                    IMAGE,
                    IMAGE_END
    };
    FAXState FaxState;

  protected :
    Options      * pOptions;
    PhaseTest    * pPhaseTest;
    WaveFile       wav;
    int            SampleRate;
    int            CenterFrequency;
    int            Shift;
    bool           isIQSource;
    FaxDemodulator demodulator;
    int            InBufferSize;
    short        * InBuffer;
    double         SamplesPerLine;
    int            ImageStart;
    int            mCurrRow, mLineSample, mPixel, mPixelSamples, mLastCol;
    int            line_end_sample, last_line_start, file_format, threshold;
    double         line_end_sample_d, line_pixels_div_line_samples;
    
    int  PhasingPosition(int * BlackWhiteErrBuffer, int nBlackWhiteErrBuffer, int iStart, int nLines, int nCheckLength, int nLineLength, int nWitheLenght);
    bool Phasing(int * PhasingBuffer, int PhasingBufferCount);
    void Separator();
    void Image(int * Buffer, int BufferCount);
};

#endif  /* CMDFAX_H */
