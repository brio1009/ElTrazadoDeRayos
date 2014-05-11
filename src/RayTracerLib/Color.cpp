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

#include "./Color.h"

// _____________________________________________________________________________
Color::Color(const Color& color) : _r(color.r()),
                                   _g(color.g()),
                                   _b(color.b()),
                                   _a(color.a()) { }


// _____________________________________________________________________________
Color& Color::operator+=(const Color& rhs) {
  // TODO(cgissler, 05/08/2014): Think about how to handle out of range colors.
  if (!setR(_r + rhs.r())
      || !setG(_g + rhs.g())
      || !setB(_b + rhs.b())) {
    // TODO(cgissler, 05/08/2014): Check if alpha should be multiplied.
    // setA(_a + rhs.a());
    // Make the color white.
    setR(255);
    setG(255);
    setB(255);
  }
  return *this;
}

// _____________________________________________________________________________
Color Color::operator+(const Color& rhs) {
  Color newColor(*this);  // Copy constructed.
  newColor += rhs;
  return newColor;
}

// _____________________________________________________________________________
Color& Color::operator*=(const float& rhs) {
  // TODO(cgissler, 05/08/2014): Think about how to handle out of range colors.
  if (!setR(floor((rhs * _r) + 0.5f))
      || !setG(floor((rhs * _g) + 0.5f))
      || !setB(floor((rhs * _b) + 0.5f))) {
    // TODO(cgissler, 05/08/2014): Check if alpha should be multiplied.
    // setA(floor((rhs * _a) + 0.5f));
    // Make the color white.
    setR(255);
    setG(255);
    setB(255);
  }
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
  // TODO(cgissler, 05/08/2014): Think about how to handle out of range colors.
  if (!setR(floor((rhs * _r) + 0.5f))
      || !setG(floor((rhs * _g) + 0.5f))
      || !setB(floor((rhs * _b) + 0.5f))) {
    // TODO(cgissler, 05/08/2014): Check if alpha should be multiplied.
    // setA(floor((rhs * _a) + 0.5f));
    // Make the color white.
    setR(255);
    setG(255);
    setB(255);
  }
  return *this;
}

// _____________________________________________________________________________
Color Color::operator*(const double& rhs) {
  Color newColor(*this);  // Copy constructed.
  newColor *= rhs;
  return newColor;
}

// _____________________________________________________________________________
bool Color::setR(const int r) {
  if (r > 255) {
    _r = 255;
    return false;
  } else if (r < 0) {
    _r = 0;
    return false;
  }
  _r = r;
  return true;
}

// _____________________________________________________________________________
bool Color::setG(const int g) {
  if (g > 255) {
    _g = 255;
    return false;
  } else if (g < 0) {
    _g = 0;
    return false;
  }
  _g = g;
  return true;
}

// _____________________________________________________________________________
bool Color::setB(const int b) {
  if (b > 255) {
    _b = 255;
    return false;
  } else if (b < 0) {
    _b = 0;
    return false;
  }
  _b = b;
  return true;
}

// _____________________________________________________________________________
bool Color::setA(const int a) {
  if (a > 255) {
    _a = 255;
    return false;
  } else if (a < 0) {
    _a = 0;
    return false;
  }
  _a = a;
  return true;
}

