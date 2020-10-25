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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#include "FSKDecoder.h"
#include "Logger.h"
#include "DecoderIO.h"

#include "Version.h"

FSKDecoderOptions DecoderOptions;
/*
typedef struct tagFSKDecoderOptions {

  int            Shift;
  int            CenterFreq;
  int            WindowSize;     // real samples used
  int            FFTSize;        // >= WindowSize. If FFTSize > WindowSize, difference is zero padded
  int            WindowFunction;
  float          IncreaseFactor;
  float          DecreaseFactor;
  float          Threshold;
  int            SampleRate;
  bool           xchgIQ;
  FSKDecoderType DecoderType;
  char        ** ppOutputBuffer;
  int          * pnOutputBuffer;
} FSKDecoderOptions;
*/

void printHelp(int ret) {
  printf("-h --help                  print this screen\n");
  printf("-v --verbose               prints any details\n");
  printf("-t --decoder-type          decoder type;\n");
  printf("                           rtty | rtty-iq | navtex | navtex-iq\n");
  printf("-S --shift                 signal shift\n");
  printf("-c --center-freq           center frequency\n");
  printf("                           --center-freq=nnn for negative values\n");
  printf("-w --window-size           fft window size\n");
  printf("-W --window-func           fft window function\n");
  printf("                           by number or by name\n");
  printf("-F --fft-size              fft size >= fft window size\n");
  printf("-f --file                  wave file for input\n"); 
  printf("-D --audio-device          alsa audio device for input\n");
  printf("-i --incr-factor           factor for fading compensation\n");
  printf("-d --decr-factor           factor for fading compensation\n");
  printf("-R --samplerate            sample rate, for audio input only\n");
  printf("-x --xchg-i-q              exchange I - Q, for IQ sources only\n");
  printf("                           the window functions are:\n");
  printf("                                0  Rectangular \n");
  printf("                                1  Bartlett\n");
  printf("                                2  Hamming\n");
  printf("                                3  Hann\n");
  printf("                                4  Blackman\n");
  printf("                                5  Blackman-Harris\n");
  printf("                                6  Welch\n");
  printf("                                7  Gaussian(a=2.5)\n");
  printf("                                8  Gaussian(a=3.5)\n");
  printf("                                9  Gaussian(a=4.5)\n");
  printf("                               10  Flat-Top\n\n");
  
  if (ret >= 0)
    exit(ret);
}
static struct option long_options1[] = {
      { "help",          no_argument,       0, 'h' },
      { "verbose",       no_argument,       0, 'v' },
      { "decoder-type",  required_argument, 0, 't' },
      { "shift",         required_argument, 0, 'S' },
      { "center-freq",   required_argument, 0, 'c' },
      { "window-size",   required_argument, 0, 'w' },
      { "window-func",   required_argument, 0, 'W' },
      { "fft-size",      required_argument, 0, 'F' },
      { "file",          required_argument, 0, 'f' },
      { "audio-device",  required_argument, 0, 'D' },  
      { "incr-factor",   required_argument, 0, 'i' },
      { "decr-factor",   required_argument, 0, 'd' },
      { "samplerate",    required_argument, 0, 'R' },
      { "xchg-i-q",      no_argument,       0, 'x' },
      { NULL,            0,                 0,  0  }
  };

static const char * short_options = "hvt:S:c:w:W:F:f:D:i:d:R:xB:";

