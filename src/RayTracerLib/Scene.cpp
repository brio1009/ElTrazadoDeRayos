/*
The MIT License (MIT)

Copyright (c) 2014 CantTouchDis
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

#define GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>
#include <ctime>
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
#include "./ShadowMaterial.h"
#include "./CheckerboardMaterial.h"
#include "./GlassMaterial.h"
#include "./CompoundShape.h"
#include "./PointLight.h"

using std::vector;

void Scene::compoundTestScene() {
  // Compound shape.
  Box* box = new Box(20, 20, 20);
  glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(-30, -20, -15));
  box->transform(trans);

  Ellipsoid* ellipsoid = new Ellipsoid(10, 10, 10);
  // trans = glm::translate(glm::mat4(1.0), glm::vec3(10, 10, 10));
  trans = glm::translate(glm::mat4(1.0), glm::vec3(-20, -10, -5));
  ellipsoid->transform(trans);

  CompoundShape* cmpdShape = new CompoundShape();
  cmpdShape->setLeftShapePtr(box);
  cmpdShape->setRightShapePtr(ellipsoid);
  trans = glm::translate(glm::mat4(1.0), glm::vec3(-30, -20, -15));
  cmpdShape->transform(trans);
  _shapes.push_back(cmpdShape);

  // Ground plane.
  Plane* plane0 = new Plane(0, 1, 0);
  plane0->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, -30, 0)));
  plane0->setMaterialPtr(new CheckerboardMaterial(new ShadowMaterial(),
                                  new ShadowMaterial(Color(1, 1, 1)), 10, 10));
  _shapes.push_back(plane0);

  // Lights.
  int numLights = 1;
  int lightsPerDimension = static_cast<int>(sqrt(numLights));
  for (int x = 0; x < lightsPerDimension; ++x) {
    for (int z = 0; z < lightsPerDimension; ++z) {
      Light* light = new PointLight(glm::vec4((-lightsPerDimension  + static_cast<float>(x)) * 3, 10, -39 + static_cast<float>(z) * 3, 1));
      light->setLightColor(Color(1.0f / numLights, 1.0f / numLights, 1.0f / numLights, 1.0f / numLights));
      _lights.push_back(light); 
    }
  }
}

// TODO(allofus, Wed May 21 17:12:00 CEST 2014): Remove if we have a scene load.
void Scene::defaultScene() {
  Ellipsoid* ell = new Ellipsoid(20, 10, 20);
  ell->transform(glm::translate(glm::mat4(1.0), glm::vec3(50, 20, 0)));
  _shapes.push_back(ell);
  Ellipsoid* ell1 = new Ellipsoid(10, 30, 30);
  glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 31));
  trans = glm::rotate(trans, 3.141f/2, glm::vec3(0, 1, 0));
  ell1->transform(trans);
  ell1->setMaterialPtr(new GlassMaterial(RefractiveIndex::mirror));
  _shapes.push_back(ell1);
  Box* ell3 = new Box(20, 20, 20);
  ell3->transform(glm::translate(glm::mat4(1.0), glm::vec3(-30, -0, -50)));
  _shapes.push_back(ell3);
  // ell3->setMaterialPtr(new GlassMaterial(RefractiveIndex::glass));
//  Ellipsoid* ell2 = new Ellipsoid(5, 5, 5);
//  _shapes.push_back(ell2);
  // Compound test
  Box* b1 = new Box(10, 10, 10);
  Ellipsoid* e1 = new Ellipsoid(5, 20, 5);

  // Glass ball.
  Ellipsoid* ball = new Ellipsoid(10, 10, 10);
  trans = glm::translate(glm::mat4(1.0), glm::vec3(0, -20, -11));
  ball->transform(trans);
  ball->setMaterialPtr(new GlassMaterial(RefractiveIndex::glass));
  _shapes.push_back(ball);

  /*
  CompoundShape* comp1 = new CompoundShape();
  comp1->setLeftShapePtr(e1);
  comp1->setRightShapePtr(b1);
  trans = glm::translate(glm::mat4(1.0), glm::vec3(0, 10, -30));
  comp1->transform(trans);
  _shapes.push_back(comp1);
  */

  // Ground plane.
  Plane* plane0 = new Plane(0, 1, 0);
  plane0->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, -30, 0)));
  plane0->setMaterialPtr(new CheckerboardMaterial(new ShadowMaterial(),
                                  new ShadowMaterial(Color(1, 1, 1)), 10, 10));
  _shapes.push_back(plane0);

  // Generate Lights for the scene.
  // Light 1
  // TODO(bauschp): fix mem leaks.
  // Light* light = new PointLight(glm::vec4(0, 0, -39, 1));
  // light->setLightColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
  // _lights.push_back(light);
  int numLights = 16;
  int lightsPerDimension = static_cast<int>(sqrt(numLights));
  for (int x = 0; x < lightsPerDimension; ++x) {
    for (int z = 0; z < lightsPerDimension; ++z) {
      Light* light = new PointLight(glm::vec4((-lightsPerDimension  + static_cast<float>(x)) * 3, 10, -39 + static_cast<float>(z) * 3, 1));
      light->setLightColor(Color(1.0f / numLights, 1.0f / numLights, 1.0f / numLights, 1.0f / numLights));
      _lights.push_back(light); 
    }
  }
  // Light 2
  // light = new PointLight(glm::vec4(0, 20, 0, 1));
  // light->setLightColor(Color(0.0f, 1.0f, 0.0f, 1.0f));
  // _lights.push_back(light);
}
// _____________________________________________________________________________
Scene::Scene() {
  compoundTestScene();
}

// _____________________________________________________________________________
Scene::~Scene() {
  // TODO(allofus, Sun May 11 15:33:16 CEST 2014): Think of how to clean up here
  for (auto it = _shapes.begin(); it != _shapes.end(); ++it)
    delete *it;
  for (auto it = _lights.begin(); it != _lights.end(); ++it)
    delete *it;
}

// _____________________________________________________________________________
IntersectionInfo Scene::traceRay(const Ray& ray) const {
  REAL smallestT = std::numeric_limits<REAL>::max();
  IntersectionInfo info;
  // Loop over all objects.
  for (size_t i = 0; i < _shapes.size(); ++i) {
    IntersectionInfo tmpInfo = _shapes.at(i)->getIntersectionInfo(ray);
    if (tmpInfo.materialPtr && tmpInfo.t < smallestT) {
      smallestT = tmpInfo.t;
      info = tmpInfo;
    }
  }
  return info;
}
