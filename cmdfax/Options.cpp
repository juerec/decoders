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

#include "Options.h"

#include <string.h>
#include <getopt.h>
#include <stdlib.h>


Options * Options::pOptions;

Options::Options() {

  InputFilename    = "fax.wav";
  OutputFilename   = NULL;
  OutputFN         = NULL;
  file_format      = 8;
  turnLeft         = false;
  turnRight        = false;
  CenterFrequency  = 1500;
  Shift            = 850;
  verbose          = 1;
  filterType       = 2;
  StartThreshold   = 2;
  StopThreshold    = 2;
  SlantCorr        = 0;
  Limits           = "0";
  pOptions         = this;
}

Options::~Options() {
  if (OutputFN != NULL)
    delete [] OutputFN;
}

void Options::read(int argc, char * argv[]) {
static struct option long_options[] = {
      { "help",             no_argument,       0, 'h' },
      { "file",             required_argument, 0, 'f' },
      { "outfile",          required_argument, 0, 'o' },
      { "format",           required_argument, 0, 'O' },
      { "turn-left",        no_argument,       0, 'l' },
      { "turn-right",       no_argument,       0, 'r' },
      { "center-freq",      required_argument, 0, 'c' },
      { "shift",            required_argument, 0, 's' },
      { "verbose",          required_argument, 0, 'v' },
      { "filter-type",      required_argument, 0, 'F' },
      { "start-threshold",  required_argument, 0, 't' },
      { "stop-thershold",   required_argument, 0, 'u' },
      { "slant-corr",       required_argument, 0, 'S' },
      { "limit",            required_argument, 0, 'L' },
      { NULL,               0,                 0,  0  } 
};
int   c, option_index, i;

  while ((c = getopt_long(argc, argv, "hf:o:O:lrc:s:v:F:t:u:S:L:", long_options, &option_index)) != -1) {
    switch (c) {
    case 'f' :
      InputFilename = optarg;
      break;
    case 'o' :
      OutputFilename = optarg;
      break;
    case 'h' :
      printHelp();
      break;
    case 'O' :
      if (strcmp(optarg, "1") != 0 && strcmp(optarg, "8") != 0 && strcmp(optarg, "32") != 0)
        printHelp();
      file_format = *optarg - '0';
      break;
    case 'l' :
      turnLeft = true;
      turnRight = false;
      break;
    case 'r' :
      turnLeft = false;
      turnRight = true;
      break;
    case 'c' :
      CenterFrequency = atoi(optarg);
      break;
    case 's' :
      Shift = atoi(optarg);
      break;
    case 'v' :
      if (optarg == NULL) {
        verbose = 2;
      } else {
        if (*optarg < '0' || *optarg > '9')
          printHelp();
        verbose = atoi(optarg);
        if (verbose < 0 || verbose > 3)
          printHelp();
      }
      break;
    case 'F' :
      filterType = -1;
      if (optarg != NULL) {
        if (strcmp(optarg, "l0") == 0)
          filterType = 0;
        if (strcmp(optarg, "l1") == 0)
          filterType = 1;
        if (strcmp(optarg, "l2") == 0)
          filterType = 2;
        if (strcmp(optarg, "l3") == 0)
          filterType = 3;
        if (strcmp(optarg, "l4") == 0)
          filterType = 4;
      }
      if (filterType == -1)
        printHelp();
      break;
    case 't' :
      StartThreshold = atoi(optarg);
      if (optarg[0] < '0' || optarg[0] > '9' ||
          StartThreshold < 1 || StartThreshold > 3)
        printHelp();
      break;
    case 'u' :
      StopThreshold = atoi(optarg);
      if (optarg[0] < '0' || optarg[0] > '9' ||
          StopThreshold < 1 || StopThreshold > 3)
        printHelp();
      break;
    case 'S' :
      SlantCorr = atoi(optarg);
      if (optarg[0] < '0' || optarg[0] > '9' ||
          SlantCorr < -100 || SlantCorr > 100)
        printHelp();
      break;
    case 'L' :
      i = 0;
      while ((optarg[i] >= '0' && optarg[i] <= '9') || optarg[i] == ',')
        i++;
      if (optarg[i] != 0)
        printHelp();
      Limits = optarg;
      break;
    case ':' :
      printf("missing parameter for option %c\n", optopt);
      printHelp();
      break;
    case '?':
      printf("unknown option (%c)\n", optopt);
      printHelp();
      break;
    default :
      printf("unknown option (%c)\n", optopt);
      printHelp();
      break;
    } 
  }
  if (OutputFilename == NULL) {
    OutputFN = new char [strlen(InputFilename) + 6];
    strcpy(OutputFN, InputFilename);
    char * S = strstr(OutputFN, ".wav");
    if (S == NULL || S != strrchr(OutputFN, '.'))
      strcat(OutputFN, ".bmp");
    else
      strcpy(S, ".bmp");
    OutputFilename = OutputFN;
  }
}

void Options::printHelp() {

  printf("--help             -h     prints this help\n");
  printf("--file             -f     input file                       [fax.wav]\n");
  printf("--outfile          -o     output file                  [<input>.bmp]\n");
  printf("--turn-left        -l     turn left image\n");
  printf("--turn-right       -r     turn right image\n");
  printf("--format           -O     1|8|32 bit image format                [8]\n");
  printf("--center-freq      -c     Center frequency                    [1500]\n");
  printf("--shift            -s     Shift (fwhite - fblack)              [850]\n");
  printf("--filter           -F     l0|l1|l2|l3|l4                        [l2]\n");
  printf("--start-threshold  -t     1|2|3                                  [2]\n");
  printf("--stop-thershold   -u     1|2|3                                  [2]\n");
  printf("--slant-corr       -S     -100...100                             [0]\n");
  printf("--limits           -L     comma sep. max length of fax in sec.   [0]\n");
  printf("                          0 for unlimited\n");
  printf("--verbose          -v     print more informations [0|1|2|3]      [1]\n");
  exit(0);
}

void Options::printOptions() {
  
  if (verbose >= 3) {
    printf("infile            = %s\n", InputFilename);
    printf("outfile           = %s\n", OutputFilename);
    printf("turn-left         = %s\n", (turnLeft ? "yes" : "no"));
    printf("turn-right        = %s\n", (turnRight ? "yes" : "no"));
    printf("output format     = %6i Bit\n", file_format);
    printf("center freqency   = %6i\n", CenterFrequency);
    printf("shift             = %6i\n",  Shift);
    printf("filter            =     l%i\n", filterType);
    printf("start threshold   = %6i\n", StartThreshold);
    printf("stop thershold    = %6i\n", StopThreshold);
    printf("slant correction  = %6i\n", SlantCorr);
    printf("limits            = %s\n",  Limits);
    printf("verbose           = %6i\n", verbose);
  }
}


// if iLimit > count of supplied limits then return the last supplied limit
int Options::Limit(int iLimit) {
int i;
int limit = 0;
const char * S;

  i = 0;
  S = Limits;
  
  for (i = 0; i <= iLimit; i++) {
    limit = atoi(S);
    S = strchr(S, ',');
    if (S == NULL)
      break;
    S++;
  }
  
  return limit;    
}
