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

#include "./GlassMaterial.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <cmath>

#include "../Color.h"
#include "../IntersectionInfo.h"
#include "../Ray.h"
#include "../Scene.h"

// _____________________________________________________________________________
Color GlassMaterial::getColor(const IntersectionInfo& intersectionInfo,
                              const Ray& incomingRay,
                              const Scene& scene) const {
  // First we calculate if the stopping condition is reached.
  if (stoppingCriteriaMet(incomingRay)) {
    // We won't advance further.
    return stoppingColor();
  }
  // Initialize some quantities that are needed for the calculation.
  // The surface normal at the intersection.
  glm::vec4 normNormal = glm::normalize(intersectionInfo.normal);
  // Direction of the ray.
  glm::vec4 normView = glm::normalize(-incomingRay.direction());
  // The rays incoming azimuth angle. (North = normal)
  float tau1 = glm::angle(normNormal, normView);

  // Calculate the variables for snell's law.
  bool inside = false;
  // Do we intersect from within?
  if (tau1 > glm::pi<float>() / 2) {
    inside = true;
    normNormal = -normNormal;
    tau1 = glm::angle(normNormal, normView);
  }
  // The axis to rotate around.
  glm::vec3 axis = glm::cross(glm::vec3(normNormal), glm::vec3(normView));

  // Set the refraction index for the materials.
  float n1, n2;
  if (inside) {
    // TODO(bauschp): FIX.
    n2 = incomingRay.rayInfo().topRefractiveIndex();
    // if within the material the index is the next one in the stack.
    n1 = m_refractiveIndex;
  } else {
    n1 = incomingRay.rayInfo().topRefractiveIndex();
    n2 = m_refractiveIndex;
  }
  // Calculate tau2 (Snell).
  float totalRef = (n1 / n2) * sin(tau1);
  float tau2 = asin(totalRef);

  // Calculate the reflection direction.
  // TODO(bauschp, Di 23. Jun 10:15:34 CEST 2015): Is there a more efficient
  // way to do this?
  glm::vec4 reflectionDirection = glm::rotate(normNormal, -tau1, axis);
  glm::vec4 refractionDirection =
      glm::rotate(normNormal, glm::pi<float>() + tau2, axis);

  // At this point the graphic from wikipedias Snell's Law Article describes
  // our problem precisely.
  // Now we have to calculate the distribution of reflected and refracted light.
  // Fresnel's equation will help us:
  // TODO(bauschp, Di 23. Jun 09:58:52 CEST 2015): Real Fresnel.
  // Schlick's approximation.
  float refl = (n1 - n2) / (n1 + n2);
  refl *= refl;
  float distance = 0.0f;
  Color outColor;
  const RayInfo& oldRayInfo = incomingRay.rayInfo();
  // Is there any refraction?
  if (totalRef < 1) {
    // We have refraction.
    Ray refractionRay;
    refractionRay.setOrigin(intersectionInfo.hitPoint);
    refractionRay.setDirection(refractionDirection);
    refractionRay.rayInfo().setRefractionStack(oldRayInfo.copyStack());
    // When we refract, we have to modify our refraction stack.
    if (inside)  // This fails if materials intersect each other.
      refractionRay.rayInfo().popRefractiveIndex();
    else
      refractionRay.rayInfo().pushRefractiveIndex(n2);
    refractionRay.rayInfo().colorContribution =
        oldRayInfo.colorContribution * (1 - refl);
    IntersectionInfo info = scene.traceRay(refractionRay);
    if (info.materialPtr) {
      distance = info.t;
      outColor = info.materialPtr->getColor(info, refractionRay, scene);
    } else
      outColor = scene.backgroundColor(refractionRay);
    outColor *= 1 - refl;
    if (!inside)
      outColor *= Color(expf(m_transparencyFactor * m_color.r() * -distance),
                        expf(m_transparencyFactor * m_color.g() * -distance),
                        expf(m_transparencyFactor * m_color.b() * -distance));
  } else {
    // Total refraction!
    refl = 1.0f;
  }
  // Reflection won't change our refraction stack.
  Ray reflectionRay;
  reflectionRay.setOrigin(intersectionInfo.hitPoint);
  reflectionRay.setDirection(reflectionDirection);
  reflectionRay.rayInfo().setRefractionStack(oldRayInfo.copyStack());
  reflectionRay.rayInfo().colorContribution =
      oldRayInfo.colorContribution * refl;
  IntersectionInfo info = scene.traceRay(reflectionRay);
  Color tmpColor;
  if (info.materialPtr) {
    distance = info.t;
    tmpColor = info.materialPtr->getColor(info, reflectionRay, scene);
  } else
    tmpColor = scene.backgroundColor(reflectionRay);
  Color transmission(expf(m_transparencyFactor * m_color.r() * -distance),
                     expf(m_transparencyFactor * m_color.g() * -distance),
                     expf(m_transparencyFactor * m_color.b() * -distance));
  if (!inside)
    transmission = Color(1, 1, 1);
  tmpColor *= refl * transmission;
  outColor += tmpColor;

  return outColor;
}
