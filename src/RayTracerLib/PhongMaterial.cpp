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

#include <cmath>
#include <glm/glm.hpp>
#include "./Color.h"
#include "./Constants.h"
#include "./Light.h"
#include "./DirectionalLight.h"
#include "./PhongMaterial.h"
#include "./Ray.h"

// _____________________________________________________________________________
Color PhongMaterial::getColor(const glm::vec4& position,
                              const glm::vec4& normal,
                              const glm::vec4& incomingRayDir,
                              const Scene& scene) const {
  // Generate a temp. light.
  DirectionalLight light(glm::vec4(-1, -1, 0, 0));
  // TODO(allofus, Thu May  8 15:27:52 CEST 2014): Add to constructor.
  float ka = 0.1f;
  float kd = 0.2f;
  float ks = 0.7f;
  // Ambient Term
  Color ambient(_color.r * ka, _color.g * ka, _color.b * ka, 255);
  // Sum over Lights and get diffusal and specular components.
  // TODO(allofus, Thu May  8 14:55:00 CEST 2014): loop!!
  Ray shadowRay = light.getDirection(position);
  light.setLightColor(0, 255, 0, 255, Light::DIFFUSE);
  light.setLightColor(0, 255, 0, 255, Light::SPECULAR);
  const Color& lightDiff = light.getColorComponent(Light::DIFFUSE);
  const Color& lightSpec = light.getColorComponent(Light::SPECULAR);
  REAL scale = glm::dot(shadowRay.dir, glm::normalize(normal));
  scale = scale > 0 ? scale : 0;
  glm::vec4 reflectionDir(
      2 * scale * normal.x - shadowRay.dir.x,
      2 * scale * normal.y - shadowRay.dir.y,
      2 * scale * normal.z - shadowRay.dir.z,
      0);
  REAL refl = glm::dot(incomingRayDir, reflectionDir);
  //refl *= refl;
  refl = pow(refl, 20.0f);
  Color diff(
      kd * scale * lightDiff.r,
      kd * scale * lightDiff.g,
      kd * scale * lightDiff.b,
      255);
  Color spec(
      ks * refl * lightSpec.r,
      ks * refl * lightSpec.g,
      ks * refl * lightSpec.b,
      255);
  return Color(
      ambient.r + diff.r + spec.r,
      ambient.g + diff.g + spec.g,
      ambient.b + diff.b + spec.b,
      255);
}