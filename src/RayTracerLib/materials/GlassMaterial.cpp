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

// _____________________________________________________________________________
Color GlassMaterial::reflectionColor(const glm::vec4& normal,
    const glm::vec3& axis,
    const float& angle,
    const glm::vec4& position,
    unsigned char currentDepth,
    const Scene& scene) const {
  // If the depth is too high we just return some color.
  // TODO(all, Mon May 26 13:49:04 CEST 2014): What color to return?
  if (currentDepth >= constants::maxDepth) {
    return Color(0, 0, 0);
  }
  // Get reflected color.
  Ray newRay;
  newRay.setDirection(glm::rotate(normal,
                                  static_cast<float>(angle),
                                  axis));
  newRay.setOrigin(position);
  newRay.rayInfo().depth = currentDepth + 1;
  IntersectionInfo info = scene.traceRay(newRay);
  if (info.materialPtr) {
    return info.materialPtr->getColor(info,
                                      newRay,
                                      scene);
  }
  return scene.backgroundColor(newRay);
}

// _____________________________________________________________________________
Color GlassMaterial::getColor(const IntersectionInfo& intersectionInfo,
                              const Ray& incomingRay,
                              const Scene& scene) const {
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
  float refl = pow((n1 - n2) / (n1 + n2), 2.0f);
  // SNELLS LAW
  if (((n1 / n2) * sin(tau1)) < 1) {
    materialColor = (1 - refl)
        * reflectionColor(normNormal,
                          axis,
                          tau2,
                          intersectionInfo.hitPoint,
                          incomingRay.rayInfo().depth,
                          scene);
  } else {
    refl = 1.0f;
  }
  materialColor += refl
      * reflectionColor(normNormal,
                        axis,
                        -tau1,
                        intersectionInfo.hitPoint,
                        incomingRay.rayInfo().depth,
                        scene);
  return materialColor;
}
