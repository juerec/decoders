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

#ifndef ASOUND_CAPTURE
#define ASOUND_CAPTURE

#include <alsa/asoundlib.h>

class ASoundCapture {

  protected :
    snd_pcm_t * capture_handle;
    int Channels;

  public :
    ASoundCapture();
    ~ASoundCapture();

    bool openCapture(const char * hwdevice, uint Samplerate, int Channels, snd_pcm_format_t Format = SND_PCM_FORMAT_S16_LE);
    void closeCapture();
    int readCapture(short * Buffer, int nSamples);

};

#endif // ASOUND_CAPTURE
