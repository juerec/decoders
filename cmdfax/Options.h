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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdio.h>

class Options {

  public :
    Options();
    ~Options();
    void read(int argc, char * argv[]);
    void printHelp();
    void printOptions();
    int  Limit(int iLimit);

    const char * InputFilename;
    const char * OutputFilename;
    char       * OutputFN;
    int          file_format;
    bool         turnLeft;
    bool         turnRight;
    int          CenterFrequency;
    int          Shift;
    int          verbose;
    int          filterType;
    int          StartThreshold;
    int          StopThreshold;
    int          SlantCorr;
    const char * Limits;
    static Options * pOptions;
};


#endif /* OPTIONS_H */
