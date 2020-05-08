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

#include "BMP.h"

#include <errno.h>
#include <string.h>


bmp_palette_entry black_white_palette [256] = { {   0,  0,   0,   0 },
                                                { 255, 255, 255,  0 } };

bmp_palette_entry grayscale_palette [256] = {
                                             {   0,   0,   0,   0 },
                                             {   1,   1,   1,   0 },
                                             {   2,   2,   2,   0 },
                                             {   3,   3,   3,   0 },
                                             {   4,   4,   4,   0 },
                                             {   5,   5,   5,   0 },
                                             {   6,   6,   6,   0 },
                                             {   7,   7,   7,   0 },
                                             {   8,   8,   8,   0 },
                                             {   9,   9,   9,   0 },

                                             {  10,  10,  10,   0 },
                                             {  11,  11,  11,   0 },
                                             {  12,  12,  12,   0 },
                                             {  13,  13,  13,   0 },
                                             {  14,  14,  14,   0 },
                                             {  15,  15,  15,   0 },
                                             {  16,  16,  16,   0 },
                                             {  17,  17,  17,   0 },
                                             {  18,  18,  18,   0 },
                                             {  19,  19,  19,   0 },

                                             {  20,  20,  20,   0 },
                                             {  21,  21,  21,   0 },
                                             {  22,  22,  22,   0 },
                                             {  23,  23,  23,   0 },
                                             {  24,  24,  24,   0 },
                                             {  25,  25,  25,   0 },
                                             {  26,  26,  26,   0 },
                                             {  27,  27,  27,   0 },
                                             {  28,  28,  28,   0 },
                                             {  29,  29,  29,   0 },

                                             {  30,  30,  30,   0 },
                                             {  31,  31,  31,   0 },
                                             {  32,  32,  32,   0 },
                                             {  33,  33,  33,   0 },
                                             {  34,  34,  34,   0 },
                                             {  35,  35,  35,   0 },
                                             {  36,  36,  36,   0 },
                                             {  37,  37,  37,   0 },
                                             {  38,  38,  38,   0 },
                                             {  39,  39,  39,   0 },

                                             {  40,  40,  40,   0 },
                                             {  41,  41,  44,   0 },
                                             {  42,  42,  42,   0 },
                                             {  43,  43,  43,   0 },
                                             {  44,  44,  44,   0 },
                                             {  45,  45,  45,   0 },
                                             {  46,  46,  46,   0 },
                                             {  47,  47,  47,   0 },
                                             {  48,  48,  48,   0 },
                                             {  49,  49,  49,   0 },

                                             {  50,  50,  50,   0 },
                                             {  51,  51,  51,   0 },
                                             {  52,  52,  52,   0 },
                                             {  53,  53,  53,   0 },
                                             {  54,  54,  54,   0 },
                                             {  55,  55,  55,   0 },
                                             {  56,  56,  56,   0 },
                                             {  57,  57,  57,   0 },
                                             {  58,  58,  58,   0 },
                                             {  59,  59,  59,   0 },

                                             {  60,  60,  60,   0 },
                                             {  61,  61,  61,   0 },
                                             {  62,  62,  62,   0 },
                                             {  63,  63,  63,   0 },
                                             {  64,  64,  64,   0 },
                                             {  65,  65,  65,   0 },
                                             {  66,  66,  66,   0 },
                                             {  67,  67,  67,   0 },
                                             {  68,  68,  68,   0 },
                                             {  69,  69,  69,   0 },

                                             {  70,  70,  70,   0 },
                                             {  71,  71,  71,   0 },
                                             {  72,  72,  72,   0 },
                                             {  73,  73,  73,   0 },
                                             {  74,  74,  74,   0 },
                                             {  75,  75,  75,   0 },
                                             {  76,  76,  76,   0 },
                                             {  77,  77,  77,   0 },
                                             {  78,  78,  78,   0 },
                                             {  79,  79,  79,   0 },

                                             {  80,  80,  80,   0 },
                                             {  81,  81,  81,   0 },
                                             {  82,  82,  82,   0 },
                                             {  83,  83,  83,   0 },
                                             {  84,  84,  84,   0 },
                                             {  85,  85,  85,   0 },
                                             {  86,  86,  86,   0 },
                                             {  87,  87,  87,   0 },
                                             {  88,  88,  88,   0 },
                                             {  89,  89,  89,   0 },

                                             {  90,  90,  90,   0 },
                                             {  91,  91,  91,   0 },
                                             {  92,  92,  92,   0 },
                                             {  93,  93,  93,   0 },
                                             {  94,  94,  94,   0 },
                                             {  95,  95,  95,   0 },
                                             {  96,  96,  96,   0 },
                                             {  97,  97,  97,   0 },
                                             {  98,  98,  98,   0 },
                                             {  99,  99,  99,   0 },

                                             { 100, 100, 100,  0 },
                                             { 101, 101, 101,  0 },
                                             { 102, 102, 102,  0 },
                                             { 103, 103, 103,  0 },
                                             { 104, 104, 104,  0 },
                                             { 105, 105, 105,  0 },
                                             { 106, 106, 106,  0 },
                                             { 107, 107, 107,  0 },
                                             { 108, 108, 108,  0 },
                                             { 109, 109, 109,  0 },

                                             { 110, 110, 110,  0 },
                                             { 111, 111, 111,  0 },
                                             { 112, 112, 112,  0 },
                                             { 113, 113, 113,  0 },
                                             { 114, 114, 114,  0 },
                                             { 115, 115, 115,  0 },
                                             { 116, 116, 116,  0 },
                                             { 117, 117, 117,  0 },
                                             { 118, 118, 118,  0 },
                                             { 119, 119, 119,  0 },

                                             { 120, 120, 120,  0 },
                                             { 121, 121, 121,  0 },
                                             { 122, 122, 122,  0 },
                                             { 123, 123, 123,  0 },
                                             { 124, 124, 124,  0 },
                                             { 125, 125, 125,  0 },
                                             { 126, 126, 126,  0 },
                                             { 127, 127, 127,  0 },
                                             { 128, 128, 128,  0 },
                                             { 129, 129, 129,  0 },


                                             { 130, 130, 130,  0 },
                                             { 131, 131, 131,  0 },
                                             { 132, 132, 132,  0 },
                                             { 133, 133, 133,  0 },
                                             { 134, 134, 134,  0 },
                                             { 135, 135, 135,  0 },
                                             { 136, 136, 136,  0 },
                                             { 137, 137, 137,  0 },
                                             { 138, 138, 138,  0 },
                                             { 139, 139, 139,  0 },


                                             { 140, 140, 140,  0 },
                                             { 141, 141, 141,  0 },
                                             { 142, 142, 142,  0 },
                                             { 143, 143, 143,  0 },
                                             { 144, 144, 144,  0 },
                                             { 145, 145, 145,  0 },
                                             { 146, 146, 146,  0 },
                                             { 147, 147, 147,  0 },
                                             { 148, 148, 148,  0 },
                                             { 149, 149, 149,  0 },

                                             { 150, 150, 150,  0 },
                                             { 151, 151, 151,  0 },
                                             { 152, 152, 152,  0 },
                                             { 153, 153, 153,  0 },
                                             { 154, 154, 154,  0 },
                                             { 155, 155, 155,  0 },
                                             { 156, 156, 156,  0 },
                                             { 157, 157, 157,  0 },
                                             { 158, 158, 158,  0 },
                                             { 159, 159, 159,  0 },

                                             { 160, 160, 160,  0 },
                                             { 161, 161, 161,  0 },
                                             { 162, 162, 162,  0 },
                                             { 163, 163, 163,  0 },
                                             { 164, 164, 164,  0 },
                                             { 165, 165, 165,  0 },
                                             { 166, 166, 166,  0 },
                                             { 167, 167, 167,  0 },
                                             { 168, 168, 168,  0 },
                                             { 169, 169, 169,  0 },

                                             { 170, 170, 170,  0 },
                                             { 171, 171, 171,  0 },
                                             { 172, 172, 172,  0 },
                                             { 173, 173, 173,  0 },
                                             { 174, 174, 174,  0 },
                                             { 175, 175, 175,  0 },
                                             { 176, 176, 176,  0 },
                                             { 177, 177, 177,  0 },
                                             { 178, 178, 178,  0 },
                                             { 179, 179, 179,  0 },

                                             { 180, 180, 180,  0 },
                                             { 181, 181, 181,  0 },
                                             { 182, 182, 182,  0 },
                                             { 183, 183, 183,  0 },
                                             { 184, 184, 184,  0 },
                                             { 185, 185, 185,  0 },
                                             { 186, 186, 186,  0 },
                                             { 187, 187, 187,  0 },
                                             { 188, 188, 188,  0 },
                                             { 189, 189, 189,  0 },

                                             { 190, 190, 190,  0 },
                                             { 191, 191, 191,  0 },
                                             { 192, 192, 192,  0 },
                                             { 193, 193, 193,  0 },
                                             { 194, 194, 194,  0 },
                                             { 195, 195, 195,  0 },
                                             { 196, 196, 196,  0 },
                                             { 197, 197, 197,  0 },
                                             { 198, 198, 198,  0 },
                                             { 199, 199, 199,  0 },

                                             { 200, 200, 200,  0 },
                                             { 201, 201, 201,  0 },
                                             { 202, 202, 202,  0 },
                                             { 203, 203, 203,  0 },
                                             { 204, 204, 204,  0 },
                                             { 205, 205, 205,  0 },
                                             { 206, 206, 206,  0 },
                                             { 207, 207, 207,  0 },
                                             { 208, 208, 208,  0 },
                                             { 209, 209, 209,  0 },

                                             { 210, 210, 210,  0 },
                                             { 211, 211, 211,  0 },
                                             { 212, 212, 212,  0 },
                                             { 213, 213, 213,  0 },
                                             { 214, 214, 214,  0 },
                                             { 215, 215, 215,  0 },
                                             { 216, 216, 216,  0 },
                                             { 217, 217, 217,  0 },
                                             { 218, 218, 218,  0 },
                                             { 219, 219, 219,  0 },

                                             { 220, 220, 220,  0 },
                                             { 221, 221, 221,  0 },
                                             { 222, 222, 222,  0 },
                                             { 223, 223, 223,  0 },
                                             { 224, 224, 224,  0 },
                                             { 225, 225, 225,  0 },
                                             { 226, 226, 226,  0 },
                                             { 227, 227, 227,  0 },
                                             { 228, 228, 228,  0 },
                                             { 229, 229, 229,  0 },


                                             { 230, 230, 230,  0 },
                                             { 231, 231, 231,  0 },
                                             { 232, 232, 232,  0 },
                                             { 233, 233, 233,  0 },
                                             { 234, 234, 234,  0 },
                                             { 235, 235, 235,  0 },
                                             { 236, 236, 236,  0 },
                                             { 237, 237, 237,  0 },
                                             { 238, 238, 238,  0 },
                                             { 239, 239, 239,  0 },


                                             { 240, 240, 240,  0 },
                                             { 241, 241, 241,  0 },
                                             { 242, 242, 242,  0 },
                                             { 243, 243, 243,  0 },
                                             { 244, 244, 244,  0 },
                                             { 245, 245, 245,  0 },
                                             { 246, 246, 246,  0 },
                                             { 247, 247, 247,  0 },
                                             { 248, 248, 248,  0 },
                                             { 249, 249, 249,  0 },

                                             { 250, 250, 250,  0 },
                                             { 251, 251, 251,  0 },
                                             { 252, 252, 252,  0 },
                                             { 253, 253, 253,  0 },
                                             { 254, 254, 254,  0 },
                                             { 255, 255, 255,  0 } };

