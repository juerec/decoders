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

#include <string.h>

#include "DecoderIO.h"
#include "Logger.h"

DecoderIO::DecoderIO() {
  InputSource = INPUT_NONE;
}

DecoderIO::~DecoderIO() {
  if (InputSource != INPUT_NONE)
    close();
}

int DecoderIO::init(const char * DeviceName, bool isSoundDevice, bool isIQ, int * pSampleRate) {
int Channels;  

  if (isIQ)
    Channels = 2;
  else
    Channels = 1;

  if (!isSoundDevice) {
    if (!Wave.open(DeviceName)) {
      logger.error("DecoderIO::init()", __LINE__, " cannot open wave file: \"%s\"\n", DeviceName);
      // printf("DecoderIO::init() %i: cannot open wave file: \"%s\"\n", __LINE__, DeviceName);
      if (errno != 0)
        logger.error("DecoderIO::init()", __LINE__, " %s\n", strerror(errno));
	// printf("DecoderIO::init() %i:  %s\n", __LINE__, strerror(errno));
      else
        logger.error("DecoderIO::init()", __LINE__, " no valid wave file.\n");
        // printf("DecoderIO::init() %i:  no valid wave file.\n", __LINE__);
      return -1;
    }
    if (Wave.channels() != Channels) {
      logger.error("DecoderIO::init()", __LINE__, " incorrect channel count of %i, expected: %i\n", Wave.channels(), Channels);
      // printf("DecoderIO::init() %i: incorrect channel count of %i, expected: %i\n", __LINE__, Wave.channels(), Channels);
      return -1;
    }
    if (Wave.sampleRate() % 4000 != 0 || Wave.sampleRate() < 8000) {
      logger.error("DecoderIO::init()", __LINE__, " incorrect sample rate of %i, expected: an integer multiply of 4000, >= 8000\n", Wave.sampleRate());
      // printf("DecoderIO::init() %i: incorrect sample rate of %i, expected: an integer multiply of 4000, >= 8000\n", __LINE__, Wave.sampleRate());
      return -1;
    }
    *pSampleRate = Wave.sampleRate();
    InputSource  = INPUT_WAVEFILE;
  }

  if (isSoundDevice) {
    if (*pSampleRate % 4000 != 0) {
      logger.error("DecoderIO::init()", __LINE__, " incorrect sample rate of %i, expected: an integer multiply of 4000, >= 8000\n", *pSampleRate);
      // printf("DecoderIO::init() %i: incorrect sample rate of %i, expected: an integer multiply of 4000, >= 8000\n", __LINE__, *pSampleRate);
      return 1;
    }
    if (!Sound.openCapture(DeviceName, *pSampleRate, Channels))
      return -1;
    InputSource = INPUT_SOUND;
  }
  return InputSource == INPUT_NONE;
}

int DecoderIO::read(short * Samples, int nSamples) {
  if (InputSource == INPUT_WAVEFILE) {
    if (Wave.eof())
      return 0;
    return Wave.readSamples(Samples, nSamples);
  }
  if (InputSource == INPUT_SOUND)
    return Sound.readCapture(Samples, nSamples);
  return 0;
}

bool DecoderIO::eof() {
  return InputSource == INPUT_WAVEFILE && Wave.eof();
}

void DecoderIO::close() {
  if (InputSource == INPUT_WAVEFILE)
    Wave.close();
  if (InputSource == INPUT_SOUND)
    Sound.closeCapture();
  InputSource = INPUT_NONE;
}
