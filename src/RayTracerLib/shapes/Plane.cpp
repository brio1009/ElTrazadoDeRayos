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

#include "./Plane.h"

#include <glm/glm.hpp>

#include <vector>

#include "./Constants.h"
#include "./Ray.h"
#include "./Solver.h"

using std::vector;

const char* Plane::name = "Plane";

// ___________________________________________________________________________
vector<REAL> Plane::intersect(const Ray& ray) const {
  // ax + by + cz + d = 0
  // a * (px + uxt) + b (py + uyt) + c (pz + uzt) + d = 0
  // apx + bpy + cpz + d + auxt + buyt + cuzt = 0;

  // Bring vector to unit space.
  Ray transRay = _inverseTransform * ray;
  const glm::vec4& transPos = transRay.origin();
  const glm::vec4& transDir = transRay.direction();

  REAL b = _nX * transPos[0] + _nY * transPos[1] + _nZ * transPos[2];
  REAL a = _nX * transDir[0] + _nY * transDir[1] + _nZ * transDir[2];

  // TODO(bauschp): put into own method and reuse.
  vector<REAL> out;
  solve::solveLinearEquation(&out, a, b);
  return out;
}

// ___________________________________________________________________________
glm::vec4 Plane::getNormalAt(const glm::vec4& p) const {
  return _transformation * glm::vec4(_nX, _nY, _nZ, 0);
}