bmp_palette_entry * bmp_header::palette = black_white_palette;


/************************************************

  class  bmp_file_header

************************************************/

bmp_file_header::bmp_file_header(int bits_per_pixel) {
int palette_size;

  magic[0] = 'B';
  magic[1] = 'M';
  size     = 0;
  reserved = 0;
  switch (bits_per_pixel) {
  case 1 :
    palette_size = 2 * sizeof(bmp_palette_entry);
    break;
  case 4 :
    palette_size = 16 * sizeof(bmp_palette_entry);
    break;
  case 8 :
    palette_size = 256 * sizeof(bmp_palette_entry);
    break;
  default :
    palette_size = 0;
  }
  offset   = 14 + 40 + palette_size;
}

int bmp_file_header::write(FILE * f, unsigned int file_size) {

  size = file_size;
  if ((fseek(f, 0, SEEK_SET) < 0) ||
      (fwrite(magic, sizeof(magic), 1, f) != 1) ||
      (fwrite(&size, sizeof(size), 1, f) != 1) ||
      (fwrite(&reserved, sizeof(reserved), 1, f) != 1) ||
      (fwrite(&offset, sizeof(offset), 1, f) != 1)) {
    fprintf(stderr, "Error writing BMP header: %s\n", strerror(errno));
    return 0;
  }
  return 1;
}

