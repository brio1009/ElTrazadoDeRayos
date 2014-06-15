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

#include "./Box.h"

#include <glm/glm.hpp>

#include <algorithm>
#include <cstdio>
#include <limits>
#include <utility>
#include <vector>

#include "./Constants.h"
#include "../Solver.h"
#include "./Ray.h"

using std::vector;

// _____________________________________________________________________________
Box::Box(REAL x, REAL y, REAL z) : _rX(x), _rY(y), _rZ(z) {
}

// _____________________________________________________________________________
vector<REAL> Box::intersect(const Ray& ray) const {
  // Bring vector to unit space.
  Ray transRay = _inverseTransform * ray;
  // This is the slab method.
  const glm::vec4& rayDir = transRay.direction();
  glm::vec4 invRayDir;

  invRayDir.x = (!solve::isZero(rayDir.x)) ? (1.0f / rayDir.x)
                                  : std::numeric_limits<float>::max();
  invRayDir.y = (!solve::isZero(rayDir.y)) ? (1.0f / rayDir.y)
                                  : std::numeric_limits<float>::max();
  invRayDir.z = (!solve::isZero(rayDir.z)) ? (1.0f / rayDir.z)
                                  : std::numeric_limits<float>::max();
  invRayDir.w = 0.0f;

  glm::vec4 t1 = (getMinPosition() - transRay.origin()) * invRayDir;
  glm::vec4 t2 = (getMaxPosition() - transRay.origin()) * invRayDir;

  glm::vec4 tMin = glm::min(t1, t2);
  glm::vec4 tMax = glm::max(t1, t2);

  vector<REAL> hits(2);

  hits[0] = std::max(tMin.x, std::max(tMin.y, tMin.z));
  hits[1] = std::min(tMax.x, std::min(tMax.y, tMax.z));

  if (hits[1] >= hits[0])
    return hits;

  // Else.
  return vector<REAL>();
}
// _____________________________________________________________________________
glm::vec4 Box::getNormalAt(const glm::vec4& p) const {
  glm::vec4 pInObjectCoord = _inverseTransform * p;

  // We just have six cases (three real ones + the inverse).
  // Just check where we are the closest.
  glm::vec3 extent(_rX, _rY, _rZ);
  glm::vec3 dis = extent - glm::abs(glm::vec3(pInObjectCoord));
  glm::vec4 out;
  if (dis.x < dis.y && dis.x < dis.z) {
    // Normal in x direction.
    if (pInObjectCoord.x <= 0) {
      out = glm::vec4(-1, 0, 0, 0);
    } else {
      out = glm::vec4(1, 0, 0, 0);
    }
  } else if (dis.y < dis.x && dis.y < dis.z) {
    // Normal in y direction.
    if (pInObjectCoord.y <= 0) {
      out = glm::vec4(0, -1, 0, 0);
    } else {
      out = glm::vec4(0, 1, 0, 0);
    }
  } else {
    // Normal in z direction.
    if (pInObjectCoord.z <= 0) {
      out = glm::vec4(0, 0, -1, 0);
    } else {
      out = glm::vec4(0, 0, 1, 0);
    }
  }
  return glm::normalize(_transformation * out);
}


// _____________________________________________________________________________
glm::vec4 Box::getMinPosition() const {
  // We just calculate the position.
  return glm::vec4(-0.5 * _rX, -0.5 * _rY, -0.5 * _rZ, 1.0);
}

// _____________________________________________________________________________
glm::vec4 Box::getMaxPosition() const {
  // We just calculate the position.
  return glm::vec4(0.5 * _rX, 0.5 * _rY, 0.5 * _rZ, 1.0);
}
