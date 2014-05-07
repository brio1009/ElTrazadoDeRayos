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
#ifndef RAYTRACERLIB_ELLIPSOID_H_
#define RAYTRACERLIB_ELLIPSOID_H_

#include <glm/glm.hpp>
#include <vector>

#include "./Constants.h"
#include "./Shape.h"

// A Primitive is a Shape that is defined in its own.
class Ellipsoid : public Shape {
 public:
  Ellipsoid(REAL x, REAL y, REAL z);
  virtual ~Ellipsoid() { }
  // ___________________________________________________________________________
  virtual std::vector<REAL> intersect(const Ray& ray) const;
  // ___________________________________________________________________________
  virtual void getAppearenceAt(const glm::vec4& p) const;
 private:
  REAL rX;
  REAL rY;
  REAL rZ;
};

#endif  // RAYTRACERLIB_ELLIPSOID_H_

