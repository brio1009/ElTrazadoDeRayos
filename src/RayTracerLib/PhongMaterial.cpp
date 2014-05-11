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
#include <cassert>
#include <cmath>
#include <glm/glm.hpp>
#include "./Color.h"
#include "./Constants.h"
#include "./Light.h"
#include "./PointLight.h"
#include "./PhongMaterial.h"
#include "./Ray.h"

// _____________________________________________________________________________
Color PhongMaterial::getColor(const glm::vec4& position,
                              const glm::vec4& normal,
                              const glm::vec4& incomingRayDir,
                              const Scene& scene) const {
  // Generate a temp. light.
  PointLight light(glm::vec4(0, 0, 0, 1));
  light.setLightColor(Color(255, 255, 255, 255));

  // TODO(allofus, Thu May  8 15:27:52 CEST 2014): Add to constructor.
  float ka = 0.1f;
  float kd = 0.6f;
  float ks = 0.8f;


  // TODO(allofus, Sun May 11 16:54:17 CEST 2014): change lightcolor to vec3
  // cause it should be between 0 and 1.
  const Color& lightColor = light.getColor();
  glm::vec3 intensity(lightColor.r() / 255.0f,
      lightColor.g() / 255.0f,
      lightColor.b() / 255.0f);
  glm::vec3 sumIntensity(0, 0, 0);
  Ray lightRay = light.getRay(position);
  sumIntensity += ambientTerm(intensity, ka);
  sumIntensity += diffuseTerm(intensity, -lightRay.dir, normal, kd);
  sumIntensity += specularTerm(intensity, -lightRay.dir,
      normal, incomingRayDir, ks);

  return Color(_color.r() * sumIntensity.x, _color.g() * sumIntensity.y, _color.b() * sumIntensity.z, 255);
  // Ambient Term.
  Color ambient(ka * _color);

  // Sum over Lights and get diffusal and specular components.
  // TODO(allofus, Thu May  8 14:55:00 CEST 2014): Loop over all the real lights
  // in the scene.
  // Ray lightRay = light.getRay(position);
  float r = lightColor.r();
  float g = lightColor.g();
  float b = lightColor.b();
  // Add the ambientColor.
//  Color resultingColor(ambient);
  glm::vec3 resultingColor( ambient.r(), ambient.g(), ambient.b());
  // Add the diffuse color
  float reflectionDot = glm::dot(-lightRay.dir, glm::normalize(normal));
  if (reflectionDot > 0.0) {
//    resultingColor += Color(kd * reflectionDot * radianceMulti * lightColor);
    resultingColor += glm::vec3(r, g, b) * kd * reflectionDot;
    // Add the specular color.
    glm::vec4 reflectionDir(static_cast<float>(2 * reflectionDot) * normal
                            - lightRay.dir);

    float refl = glm::dot(glm::normalize(-incomingRayDir),
                         glm::normalize(reflectionDir));
    // if (refl < 0.0) {
      refl = pow(refl, 4.0f);
//      resultingColor += Color(ks * refl * lightColor);
        resultingColor += ks * refl * glm::vec3(r, g, b);
    // }
  }
//  return resultingColor;
  return Color(resultingColor.x, resultingColor.y, resultingColor.z, 255);
}

// _____________________________________________________________________________
glm::vec3 PhongMaterial::ambientTerm(const glm::vec3& color,
    const float skalar) const {
  return skalar * color;
}
// _____________________________________________________________________________
glm::vec3 PhongMaterial::diffuseTerm(const glm::vec3& color,
    const glm::vec4& lightDir,
    const glm::vec4& normal,
    const float skalar) const {
  // Assuming normal and lightDir are normalized.
  assert(glm::length(lightDir) == 1);
  assert(glm::length(normal) == 1);
  float dot = std::max(0.0f, glm::dot(lightDir, normal));
  return skalar * dot * color;
}
// _____________________________________________________________________________
glm::vec3 PhongMaterial::specularTerm(const glm::vec3& color,
    const glm::vec4& lightDir,
    const glm::vec4& normal,
    const glm::vec4& viewer,
    const float skalar) const {
  // Assuming normal and lightDir are normalized.
  assert(glm::length(lightDir) == 1);
  assert(glm::length(normal) == 1);
  float lXn = std::max(0.0f, glm::dot(lightDir, normal));
  glm::vec4 reflectionDir = 2.0f * lXn * normal - lightDir;
  float dot = std::max(0.0f, glm::dot(reflectionDir, viewer));
  dot = pow(dot, 16.0);
  return skalar * dot * color;
}
