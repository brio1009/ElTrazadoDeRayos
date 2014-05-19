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
#ifndef RAYTRACERLIB_INTERSECTIONINFO_H_
#define RAYTRACERLIB_INTERSECTIONINFO_H_

#include <glm/glm.hpp>
#include "./Constants.h"

// Forward declaration.
class Material;

struct IntersectionInfo {
  REAL t;
  glm::vec4 hitPoint;
  glm::vec4 normal;
  const Material* materialPtr;
  IntersectionInfo() : t(0), hitPoint(0), normal(0), materialPtr(nullptr) { }
  IntersectionInfo(const REAL t,
                   const glm::vec4& hitPoint,
                   const glm::vec4& normal,
                   const Material* materialPtr)
  : t(t), hitPoint(hitPoint), normal(normal), materialPtr(materialPtr) { }

  /// Copy constructor.
  IntersectionInfo(const IntersectionInfo& i)
    : t(i.t), hitPoint(i.hitPoint), normal(i.normal), materialPtr(i.materialPtr)
  { }
};

#endif  // RAYTRACERLIB_INTERSECTIONINFO_H_
