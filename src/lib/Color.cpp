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

#include "./Color.h"

#include <algorithm>
#include <limits>

// _____________________________________________________________________________
Color::Color(const Color& color)
    : _r(color.r()), _g(color.g()), _b(color.b()), _a(color.a()) {}

// _____________________________________________________________________________
Color& Color::operator+=(const Color& rhs) {
  setR(_r + rhs.r());
  setG(_g + rhs.g());
  setB(_b + rhs.b());
  return *this;
}

// _____________________________________________________________________________
Color Color::operator+(const Color& rhs) {
  Color newColor(*this);  // Copy constructed.
  newColor += rhs;
  return newColor;
}

// _____________________________________________________________________________
Color& Color::operator*=(const Color& rhs) {
  setR(rhs._r * _r);
  setG(rhs._g * _g);
  setB(rhs._b * _b);
  return *this;
}

// _____________________________________________________________________________
Color Color::operator*(const Color& rhs) {
  Color newColor(*this);  // Copy constructed.
  newColor *= rhs;
  return newColor;
}

// _____________________________________________________________________________
Color& Color::operator*=(const float& rhs) {
  setR(rhs * _r);
  setG(rhs * _g);
  setB(rhs * _b);
  return *this;
}

// _____________________________________________________________________________
Color Color::operator*(const float& rhs) {
  Color newColor(*this);  // Copy constructed.
  newColor *= rhs;
  return newColor;
}

// _____________________________________________________________________________
Color& Color::operator*=(const double& rhs) {
  setR(static_cast<float>(rhs) * _r);
  setG(static_cast<float>(rhs) * _g);
  setB(static_cast<float>(rhs) * _b);
  return *this;
}

// _____________________________________________________________________________
Color Color::operator*(const double& rhs) {
  Color newColor(*this);  // Copy constructed.
  newColor *= rhs;
  return newColor;
}

// _____________________________________________________________________________
Color Color::outOfRangeColor() const {
  // Just scale it to the max value.
  double invMax = 1.0 / std::max(_r, std::max(_g, _b));
  return invMax * (*this);
}

// _____________________________________________________________________________
void Color::setR(const float r) {
  _r = r;
}

// _____________________________________________________________________________
void Color::setG(const float g) {
  _g = g;
}

// _____________________________________________________________________________
void Color::setB(const float b) {
  _b = b;
}

// _____________________________________________________________________________
void Color::setA(const float a) {
  _a = a;
}

// _____________________________________________________________________________
int Color::getRAsInt() const {
  if (isOutOfRange()) {
    return floatToInt(outOfRangeColor().r());
  }
  return floatToInt(_r);
}

// _____________________________________________________________________________
int Color::getGAsInt() const {
  if (isOutOfRange()) {
    return floatToInt(outOfRangeColor().g());
  }
  return floatToInt(_g);
}

// _____________________________________________________________________________
int Color::getBAsInt() const {
  if (isOutOfRange()) {
    return floatToInt(outOfRangeColor().b());
  }
  return floatToInt(_b);
}

// _____________________________________________________________________________
int Color::getAAsInt() const {
  if (isOutOfRange()) {
    return floatToInt(outOfRangeColor().a());
  }
  return floatToInt(_a);
}

// _____________________________________________________________________________
bool Color::isOutOfRange() const {
  return (_r > 1.0f || _g > 1.0f || _b > 1.0f || _a > 1.0f);
}

// _____________________________________________________________________________
int Color::floatToInt(const float val) const {
  return std::min(255, std::max(0, static_cast<int>(val * 255)));
}
