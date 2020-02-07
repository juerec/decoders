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

#ifndef WINDOWFUNC_H
#define WINDOWFUNC_H

// #include <complex.h>

unsigned int NumWindowFuncs();
const char ** WindowFuncNames();
const char *WindowFuncName(int whichFunction);
int WindowFuncId(const char * WindowFuncName);
void WindowFunc(unsigned int whichFunction, unsigned int NumSamples, float *in);
#ifdef COMPLEX_H
void WindowFuncComplex(int whichFunction, int NumSamples, float complex *in);
#endif

#endif /* WINDOWFUNC_H */
