/*
The MIT License (MIT)

Copyright (c) 2014 CantTouchDis

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
#include <fstream>
#include <vector>

using std::ofstream;
using std::vector;

// _____________________________________________________________________________
Image::Image(const int width, const int height)
  : _width(width), _height(height)
{
  // We need to be sure, that width and height are both > 0.
  assert(_width > 0);
  assert(_height > 0);
  // Resize the pixel vector and put only black pixels in it.
  _data.assign(_width * _height, Pixel());
}

// _____________________________________________________________________________
Image::~Image()
{
}

// _____________________________________________________________________________
void Image::saveAsBMP(const std::string& filePath) const
{
  // Open the stream.
  ofstream stream(filePath, std::ofstream::out);

  // Check if everything went well.
  if (!stream.good())
  {
    return;
  }


  // Code taken from
  // http://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries.
  unsigned char file[14] = {
    'B', 'M',  // magic
    0, 0, 0, 0,  // size in bytes
    0, 0,  // app data
    0, 0,  // app data
    40 + 14, 0, 0, 0  // start of data offset
  };
  unsigned char info[40] = {
    40, 0, 0, 0,  // info hd size
    0, 0, 0, 0,  // width
    0, 0, 0, 0,  // heigth
    1, 0,  // number color planes
    24, 0,  // bits per pixel
    0, 0, 0, 0,  // compression is none
    0, 0, 0, 0,  // image bits size
    0x13, 0x0B, 0, 0,  // horz resoluition in pixel / m
    0x13, 0x0B, 0, 0,  // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
    0, 0, 0, 0,  // #colors in pallete
    0, 0, 0, 0,  // #important colors
  };

  int w = _width;
  int h = _height;

  int padSize  = (4 - w % 4) % 4;
  int sizeData = w * h * 3 + h * padSize;
  int sizeAll  = sizeData + sizeof(file) + sizeof(info);

  file[2] = (unsigned char)(sizeAll);
  file[3] = (unsigned char)(sizeAll >> 8);
  file[4] = (unsigned char)(sizeAll >> 16);
  file[5] = (unsigned char)(sizeAll >> 24);

  info[4] = (unsigned char)(w);
  info[5] = (unsigned char)(w >> 8);
  info[6] = (unsigned char)(w >> 16);
  info[7] = (unsigned char)(w >> 24);

  info[8] = (unsigned char)(h);
  info[9] = (unsigned char)(h >> 8);
  info[10] = (unsigned char)(h >> 16);
  info[11] = (unsigned char)(h >> 24);

  info[24] = (unsigned char)(sizeData);
  info[25] = (unsigned char)(sizeData >> 8);
  info[26] = (unsigned char)(sizeData >> 16);
  info[27] = (unsigned char)(sizeData >> 24);

  stream.write((char*)file, sizeof(file));
  stream.write((char*)info, sizeof(info));

  unsigned char pad[3] = {0, 0, 0};

  for (int y = 0; y < h; ++y)
  {
      for (int x = 0; x < w; ++x)
      {
          const Pixel& tmpPixel = getPixel(x, y);
          unsigned char pixel[3];
          pixel[0] = tmpPixel.r;
          pixel[1] = tmpPixel.g;
          pixel[2] = tmpPixel.b;

          stream.write((char*)pixel, 3);
      }
      stream.write((char*)pad, padSize);
  }
  stream.close();
}

// _____________________________________________________________________________
Pixel* Image::operator()(const int x, const int y)
{
  return &(_data[x + _width * y]);
}

// _____________________________________________________________________________
const Pixel& Image::getPixel(const int x, const int y) const
{
  return _data.at(x + _width * y);
}