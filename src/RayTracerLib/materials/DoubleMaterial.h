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
#ifndef RAYTRACERLIB_MATERIALS_DOUBLEMATERIAL_H_
#define RAYTRACERLIB_MATERIALS_DOUBLEMATERIAL_H_

#include <glm/glm.hpp>
#include "../Material.h"
#include "../Constants.h"

// Forward declaration.
struct IntersectionInfo;
class Color;
class Ray;

/// Phong material that shades the object with the phong reflection model.
class DoubleMaterial : public Material {
 public:
  /// Default onstructor.
  DoubleMaterial(const Material* mat1,
                 const Material* mat2,
                 const REAL uSize,
                 const REAL vSize)
    : Material(),
  _material1Ptr(mat1),
  _material2Ptr(mat2),
  _uSize(uSize),
  _vSize(vSize)
  { }

  /// Returns the color for the given intersection point & normal.
  virtual Color getColor(const IntersectionInfo& intersectionInfo,
                         const Ray& incomingRay,
                         const Scene& scene) const;

 protected:
  /// Bool.
  bool expandingSquares(const glm::vec2& coords) const;
  bool checkerBoard(const glm::vec2& coords) const;
  bool dotBoard(const glm::vec2& coords) const;

  // Member.
  // TODO(cgissler, 05.06.2014): Change this to use shared_ptr.
  /// Pointer to the first material.
  const Material* _material1Ptr;
  /// Pointer to the second material.
  const Material* _material2Ptr;

  /// Defines in which 
  REAL _uSize;
  REAL _vSize;
};

#endif  // RAYTRACERLIB_MATERIALS_DOUBLEMATERIAL_H_