/************************************************

  class  bmp_dib_header

************************************************/

bmp_dib_header::bmp_dib_header(int width, int height, int bits_per_pixel) {

  sizeof_header           = 40;
  bmp_dib_header::width   = width;
  bmp_dib_header::height  = height;
  num_planes              = 1;
  this->bits_per_pixel    = bits_per_pixel;
  compression             = 0;

  image_size              = 0;
  horizaontal_resolution  = 0;
  vertical_resolution     = 0;
  switch (bits_per_pixel) {
  case 1 :
    colors_in_palette = 2;
    break;
  case 4 :
    colors_in_palette = 16;
    break;
  case 8 :
    colors_in_palette = 256;
    break;
  default :
    colors_in_palette = 0;
  }
  num_importen_colors = colors_in_palette;
}

int bmp_dib_header::write(FILE * f) {

  if ((fwrite(&sizeof_header, sizeof(sizeof_header), 1, f) != 1) ||
      (fwrite(&width, sizeof(width), 1, f) != 1) ||
      (fwrite(&height, sizeof(height), 1, f) != 1) ||
      (fwrite(&num_planes, sizeof(num_planes), 1, f) != 1) ||
      (fwrite(&bits_per_pixel, sizeof(bits_per_pixel), 1, f) != 1) ||
      (fwrite(&compression, sizeof(compression), 1, f) != 1) ||
      (fwrite(&image_size, sizeof(image_size), 1, f) != 1) ||
      (fwrite(&horizaontal_resolution, sizeof(horizaontal_resolution), 1, f) != 1) ||
      (fwrite(&vertical_resolution, sizeof(vertical_resolution), 1, f) != 1) ||
      (fwrite(&colors_in_palette, sizeof(colors_in_palette), 1, f) != 1) ||
      (fwrite(&num_importen_colors, sizeof(num_importen_colors), 1, f) != 1)) {
    fprintf(stderr, "Error writing DIB header: %s\n", strerror(errno));
    return 0;
  }
  return 1;
}

