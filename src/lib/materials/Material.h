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
#ifndef RAYTRACERLIB_MATERIALS_MATERIAL_H_
#define RAYTRACERLIB_MATERIALS_MATERIAL_H_

#include <glm/glm.hpp>

#include "factories/PropertyInterface.h"
// Forward declaration.
class Color;
struct IntersectionInfo;
class Ray;
class Scene;

///
class Material : public PropertyInterface<Material> {
 public:
  /// Destructor.
  virtual ~Material() {}

  /// Returns the color for the given positition and normal.
  virtual Color getColor(const IntersectionInfo& intersectionInfo,
                         const Ray& incomingRay,
                         const Scene& scene) const = 0;

  /// Name of the shape used to serialize/deserialize.
  static const char* name;

 protected:
  /// Returns a color that should be used if the ray is not further
  /// traced (i.e. the stopping criteria is met).
  Color stoppingColor() const;

  /// Returns true if the stopping criteria is met (either
  /// max depth or min color contribution).
  bool stoppingCriteriaMet(const Ray& ray) const;
};

#endif  // RAYTRACERLIB_MATERIALS_MATERIAL_H_
