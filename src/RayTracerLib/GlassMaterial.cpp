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
#include "./Color.h"
#include "./Constants.h"
#include "./GlassMaterial.h"
#include "./IntersectionInfo.h"
#include "./Ray.h"
#include "./Scene.h"

size_t depth = 0;
// _____________________________________________________________________________
Color GlassMaterial::reflectionColor(const glm::vec4& normal,
    const glm::vec4& viewDir,
    const float& angle,
    const glm::vec4& position,
    const Scene& scene) const {
  // Calculate reflectivy
  glm::vec3 axis = glm::cross(glm::vec3(normal),
                              glm::vec3(viewDir));
  // get reflected color
  Ray newRay;
  newRay.setDirection(glm::rotate(normal,
                                  static_cast<float>(angle),
                                  axis));
  // the epsilon in scene doesnt seem to do its job.
  newRay.setOrigin(position/* + newRay.direction()*/);
  IntersectionInfo info = scene.traceRay(newRay);
  if (info.materialPtr && depth < 10/* && info.materialPtr != this*/) {
    return info.materialPtr->getColor(info.hitPoint,
                                              info.normal,
                                              newRay,
                                              scene);
  }
  if (depth >= 10) {
    return _color;
  }
  return Color(0, 0, 0);
}

// _____________________________________________________________________________
Color GlassMaterial::getColor(const glm::vec4& position,
                              const glm::vec4& normal,
                              const Ray& incomingRay,
                              const Scene& scene) const {
  depth++;
  Color materialColor(0, 0, 0);
  // next ray placeholder
  Ray nextRay;
  glm::vec4 normNormal = glm::normalize(normal);
  glm::vec4 normDir = glm::normalize(-incomingRay.direction());
  float tau1 = glm::orientedAngle(glm::vec3(normNormal), glm::vec3(normDir), glm::cross(glm::vec3(normNormal), glm::vec3(normDir)));
  // Refraction part of this material.
  float n1(0), n2(0);
  if (abs(tau1) > 3.1415 / 2) {
    n1 = RefractiveIndex::glass;
    n2 = RefractiveIndex::air;
    normNormal = -normNormal;
    tau1 -= 3.14159;
  } else {
    // We enter the material.
    n1 = RefractiveIndex::air;
    n2 = RefractiveIndex::glass;
  }
  // Snells law.
  if (((n1 / n2) * sin(tau1)) > 1) {
    // There is no refraction. can only happen from dense to less dense mat.
    materialColor = reflectionColor(normNormal, normDir, -tau1, position, scene);
  } else {
    // outgoing angle (into material.)
    float tau2 = asin((n1 / n2) * sin(tau1));
    float refl = pow((n1 - n2) / (n1 + n2), 2.0);
    materialColor += (1.0 - refl)
        * reflectionColor(-normNormal, normDir, tau2, position, scene);
    // Reflected part.
    materialColor += refl
        * reflectionColor(normNormal, normDir, -tau1, position, scene);
  }
  --depth;
  return materialColor;
  /*
  // First check if we leave or enter the material.
  // This can be done with the normal.
  REAL dotProduct = glm::dot(normal, -incomingRay.direction());
  float n1(0), n2(0);
  if (dotProduct < 0) {
    // We leave the material.
    n1 = _refractiveIndex;
    n2 = RefractiveIndex::air;
    // Calculate the distance we travelled inside the glass.
    REAL distance = glm::length(incomingRay.origin() - position);

    materialColor += distance * _transparencyFactor * _color;
  } else {
    // We enter the material.
    n1 = RefractiveIndex::air;
    n2 = _refractiveIndex;
  }

  // Calculate the angle of the new ray.
  // glm::angle(incomingRayDir, normal);
  REAL newAngle = sin(angle) * (n1 / n2);
  REAL angleChange = newAngle - angle;
  glm::vec3 axis = glm::cross(glm::vec3(normal),
                              glm::vec3(incomingRay.direction()));
  Ray newRay;
  newRay.setOrigin(position);
  newRay.setDirection(glm::rotate(incomingRay.direction(),
                                  static_cast<float>(angleChange),
                                  axis));

  IntersectionInfo info = scene.traceRay(newRay);
  Color returnColor;
  if (info.materialPtr) {
    returnColor += info.materialPtr->getColor(info.hitPoint,
                                              info.normal,
                                              newRay,
                                              scene);
  }

  // Build the returncolor.
  returnColor += materialColor;
  return returnColor;
  */
}
