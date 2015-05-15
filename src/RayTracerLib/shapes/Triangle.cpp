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

#include "./Triangle.h"

#include <glm/glm.hpp>
#include <glm/gtx/fast_square_root.hpp>  // fastNormalize
#include <cstdio>
#include <vector>
#include "../Constants.h"
#include "../Ray.h"

using std::vector;
using glm::vec3;

std::vector<REAL> intersectTriangles(
    const Ray& ray,
    const std::vector<glm::vec3>& vertices,
    const size_t& begin, const size_t& end,
    std::vector<size_t>* hitTriangles) {
  vector<REAL> result;
  // Check if begin and end are a multiple of 3.
  if (end <= begin || (end - begin) % 3 != 0) {
    perror("begin and end invalid.\n");
    return result;
  }
  // begin and end are valid.
  // clear the hit triangle vector.
  hitTriangles->clear();
  // Loop over all the triangles.
  for (size_t i = begin; i < end; i += 3) {
    vec3 e1 = vertices.at(i + 1) - vertices.at(i);
    vec3 e2 = vertices.at(i + 2) - vertices.at(i);
    vec3 s = ray.originVec3() - vertices.at(i);

    vec3 surfaceNormal = glm::fastNormalize(glm::cross(e1, e2));
    // Check if the ray is parallel to the triangle surface. (No Intersection)
    if (glm::dot(surfaceNormal, ray.directionVec3()) == 0) {
      // No intersection
      continue;
    }
    vec3 sxe1 = glm::cross(s, e1);
    vec3 dxe2 = glm::cross(ray.directionVec3(), e2);
    float factor = 1.0f / glm::dot(dxe2, e1);
    float b1 = factor * glm::dot(dxe2, s);
    float b2 = factor * glm::dot(sxe1, ray.directionVec3());
    float t = factor * glm::dot(sxe1, e2);
    // Check if bar Coordinates are inside the triangle.
    if (b1 > 0.0f && b2 > 0.0f && b1 + b2 < 1.0f) {
      // Intersection!!!!
      hitTriangles->push_back(i);
      // TODO(bauschp): what???
      result.push_back(t);
    }
  }
  return result;
}
