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

#ifndef PHASETESTREAL_H
#define PHASETESTREAL_H

#include "PhaseTest.h"

#include "kiss_fftr.h"


class PhaseTestReal : public PhaseTest {

  public :
    PhaseTestReal();
    ~PhaseTestReal();

    virtual void init(int SampleRate, int CenterFrequency);
    virtual bool runTestStart(short * Samples, int * Deviation);
    virtual bool runTestStop(short * Samples);

  protected :
    kiss_fft_scalar  * FFTInput;
    kiss_fftr_cfg      fft_cfg;

    virtual void runFFT(short * Samples);
};

#endif // PHASETESTREAL_H
