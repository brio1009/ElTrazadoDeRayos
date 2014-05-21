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
#include "./PhongMaterial.h"
#include <glm/glm.hpp>
#include <cassert>
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

// _____________________________________________________________________________
PhongMaterial::PhongMaterial() {
  // Construct a random color.
  // TODO(allofus, Sun May 11 14:12:21 CEST 2014): change to threadsafe alt.
  _color.setR((rand() % 255) / 255.0f);  //NOLINT
  _color.setG((rand() % 255) / 255.0f);  //NOLINT
  _color.setB((rand() % 255) / 255.0f);  //NOLINT
  _color.setA(1.0f);
}

// _____________________________________________________________________________
Color PhongMaterial::getColor(const IntersectionInfo& intersectionInfo,
                              const Ray& incomingRay,
                              const Scene& scene) const {
  // TODO(allofus, Thu May  8 15:27:52 CEST 2014): Add to constructor.
  float ka = 0.1f;
  float kd = 0.4f;
  float ks = 0.5f;

  // cause it should be between 0 and 1.
  Color sumIntensity(0, 0, 0);
  sumIntensity += ambientTerm(Color(1, 1, 1), ka);
  const std::vector<Light*> lights = scene.lights();
  for (size_t i = 0; i < lights.size(); ++i) {
    const Color& lightColor = lights.at(i)->getColor();
    // TODO(bauschp, Sun May 11 21:09:39 CEST 2014) norm if works.
    glm::vec4 normNormal = glm::normalize(intersectionInfo.normal);
    Ray lightRay = lights.at(i)->getRay(intersectionInfo.hitPoint);
    sumIntensity += diffuseTerm(lightColor,
                                -lightRay.direction(),
                                normNormal, kd);
    sumIntensity += specularTerm(lightColor, -lightRay.direction(),
        normNormal, -incomingRay.direction(), ks);
  }
  return color() * sumIntensity;
}

// _____________________________________________________________________________
Color PhongMaterial::ambientTerm(const Color& color,
    const float skalar) const {
  return skalar * color;
}

// _____________________________________________________________________________
Color PhongMaterial::diffuseTerm(const Color& color,
    const glm::vec4& lightDir,
    const glm::vec4& normal,
    const float skalar) const {
  // Assuming normal and lightDir are normalized.
  assert(lightDir[3] == 0);
  assert(glm::length(lightDir) > 1.0 - 1e-5
      && glm::length(lightDir) < 1.0 + 1e-5);
  assert(normal[3] == 0);
  assert(glm::length(normal) > 1.0 - 1e-5
      && glm::length(normal) < 1.0 + 1e-5);
  float dot = std::max(0.0f, glm::dot(lightDir, normal));
  return skalar * dot * color;
}

// _____________________________________________________________________________
Color PhongMaterial::specularTerm(const Color& color,
    const glm::vec4& lightDir,
    const glm::vec4& normal,
    const glm::vec4& viewer,
    const float skalar,
    const float shininess) const {
  // Assuming normal and lightDir are normalized.
  assert(lightDir[3] == 0);
  assert(glm::length(lightDir) > 1.0 - 1e-5
      && glm::length(lightDir) < 1.0 + 1e-5);
  assert(normal[3] == 0);
  assert(glm::length(normal) > 1.0 - 1e-5
      && glm::length(normal) < 1.0 + 1e-5);

  float lXn = glm::dot(lightDir, normal);
  glm::vec4 reflectionDir = 2.0f * lXn * normal - lightDir;

  assert(reflectionDir[3] == 0);
  assert(glm::length(reflectionDir) > 1.0 - 1e-5
      && glm::length(reflectionDir) < 1.0 + 1e-5);

  float dot = std::max(0.0f, glm::dot(reflectionDir, viewer));
  dot = pow(dot, shininess);
  return skalar * dot * color;
}
