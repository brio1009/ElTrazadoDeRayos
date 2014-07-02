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
#include "./Box.h"
#include "./Ellipsoid.h"
#include "./Plane.h"
#include "./materials/ColorMaterial.h"
#include "./materials/PhongMaterial.h"
#include "./materials/ShadowMaterial.h"
#include "./materials/DoubleMaterial.h"
#include "./materials/MonteCarloMaterial.h"
#include "./materials/GlassMaterial.h"
#include "./CompoundShape.h"
#include "./lights/PointLight.h"
#include "./lights/AreaLight.h"
#include "./factories/Factory.h"

using std::vector;

void Scene::cgCube() {
  // Main cube.
  Box* box = new Box(40, 40, 40);
  box->setMaterialPtr(new ShadowMaterial(Color(1.0f, 0.6f, 0.75f)));

  // Sphere.
  Ellipsoid* ellipsoid = new Ellipsoid(26, 26, 26);
  ellipsoid->setMaterialPtr(new ShadowMaterial(Color(0.1f, 0.1f, 0.9f)));

  // Compound Shape of them.
  CompoundShape* roundedBox = new CompoundShape(box, ellipsoid);
  roundedBox->setOperator(CompoundShape::Operator::intersectionOp);

  // Two tubes.
  Ellipsoid* tube1 = new Ellipsoid(10000, 12, 12);
  Ellipsoid* tube2 = new Ellipsoid(12, 10000, 12);
  // Compound shape of them.
  CompoundShape* twoTubes = new CompoundShape(tube1, tube2);
  twoTubes->setOperator(CompoundShape::Operator::unionOp);
  // Third tube.
  Ellipsoid* tube3 = new Ellipsoid(12, 12, 10000);
  // Compound shape of them.
  CompoundShape* threeTubes = new CompoundShape(twoTubes, tube3);
  threeTubes->setOperator(CompoundShape::Operator::unionOp);
  threeTubes->setMaterialPtr(new ShadowMaterial(Color(0, 1, 0)));
  // threeTubes->setMaterialPtr(new GlassMaterial(RefractiveIndex::mirror));
  threeTubes->setUseChildMaterials(false);

  // Main compound shape.
  CompoundShape* mainObject = new CompoundShape(roundedBox, threeTubes);
  mainObject->setOperator(CompoundShape::Operator::minusOp);

  _shapes.push_back(mainObject);

  // Ground plane.
  Plane* plane0 = new Plane(0, 1, 0);
  plane0->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, -40, 0)));
  plane0->setMaterialPtr(new DoubleMaterial(new ShadowMaterial(),
                                  new ShadowMaterial(Color(1, 1, 1)), 10, 10));
  _shapes.push_back(plane0);

  // Lights.
  int numLights = 2;
  int lightsPerDimension = static_cast<int>(sqrt(numLights / 2.0));
  for (int x = 0; x < lightsPerDimension; ++x) {
    for (int z = 0; z < lightsPerDimension; ++z) {
      Light* light = new PointLight(
            glm::vec4((-lightsPerDimension + static_cast<float>(x)) * 3,
            20,
            50 + static_cast<float>(z) * 3,
            1));
      light->setLightColor(
            Color(
                  1.0f / numLights,
                  1.0f / numLights,
                  1.0f / numLights,
                  1.0f / numLights));
      _lights.push_back(light);
    }
  }

  for (int x = 0; x < lightsPerDimension; ++x) {
    for (int z = 0; z < lightsPerDimension; ++z) {
      Light* light = new PointLight(
            glm::vec4((static_cast<float>(x)),
            0,
            static_cast<float>(z),
            1));
      light->setLightColor(
            Color(
                  1.0f / numLights,
                  1.0f / numLights,
                  1.0f / numLights,
                  1.0f / numLights));
      _lights.push_back(light);
    }
  }
}