int main(int argc, char * argv[]) {
char    WaveFilename[256] = "";
char    AudioDevice[64]   = "";
bool    isWaveFilename = false;
bool    isAudioDevice = false;
bool    isShift = false;
int     Channels;
int     c;
int     option_index;
int     ret;
bool    isIQ;
short * SampleBuffer;
int     SampleBufferSize;
int     nSampleBuffer;
bool    eof = false;
bool    isSampleRate = false;

  printf("cmdfsk %s\n", VERSION);

  fskDecoderInitOptions(&DecoderOptions);

  while ((c = getopt_long(argc, argv, short_options, long_options1, &option_index)) != -1) {
    switch (c) {
    case 'h' :
      printHelp(0);
      break;
    case 't' :
      if (!strcmp(optarg, "rtty")) {
        DecoderOptions.DecoderType = TYPE_RTTY_REAL;
        break;
      }
      if (!strcmp(optarg, "rtty-iq")) {
        DecoderOptions.DecoderType = TYPE_RTTY_IQ;
        break;
      }
      if (!strcmp(optarg, "navtex")) {
        DecoderOptions.DecoderType = TYPE_NAVTEX_REAL;
        break;
      }
      if (!strcmp(optarg, "navtex-iq")) {
        DecoderOptions.DecoderType = TYPE_NAVTEX_IQ;
        break;
      }
      printf("invalid docoder type\n");
      printHelp(1);
      break;
   case 'F' :
      DecoderOptions.FFTSize = atoi(optarg);
      break;
    case 'S' :
      DecoderOptions.Shift = atoi(optarg);
      isShift = true;
      break;
    case 'w' :
      DecoderOptions.WindowSize = atoi(optarg);
      break;
    case 'W' :
      DecoderOptions.WindowFunction = atoi(optarg);
      break;
    case 'i' :
      DecoderOptions.IncreaseFactor = atof(optarg);
      break;
    case 'd' :
      DecoderOptions.DecreaseFactor = atof(optarg);
      break;
    case 'f' :
      if (optarg != NULL) {
        strncpy(WaveFilename, optarg, sizeof(WaveFilename) - 1);
        WaveFilename[sizeof(WaveFilename) - 1] = 0;
        isWaveFilename = strlen(WaveFilename) > 0;
      }
      break;
    case 'D' :
      if (optarg != NULL) {
        strncpy(AudioDevice, optarg, sizeof(AudioDevice) - 1);
        AudioDevice[sizeof(AudioDevice) - 1] = 0;
        isAudioDevice = strlen(AudioDevice) > 3;
      }
      break;
    case 'R' :
      DecoderOptions.SampleRate = atoi(optarg);
      isSampleRate = true;
      break;
    case 'x' :
      DecoderOptions.xchgIQ = true;
      break;
    case 'v' :
      DecoderOptions.verbose = 1;
      break;
    case 'c' :
      if (strstr(optarg, "enter-freq=") == optarg)
        DecoderOptions.CenterFreq = atoi(optarg + 11);
      else
        DecoderOptions.CenterFreq = atoi(optarg);
      break;
    case ':' :
      printf("ERROR: Missing parameter for option %c\n", optopt);
      printHelp(1);
    }
  }

  if (DecoderOptions.DecoderType == TYPE_INVALID)
    printHelp(1);

  isIQ = DecoderOptions.DecoderType == TYPE_RTTY_IQ || DecoderOptions.DecoderType == TYPE_NAVTEX_IQ;
  Channels = isIQ ? 2 : 1;

  if (!isShift)
    if (DecoderOptions.DecoderType == TYPE_NAVTEX_REAL || DecoderOptions.DecoderType == TYPE_NAVTEX_IQ)
      DecoderOptions.Shift = 170;
    else
      DecoderOptions.Shift = 450;

  // End parse command line

  // open input
  if (isWaveFilename && isAudioDevice) {
    printf("Provide file input OR sound input, not both!\n");
    exit(1);
  }

  if (!isWaveFilename && !isAudioDevice) {
    strcpy(WaveFilename, "stdin");
    isWaveFilename = true;
  }
  if (DecoderOptions.verbose = 1 && isWaveFilename && strcmp(WaveFilename, "stdin") == 0 && !isSampleRate)
    printf("Warning: No samplerate specified, using default %i\n", DecoderOptions.SampleRate);

  if (DecoderOptions.verbose)
    printf("  Input            = %s\n", isAudioDevice ? AudioDevice : WaveFilename);

  DecoderIO io;
  ret = io.init(isAudioDevice ? AudioDevice : WaveFilename, 
                isAudioDevice,
                isIQ,
                &DecoderOptions.SampleRate);
  if (ret)
    printHelp(1);

  FSKDecoderHandle handle = fskDecoderInit(&DecoderOptions);
  if (handle == NULL)
    printHelp(1);

  SampleBufferSize = (DecoderOptions.SampleRate >> 3) * Channels; // 1/8 Second
  SampleBuffer = new short [SampleBufferSize];


  while (!eof) {
    nSampleBuffer = io.read(SampleBuffer, SampleBufferSize);
    // error ?
    if (nSampleBuffer < 0)
      break;

    // eof ?
    if (nSampleBuffer < SampleBufferSize)
      eof = io.eof();

    fskDecoderProcessSamples(handle, SampleBuffer, nSampleBuffer);

    FSKDecodedText * txt = fskDecoderOutput(handle);

    printf("%s", txt->Text);

    fskDecoderFreeOutput(txt);
  }
  printf("\n");

  fskDecoderCleanUp(handle);

  io.close();

  delete [] SampleBuffer;

  return 0;
}
