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
#include "./PointLight.h"
#include "./PhongMaterial.h"
#include "./Ray.h"

// _____________________________________________________________________________
Color PhongMaterial::getColor(const glm::vec4& position,
                              const glm::vec4& normal,
                              const glm::vec4& incomingRayDir,
                              const Scene& scene) const {
  // Generate a temp. light.
  PointLight light(glm::vec4(0, -40, 0, 1));
  light.setLightColor(Color(0, 255, 0, 255));

  // TODO(allofus, Thu May  8 15:27:52 CEST 2014): Add to constructor.
  float ka = 0.2f;
  float kd = 0.3f;
  float ks = 0.3f;

  // Ambient Term.
  Color ambient(ka * _color);

  // Sum over Lights and get diffusal and specular components.
  // TODO(allofus, Thu May  8 14:55:00 CEST 2014): Loop over all the real lights
  // in the scene.
  Ray lightRay = light.getRay(position);
  const Color& lightColor = light.getColor();
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
