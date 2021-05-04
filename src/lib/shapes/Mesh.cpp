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

#include "./Mesh.h"

#include <cassert>
#include <string>
#include <vector>
#include <limits>

#include "../Ray.h"
#include "../Solver.h"
#include "../parser/ObjParser.h"
#include "./Triangle.h"

using std::string;
using std::vector;

// _____________________________________________________________________________
Mesh::Mesh() {
}
// _____________________________________________________________________________
const std::vector<glm::vec3>& Mesh::getVertices() const {
  return m_Vertices;
}
// _____________________________________________________________________________
const std::vector<glm::vec3>& Mesh::getNormals() const {
  return m_Normals;
}

// _____________________________________________________________________________
void Mesh::loadObjFromFile(const string& filename) {
  parser::parseObjFile(
      filename,
      &m_Vertices,
      &m_Normals,
      &m_UVCoords);
}
IntersectionInfo Mesh::getIntersectionInfo(
    const Ray& ray,
    const REAL minimumT,
    const REAL maximumT) const {
  REAL smallestT = std::numeric_limits<REAL>::max();
  size_t smallestTriangleHit = m_Vertices.size();
  vector<size_t> hitTriangles;
  vector<float> b1;
  vector<float> b2;
  // Test the object for a hit.
  Ray transformedRay = _inverseTransform * ray;
  vector<REAL> hits = intersectTriangles(
        transformedRay,
        m_Vertices,
        0,
        m_Vertices.size(),
        &hitTriangles,
        &b1, &b2);
  bool hit(false);
  // Loop over
  for (size_t j = 0; j < hits.size(); ++j) {
    if (hits.at(j) >= minimumT
        && hits.at(j) < smallestT
        && hits.at(j) <= maximumT) {
      smallestT = hits.at(j);
      smallestTriangleHit = hitTriangles.at(j);
      hit = true;
    }
  }
  if (hit) {
    glm::vec4 position = ray.origin()
                         + static_cast<float>(smallestT) * ray.direction();
    // Return the Intersectioninfo.
    // TODO(bauschp): modify to use texture coordinates.
    float beta = b1[smallestTriangleHit];
    float gamma = b2[smallestTriangleHit];
    float alpha = 1 - beta - gamma;
    return IntersectionInfo(smallestT,
            position,
            glm::normalize(getTransformMatrix() * glm::vec4(
              ((alpha * m_Normals[smallestTriangleHit])
              + (beta * m_Normals[smallestTriangleHit + 1])
              + (gamma * m_Normals[smallestTriangleHit + 2])), 0.0f)),
            getMaterialPtr(),
            glm::vec2(0.0f));
  }
  // Else.
  return IntersectionInfo();
}

// _____________________________________________________________________________
glm::vec4 Mesh::getNormalAt(const glm::vec4& p) const {
  return p;
}

// _____________________________________________________________________________
vector<REAL> Mesh::intersect(const Ray& ray) const {
  // Should never be called.
  return vector<REAL>();
}
