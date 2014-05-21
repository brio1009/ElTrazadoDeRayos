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
#include "./ShadowMaterial.h"
#include <glm/glm.hpp>
#include <cassert>
#include <cmath>
#include <algorithm>
#include "./Color.h"
#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Scene.h"
#include "./PointLight.h"
#include "./Light.h"
#include "./Ray.h"

// _____________________________________________________________________________
Color ShadowMaterial::getColor(const IntersectionInfo& intersectionInfo,
                               const Ray& incomingRay,
                               const Scene& scene) const {
  // Generate a temp. light.
  PointLight light(glm::vec4(0, 0, 0, 1));
  light.setLightColor(Color(1.0f, 1.0f, 1.0f, 1.0f));

  // TODO(allofus, Thu May  8 15:27:52 CEST 2014): Add to constructor.
  float ka = 0.1f;
  float kd = 0.4f;
  float ks = 0.5f;


  const Color& lightColor = light.getColor();
  Color sumIntensity(0, 0, 0);
  // TODO(bauschp, Sun May 11 21:09:39 CEST 2014) norm if works.
  glm::vec4 normNormal = glm::normalize(intersectionInfo.normal);
  Ray lightRay = light.getRay(intersectionInfo.hitPoint);
  Ray shadowRay(intersectionInfo.hitPoint, -lightRay.direction());
  IntersectionInfo hitInfo = scene.traceRay(shadowRay);
  glm::vec4 diff = intersectionInfo.hitPoint - lightRay.origin();
  float distanceToLight = glm::dot(diff, diff);
  diff = intersectionInfo.hitPoint - hitInfo.hitPoint;
  float distanceToHit = glm::dot(diff, diff);
  sumIntensity += ambientTerm(lightColor, ka);
  if (!hitInfo.materialPtr || distanceToLight < distanceToHit) {
    sumIntensity += diffuseTerm(lightColor,
                                -lightRay.direction(),
                                normNormal,
                                kd);
    sumIntensity += specularTerm(lightColor, -lightRay.direction(),
        normNormal, -incomingRay.direction(), ks);
  }

  return color() * sumIntensity;
}
