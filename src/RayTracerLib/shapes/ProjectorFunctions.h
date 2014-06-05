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
#ifndef RAYTRACERLIB_SHAPES_PROJECTORFUNCTIONS_H
#define RAYTRACERLIB_SHAPES_PROJECTORFUNCTIONS_H

#include <glm/glm.hpp>
#include "../Constants.h"

/// These are some basic projector functions to map a point to a texture
/// coordinate.
namespace ProjectorFunctions {
  /// Texture coords projection function plane.
  inline glm::vec2 textureProjectionPlaneXZ(const glm::vec4& localPoint) {
    return glm::vec2(localPoint.x, localPoint.z);
  }

  /// Texture coords projection function sphere.
  inline glm::vec2 textureProjectionSphere(const glm::vec4& localPoint) {
    // Get the unit vector to the position.
    glm::vec3 d(-localPoint);
    glm::normalize(d);
    return glm::vec2(0.5 + (atan2(d.z, d.x) / (2.0 * constants::PI)),
                      0.5 - (asin(d.y) / constants::PI));
  }
}

#endif  // RAYTRACERLIB_SHAPES_PROJECTORFUNCTIONS_H