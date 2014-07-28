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

#pragma once
#ifndef RAYTRACERLIB_COLOR_H_
#define RAYTRACERLIB_COLOR_H_

#include <algorithm>
#include <cmath>
#include <utility>

/// Class that holds the pixel data.
class Color {
 public:
  /// Default Constructor.
  Color() : _r(0), _g(0), _b(0), _a(1.0f) { }

  /// Constructor with given colors.
  Color(const float r,
        const float g,
        const float b,
        const float a = 1.0f) : _r(r), _g(g), _b(b), _a(a) { }

  /// Copy constructor. Cant be explicit or else it does not compile.
  Color(const Color& color);

  /// Move constructor. Needed for C++11.
  Color(Color&& otherColor) {
    swap(*this, otherColor);
  }

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

  /// +=-operator.
  Color& operator+=(const Color& rhs);
  /// +-operator.
  Color operator+(const Color& rhs);

  /// *=-operator for Color.
  Color& operator*=(const Color& rhs);
  /// *-operator for Color.
  Color operator*(const Color& rhs);
  /// *-operator turned around for Color.
  friend inline Color operator*(const Color& lhs, Color rhs) {
    rhs *= lhs;
    return rhs;
  }

  /// *=-operator for floats.
  Color& operator*=(const float& rhs);
  /// *-operator for floats.
  Color operator*(const float& rhs);
  /// *-operator turned around for floats.
  friend inline Color operator*(const float& lhs, Color rhs) {
    rhs *= lhs;
    return rhs;
  }

  /// *=-operator for doubles.
  Color& operator*=(const double& rhs);
  /// *-operator for doubles.
  Color operator*(const double& rhs);
  /// *-operator turned around for doubles.
  friend inline Color operator*(const double& lhs, Color rhs) {
    rhs *= lhs;
    return rhs;
  }

  /// R-getter.
  inline float r() const { return _r; }
  /// G-getter.
  inline float g() const { return _g; }
  /// B-getter.
  inline float b() const { return _b; }
  /// A-getter.
  inline float a() const { return _a; }

  /// R-getter in range 0-255.
  int getRAsInt() const;
  /// G-getter in range 0-255.
  int getGAsInt() const;
  /// B-getter in range 0-255.
  int getBAsInt() const;
  /// A-getter in range 0-255.
  int getAAsInt() const;

  /// R-setter. Returns false if it was clipped.
  void setR(const float r);
  /// G-setter. Returns false if it was clipped.
  void setG(const float g);
  /// B-setter. Returns false if it was clipped.
  void setB(const float b);
  /// A-setter. Returns false if it was clipped.
  void setA(const float a);

 private:
  /// Check if this color is out of the displayable color range.
  bool isOutOfRange() const;

  /// Returns the out of range color.
  Color outOfRangeColor() const;

  /// Conversion from float to int colors.
  int floatToInt(const float val) const;

  /// Color that is returned when out of range.
  static const Color _outOfRangeColor;

  // Member.
  float _r;
  float _g;
  float _b;
  float _a;
};

#endif  // RAYTRACERLIB_COLOR_H_
