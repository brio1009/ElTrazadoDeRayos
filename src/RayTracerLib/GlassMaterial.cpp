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

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "./Color.h"
#include "./Constants.h"
#include "./GlassMaterial.h"
#include "./IntersectionInfo.h"
#include "./Ray.h"
#include "./Scene.h"

// _____________________________________________________________________________
Color GlassMaterial::getColor(const glm::vec4& position,
                              const glm::vec4& normal,
                              const glm::vec4& incomingRayDir,
                              const Scene& scene) const {
  // First check if we leave or enter the material.
  // This can be done with the normal.
  REAL dotProduct = glm::dot(normal, incomingRayDir);
  float n1(0), n2(0);
  if (dotProduct < 0) {
    // We leave the material.
    n1 = _refractiveIndex;
    n2 = RefractiveIndex::air;
  } else {
    // We enter the material.
    n1 = RefractiveIndex::air;
    n2 = _refractiveIndex;
  }

  // Calculate the angle of the new ray.
  // glm::angle(incomingRayDir, normal);
  REAL angle = acos(dotProduct);
  REAL newAngle = sin(angle) * (n1 / n2);
  float angleChange = newAngle - angle;
  glm::vec3 axis = glm::cross(glm::vec3(normal), glm::vec3(incomingRayDir));
  Ray newRay;
  newRay.pos = position;
  newRay.dir = glm::rotate(incomingRayDir, angleChange, axis);

  IntersectionInfo info = scene.traceRay(newRay);
  Color addColor;
  if (info.materialPtr) {
    return info.materialPtr->getColor(info.hitPoint,
                                          info.normal,
                                          newRay.dir,
                                          scene);
  }

  // Build the returncolor.
  Color returnColor(0, 0, 0);
  return returnColor;
}
