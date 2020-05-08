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

#ifndef PHASETEST_H
#define PHASETEST_H

#include "kiss_fft.h"


class PhaseTest {

  public :
    PhaseTest();
    virtual ~PhaseTest();

    virtual void init(int SampleRate, int CenterFrequency);
    void setDeviaion(int Deviation);
    int  deltaF(int nBins);
    virtual bool runTestStart(short * Samples, int * Deviation) = 0;
    virtual bool runTestStop(short * Samples) = 0;

  protected :

    kiss_fft_cpx    * FFTOutput;
    kiss_fft_scalar * Window;
    float           * LineBuffer;
    int               nFFT;
    
    int               CenterFrequency;

    int  BinsStart[5];
    int  BinsEnd[5];
    int  BinStart;
    int  BinEnd;
    int  SampleRate;
    int  DeltaStart;
    int  DeltaEnd;
    int  Delta;

    bool checkStart(int * Deviation);
    bool checkStop();

    int BinNr(int Frequency);
    virtual void runFFT(short * Samples) = 0;
};

#endif // PHASETEST_H
