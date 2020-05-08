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

#include <errno.h>
#include <string.h>

#include "FaxImage.h"


FaxImage::FaxImage()
          : BMP(),
            last_saved_row(-1) {
}

void FaxImage::reset(int width, int height, int bits_per_pixel) {

  last_saved_row = -1;
  BMP::reset(width, height, bits_per_pixel);
}

int FaxImage::write(const char * file_name) {
int ok;

  ok = BMP::write(file_name);
  if (ok)
    last_saved_row = height();
  return ok;
}

int FaxImage::save() {
  return write(NULL);
}
