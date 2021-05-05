/*
The MIT License (MIT)

Copyright (c) 2015 CantTouchDis <bauschp@informatik.uni-freiburg.de>
Copyright (c) 2015 brio1009 <christoph1009@gmail.com>

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

#include "./VectorDataStructure.h"

#include <limits>
#include <vector>

#include "../IntersectionInfo.h"
#include "../Ray.h"
#include "../shapes/Shape.h"


namespace accelerationstructures {
// _____________________________________________________________________________
VectorDataStructure::VectorDataStructure(std::vector<Shape*>&& shapes)
    : m_Shapes(shapes) {}
// _____________________________________________________________________________
VectorDataStructure::VectorDataStructure() : m_Shapes() {}
// _____________________________________________________________________________
VectorDataStructure::~VectorDataStructure() {
  for (auto it = m_Shapes.begin(); it != m_Shapes.end(); ++it)
    delete *it;
}
// _____________________________________________________________________________
IntersectionInfo VectorDataStructure::traceRay(const Ray& ray) const {
  REAL smallestT = std::numeric_limits<REAL>::max();
  IntersectionInfo info;
  // Loop over all objects.
  for (size_t i = 0; i < m_Shapes.size(); ++i) {
    IntersectionInfo tmpInfo = m_Shapes.at(i)->getIntersectionInfo(ray);
    if (tmpInfo.materialPtr && tmpInfo.t < smallestT) {
      smallestT = tmpInfo.t;
      info = tmpInfo;
    }
  }
  return info;
}
// _____________________________________________________________________________
void VectorDataStructure::addShape(Shape* shape) {
  m_Shapes.push_back(shape);
}
size_t VectorDataStructure::size() const {
  return m_Shapes.size();
}
}  // namespace accelerationstructures
