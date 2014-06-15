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
#ifndef RAYTRACERLIB_SHAPES_SHAPE_H_
#define RAYTRACERLIB_SHAPES_SHAPE_H_

#include <glm/glm.hpp>

#include <cstdlib>
#include <limits>
#include <vector>

#include "./Constants.h"
#include "./Color.h"
#include "./IntersectionInfo.h"
#include "./ShadowMaterial.h"
#include "./PhongMaterial.h"
#include "./ColorMaterial.h"
#include "./Spatial.h"

// Forward declaration.
class Ray;

// This abstract class is used to define essencial parts to render a "Shape"
class Shape : public Spatial {
 public:
  /// Constructor.
  Shape();

  /// Destructor.
  virtual ~Shape();

  ///
  /// TODO(all, 05/17/2014): Find the right epsilon.
  virtual IntersectionInfo getIntersectionInfo(const Ray& ray,
    const REAL minimumT = constants::TEPSILON,
    const REAL maximumT = std::numeric_limits<REAL>::max()) const;

  // Intersects the Ray with this Shape and returns the values for t
  // rPos + rDir * t that intersect the surface of this Shape.
  virtual std::vector<REAL> intersect(const Ray& ray) const = 0;

  /// Setter for the material pointer. Be careful, the old material is not
  /// automatically deleted!
  void setMaterialPtr(const Material* materialPtr) {
    _materialPtr = materialPtr;
  }

  /// Getter for the material pointer.
  const Material* getMaterialPtr() const { return _materialPtr; }

 protected:
  // Returnes the appearence of the surface Point p
  // if p isn't on the surface everything can happen.
  // TODO(allofus): what should be returned here?
  virtual glm::vec4 getNormalAt(const glm::vec4& p) const = 0;

  /// Uv coordinates.
  virtual glm::vec2 getTextureCoord(const glm::vec4& p) const;

 private:
  const Material* _materialPtr;
};

#endif  // RAYTRACERLIB_SHAPES_SHAPE_H_
