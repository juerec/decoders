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

#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <sys/types.h>

#define N_ALLOC_HEIGHT 1400

class BMP;

typedef struct tag_bmp_palette_entry {
  unsigned char blue;
  unsigned char green;
  unsigned char red;
  unsigned char reserved;
} bmp_palette_entry;


class bmp_file_header {
public :

  bmp_file_header(int bits_per_pixel);

  char           magic[2];                // must always be set to 'BM' to declare that this is a .bmp-file.
  unsigned int   size;                    // specifies the size of the file in bytes.
  unsigned int   reserved;                // must always be set to zero.
  unsigned int   offset;                  // specifies the offset from the beginning of the file to the bitmap data.

  int write(FILE * f, unsigned int file_size);
};

class bmp_dib_header {

  friend class BMP;
  friend class bmp_header;

public :
  bmp_dib_header(int width, int higth, int bits_per_pixel);

protected :
  unsigned int    sizeof_header;          // specifies the size of the BITMAPINFOHEADER structure, in bytes.
  unsigned int    width;                  // specifies the width of the image, in pixels.
  unsigned int    height;                 // specifies the height of the image, in pixels.
  unsigned short  num_planes;             // specifies the number of planes of the target device, must be set to zero.
  unsigned short  bits_per_pixel;         // specifies the number of bits per pixel.
  unsigned int    compression;            // Specifies the type of compression, usually set to zero (no compression).
  unsigned int    image_size;             // specifies the size of the image data, in bytes. If there is no compression, it is valid to set this member to zero.
  unsigned int    horizaontal_resolution; // specifies the the horizontal pixels per meter on the designated targer device, usually set to zero.
  unsigned int    vertical_resolution;    // specifies the the vertical pixels per meter on the designated targer device, usually set to zero.
  unsigned int    colors_in_palette;      // specifies the number of colors used in the bitmap, if set to zero the number of colors is calculated using the biBitCount member.
  unsigned int    num_importen_colors;    // specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important.

public :
  int write(FILE * f);
  unsigned int line_size();
};

class bmp_header {

  friend class BMP;

protected :

  static         bmp_palette_entry  * palette;
  unsigned int   allocated_hight;

  bmp_file_header     file_header;
  bmp_dib_header      dib_header;

public :

  bmp_header(int width, int higth, int bits_per_pixel);
  ~bmp_header();

  int write(FILE * f);
  unsigned int line_size() { return dib_header.line_size(); }
};


class BMP {

protected :

  bmp_header       header;
  char           * file_name;
  unsigned char  * data;
  unsigned int     allocated_height;
  int              LineSize;

public :

  BMP(int width = 256, int height = 0, int bits_per_pixel = 1);
  ~BMP();

  void set_file_name(const char * file_name);
  FILE * open_output_file();
  int  write(const char * file_name = NULL);
  int  set_pixel(unsigned int x, unsigned int y, unsigned char color);

  inline void set_pixel_1(unsigned int x, unsigned int y, unsigned char color) {
  unsigned char   mask;
  unsigned int    mask_start;
  unsigned char * mask_ptr;

    if (x < header.dib_header.width) {
      realloc_data(y);
      mask_start = x / 8;
      mask_ptr = data + LineSize * y + mask_start;
      mask = 1 << (7 - x % 8);
      if (color) {
        *mask_ptr |= mask;
      } else {
        mask = 0xFF ^ mask;
        *mask_ptr &= mask;
      }
    }
  }
  inline void set_pixel_8(unsigned int x, unsigned int y, unsigned char color) {
    if (x < header.dib_header.width) {
      realloc_data(y);
      data[(allocated_height - (++y)) * LineSize + x] = color;
    }
  }

  int set_pixel_32(unsigned int x, unsigned int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
    realloc_data(y);
    unsigned char * pixel = data + ((allocated_height - (++y)) * LineSize + (x << 2));
    *pixel++ = red;
    *pixel++ = green;
    *pixel++ = blue;
    *pixel   = alpha;
    return 0;
  }

protected :
  static inline unsigned char pixel_1(uint x, uint y, unsigned char * data, uint line_size) {
  unsigned char   mask;
  unsigned int    mask_start;
  unsigned char * mask_ptr;

    mask_start = x >> 3;
    mask_ptr = data + line_size * y + mask_start;
    mask = 1 << (7 - x % 8);
    return *mask_ptr & mask;
  }
  static inline void set_pixel_1(unsigned char pixel, uint x, uint y, unsigned char * data, uint line_size) {
  unsigned char   mask;
  unsigned int    mask_start;
  unsigned char * mask_ptr;

    mask_start = x >> 3;
    mask_ptr = data + line_size * y + mask_start;
    mask = 1 << (7 - x % 8);
    if (pixel) {
      *mask_ptr |= mask;
    } else {
      mask = 0xFF ^ mask;
      *mask_ptr &= mask;
    }
  }

  static inline unsigned char pixel_8(uint x, uint y, unsigned char * data, uint line_size, uint allocated_hight) {
    return *(data + (allocated_hight - (++y)) * line_size + x);
  }
  static inline void set_pixel_8(unsigned char pixel, uint x, uint y, unsigned char * data, uint line_size, uint allocated_hight) {
    *(data + (allocated_hight - (++y)) * line_size + x) = pixel;
  }

public :
  unsigned int bits_per_pixel();
  unsigned int width();
  unsigned int height();
  unsigned int pixel_offset() { return header.file_header.offset; }

  void reset(int width, int height, int bits_per_pixel);

  void turnLeft();
  void turnRight();
//  void test();

protected :

  static unsigned int check_width(unsigned int width);
  static unsigned int check_height(unsigned int height);
  static unsigned int check_bpp(unsigned int bits_per_pixel);


  unsigned int line_size() { return header.line_size(); }

  inline void realloc_data(unsigned int height) {
    if (height >= allocated_height) {
      alloc_new(height);
    }
    if (height > header.dib_header.height) {
      header.dib_header.height = height;
    }
  }

  void alloc_new(int height);

};

#endif /* BMP_H */
