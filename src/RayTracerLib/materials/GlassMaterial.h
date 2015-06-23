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
#ifndef RAYTRACERLIB_MATERIALS_GLASSMATERIAL_H_
#define RAYTRACERLIB_MATERIALS_GLASSMATERIAL_H_

#include "./Material.h"
#include "./Constants.h"
#include "../Color.h"

/// GLass-like materials.
class GlassMaterial : public Material {
 public:
  /// Constructor.
  explicit GlassMaterial(const float refractiveIndex)
    : m_refractiveIndex(refractiveIndex),
      m_color(1, 0.0, 0.0),
      m_transparencyFactor(5.0f) {}

  /// Returns the color for the given position, normal and ray direction.
  virtual Color getColor(const IntersectionInfo& intersectionInfo,
                         const Ray& incomingRay,
                         const Scene& scene) const;

 protected:
  /// returns the color of the reflected part of this material.
  Color reflectionColor(const glm::vec4& normal,
      const glm::vec3& axis,
      const float& angle,
      const glm::vec4& position,
      const unsigned char currentDepth,
      const float& colorContribution,
      const Scene& scene,
      float* distance) const;

  /// Defines the refractive index which we use to compute the ray directions.
  float m_refractiveIndex;
  /// Defines the material color. Together with the transparency factor this is
  /// is used to "fog" the material with distance.
  Color m_color;

  /// Material transparency factor.
  REAL m_transparencyFactor;
};

#endif  // RAYTRACERLIB_MATERIALS_GLASSMATERIAL_H_

