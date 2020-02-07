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

#include <math.h>
#include <string.h>

/*
 * Windowing Functions
 */

#define N_FUNCTIONS  11

unsigned int NumWindowFuncs() {
   return N_FUNCTIONS;
}

const char * WndNames[N_FUNCTIONS] = {
  "Rectangular",
  "Bartlett",
  "Hamming",
  "Hann",
  "Blackman",
  "Blackman-Harris",
  "Welch",
  "Gaussian(a=2.5)",
  "Gaussian(a=3.5)",
  "Gaussian(a=4.5)",
  "Flat-Top"
};

const char ** WindowFuncNames() {
  return WndNames;
}

const char *WindowFuncName(int whichFunction) {

  if (whichFunction  >= 0 && whichFunction < N_FUNCTIONS)
    return WndNames[whichFunction];
  return  "invalid function";
}

int WindowFuncId(const char * WindowFuncName) {

  for (int i = 0; i < N_FUNCTIONS; i++)
    if (!strcmp(WindowFuncName, WndNames[i]))
      return i;
  return -1;
}

void WindowFunc(unsigned int whichFunction, unsigned int NumSamples, float *in) {
unsigned int i;
double A;
float  f;

   if (whichFunction == 0 || whichFunction > N_FUNCTIONS)
      whichFunction = 2;

   if (whichFunction == 1) {
      // Bartlett (triangular) window
      for (i = 0; i < NumSamples / 2; i++) {
         in[i] *= (i / (float) (NumSamples / 2));
         in[i + (NumSamples / 2)] *=
             (1.0f - (i / (float) (NumSamples / 2)));
      }
   }

   if (whichFunction == 2) {
      // Hamming
      for (i = 0; i < NumSamples; i++)
         in[i] *= 0.54f - 0.46f * cosf(2.0f * float(M_PI) * i / (NumSamples - 1));
   }

   if (whichFunction == 3) {
      // Hanning
      for (i = 0; i < NumSamples; i++)
         in[i] *= 0.50f - 0.50f * cosf(2 * float(M_PI) * i / (NumSamples - 1));
   }

   if (whichFunction == 4) {
      // Blackman
      for (i = 0; i < NumSamples; i++) {
          in[i] *= 0.42f - 0.5f * cosf(2 * float(M_PI) * i / (NumSamples - 1)) + 0.08f * cosf(4 * float(M_PI) * i / (NumSamples - 1));
      }
   }

   if (whichFunction == 5) {
      // Blackman-Harris
      for (i = 0; i < NumSamples; i++) {
          in[i] *= 0.35875 - 0.48829 * cos(2 * M_PI * i /(NumSamples-1)) + 0.14128 * cos(4 * M_PI * i/(NumSamples-1)) - 0.01168 * cos(6 * M_PI * i/(NumSamples-1));
      }
   }

   if (whichFunction == 6) {
      // Welch
      for (i = 0; i < NumSamples; i++) {
          in[i] *= 4*i/(float)NumSamples*(1-(i/(float)NumSamples));
      }
   }

   if (whichFunction == 7) {
      // Gaussian (a=2.5)
      // Precalculate some values, and simplify the fmla to try and reduce overhead
      A=-2*2.5*2.5;

      for (i = 0; i < NumSamples; i++) {
          // full
          // in[i] *= exp(-0.5*(A*((i-NumSamples/2)/NumSamples/2))*(A*((i-NumSamples/2)/NumSamples/2)));
          // reduced
          in[i] *= exp(A*(0.25 + ((i/(float)NumSamples)*(i/(float)NumSamples)) - (i/(float)NumSamples)));
      }
   }

   if (whichFunction == 8) {
      // Gaussian (a=3.5)
      A=-2*3.5*3.5;

      for (i = 0; i < NumSamples; i++) {
          // reduced
          in[i] *= exp(A*(0.25 + ((i/(float)NumSamples)*(i/(float)NumSamples)) - (i/(float)NumSamples)));
      }
   }

   if (whichFunction == 9) {
      // Gaussian (a=4.5)
      A=-2*4.5*4.5;

      for (i = 0; i < NumSamples; i++) {
          // reduced
          in[i] *= exp(A*(0.25 + ((i/(float)NumSamples)*(i/(float)NumSamples)) - (i/(float)NumSamples)));
      }
   }
   
   if (whichFunction == 10) {
      // Flat-Top
     float pi2 = M_PI * 2;
     float pi4 = M_PI * 4;
     float pi6 = M_PI * 6;
     float pi8 = M_PI * 8;
     float M   = (float) (NumSamples - 1);
     
     for (i = 0; i < NumSamples; i++)
       in[i] *= 1.0f - 1.93f  * cos(pi2 * i / M)
                     + 1.29f  * cos(pi4 * i / M)
                     - 0.388f * cos(pi6 * i / M)
                     + 0.028f * cos(pi8 * i / M);
   }

}

#ifdef _COMPLEX_H

void WindowFuncComplex(int whichFunction, int NumSamples, float complex *in) {
float * tmpWindow;
int i;

  tmpWindow = new float[NumSamples];

  for (i = 0; i < NumSamples; i++)
    tmpWindow[i] = 1.0f;

  WindowFunc(whichFunction, NumSamples, rWindow);

  for (i = 0; i < NumSamples; i++)
    in[i] = tmpWindow[i] + 0.0f * I;

  delete [] tmpWindow;
}
#endif // _COMPLEX_H
