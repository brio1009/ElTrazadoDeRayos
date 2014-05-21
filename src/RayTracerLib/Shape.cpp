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

#include "./Shape.h"

#include <glm/glm.hpp>

#include <cstdlib>
#include <limits>
#include <vector>

#include "./Constants.h"
#include "./Color.h"
#include "./IntersectionInfo.h"
#include "./Ray.h"
#include "./ShadowMaterial.h"

using std::vector;

// _____________________________________________________________________________
Shape::Shape() {
  // Construct a random color.
  Color tmpColor;
  // TODO(allofus, Sun May 11 14:12:21 CEST 2014): change to threadsafe alt.
  tmpColor.setR((rand() % 255) / 255.0f);  //NOLINT
  tmpColor.setG((rand() % 255) / 255.0f);  //NOLINT
  tmpColor.setB((rand() % 255) / 255.0f);  //NOLINT
  tmpColor.setA(1.0f);
  _materialPtr = new ShadowMaterial(tmpColor);
}

// _____________________________________________________________________________
Shape::~Shape() {
  if (_materialPtr) {
    delete _materialPtr;
  }
}

// _____________________________________________________________________________
IntersectionInfo Shape::getIntersectionInfo(const Ray& ray,
    const REAL minimumT,
    const REAL maximumT) const {
  REAL smallestT = std::numeric_limits<REAL>::max();
  // Test the object for a hit.
  vector<REAL> hits = intersect(ray);
  bool hit(false);
  // Loop over
  for (size_t j = 0; j < hits.size(); ++j) {
    if (hits.at(j) >= minimumT
        && hits.at(j) < smallestT
        && hits.at(j) <= maximumT) {
      smallestT = hits.at(j);
      hit = true;
    }
  }
  if (hit) {
    glm::vec4 position = ray.origin()
                         + static_cast<float>(smallestT) * ray.direction();
    // Return the Intersectioninfo.
    return IntersectionInfo(smallestT,
                            position,
                            getNormalAt(position),
                            getMaterialPtr(),
                            getTextureCoord(position));
  }
  // Else.
  return IntersectionInfo();
}

// _____________________________________________________________________________
glm::vec2 Shape::getTextureCoord(const glm::vec4& p) const {
  glm::vec4 trans = _inverseTransform * p;
  return (glm::vec2(trans.x, trans.z));
}
