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
#ifndef RAYTRACERLIB_MATERIALS_BRDF_H_
#define RAYTRACERLIB_MATERIALS_BRDF_H_

#include <glm/glm.hpp>
#include <cstdint>
// Forward declaration.
struct IntersectionInfo;
class Ray;

/// This abstract class describes the Material as BRDF.
/// It provides theta and phi values for samples on the hemisphere.
/// It also provides a density for theta and phi, that is nessecary,
/// to compute the correct solution for the rendering equation with,
/// non-uniform samples.
class BRDF {
 public:
  virtual ~BRDF() { }
  /// generates a theta and phi for given position. (position not used yet, but
  /// there could be uses for it when creating subsurfacescattering materials.
  virtual glm::vec2 generateHemisphereSample(
        const Ray& incomingRay,
        const IntersectionInfo& intersectionInfo,
        const size_t& num) const = 0;

  /// Returnes how many samples should be generated for a point on the surface.
  virtual size_t getSampleCount() const { return 1; }
  /// Returnes the value p(X_i) that is used in the rendering equation.
  virtual float getPDFOfX(const glm::vec2& sample) const = 0;
  /// Returnes the value of the BRDF at given position with given omegas.
  virtual float evaluateBRDF(
          const IntersectionInfo& info,
          const glm::vec4& directionIn,
          const glm::vec4& directionOut) const = 0;
};
#endif  // RAYTRACERLIB_MATERIALS_BRDF_H_

