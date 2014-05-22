/*
The MIT License (MIT)

Copyright (c) 2014 CantTouchDis
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
#ifndef RAYTRACERLIB_CHECKERBOARDMATERIAL_H_
#define RAYTRACERLIB_CHECKERBOARDMATERIAL_H_

#include <glm/glm.hpp>
#include "./Material.h"
#include "./Color.h"
#include "./Constants.h"

// Forward declaration.
struct IntersectionInfo;

/// Phong material that shades the object with the phong reflection model.
class CheckerboardMaterial : public Material {
 public:
  /// Constructor.
  CheckerboardMaterial(const Material* mat1,
                       const Material* mat2,
                       const REAL uSize,
                       const REAL vSize)
    : Material(),
  _material1Ptr(mat1),
  _material2Ptr(mat2),
  _uSize(uSize),
  _vSize(vSize)
  { }

  virtual Color getColor(const IntersectionInfo& intersectionInfo,
                         const Ray& incomingRay,
                         const Scene& scene) const;

 protected:
  /// Bool.
  bool expandingSquares(const glm::vec2& coords) const;
  bool checkerBoard(const glm::vec2& coords) const;
  bool dotBoard(const glm::vec2& coords) const;

  /// Member.
  const Material* _material1Ptr;
  const Material* _material2Ptr;
  REAL _uSize;
  REAL _vSize;
};

#endif  // RAYTRACERLIB_CHECKERBOARDMATERIAL_H_

