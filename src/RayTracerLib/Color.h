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
#ifndef RAYTRACERLIB_COLOR_H_
#define RAYTRACERLIB_COLOR_H_

#include <utility>

/// Class that holds the pixel data.
class Color {
 public:
  /// Default Constructor.
  Color() : _r(0), _g(0), _b(0), _a(255) { }

  /// Constructor with given colors.
  Color(const int r,
        const int g,
        const int b,
        const int a = 255) : _r(r), _g(g), _b(b), _a(a) { }

  /// Copy constructor. Cant be explicit or else it does not compile.
  Color(const Color& color);

  /// Destructor.
  virtual ~Color() { }

  /// Swap operator.
  friend void swap(Color& first, Color& second) {
      std::swap(first._r, second._r); 
      std::swap(first._g, second._g); 
      std::swap(first._b, second._b); 
      std::swap(first._a, second._a); 
  }

  /// Copy-assigment operator.
  Color& operator=(const Color& otherColor) {
      Color tmp(otherColor);
      swap(*this, tmp);

      return *this;
  }

  /// R-getter.
  int r() const { return _r; }
  /// G-getter.
  int g() const { return _g; }
  /// B-getter.
  int b() const { return _b; }
  /// A-getter.
  int a() const { return _a; }

  /// R-setter.
  void setR(const int r) { _r = r; }
  /// G-setter.
  void setG(const int g) { _g = g; }
  /// B-setter.
  void setB(const int b) { _b = b; }
  /// A-setter.
  void setA(const int a) { _a = a; }

 private:
  // Member.
  int _r;
  int _g;
  int _b;
  int _a;
};

#endif  // RAYTRACERLIB_COLOR_H_
