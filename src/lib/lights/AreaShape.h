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
#ifndef RAYTRACERLIB_LIGHTS_AREASHAPE_H_
#define RAYTRACERLIB_LIGHTS_AREASHAPE_H_

#include <cstdio>
#include <ctime>
#include <glm/glm.hpp>
#include <limits>
#include <utility>
#include "../Constants.h"

#include "../IntersectionInfo.h"
/// Forward declaration.
class Ray;

///
class ImportantShape {
 public:
  /// Returns the number of samples.
  virtual size_t numSamples() const = 0;

  /// Get the sample number num
  virtual glm::vec4 getSample(const size_t sampleNr) const = 0;

  /// Area of this object.
  virtual float area() const = 0;

  /// We need to get a normal of this shape at a given position.
  virtual glm::vec4 getNormal(const glm::vec4& p) const = 0;
};

///
template <class T>
class AreaShape : public T, public ImportantShape {
 public:
  /// Constructor calls the constructor of the base class.
  template <typename... Args>
  AreaShape(Args... args) : T(std::forward<Args>(args)...) {}

  explicit AreaShape(const T& other) : T(other) {}
  /// We override this here, so we can set the boolean
  /// importantShape to true.
  virtual IntersectionInfo getIntersectionInfo(
      const Ray& ray,
      const REAL minimumT = constants::TEPSILON,
      const REAL maximumT = std::numeric_limits<REAL>::max()) const;

  /// Returns the number of samples.
  size_t numSamples() const;

  /// Get the sample number num
  glm::vec4 getSample(const size_t sampleNr) const;

  /// Area of this object.
  float area() const;

  /// We need to have the getNormalAt() method from Shape.
  virtual glm::vec4 getNormal(const glm::vec4& p) const {
    return T::getNormalAt(p);
  }

  static const char* name;

  AreaShape<T>* create() const;
};
// Implementations of template class functions.
template <class T>
IntersectionInfo AreaShape<T>::getIntersectionInfo(const Ray& ray,
                                                   const REAL minimumT,
                                                   const REAL maximumT) const {
  IntersectionInfo info = T::getIntersectionInfo(ray, minimumT, maximumT);
  info.hitImportantShape = true;
  return info;
}

template <typename T>
AreaShape<T>* AreaShape<T>::create() const {
  return new AreaShape<T>();
}

#endif  // RAYTRACERLIB_LIGHTS_AREASHAPE_H_
