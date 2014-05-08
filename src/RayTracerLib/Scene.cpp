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

#include <glm/gtc/matrix_transform.hpp>
#include <limits>
#include <vector>
#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Scene.h"
#include "./Shape.h"
#include "./Ray.h"
#include "./Color.h"
#include "./ColorMaterial.h"
#include "./Ellipsoid.h"
#include "./Plane.h"
#include "./PhongMaterial.h"

using std::vector;

// _____________________________________________________________________________
Scene::Scene() {
  Ellipsoid* ell = new Ellipsoid(20, 10, 20);
  ell->transform(glm::translate(glm::mat4(1.0), glm::vec3(50, 10, 0)));
  _shapes.push_back(ell);
  Ellipsoid* ell1 = new Ellipsoid(10, 30, 10);
  ell1->transform(glm::translate(glm::mat4(1.0), glm::vec3(-30, -5, 0)));
  _shapes.push_back(ell1);
}

// _____________________________________________________________________________
IntersectionInfo Scene::traceRay(const Ray& ray) const {
  REAL smallestT = std::numeric_limits<REAL>::max();
  Shape* shapePtr = nullptr;
  size_t objectIndex = 0;
  // TODO(cgissler, 05/07/2014): Add real normals and materials.
  // Loop over all objects.
  for (size_t i = 0; i < _shapes.size(); ++i) {
    // Test the object for a hit.
    vector<REAL> hits = _shapes.at(i)->intersect(ray);
    // Loop over the p and choose the smallest that is bigger 0.
    for (size_t j = 0; j < hits.size(); ++j) {
      if (hits.at(j) > 0 && hits.at(j) < smallestT) {
        smallestT = hits.at(j);
        shapePtr = _shapes[i];
        objectIndex = i;
      }
    }
  }
  if (shapePtr) {
    glm::vec4 position = ray.pos + (float)smallestT * ray.dir;
    if (objectIndex == 0) {
      return IntersectionInfo(position,
                              shapePtr->getNormalAt(position),
                              new PhongMaterial(Color(0, 0, 255, 255)));
    } else {
      return IntersectionInfo(position,
                              shapePtr->getNormalAt(position),
                              new PhongMaterial(Color(255, 0, 0, 255)));
    }
  }
  return IntersectionInfo();
}
