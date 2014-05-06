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

#pragma once
#ifndef RAYTRACERLIB_IMAGE_H_
#define RAYTRACERLIB_IMAGE_H_

#include <string>
#include <vector>

//! Class that holds the pixel data.
class Pixel
{
public:
  //! Default Constructor.
  Pixel() : r(0), g(0), b(0), a(0) {}
  // Member.
  int r;
  int g;
  int b;
  int a;
};

//! This class provides an easy to use interface to write and read pixel
//! values of an image.
class Image
{
public:
  //! Constructor.
  Image(const int width, const int height);

  //! Destructor.
  ~Image();

  //! Saves the current image under the given name.
  void saveAsBMP(const std::string& filePath) const;

  //! Read and write access for a specific pixel.
  Pixel* operator()(const int x, const int y);

  //! Const-getter for a pixel.
  const Pixel& getPixel(const int x, const int y) const;

private:
  //! Vector with pixels.
  std::vector<Pixel> _data;
  //! Saves the height.
  int _height;
  //! Saves the width.
  int _width;
};

#endif  // RAYTRACERLIB_IMAGE_H_