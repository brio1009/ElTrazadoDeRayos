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
#include "./Box.h"
#include "./Ellipsoid.h"
#include "./Plane.h"
#include "./PhongMaterial.h"
#include "./GlassMaterial.h"

using std::vector;

// _____________________________________________________________________________
Scene::Scene() {
  Ellipsoid* ell = new Ellipsoid(20, 10, 20);
  ell->transform(glm::translate(glm::mat4(1.0), glm::vec3(50, 20, 0)));
  _shapes.push_back(ell);
  Ellipsoid* ell1 = new Ellipsoid(10, 30, 10);
  ell1->transform(glm::translate(glm::mat4(1.0), glm::vec3(-30, -0, 0)));
  ell1->setMaterialPtr(new GlassMaterial(RefractiveIndex::glass));
  //_shapes.push_back(ell1);
  Box* ell3 = new Box(10, 30, 30);
  ell3->transform(glm::translate(glm::mat4(1.0), glm::vec3(-50, -0, 20)));
  _shapes.push_back(ell3);
//  Ellipsoid* ell2 = new Ellipsoid(5, 5, 5);
//  _shapes.push_back(ell2);
  Plane* plane0 = new Plane(0, 1, 0);
  plane0->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, -30, 0)));
  _shapes.push_back(plane0);
}

// _____________________________________________________________________________
Scene::~Scene() {
  // TODO(allofus, Sun May 11 15:33:16 CEST 2014): Think of how to clean up here
  for (auto it = _shapes.begin(); it != _shapes.end(); ++it)
    delete *it;
}

// _____________________________________________________________________________
IntersectionInfo Scene::traceRay(const Ray& ray) const {
  REAL smallestT = std::numeric_limits<REAL>::max();
  Shape* shapePtr = nullptr;
  // TODO(cgissler, 05/07/2014): Add real normals and materials.
  // Loop over all objects.
  for (size_t i = 0; i < _shapes.size(); ++i) {
    // Test the object for a hit.
    vector<REAL> hits = _shapes.at(i)->intersect(ray);
    // Loop over the p and choose the smallest that is bigger 0.
    for (size_t j = 0; j < hits.size(); ++j) {
      // TODO(all, 05/17/2014): Find the right epsilon.
      if (hits.at(j) > 10.0f * constants::EPSILON && hits.at(j) < smallestT) {
        smallestT = hits.at(j);
        shapePtr = _shapes[i];
      }
    }
  }
  if (shapePtr) {
    glm::vec4 position = ray.origin()
                         + static_cast<float>(smallestT) * ray.direction();
    // Return the Intersectioninfo.
    return IntersectionInfo(position,
                            shapePtr->getNormalAt(position),
                            shapePtr->getMaterialPtr());
  }
  return IntersectionInfo();
}
