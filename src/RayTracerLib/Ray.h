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
#ifndef RAYTRACERLIB_RAY_H_
#define RAYTRACERLIB_RAY_H_

#include <glm/glm.hpp>
#include "./RayInfo.h"

/// Ray.
class Ray {
 public:
  /// Constructor. Constructs ray with origin (0, 0, 0) and direction (1, 0, 0).
  Ray() : _origin(0, 0, 0, 1), _direction(1, 0, 0, 0) { }
  /// Constructor with given origin and direction.
  Ray(const glm::vec4& origin, const glm::vec4& direction);
  /// Setter for origin. Autmatically brings the 4th component to 1.
  void setOrigin(const glm::vec4& origin);
  /// Setter for direction. Normalizes the direction.
  void setDirection(const glm::vec4& direction);

  /// Getter for origin.
  const glm::vec4& origin() const { return _origin; }
  /// Getter for direction.
  const glm::vec4& direction() const { return _direction; }

  /// Getter for origin as vec3.
  glm::vec3 originVec3() const { return glm::vec3(_origin); }
  /// Getter for direction as vec3.
  glm::vec3 directionVec3() const { return glm::vec3(_origin); }

  /// *-operator turned around for Ray.
  friend inline Ray operator*(const glm::mat4& lhs, const Ray& rhs) {
    Ray returnRay;
    returnRay.setDirection(lhs * rhs.direction());
    returnRay.setOrigin(lhs * rhs.origin());
    return returnRay;
  }
  /// *-operator turned around for Ray.
  friend inline Ray operator*(const Ray& lhs, const glm::mat4& rhs) {
    Ray returnRay;
    returnRay.setDirection(lhs.direction() * rhs);
    returnRay.setOrigin(lhs.origin() * rhs);
    return returnRay;
  }

  /// Returns the ray info.
  RayInfo& rayInfo() { return _rayInfo; }
  const RayInfo& rayInfo() const { return _rayInfo; }

 private:
  /// This saves additional data, e.g. ray depth, refractive index.
  RayInfo _rayInfo;
  /// Origin of the ray.
  glm::vec4 _origin;
  /// Direction of the ray.
  glm::vec4 _direction;
};

#endif  // RAYTRACERLIB_RAY_H_


