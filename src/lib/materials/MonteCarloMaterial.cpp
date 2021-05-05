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
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <memory>
#include <vector>
#include "./Color.h"
#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Ray.h"
#include "./Scene.h"
#include "./Solver.h"
#include "lights/AreaShape.h"
#include "lights/Light.h"
#include "materials/BRDF.h"
#include "materials/PhongBRDF.h"
#include "samplers/AdaptiveSampler.h"

using std::vector;

const char* MonteCarloMaterial::name = "MonteCarloMaterial";

// _____________________________________________________________________________
MonteCarloMaterial::MonteCarloMaterial() {
  m_BRDF = std::make_shared<PhongBRDF>();
}
// _____________________________________________________________________________
MonteCarloMaterial::MonteCarloMaterial(const Color& color,
                                       std::shared_ptr<BRDF> brdf)
    : m_Color(color) {
  m_BRDF = brdf;
}
// _____________________________________________________________________________
MonteCarloMaterial::MonteCarloMaterial(const Color& color) : m_Color(color) {
  m_BRDF = std::make_shared<PhongBRDF>();
}

// _____________________________________________________________________________
Color MonteCarloMaterial::getColor(const IntersectionInfo& intersectionInfo,
                                   const Ray& incomingRay,
                                   const Scene& scene) const {
  // Check if we should further advance.
  if (stoppingCriteriaMet(incomingRay)) {
    return stoppingColor();
  }

  // Signals if the important shapes should be handled separately with the
  // are form of the rendering equation.
  bool sampleImportantShapes(true);

  // Number of samples in the hemisphere.
  size_t hemisphereSamples = m_BRDF->getSampleCount();

  // Normal of the object at the position.
  glm::vec3 normal = glm::vec3(glm::normalize(intersectionInfo.normal));

  // Get the tangent.
  glm::vec3 tangent(0, 0, 1);

  glm::vec3 up(0, 1, 0);
  glm::vec3 diff = glm::abs(normal) - up;

  if (!solve::isZero(glm::dot(diff, diff))) {
    tangent = glm::cross(normal, up);
  }

  // -------------------------- LIGHT SAMPLING --------------------------------
  // Color of the important shapes.
  Color importantShapeColor(0, 0, 0);
  if (sampleImportantShapes && scene.importantShapes().size() != 0) {
    // Loop over the important objects.
    for (size_t i = 0; i < scene.importantShapes().size(); ++i) {
      const ImportantShape* const shapePtr = scene.importantShapes().at(i);
      // Get the number of samples.
      size_t numSamples = shapePtr->numSamples();
      for (size_t t = 0; t < numSamples; ++t) {
        // Get the sample point.
        glm::vec4 direction =
            shapePtr->getSample(t) - intersectionInfo.hitPoint;
        REAL distance = glm::length(direction);
        // Build a ray to there.
        Ray newRay;
        newRay.setDirection(direction);
        newRay.setOrigin(intersectionInfo.hitPoint);
        newRay.rayInfo().depth = incomingRay.rayInfo().depth + 1;
        float theta = glm::angle(intersectionInfo.normal, newRay.direction());
        float cosTheta = cos(theta);
        if (cosTheta <= 0.0f) {
          continue;
        }
        newRay.rayInfo().colorContribution =
            incomingRay.rayInfo().colorContribution * cosTheta;
        IntersectionInfo info = scene.traceRay(newRay);
        if (info.materialPtr && info.hitImportantShape) {
          // Check if we really hit the important shape.
          // TODO(cgissler, 21/07/2014): Think about how we can do this better
          // than using the distance.
          if (std::abs(info.t - distance) < constants::TEPSILON) {
            // Calculate the cos(thetaPrime) for the shape.
            float cosThetaPrime = glm::dot(shapePtr->getNormal(info.hitPoint),
                                           -newRay.direction());
            if (cosThetaPrime <= 0.0f) {
              continue;
            }
            // Calculate the phi.
            float phi =
                glm::orientedAngle(glm::vec3(intersectionInfo.normal),
                                   glm::vec3(newRay.direction()), tangent);
            //
            float brdfValue = m_BRDF->evaluateBRDF(
                intersectionInfo,         // Position on the surface.
                incomingRay.direction(),  // incoming direction.
                newRay.direction());      // outgoing direction.
            float invPDFValue = 1.0f / m_BRDF->getPDFOfX(glm::vec2(phi, theta));
            importantShapeColor +=
                (shapePtr->area() / (distance * distance * numSamples)) *
                cosThetaPrime * cosTheta * invPDFValue * brdfValue *
                info.materialPtr->getColor(info, newRay, scene);
          }
        }
      }
    }
  }

  // -------------------- RANDOM HEMISPHERE SAMPLING --------------------------
  // Color of the random hemisphere.
  Color hemisphereColor(0, 0, 0);

  if (hemisphereSamples != 0) {
    // Normal of the object at the position.
    glm::vec3 normal = glm::vec3(glm::normalize(intersectionInfo.normal));

    // Get the tangent.
    glm::vec3 tangent(0, 0, 1);

    glm::vec3 up(0, 1, 0);
    glm::vec3 diff = glm::abs(normal) - up;

    if (!solve::isZero(glm::dot(diff, diff))) {
      tangent = glm::cross(normal, up);
    }

    // Shoot sample rays into the hemisphere.
    for (size_t i = 0; i < hemisphereSamples; ++i) {
      // Get a sample on a circle around the hitpoint.
      // omega.x = phi
      // omega.y = theta
      glm::vec2 omega =
          m_BRDF->generateHemisphereSample(incomingRay, intersectionInfo, i);

      // Get the direction from phi and theta.
      glm::vec3 rotatedTangent =
          glm::rotate(tangent, static_cast<float>(omega.x), normal);
      // Get the cross between rotatedTangent and normal, so we can rotate the
      // rotatedTangent towards the normal.
      glm::vec3 crossTangent = glm::cross(rotatedTangent, normal);

      rotatedTangent =
          glm::rotate(normal, static_cast<float>(omega.y), crossTangent);

      glm::vec4 direction = glm::vec4(rotatedTangent, 0);

      // Get reflected color.
      Color lightColor(0, 0, 0);
      Ray newRay;
      newRay.setDirection(direction);
      newRay.setOrigin(intersectionInfo.hitPoint);
      newRay.rayInfo().depth = incomingRay.rayInfo().depth + 1;
      // TODO(bauschp, Wed Jul 30 15:01:24 CEST 2014): is this right?
      newRay.rayInfo().colorContribution =
          incomingRay.rayInfo().colorContribution * cos(omega.y);
      IntersectionInfo info = scene.traceRay(newRay);
      if (info.materialPtr) {
        // Don't use this sample if we sample important shapes seperately and
        // hit an important shape.
        // TODO(cgissler, 07/21/2014): Should we generate a new random sample?
        // What happens if all random samples hit an important object?
        if (sampleImportantShapes && info.hitImportantShape)
          continue;
        lightColor = info.materialPtr->getColor(info, newRay, scene);
      } else {
        lightColor = scene.backgroundColor(newRay);
      }
      float brdfValue =
          m_BRDF->evaluateBRDF(intersectionInfo,  // Position on the surface.
                               incomingRay.direction(),  // incoming direction.
                               newRay.direction());      // outgoing direction.
      float invPDFValue = 1.0f / m_BRDF->getPDFOfX(omega);  // outgoing.
      // Add the color to the return intensity.
      hemisphereColor += lightColor * cos(omega.y) * brdfValue * invPDFValue;
    }
    hemisphereColor *= (1.0f / hemisphereSamples);
  }

  // Combine and return the color.
  return m_Color * (hemisphereColor + importantShapeColor);
}
