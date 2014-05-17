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
#ifndef RAYTRACERLIB_GLASSMATERIAL_H_
#define RAYTRACERLIB_GLASSMATERIAL_H_

#include "Material.h"
#include "Constants.h"

/// Refraction indices we know.
namespace RefractiveIndex {
  static const float glass = 1.52f;
  static const float water = 1.333f;
  static const float air = 1.00293f;
  static const float diamond = 2.42f;
};

/// GLass-like materials.
class GlassMaterial : public Material {
public:
  /// Constructor.
  GlassMaterial(const float refractiveIndex)
    : _refractiveIndex(refractiveIndex),
      _transparencyFactor(0.0f),
      _color(1.0, 0.0, 0.0) { }

  /// Returns the color for the given position, normal and ray direction.
  virtual Color getColor(const glm::vec4& position,
      const glm::vec4& normal,
      const Ray& incomingRay,
      const Scene& scene) const;

 protected:
  /// returns the color of the reflected part of this material.
  Color reflectionColor(const glm::vec4& normal,
      const glm::vec4& viewDir,
      const float& angle,
      const glm::vec4& position,
      const Scene& scene) const;
  
  /// Defines the refractive index which we use to compute the ray directions.
  float _refractiveIndex;
  /// Defines the material color. Together with the transparency factor this is
  /// is used to "fog" the material with distance.
  Color _color;

  /// Material transparency factor.
  REAL _transparencyFactor;
};

#endif  // RAYTRACERLIB_GLASSMATERIAL_H_
