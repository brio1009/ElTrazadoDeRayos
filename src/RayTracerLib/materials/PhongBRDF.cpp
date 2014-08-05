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
#include "./PhongBRDF.h"

#include <algorithm>
#include "../Constants.h"

// _____________________________________________________________________________
glm::vec2 PhongBRDF::generateHemisphereSample(
      const Ray& incomingRay,
      const IntersectionInfo& intersectionInfo,
      const size_t& num) const {
  glm::vec2 out(
        rand()/static_cast<float>(RAND_MAX),   //NOLINT
        rand()/static_cast<float>(RAND_MAX));  //NOLINT
  out.x *= 2.0f * constants::PI;
  out.y = acos(1.0f - out.y);
  return out;
}

// _____________________________________________________________________________
float PhongBRDF::getPDFOfX(const glm::vec2& sample) const {
  return 0.5f / constants::PI;
}

// _____________________________________________________________________________
float PhongBRDF::evaluateBRDF(
        const glm::vec4& position,
        const glm::vec4& directionIn,
        const glm::vec4& directionOut) const {
  float kd = 0.7f;
  float ks = 0.3f;
  float shininess = 2.0f;
  // Diffuse Term
  float overallBRDFValue = kd / constants::PI;
  // Specular Term
  float dot = std::max(0.0f, glm::dot(-directionIn, directionOut));
  overallBRDFValue += ks * pow(dot, shininess);
  return overallBRDFValue;
}
