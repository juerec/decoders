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

#ifndef FAXIMAGE_H
#define FAXIMAGE_H


#include "BMP.h"

class FaxImage : public BMP {

protected :

  int last_saved_row;

public:

  FaxImage();

  void reset(int width, int height, int bits_per_pixel);
  int write(const char * file_name = NULL);
  int save();
  int line_count();


};

#endif /* FAXIMAGE_H */
