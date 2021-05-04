/*
The MIT License (MIT)

Copyright (c) 2014 CantTouchDis <bauschp@informatik.uni-freiburg.de>
Copyright (c) 2014 brio1009 <christoph1009@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "./Image.h"
#include <cassert>
#include <cstring>
#include <string>
#include <vector>
#include "./Color.h"

using std::ofstream;
using std::vector;

// _____________________________________________________________________________
Image::Image(const int width, const int height)
  : _height(height), _width(width) {
  // We need to be sure, that width and height are both > 0.
  assert(_width > 0);
  assert(_height > 0);
  // Resize the pixel vector and put only black pixels in it.
  _data.assign(_width * _height, Color());
}

// _____________________________________________________________________________
Image::~Image() {
}

// _____________________________________________________________________________
void Image::saveAsPPM(const std::string& filePath) const {
  // Open file.
#ifdef WINDOWS
  FILE* f;
  fopen_s(&f, filePath.c_str(), "wb");
#else
  FILE* f = fopen(filePath.c_str(), "wb");
#endif  // WINDOWS
  if (!f) {
    // Could not open file.
    return;
  }
  fprintf(f, "P6 \n");
  fprintf(f, "%d %d \n255\n", _width, _height);
  for (int j = 0; j < _height; ++j) {
    for (int i = 0; i < _width; ++i) {
      const Color& tmpPixel = getPixel(i, j);
      unsigned char color = static_cast<unsigned char>(tmpPixel.getRAsInt());
      fwrite(&color, sizeof(unsigned char), 1, f);
      color = static_cast<unsigned char>(tmpPixel.getGAsInt());
      fwrite(&color, sizeof(unsigned char), 1, f);
      color = static_cast<unsigned char>(tmpPixel.getBAsInt());
      fwrite(&color, sizeof(unsigned char), 1, f);
    }
  }
  fclose(f);
}

// _____________________________________________________________________________
void Image::saveAsBMP(const std::string& filePath) const {
  // Code from
  // http://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries

  // Open file.
#ifdef WINDOWS
  FILE* f;
  fopen_s(&f, filePath.c_str(), "wb");
#else
  FILE* f = fopen(filePath.c_str(), "wb");
#endif  // WINDOWS
  if (!f) {
    // Could not open file.
    return;
  }

  unsigned char* img = NULL;
  int imgSize = 3 * _width * _height;
  int filesize = 54 + imgSize;
  if (img)
    free(img);
  img = (unsigned char*)malloc(imgSize);
  memset(img, 0, imgSize);

  for (int i = 0; i < _width; ++i) {
    for (int j = 0; j < _height; ++j) {
      int x = i;
      // TODO(cgissler, 09/05/2014): Do we really want to flip the image?
      int y = j;
      const Color& tmpPixel = getPixel(x, y);
      img[(x + y * _width) * 3 + 2] =
    static_cast<unsigned char>(tmpPixel.getRAsInt());
      img[(x + y * _width) * 3 + 1] =
    static_cast<unsigned char>(tmpPixel.getGAsInt());
      img[(x + y * _width) * 3 + 0] =
    static_cast<unsigned char>(tmpPixel.getBAsInt());
    }
  }

  unsigned char bmpfileheader[14] = {'B', 'M',
      0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
  unsigned char bmpinfoheader[40] = {40, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
  unsigned char bmppad[3] = {0, 0, 0};

  bmpfileheader[2] = (unsigned char)(filesize);
  bmpfileheader[3] = (unsigned char)(filesize >> 8);
  bmpfileheader[4] = (unsigned char)(filesize >> 16);
  bmpfileheader[5] = (unsigned char)(filesize >> 24);

  bmpinfoheader[4] = (unsigned char)(_width);
  bmpinfoheader[5] = (unsigned char)(_width >> 8);
  bmpinfoheader[6] = (unsigned char)(_width >> 16);
  bmpinfoheader[7] = (unsigned char)(_width >> 24);
  bmpinfoheader[8] = (unsigned char)(_height);
  bmpinfoheader[9] = (unsigned char)(_height >> 8);
  bmpinfoheader[10] = (unsigned char)(_height >> 16);
  bmpinfoheader[11] = (unsigned char)(_height >> 24);

  fwrite(bmpfileheader, 1, 14, f);
  fwrite(bmpinfoheader, 1, 40, f);
  for (int i = 0; i < _height; ++i) {
      fwrite(img + (_width * (_height - i - 1) * 3), 3, _width, f);
      fwrite(bmppad, 1, (4 - (_width * 3) % 4) % 4, f);
  }
  fclose(f);
  free(img);
}

// _____________________________________________________________________________
Color* Image::operator()(const int x, const int y) {
  return &(_data[x + _width * y]);
}

// _____________________________________________________________________________
const Color& Image::getPixel(const int x, const int y) const {
  return _data.at(x + _width * y);
}

// _____________________________________________________________________________
void Image::setPixel(const int x, const int y, const Color& color) {
  assert(x >= 0 && x < _width);
  assert(y >= 0 && y < _height);

  _data[x + _width * y] = color;
}
