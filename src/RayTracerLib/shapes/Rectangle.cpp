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

#include "./Rectangle.h"

#include <glm/glm.hpp>

#include <ctime>
#include <vector>

#include "./Constants.h"
#include "./Ray.h"
#include "./Solver.h"

using std::vector;

const char* Rectangle::name = "Rectangle";

// ___________________________________________________________________________
vector<REAL> Rectangle::intersect(const Ray& ray) const {
  // Bring vector to unit space.
  Ray transRay = _inverseTransform * ray;
  const glm::vec4& transPos = transRay.origin();
  const glm::vec4& transDir = transRay.direction();

  REAL b = m_Normal.x * transPos.x
           + m_Normal.y * transPos.y
           + m_Normal.z * transPos.z;
  REAL a = m_Normal.x * transDir.x
           + m_Normal.y * transDir.y
           + m_Normal.z * transDir.z;

  vector<REAL> solutions;
  solve::solveLinearEquation(&solutions, a, b);

  // Loop over the solutions and test if they are valid.
  vector<REAL> out;
  for (size_t i = 0; i < solutions.size(); ++i) {
    glm::vec4 hitPoint = transPos
                         + transDir * static_cast<float>(solutions.at(i));
    if (hitPoint.x <= m_Extent.x && hitPoint.z <= m_Extent.y) {
      out.push_back(solutions.at(i));
    }
  }

  return out;
}

// ___________________________________________________________________________
glm::vec4 Rectangle::getNormalAt(const glm::vec4& p) const {
  return _transformation * glm::vec4(m_Normal, 0);
}
