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

#ifndef SPECTROGRAMCOMPLEX_H
#define SPECTROGRAMCOMPLEX_H

#include "Spectrogram.h"

class SpectrogramComplex : public Spectrogram {

public :

  SpectrogramComplex();
  virtual ~SpectrogramComplex();

protected :

  fftwf_complex * InBuffer;

  virtual void initFFT();
  virtual void calculateFFT(short * Samples);
  
};

#endif // SPECTROGRAMCOMPLEX_H
