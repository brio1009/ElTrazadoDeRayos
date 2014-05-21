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

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>
#include "./Color.h"
#include "./Constants.h"
#include "./GlassMaterial.h"
#include "./IntersectionInfo.h"
#include "./Ray.h"
#include "./Scene.h"

size_t depth = 0;
// _____________________________________________________________________________
Color GlassMaterial::reflectionColor(const glm::vec4& normal,
    const glm::vec3& axis,
    const float& angle,
    const glm::vec4& position,
    const Scene& scene) const {
  // get reflected color
  Ray newRay;
  newRay.setDirection(glm::rotate(normal,
                                  static_cast<float>(angle),
                                  axis));
  newRay.setOrigin(position);
  IntersectionInfo info = scene.traceRay(newRay);
  if (info.materialPtr && depth < 10/* && info.materialPtr != this*/) {
    return info.materialPtr->getColor(info,
                                      newRay,
                                      scene);
  }
  if (depth >= 10) {
    return Color(1, 1, 1);
  }
  return Color(1, 1, 1);
}

// _____________________________________________________________________________
Color GlassMaterial::getColor(const IntersectionInfo& intersectionInfo,
                              const Ray& incomingRay,
                              const Scene& scene) const {
  depth++;
  Color materialColor;
  // The axis to rotate around.
  glm::vec4 normNormal = glm::normalize(intersectionInfo.normal);
  glm::vec4 normView = glm::normalize(-incomingRay.direction());
  glm::vec3 axis = glm::cross(glm::vec3(normNormal), glm::vec3(normView));
  float tau1 = glm::angle(normNormal, normView);
  float n1, n2, tau2;
  // calculate tau1 and tau2 in respect to the normal.
  if (tau1 < glm::pi<float>() / 2) {
    n1 = RefractiveIndex::air;
    n2 = _refractiveIndex;
    tau2 = glm::pi<float>() + asin((n1 / n2) * sin(tau1));
  } else {
    // angle was bigger then 90 degrees.
    n2 = RefractiveIndex::air;
    n1 = _refractiveIndex;
    tau2 = -asin((n1 / n2) * sin(glm::pi<float>() - tau1));
  }
  // TODO(bauschp, Tue May 20 14:08:52 CEST 2014): maybe change to map lookup.
  // Should safe a lot of power calculations.
  float refl = pow((n1 - n2) / (n1 + n2), 2.0);
  // SNELLS LAW
  if (((n1 / n2) * sin(tau1)) < 1) {
    materialColor = (1 - refl)
        * reflectionColor(normNormal, axis, tau2, intersectionInfo.hitPoint, 
                          scene);
  } else {
    refl = 1.0f;
  }
  materialColor += refl
      * reflectionColor(normNormal, axis, -tau1, intersectionInfo.hitPoint,
                        scene);
  depth--;
  return materialColor;
}
