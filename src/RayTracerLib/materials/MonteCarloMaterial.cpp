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
#include "../samplers/AdaptiveSampler.h"
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
  glm::vec3 normal = glm::vec3(glm::normalize(intersectionInfo.normal));

  // Get the tangent.
  glm::vec3 tangent(0, 0, 1);

  glm::vec3 up(0, 1, 0);
  glm::vec3 diff = glm::abs(normal) - up;

  if (!solve::isZero(glm::dot(diff, diff))) {
    tangent = glm::cross(normal, up);
  }

  // boolean to decide if regular smapling.
  bool regularSampling(false);

  // Shoot sample rays into the hemisphere.
  for (size_t i = 0; i < hemisphereSamples; ++i) {
    // Get a sample on a circle around the hitpoint.
    float phi(0);
    float theta(0);
    if (regularSampling) {
      phi = AdaptiveSampler::generateHalton(i, 2);
      theta = AdaptiveSampler::generateHalton(i, 3);
    } else {
      phi = rand() / static_cast<float>(RAND_MAX);  // NOLINT
      theta = rand() / static_cast<float>(RAND_MAX);  // NOLINT
    }
    // Transform them to uniform samples.
    phi *= 2.0f * constants::PI;
    theta = acos(1.0f - theta);

    // Get the direction from phi and theta.
    glm::vec3 rotatedTangent = glm::rotate(tangent,
                                           static_cast<float>(phi),
                                           normal);
    // Get the cross between rotatedTangent and normal, so we can rotate the
    // rotatedTangent towards the normal.
    glm::vec3 crossTangent = glm::cross(rotatedTangent, normal);

    rotatedTangent = glm::rotate(normal,
                                 static_cast<float>(theta),
                                 crossTangent);

    glm::vec4 direction = glm::vec4(rotatedTangent, 0);

    // Get reflected color.
    Color lightColor(0, 0, 0);
    Ray newRay;
    newRay.setDirection(direction);
    newRay.setOrigin(intersectionInfo.hitPoint);
    newRay.rayInfo().depth = incomingRay.rayInfo().depth + 1;
    newRay.rayInfo().colorContribution = incomingRay.rayInfo().colorContribution
                                         * cos(theta);
    IntersectionInfo info = scene.traceRay(newRay);
    if (info.materialPtr) {
      lightColor = info.materialPtr->getColor(info,
                                              newRay,
                                              scene);
    } else {
      lightColor = scene.backgroundColor(newRay);
    }

    // Add the color to the return intensity.
    sumIntensity += cos(theta) * lightColor;
  }
  // Return the color.
  return color() * sumIntensity * (1.0 / hemisphereSamples);
}