unsigned int bmp_dib_header::line_size() {
unsigned int ls;
unsigned int add;

  ls = width * bits_per_pixel;
  add = (ls % 32) != 0;
  ls = (ls / 32 + add) * 4;
  return ls;
}


 /************************************************

  class bmp_header

************************************************/

bmp_header::bmp_header(int width, int higth, int bits_per_pixel)
          : file_header(bits_per_pixel),
            dib_header(width, higth, bits_per_pixel) {
}

bmp_header::~bmp_header() {}


int bmp_header::write(FILE * f) {
unsigned int file_size;
int          ret = 0;

  file_size = sizeof(bmp_file_header) +
              sizeof(bmp_dib_header) +
              sizeof(bmp_palette_entry) * dib_header.colors_in_palette +
              dib_header.height * line_size();

  if (file_header.write(f, file_size) && dib_header.write(f)) {
    if ((dib_header.colors_in_palette > 0) &&
        (fwrite(bmp_header::palette, sizeof(bmp_palette_entry), dib_header.colors_in_palette, f)
                != dib_header.colors_in_palette)) {
      fprintf(stderr, "Error writing BMP palette: %s\n", strerror(errno));
      return 0;
    }
    ret = 1;
  }
  return ret;
}

/************************************************

  class BMP

************************************************/

BMP::BMP(int width, int height, int bits_per_pixel)
   : header(check_width(width), check_height(height), check_bpp(bits_per_pixel)) {

  file_name = NULL;
  data      = NULL;
  allocated_height = 0;
  realloc_data(header.dib_header.height);

}
// ============================================== BMP::BMP

BMP::~BMP() {

  if (file_name != NULL)
    delete [] file_name;
  if (data != NULL)
    delete [] data;
}
// ============================================== BMP::~BMP

void BMP::set_file_name(const char * file_name) {

  if (this->file_name != NULL) {
    delete [] this->file_name;
    this->file_name = NULL;
  }
  if (file_name != NULL) {
    this->file_name = new char [strlen(file_name) + 2];
    strcpy(this->file_name, file_name);
  }
}
// ============================================== BMP::set_file_name

