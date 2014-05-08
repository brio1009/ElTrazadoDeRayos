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

#include <glm/glm.hpp>

#include <cstdio>
#include <vector>

#include "./Constants.h"
#include "./Ray.h"
#include "./Solver.h"
using std::vector;

Ellipsoid::Ellipsoid(REAL x, REAL y, REAL z) : _rX(x), _rY(y), _rZ(z) {
  _transformation = glm::mat4();
}

// _____________________________________________________________________________
vector<REAL> Ellipsoid::intersect(const Ray& ray) const {
  // (px^2 / _rX^2) + (2 * px * ux * t) / _rX^2 + (ux^2 + t^2) / _rX^2
  // + (py^2 / _rY^2) + (2 * py * uy * t) / _rY^2 + (uy^2 + t^2) / _rY^2
  // + (pz^2 / _rZ^2) + (2 * pz * uz * t) / _rZ^2 + (uz^2 + t^2) / _rZ^2
  // - 1

  // Bring vector to unit space.
  glm::vec4 transPos = _transformation * ray.pos;
  glm::vec4 transDir = _transformation * ray.dir;
 // printf("RAY:(%.2f,%.2f,%.2f)(%.2f,%.2f,%.2f)", transPos.x, transPos.y, transPos.z, transDir.x, transDir.y, transDir.z);
  //printf("%.2f, ", ray.pos[3]);
  // TODO(allofus): Think about outsourcing.
  REAL invRX = solve::isZero(_rX) ? 0.0 : 1.0 / _rX;
  REAL invRY = solve::isZero(_rY) ? 0.0 : 1.0 / _rY;
  REAL invRZ = solve::isZero(_rZ) ? 0.0 : 1.0 / _rZ;
  invRX *= invRX;
  invRY *= invRY;
  invRZ *= invRZ;
  // Create variables to use the solver helper.
  REAL c = (transPos[0] * transPos[0]) * invRX
           + (transPos[1] * transPos[1]) * invRY
           + (transPos[2] * transPos[2]) * invRZ
           - 1.0;
  REAL b = (2 * transPos[0] * transDir[0]) * invRX
           + (2 * transPos[1] * transDir[1]) * invRY
           + (2 * transPos[2] * transDir[2]) * invRZ;
  REAL a = transDir[0] * transDir[0] * invRX
           + transDir[1] * transDir[1] * invRY
           + transDir[2] * transDir[2] * invRZ;
  solve::Result_t res;
  solve::solveQuadraticEquation(&res, a, b, c);
  vector<REAL> out;
  for (int i = 0; i < res.numResults; ++i) {
    out.push_back(res.roots[i]);
  }
  return out;
}
// _____________________________________________________________________________
glm::vec4 Ellipsoid::getNormalAt(const glm::vec4& p) const {
  glm::vec4 trans = _transformation * p;
  return glm::vec4(
      (2 * trans.x) / (_rX * _rX),
      (2 * trans.y) / (_rY * _rY),
      (2 * trans.z) / (_rZ * _rZ),
      0);
}