void Scene::compoundTestScene() {
  // Compound shape 1.
  Box* box = new Box(20, 20, 20);
  glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(-30, -20, -15));
  // box->transform(trans);

  Ellipsoid* ellipsoid = new Ellipsoid(15, 15, 15);
  trans = glm::translate(glm::mat4(1.0), glm::vec3(10, 10, 10));
  // trans = glm::translate(glm::mat4(1.0), glm::vec3(-20, -10, -5));
  ellipsoid->transform(trans);

  CompoundShape* cmpdShape = new CompoundShape();
  cmpdShape->setLeftShapePtr(box);
  cmpdShape->setRightShapePtr(ellipsoid);
  trans = glm::translate(glm::mat4(1.0), glm::vec3(-30, -20, -15));
  cmpdShape->transform(trans);
  _shapes.push_back(cmpdShape);

  // Compound shape 2.
  Box* box2 = new Box(20, 20, 20);
  // trans = glm::translate(glm::mat4(1.0), glm::vec3(-30, -20, -15));
  // box->transform(trans);

  Ellipsoid* ellipsoid2 = new Ellipsoid(15, 15, 15);
  trans = glm::translate(glm::mat4(1.0), glm::vec3(10, 10, 10));
  // trans = glm::translate(glm::mat4(1.0), glm::vec3(-20, -10, -5));
  ellipsoid2->transform(trans);

  CompoundShape* cmpdShape2 = new CompoundShape();
  cmpdShape2->setOperator(CompoundShape::Operator::intersectionOp);
  cmpdShape2->setLeftShapePtr(box2);
  cmpdShape2->setRightShapePtr(ellipsoid2);
  trans = glm::translate(glm::mat4(1.0), glm::vec3(-15, -5, 0));
  cmpdShape2->transform(trans);
  _shapes.push_back(cmpdShape2);

  // Ground plane.
  Plane* plane0 = new Plane(0, 1, 0);
  plane0->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, -30, 0)));
  plane0->setMaterialPtr(new DoubleMaterial(new ShadowMaterial(),
                                  new ShadowMaterial(Color(1, 1, 1)), 10, 10));
  _shapes.push_back(plane0);

  // Lights.
  Light* light = new AreaLight(glm::vec4(0, 10, -39, 1), 3.0);
  light->setLightColor(Color(1, 1, 1));
  _lights.push_back(light);
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
  // ell1->setMaterialPtr(new GlassMaterial(RefractiveIndex::mirror));
  ell1->setMaterialPtr(new DoubleMaterial(new ShadowMaterial(),
                                  new ShadowMaterial(Color(1, 1, 1)), 10, 10));
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
  // ball->setMaterialPtr(new GlassMaterial(RefractiveIndex::glass));
  ball->setMaterialPtr(new DoubleMaterial(new ShadowMaterial(),
                                  new ShadowMaterial(), 0.1, 0.1));
  _shapes.push_back(ball);

  // Ground plane.
  Plane* plane0 = new Plane(0, 1, 0);
  plane0->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, -30, 0)));
  plane0->setMaterialPtr(new DoubleMaterial(new ShadowMaterial(),
                                  new ShadowMaterial(Color(1, 1, 1)), 10, 10));
  _shapes.push_back(plane0);

  // Generate Lights for the scene.
  int numLights = 16;
  int lightsPerDimension = static_cast<int>(sqrt(numLights));
  for (int x = 0; x < lightsPerDimension; ++x) {
    for (int z = 0; z < lightsPerDimension; ++z) {
      Light* light = new PointLight(
            glm::vec4((-lightsPerDimension + static_cast<float>(x)) * 3,
            10,
            -39 + static_cast<float>(z) * 3,
            1));
      light->setLightColor(
            Color(
                  1.0f / numLights,
                  1.0f / numLights,
                  1.0f / numLights,
                  1.0f / numLights));
      _lights.push_back(light);
    }
  }
}

// _____________________________________________________________________________
void monteCarloScene(vector<Shape*>* shapes, vector<Light*>* lights) {
  Plane* back = new Plane(0, 0, 1);
  back->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -10)));
  shapes->push_back(back);

  Plane* left = new Plane(1, 0, 0);
  left->transform(glm::translate(glm::mat4(1.0), glm::vec3(-10, 0, 0)));
  shapes->push_back(left);

  Plane* right = new Plane(-1, 0, 0);
  right->transform(glm::translate(glm::mat4(1.0), glm::vec3(10, 0, 0)));
  shapes->push_back(right);

  Plane* bottom = new Plane(0, 1, 0);
  bottom->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, -10, 0)));
  shapes->push_back(bottom);

  Plane* top = new Plane(0, -1, 0);
  top->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, 10, 0)));
  shapes->push_back(top);

  Plane* front = new Plane(0, 0, -1);
  front->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 26)));
  shapes->push_back(front);
  // set color of the walls.
  back->setMaterialPtr(new MonteCarloMaterial(Color(1, 1, 1)));
  front->setMaterialPtr(new MonteCarloMaterial(Color(1, 1, 1)));
  left->setMaterialPtr(new MonteCarloMaterial(Color(1, 0, 0)));
  right->setMaterialPtr(new MonteCarloMaterial(Color(0, 1, 0)));
  bottom->setMaterialPtr(new MonteCarloMaterial(Color(1, 1, 1)));
  top->setMaterialPtr(new ColorMaterial(Color(1, 1, 1)));


  Ellipsoid* ball = new Ellipsoid(3, 1, 3);
  shapes->push_back(ball);
  ball->setPosition(glm::vec4(-4, -7, 0, 1));
  ball->setMaterialPtr(new GlassMaterial(RefractiveIndex::glass));

  Box* box = new Box(5, 5, 5);
  shapes->push_back(box);
  glm::mat4 trans(glm::translate(glm::mat4(1.0), glm::vec3(4, -7.5, -2)));
  trans = glm::rotate(trans, glm::radians(-25.0f), glm::vec3(0, 1, 0));
  box->setMaterialPtr(new MonteCarloMaterial(Color(1, 1, 1)));
  box->transform(trans);
/*
  Light* light = new PointLight(glm::vec4(0, 8, 0, 1));
  light->setLightColor(Color(3, 3, 3));
  lights->push_back(light);
*/
}

// _____________________________________________________________________________
Scene::Scene() {
  /*
  PROPERTYCLASS(CompoundShape);
  printf("map value: %s\n", typeid(*(PropertyManager::classProperties["CompoundShape"])).name());
  // testMap.emplace("asd", 1);
  */
  monteCarloScene(&_shapes, &_lights);
  // defaultScene();
  // cgCube();
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