FILE * BMP::open_output_file() {
FILE      * f;
static char BM[2] = { 'B', 'M' };
int         ret;

  if (file_name == NULL) {
    fprintf(stderr, "Error: no file name given.\n");
    return NULL;
  }
  f = fopen(file_name, "w");
  if (f == NULL) {
    fprintf(stderr, "Error: opening bitmap file: \"%s\" :  %s\n", file_name, strerror(errno));
    return f;
  }
  ret = fwrite(BM, sizeof(char), 2, f);
  if (ret < 0) {
    fprintf(stderr, "Error: can not write to bitmap file: %s\n", strerror(errno));
    fclose(f);
    f = NULL;
  } else {
    fseek(f, SEEK_SET, 0);
  }
  return f;
}
// ============================================== BMP::open_output_file

int BMP::write(const char * file_name) {
unsigned int ls;
FILE       * f;
bool         ok = false;
unsigned int len;

  if (file_name != NULL)
    set_file_name(file_name);
  if (this->file_name == NULL) {
    fprintf(stderr, "Error: no file name given.\n");
    return 0;
  }
  if ((f = open_output_file()) == NULL) {
    return 0;
  }
  if (header.dib_header.height == 0) {
    fprintf(stderr, "Warning: the bitmap is empty.\n");
  }
  if (header.write(f)) {
    ok = true;
    ls = line_size();
    len = ls * height();
    if (fwrite(data + ls * (allocated_height - height()), sizeof(unsigned char), ls * height(), f) != len) {
      printf("Error writing \"%s\": %s\n", this->file_name, strerror(errno));
      ok = false;
    }
  }
  fclose(f);
  if (!ok)
    remove(file_name);
  return ok;
}
// ============================================== BMP::write

int BMP::set_pixel(unsigned int x, unsigned int y, unsigned char color) {
unsigned char   mask;
unsigned int    line_start = LineSize * y;
unsigned int    mask_start;
unsigned char * mask_ptr;

  if (x > header.dib_header.width)
    return 0;

  realloc_data(y);
//  line = data + line_start;
  switch (header.dib_header.bits_per_pixel) {
  case 1 :
    mask_start = x / 8;
    mask_ptr = data + line_start + mask_start;
    mask = 1 << (7 - x % 8);
    if (color) {
      *mask_ptr |= mask;
    } else {
      mask = 0xFF ^ mask;
      *mask_ptr &= mask;
    }
    break;
  case 8 :
    data[line_start + x] = color;
    break;
  default :
    fprintf(stderr, "set_pixel: not implemented for %i bits per pixel\n", header.dib_header.bits_per_pixel);
  }
  return 1;
}
// ============================================== BMP::set_pixel

unsigned int BMP::bits_per_pixel() {
  return header.dib_header.bits_per_pixel;
}
// ============================================== BMP::bits_per_pixel

unsigned int BMP::width() {
  return header.dib_header.width;
}
// ============================================== BMP::width

unsigned int BMP::height() {
  return header.dib_header.height;
}
// ============================================== BMP::height

unsigned int BMP::check_width(unsigned int width) {
  if (width < 1)
    width = 256;
  if (width > 0xFFFF)
    width = 0xFFFF;
  return width;
}
// ============================================== BMP::check_width

unsigned int BMP::check_height(unsigned int height) {
  if (height < 0)
    height = 0;
  if (height > 0xFFFF)
    height = 0xFFFF - N_ALLOC_HEIGHT + 1;
  return height;
}
// ============================================== BMP::check_height

unsigned int BMP::check_bpp(unsigned int bits_per_pixel) {
  switch (bits_per_pixel) {
  case 1 :
  case 4 :
  case 8 :
  case 16 :
  case 24 :
  case 32 :
    return bits_per_pixel;
  default :
    return 24;
  }
}
// ============================================== BMP::check_bpp

void BMP::alloc_new(int height) {
unsigned char * ptr;
int             h;
int             new_size;

  if (height == 0) {
    h = 1;
  } else {
    h = (height + 1) / N_ALLOC_HEIGHT;
    if ((height + 1) % N_ALLOC_HEIGHT)
      h++;
  }
  h *= N_ALLOC_HEIGHT;
  new_size = h * line_size();
  ptr = new unsigned char [new_size];
  memset(ptr, 0, new_size);
  memmove(ptr + (h - allocated_height) * line_size(), data, allocated_height * line_size());
  delete [] data;
  data = ptr;
  allocated_height = h;
}
// ============================================== BMP::alloc_new

