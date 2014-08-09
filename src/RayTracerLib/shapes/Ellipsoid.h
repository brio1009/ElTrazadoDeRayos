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
#ifndef RAYTRACERLIB_SHAPES_ELLIPSOID_H_
#define RAYTRACERLIB_SHAPES_ELLIPSOID_H_

#include <glm/glm.hpp>
#include <string>
#include <type_traits>
#include <vector>

#include "./Constants.h"
#include "shapes/Shape.h"
#include "factories/Factory.h"

// A Primitive is a Shape that is defined in its own.
class Ellipsoid : public Shape,
      protected Factory<Shape>::register_specialized<Ellipsoid> {
  // Create properties (also generates getter and setter).
  PROPERTIES(Ellipsoid,
             REAL, _rX, RadiusX,
             REAL, _rY, RadiusY,
             REAL, _rZ, RadiusZ)

 public:
  /// Default Ellipsoid constructor.
  Ellipsoid() : Ellipsoid(1.0f, 1.0f, 1.0f) { }
  /// Constructor with given radii in every axis-direction.
  Ellipsoid(REAL x, REAL y, REAL z);
  /// Destructor.
  virtual ~Ellipsoid() { }

  /// Returns intersections.
  virtual std::vector<REAL> intersect(const Ray& ray) const;

  /// Getter for the radius.
  glm::vec3 radii() const { return glm::vec3(_rX, _rY, _rZ); }

  /// Inherit the properties of Shape.
  /// TODO(bauschp, Sa 9. Aug 15:08:24 CEST 2014): add to PROPERTIES macro.
  static const char* parent;
  /// The class name. Needed for the Factory creating the object.
  static const char* name;

 protected:
  /// Override.
  virtual glm::vec4 getNormalAt(const glm::vec4& p) const;

  /// Override.
  virtual glm::vec2 getTextureCoord(const glm::vec4& p) const;

 private:
  // Radii.
  REAL _rX;
  REAL _rY;
  REAL _rZ;
};

static_assert(std::is_base_of<Shape, Ellipsoid>::value, "OK");
#endif  // RAYTRACERLIB_SHAPES_ELLIPSOID_H_
