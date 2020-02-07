/****************************************************************************
**
**  This file is a part of the program "CMDFSK".
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

#include "ASoundCapture.h"
#include "Logger.h"


ASoundCapture::ASoundCapture() {
  capture_handle = NULL;
}

ASoundCapture::~ASoundCapture() {
  closeCapture();
}

bool ASoundCapture::openCapture(const char * hwdevice, uint Samplerate, int Channels, snd_pcm_format_t Format) {
int        err;
snd_pcm_hw_params_t *hw_params;
uint      SampleRate = Samplerate;
int       SampleRateIst = 0;

  logger.debug("ASoundCapture::openCapture()", __LINE__, "hwdevice = \"%s\"   Samplerate = %i   Channels = %i\n", hwdevice == NULL ? "NULL" : hwdevice, Samplerate, Channels);

  if ((err = snd_pcm_open (&capture_handle, hwdevice, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
    logger.error("ASoundCapture::openCapture()", __LINE__, "cannot open audio device %s (%s)\n", hwdevice, snd_strerror(err));
    return false;
  }

  if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
    logger.error("ASoundCapture::openCapture()", __LINE__, "cannot allocate hardware parameter structure (%s)\n", snd_strerror (err));
    return false;
  }

  if ((err = snd_pcm_hw_params_any(capture_handle, hw_params)) < 0) {
    logger.error("ASoundCapture::openCapture()", __LINE__, "cannot initialize hardware parameter structure (%s)\n", snd_strerror (err));
    return false;
  }

  if ((err = snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    logger.error("ASoundCapture::openCapture()", __LINE__, "cannot set access type (%s)\n", snd_strerror (err));
    return false;
  }

  if ((err = snd_pcm_hw_params_set_format(capture_handle, hw_params, Format)) < 0) {
    logger.error("ASoundCapture::openCapture()", __LINE__, "cannot set sample format (%s)\n", snd_strerror (err));
    return false;
  }

  if ((err = snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &SampleRate, &SampleRateIst)) < 0) {
    logger.error("ASoundCapture::openCapture()", __LINE__, "cannot set sample rate (%s)\n", snd_strerror (err));
    return false;
  }

  if ((err = snd_pcm_hw_params_set_channels(capture_handle, hw_params, Channels)) < 0) {
    logger.error("ASoundCapture::openCapture()", __LINE__, "cannot set channel count (%s)\n", snd_strerror (err));
    return false;
  }

  if ((err = snd_pcm_hw_params(capture_handle, hw_params)) < 0) {
    logger.error("ASoundCapture::openCapture()", __LINE__, "cannot set parameters (%s)\n", snd_strerror (err));
    return false;
  }

  snd_pcm_hw_params_free(hw_params);

  if ((err = snd_pcm_prepare(capture_handle)) < 0) {
    logger.error("ASoundCapture::openCapture()", __LINE__, "cannot prepare audio interface for use (%s)\n", snd_strerror(err));
    return false;
  }
  this->Channels = Channels;
  logger.debug("ASoundCapture::openCapture()", __LINE__, "==> true\n");
  return true;
}

void ASoundCapture::closeCapture() {
  if (capture_handle != NULL) {
    snd_pcm_close(capture_handle);
    capture_handle = NULL;
  }
}

int ASoundCapture::readCapture(short * Buffer, int nSamples) {
int nToRead = nSamples / Channels;
int nread;

  logger.debug("ASoundCapture::readCapture()", __LINE__, "nSamples = %i\n", nSamples);
  nread = snd_pcm_readi(capture_handle, Buffer, nToRead);
  nToRead -= nread;
  while (nread > 0 && nToRead > 0) {
    logger.debug("ASoundCapture::readCapture()", __LINE__, "nToRead = %i\n", nToRead);
    nread = snd_pcm_readi(capture_handle, Buffer + (nSamples - nToRead * Channels), nToRead);
    nToRead -= nread;
  }

  if (nread <= 0) {
    logger.error("ASoundCapture::readCapture()", __LINE__, "read from audio interface failed (%s)\n", snd_strerror(nread));
    return 0;
  }
  return nSamples;
}
