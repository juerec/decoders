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

#include "ReSampler.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>


ReSampler::ReSampler() {
  Channels = 2;
  src_state = NULL;
  memset(&src_data, 0, sizeof(src_data));
  f1_MaxShort = 1.0 / float(SHRT_MAX);
  inBuffer = NULL;
  outBuffer = NULL;
}


ReSampler::~ReSampler() {

  if (inBuffer != NULL)
    delete [] inBuffer;
  if (outBuffer != NULL)
    delete [] outBuffer;
}


bool ReSampler::init(int oldSampleRate, int newSampleRate, int Channels, int maxInputFrames) {
double ResampleRatio;
int    error;
int    i;
int    ret;

  this->Channels = Channels;
  ResampleRatio = double(newSampleRate) / double(oldSampleRate);
  src_data.input_frames      = maxInputFrames;
  src_data.output_frames     = (((int)(double(maxInputFrames) * ResampleRatio)) | 7) + 1;
  if (ResampleRatio != 1.0) {
    inBuffer = new float[src_data.input_frames * Channels];
    outBuffer =  new float[src_data.output_frames * Channels];
  }
  src_data.data_in           = inBuffer;
  src_data.data_out          = outBuffer;
  src_data.input_frames_used = src_data.input_frames;
  src_data.output_frames_gen = 0;
  src_data.end_of_input      = 0;
  src_data.src_ratio         = ResampleRatio;
  
  if (ResampleRatio == 1.0)
    return true;

  src_state = src_new(SRC_SINC_BEST_QUALITY, Channels, &error);
  if (src_state == NULL) {
    const char * S = src_strerror(error);
    fprintf(stderr, "ReSampler::initResampler()%i: Error %i %s - initialization resampler:  oldSampleRate = %i   newSampleRate = %i   Channels = %i   maxInputFrames = %.i\n",
                    __LINE__, error, S == NULL ? "" : S, oldSampleRate, newSampleRate, Channels, maxInputFrames);
    if (inBuffer != NULL)
      delete [] inBuffer;
    if (outBuffer != NULL)
      delete [] outBuffer;
    memset(&src_data, 0, sizeof(src_data));
    return false;
  }
  
  // execute a first resample, it will eat any samples
  // all following resample-calls return a sample count exactly oldSampleRate * ResampleRatio
  for (i = 0; i < src_data.input_frames; i++)
    inBuffer[i] = 0.0f;
    // src_data.data_in[i] = 0.0f;
  if (Channels == 2)
    for (i = 0; i < src_data.input_frames; i++)
      // src_data.data_in[i + src_data.input_frames] = 0.0f;
      inBuffer[i + src_data.input_frames] = 0.0f;
  ret = src_process(src_state, &src_data);
  if (ret != 0) {
    const char * S = src_strerror(ret);
    fprintf(stderr, "ReSampler::initResampler()%i  Error src_process(): %i   %s\n", __LINE__, ret, S == NULL ? "no message available" : S);
    if (src_data.data_in != NULL)
      delete [] src_data.data_in;
    if (src_data.data_out != NULL)
      delete [] src_data.data_out;
    memset(&src_data, 0, sizeof(src_data));
    return false;
  }
  src_data.input_frames_used = src_data.input_frames;
  src_data.output_frames_gen = 0;

  return true;  
}


int ReSampler::resample(float * pIOutBuffer, float * pQOutBuffer, int * pnFramesGen, float * InBuffer, int nFramesInBuffer) {
int  iFrame, nFrames;
int  iSample, nSamples;
int  ret;

  src_data.data_in = InBuffer;
  src_data.input_frames = nFramesInBuffer;

  // In-data must be normalized to -1.0...1.0  
  for (iSample = 0, nSamples = nFramesInBuffer * 2; iSample < nSamples; iSample++) 
    InBuffer[iSample] *= f1_MaxShort;
    
  ret = src_process(src_state, &src_data);
  if (ret != 0) {
    const char * S = src_strerror(ret);
    fprintf(stderr, "ReSampler::resample()%i: Error src_process(): %i:   %s\n", __LINE__, ret, S == NULL ? "no message available" : S);
    return 0;
  }
  nFrames = src_data.output_frames_gen;    
   
  for (iFrame = 0, iSample = 0; iFrame < nFrames; iFrame++) {
    pIOutBuffer[iFrame] = src_data.data_out[iSample++] * float(SHRT_MAX);
    pQOutBuffer[iFrame] = src_data.data_out[iSample++] * float(SHRT_MAX);
  }
  *pnFramesGen = nFrames;
  return nFrames;
}
