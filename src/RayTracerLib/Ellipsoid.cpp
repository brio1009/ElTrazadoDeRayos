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

#include "./Ellipsoid.h"
#include "./Ray.h"
#include <glm/glm.hpp>

#include <vector>

using std::vector;

// _____________________________________________________________________________
vector<double> Ellipsoid::intersect(const Ray& ray) const {
  // (px^2 / rX) + (2 * px * ux * t) / rX + (ux^2 + t^2) / rX
  // + (py^2 / rY) + (2 * py * uy * t) / rY + (uy^2 + t^2) / rY
  // + (pz^2 / rZ) + (2 * pz * uz * t) / rZ + (uz^2 + t^2) / rZ
  // - 1

  // Bring vector to unit space.
  glm::vec4 transPos = ray.pos * glm::inverse(_transformation);
  glm::vec4 transDir = ray.dir * glm::inverse(_transformation);

  // TODO(allofus): Think about outsourcing.
  double invRX = isZero(rX) ? 0.0 : 1.0 / rX;
  double invRY = isZero(rY) ? 0.0 : 1.0 / rY;
  double invRZ = isZero(rZ) ? 0.0 : 1.0 / rZ;

  // Create variables to use the solver helper.
  double c = (transPos[0] * transPos[0]) * invRX
           + (transPos[1] * transPos[1]) * invRY
           + (transPos[2] * transPos[2]) * invRZ
           - 1.0;
  double b = (2 * transPos[0] * transDir[0]) * invRX
           + (2 * transPos[1] * transDir[1]) * invRY
           + (2 * transPos[2] * transDir[2]) * invRZ;
  double a = transDir[0] * invRX
           + transDir[1] * invRY
           + transDir[2] * invRZ;
  solve::Result_t res;
  solve::solveQuadraticEquation(&res, a, b, c);
  vector<double> out;
  for (int i = 0; i < res.numResults; ++i) {
    out.push_back(res.roots[i]);
  }
  return out;
}
// _____________________________________________________________________________
void Ellipsoid::getAppearenceAt(const glm::vec4& p) {
  
}
