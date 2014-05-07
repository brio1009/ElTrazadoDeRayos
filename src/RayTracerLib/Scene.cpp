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

#include <limits>
#include <vector>
#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Scene.h"
#include "./Shape.h"
#include "./Ray.h"
#include "./GreenMaterial.h"
#include "./Ellipsoid.h"

using std::vector;

// _____________________________________________________________________________
Scene::Scene() {
  Ellipsoid* ell = new Ellipsoid(20, 40, 40);
  _shapes.push_back(ell);
}

// _____________________________________________________________________________
IntersectionInfo Scene::traceRay(const Ray& ray) const {
  REAL smallestT = 0;
  Shape* shapePtr = nullptr;
  // TODO(cgissler, 05/07/2014): Add real normals and materials.
  // Loop over all objects.
  for (size_t i = 0; i < _shapes.size(); ++i) {
    // Test the object for a hit.
    vector<REAL> hits = _shapes.at(i)->intersect(ray);
    // Loop over the p and choose the smallest that is bigger 0.
    for (size_t j = 0; j < hits.size(); ++j) {
      if (hits.at(j) > 0 && hits.at(j) > smallestT) {
        smallestT = hits.at(j);
        shapePtr = _shapes[i];
      }
    }
  }
  if (shapePtr) {
    glm::vec4 position = ray.pos + (float)smallestT * ray.dir;
    return IntersectionInfo(position,
                            shapePtr->getNormalAt(position),
                            new GreenMaterial());
  }
  return IntersectionInfo();
}
