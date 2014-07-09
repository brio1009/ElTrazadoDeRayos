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

#include "./MonteCarloMaterial.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>
#include "./Color.h"
#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Scene.h"
#include "./Light.h"
#include "./Ray.h"
#include "../Solver.h"

using std::vector;

// _____________________________________________________________________________
MonteCarloMaterial::MonteCarloMaterial()
    : PhongMaterial() {
}

// _____________________________________________________________________________
Color MonteCarloMaterial::getColor(const IntersectionInfo& intersectionInfo,
                                   const Ray& incomingRay,
                                   const Scene& scene) const {
  // Check if we should further advance.
  if (stoppingCriteriaMet(incomingRay)) {
    return stoppingColor();
  }

  // TODO(allofus, Thu May  8 15:27:52 CEST 2014): Add to constructor.
  float ka = 0.0f;
  float kd = 1.0f;
  float ks = 0.0f;

  // cause it should be between 0 and 1.
  Color sumIntensity(0, 0, 0);

  // Number of samples in the hemisphere.
  size_t hemisphereSamples = 1;

  // Weighting for each light.
  const std::vector<Light*>& lights = scene.lights();

  // Normal of the object at the position.
  glm::vec4 normNormal = glm::normalize(intersectionInfo.normal);

  // Get the axis to get the tangent.
  glm::vec3 up(0, 1, 1);
  glm::normalize(up);
  if (solve::isZero(glm::dot(glm::vec3(intersectionInfo.normal), up))) {
    up = glm::vec3(1, 1, 0);
  }
  //
  glm::vec3 tangent = glm::cross(glm::vec3(intersectionInfo.normal), up);

  // Shoot sample rays into the hemisphere.
  for (size_t i = 0; i < hemisphereSamples; ++i) {
    // Get a sample on a circle around the hitpoint.
    float phi = (rand() / static_cast<float>(RAND_MAX))  // NOLINT
                        * 2.0f * constants::PI;
    glm::vec3 rotatedTangent = glm::rotate(tangent,
                                          static_cast<float>(phi),
                                          glm::vec3(intersectionInfo.normal));
    // Get the cross between rotatedTangent and normal, so we can rotate the
    // rotatedTangent towards the normal.
    glm::vec3 crossTangent = glm::cross(rotatedTangent,
                                        glm::vec3(intersectionInfo.normal));
    float theta = acos(1.0f - (rand() / static_cast<float>(RAND_MAX)));  // NOLINT

    rotatedTangent = glm::rotate(glm::vec3(intersectionInfo.normal),
                                 static_cast<float>(phi),
                                 crossTangent);
    
    glm::vec4 direction = glm::vec4(rotatedTangent, 0);

    // Get reflected color.
    Color lightColor(0, 0, 0);
    Ray newRay;
    newRay.setDirection(direction);
    newRay.setOrigin(intersectionInfo.hitPoint);
    newRay.rayInfo().depth = incomingRay.rayInfo().depth + 1;
    float dot = std::max(0.0f, glm::dot(newRay.direction(),
                                        intersectionInfo.normal));
    newRay.rayInfo().colorContribution = incomingRay.rayInfo().colorContribution
                                         * dot;
    IntersectionInfo info = scene.traceRay(newRay);
    if (info.materialPtr) {
      lightColor = info.materialPtr->getColor(info,
                                              newRay,
                                              scene);
    } else {
      lightColor = scene.backgroundColor(newRay);
    }
    // Add the color to the return intensity.
    // lightColor *= lightNumWeight;
    sumIntensity += diffuseTerm(lightColor,
                                newRay.direction(),
                                intersectionInfo.normal,
                                kd);
    sumIntensity += specularTerm(lightColor,
                                 newRay.direction(),
                                 intersectionInfo.normal,
                                 -incomingRay.direction(),
                                 ks);
  }
  // Return the color.
  return color() * sumIntensity * (1.0 / hemisphereSamples);
}
