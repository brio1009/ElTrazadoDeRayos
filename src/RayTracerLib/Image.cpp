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
#include <vector>

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
  // TODO(cgissler, 05/06/2014): Implement it!
}

// _____________________________________________________________________________
Pixel* Image::operator()(const int x, const int y)
{
  return &(_data[x + _width * y]);
}