void BMP::reset(int width, int height, int bits_per_pixel) {
  if (data != NULL) {
    delete [] data;
    data = NULL;
    allocated_height = 0;
  }
  header = bmp_header(check_width(width), check_height(height), check_bpp(bits_per_pixel));
  realloc_data(header.dib_header.height);
  switch (bits_per_pixel) {
  case 1:
    header.dib_header.colors_in_palette = 2;
    header.palette = black_white_palette;
    break;
  case 8:
    header.dib_header.colors_in_palette = 256;
    header.palette = grayscale_palette;
    break;
  default :
    header.dib_header.colors_in_palette = 0;
    header.palette = grayscale_palette;
  }
  LineSize = line_size();
}
// ============================================== BMP::reset

void BMP::turnLeft() {
uint             new_width = header.dib_header.height;
uint             new_height = header.dib_header.width;
//uint             new_w = new_width - 1;
uint             new_h = new_height - 1;
uint             new_line_size;
unsigned char * new_data;
uint             old_line_size;
unsigned int    add;

  new_line_size = new_width * header.dib_header.bits_per_pixel;
  add = (new_line_size % 32) != 0;
  new_line_size = (new_line_size / 32 + add) * 4;
  new_data = new unsigned char [new_line_size * new_height];

  old_line_size = header.dib_header.width * header.dib_header.bits_per_pixel;
  add = (old_line_size % 32) != 0;
  old_line_size = (old_line_size / 32 + add) * 4;

  switch (header.dib_header.bits_per_pixel) {
  case 1 :
    for (uint old_y = 0; old_y < header.dib_header.height; old_y++) {
      for (uint old_x = 0; old_x < header.dib_header.width; old_x++) {
        set_pixel_1(pixel_1(old_x, old_y, data, old_line_size), old_y, new_h - old_x, new_data, new_line_size);
      }
    }
    break;
  case 8 :
    for (uint old_y = 0; old_y < header.dib_header.height; old_y++) {
      for (uint old_x = 0; old_x < header.dib_header.width; old_x++) {
        set_pixel_8(pixel_8(old_x, old_y, data, old_line_size, allocated_height), old_y, new_h - old_x, new_data, new_line_size, new_height);
      }
    }
    break;
  }
  delete [] data;
  data = new_data;
  allocated_height = new_height;
  header.dib_header.width = new_width;
  header.dib_header.height = new_height;
}
// ============================================== BMP::turnLeft()

void BMP::turnRight() {
uint             new_width = header.dib_header.height;
uint             new_height = header.dib_header.width;
uint             new_w = new_width - 1;
uint             new_line_size;
unsigned char * new_data;
uint             old_line_size;
unsigned int    add;

  new_line_size = new_width * header.dib_header.bits_per_pixel;
  add = (new_line_size % 32) != 0;
  new_line_size = (new_line_size / 32 + add) * 4;
  new_data = new unsigned char [new_line_size * new_height];

  old_line_size = header.dib_header.width * header.dib_header.bits_per_pixel;
  add = (old_line_size % 32) != 0;
  old_line_size = (old_line_size / 32 + add) * 4;

  switch (header.dib_header.bits_per_pixel) {
  case 1 :
    for (uint old_y = 0; old_y < header.dib_header.height; old_y++) {
      for (uint old_x = 0; old_x < header.dib_header.width; old_x++) {
        set_pixel_1(pixel_1(old_x, old_y, data, old_line_size), new_w - old_y, old_x, new_data, new_line_size);
      }
    }
    break;
  case 8 :
    for (uint old_y = 0; old_y < header.dib_header.height; old_y++) {
      for (uint old_x = 0; old_x < header.dib_header.width; old_x++) {
        set_pixel_8(pixel_8(old_x, old_y, data, old_line_size, allocated_height), new_w - old_y, old_x, new_data, new_line_size, new_height);
      }
    }
    break;
  }
  delete [] data;
  data = new_data;
  allocated_height = new_height;
  header.dib_header.width = new_width;
  header.dib_header.height = new_height;
}
// ============================================== BMP::turnRight()
/*
void BMP::test() {
 ((unsigned long *)data)[1]  = 0xFFFFFFFF;
}
*/
