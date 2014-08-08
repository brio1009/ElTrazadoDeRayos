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

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <omp.h>

#include <ctime>
#include <limits>
#include <vector>

#include "./cameras/Camera.h"
#include "./cameras/PerspectiveCamera.h"
#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Scene.h"
#include "./Ray.h"
#include "./Color.h"
#include "./shapes/Box.h"
#include "./shapes/CompoundShape.h"
#include "./shapes/Ellipsoid.h"
#include "./shapes/Plane.h"
#include "./shapes/Rectangle.h"
#include "./shapes/Shape.h"
#include "./materials/ColorMaterial.h"
#include "./materials/PhongMaterial.h"
#include "./materials/ShadowMaterial.h"
#include "./materials/DoubleMaterial.h"
#include "./materials/MonteCarloMaterial.h"
#include "./materials/GlassMaterial.h"
#include "./lights/PointLight.h"
#include "./lights/AreaLight.h"
#include "./lights/AreaShape.h"
#include "./factories/Factory.h"

using std::vector;


// _____________________________________________________________________________
void pathTraceGlobal(vector<Shape*>* shapes,
                     vector<Light*>* lights,
                     vector<Camera*>* cameras,
                     Scene* scenePtr) {
  // Bottom floor.
  Rectangle* bottom = new Rectangle(glm::vec3(0, 1, 0), glm::vec2(20, 20));
  bottom->setPosition(glm::vec4(0, -1.875, 0, 1));
  bottom->setClipBackplane(true);
  shapes->push_back(bottom);

  bottom->setMaterialPtr(new MonteCarloMaterial(Color(0.3, 0.9, 0.4)));

  CompoundShape* sha1 = new CompoundShape(
        new Ellipsoid(1.5, 99999, 1.5),
        new Box(4, 3.75, 4));
  sha1->setUseChildMaterials(false);
  sha1->setOperator(CompoundShape::Operator::intersectionOp);
  sha1->setMaterialPtr(new MonteCarloMaterial(Color(0.8, 0.8, 0.8)));
  scenePtr->addShape(sha1);

  Box* box = new Box(10, 2.5, 1);
  shapes->push_back(box);
  glm::mat4 trans(glm::translate(glm::mat4(1.0), glm::vec3(0, -0.625, -2)));
  box->setMaterialPtr(new MonteCarloMaterial(Color(0.9, 0.9, 0.9)));
  box->transform(trans);

  Ellipsoid* ball1 = new Ellipsoid(0.6125, 0.6125, 0.6125);
  ball1->setMaterialPtr(new MonteCarloMaterial(Color(0.9, 0.3, 0.5)));
  trans = glm::translate(glm::mat4(1.0), glm::vec3(-2.1125, -1.2625, 0));
  ball1->transform(trans);
  shapes->push_back(ball1);

  Ellipsoid* ball2 = new Ellipsoid(0.5, 0.5, 0.5);
  ball2->setMaterialPtr(new MonteCarloMaterial(Color(0.9, 0.9, 0.9)));
  trans = glm::translate(glm::mat4(1.0), glm::vec3(0, -1.375, 2));
  ball2->transform(trans);
  shapes->push_back(ball2);

  Ellipsoid* ball3 = new Ellipsoid(0.8333, 0.8333, 0.8333);
  ball3->setMaterialPtr(new MonteCarloMaterial(Color(0.9, 0.9, 0.9)));
  trans = glm::translate(glm::mat4(1.0), glm::vec3(0, -1.375, 2));
  ball3->transform(trans);
  shapes->push_back(ball3);

  float angle = glm::pi<float>() * 2.0f;
  trans = glm::rotate(glm::mat4(1.0),
      (angle / 20) * (-1), glm::vec3(0, 1, 0));
  trans = glm::translate(trans, glm::vec3(0, 0, 23));
  PerspectiveCamera* cam = new PerspectiveCamera(1280, 720,
                                                 glm::radians(85.0f));
  cam->transform(trans);
  cam->setUsePostProcessing(false);
  cameras->push_back(cam);
}
// _____________________________________________________________________________
void monteCarloCSG(vector<Shape*>* shapes,
                   vector<Light*>* lights,
                   vector<Camera*>* cameras) {
  // Main cube.
  Box* box = new Box(40, 40, 40);
  box->setMaterialPtr(new MonteCarloMaterial(Color(1.0f, 0.6f, 0.75f)));

  // Sphere.
  Ellipsoid* ellipsoid = new Ellipsoid(26, 26, 26);
  ellipsoid->setMaterialPtr(new MonteCarloMaterial(Color(0.1f, 0.1f, 0.9f)));

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
  threeTubes->setMaterialPtr(new MonteCarloMaterial(Color(0, 1, 0)));
  // threeTubes->setMaterialPtr(new GlassMaterial(RefractiveIndex::mirror));
  threeTubes->setUseChildMaterials(false);

  // Main compound shape.
  CompoundShape* mainObject = new CompoundShape(roundedBox, threeTubes);
  mainObject->setOperator(CompoundShape::Operator::minusOp);

  // shapes->push_back(mainObject);

  // Ground plane.
  Rectangle* plane0 = new Rectangle(glm::vec3(0, 1, 0), glm::vec2(400, 400));
  glm::mat4 trans = glm::rotate(glm::mat4(1.0),
                                glm::radians(45.0f),
                                glm::vec3(0, 1, 0));
  trans = glm::translate(trans, glm::vec3(0, -25, 0));
  plane0->transform(trans);
  plane0->setMaterialPtr(new MonteCarloMaterial(Color(1, 1, 1)));
  /*
  plane0->setMaterialPtr(new DoubleMaterial(new MonteCarloMaterial(),
                         new MonteCarloMaterial(Color(1, 1, 1)), 10, 10));
  */
  shapes->push_back(plane0);

  // Rotate 45degree around y axis.
  trans = glm::rotate(glm::mat4(1.0),
                      glm::radians(45.0f),
                      glm::vec3(0, 1, 0));

  // Now rotate downwards by 45 degree.
  trans = glm::rotate(trans, glm::radians(20.0f), glm::vec3(-1, 0, 0));

  PerspectiveCamera* cam = new PerspectiveCamera(1280, 720,
                                                 glm::radians(85.0f));
  cam->transform(glm::translate(trans, glm::vec3(0, 0, 100)));
  cam->setUsePostProcessing(false);
  cameras->push_back(cam);
}

// _____________________________________________________________________________
void cgCube(vector<Shape*>* shapes,
            vector<Light*>* lights,
            vector<Camera*>* cameras) {
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

  shapes->push_back(mainObject);

  // Ground plane.
  Plane* plane0 = new Plane(0, 1, 0);
  plane0->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, -40, 0)));
  plane0->setMaterialPtr(new DoubleMaterial(new ShadowMaterial(),
                                  new ShadowMaterial(Color(1, 1, 1)), 10, 10));
  shapes->push_back(plane0);

  // Create a shape with the factory.
  Shape* shpa = Factory<Shape>::Create("Ellipsoid");
  shpa->setFromString("RadiusX", "100.0");
  shapes->push_back(shpa);

  // Lights.
  int numLights = 1;
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
      lights->push_back(light);
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
      lights->push_back(light);
    }
  }
}

// _____________________________________________________________________________
void compoundTestScene(vector<Shape*>* shapes,
                       vector<Light*>* lights,
                       vector<Camera*>* cameras) {
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
  shapes->push_back(cmpdShape);

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
  shapes->push_back(cmpdShape2);

  // Ground plane.
  Plane* plane0 = new Plane(0, 1, 0);
  plane0->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, -30, 0)));
  plane0->setMaterialPtr(new DoubleMaterial(new ShadowMaterial(),
                                  new ShadowMaterial(Color(1, 1, 1)), 10, 10));
  shapes->push_back(plane0);

  // Lights.
  Light* light = new AreaLight(glm::vec4(0, 10, -39, 1), 3.0);
  light->setLightColor(Color(1, 1, 1));
  lights->push_back(light);
}

// _____________________________________________________________________________
void defaultScene(vector<Shape*>* shapes,
                  vector<Light*>* lights,
                  vector<Camera*>* cameras) {
  Ellipsoid* ell = new Ellipsoid(20, 10, 20);
  ell->transform(glm::translate(glm::mat4(1.0), glm::vec3(50, 20, 0)));
  shapes->push_back(ell);

  Ellipsoid* ell1 = new Ellipsoid(10, 30, 30);
  glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 31));
  trans = glm::rotate(trans, 3.141f/2, glm::vec3(0, 1, 0));
  ell1->transform(trans);
  // ell1->setMaterialPtr(new GlassMaterial(RefractiveIndex::mirror));
  ell1->setMaterialPtr(new DoubleMaterial(new ShadowMaterial(),
                                  new ShadowMaterial(Color(1, 1, 1)), 10, 10));
  shapes->push_back(ell1);

  Box* ell3 = new Box(20, 20, 20);
  ell3->transform(glm::translate(glm::mat4(1.0), glm::vec3(-30, -0, -50)));
  shapes->push_back(ell3);

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
  shapes->push_back(ball);

  // Ground plane.
  Plane* plane0 = new Plane(0, 1, 0);
  plane0->transform(glm::translate(glm::mat4(1.0), glm::vec3(0, -30, 0)));
  plane0->setMaterialPtr(new DoubleMaterial(new ShadowMaterial(),
                                  new ShadowMaterial(Color(1, 1, 1)), 10, 10));
  shapes->push_back(plane0);

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
      lights->push_back(light);
    }
  }
}

// _____________________________________________________________________________
void monteCarloScene(vector<Shape*>* shapes,
                     vector<Light*>* lights,
                     vector<Camera*>* cameras,
                     Scene* scenePtr) {
  // Create the walls.
  // Front wall.
  Rectangle* front = new Rectangle(glm::vec3(0, 0, -1), glm::vec2(10, 10));
  front->setPosition(glm::vec4(0, 0, 10, 1));
  front->setClipBackplane(true);
  shapes->push_back(front);

  // Back wall.
  Rectangle* back = new Rectangle(glm::vec3(0, 0, 1), glm::vec2(10, 10));
  back->setPosition(glm::vec4(0, 0, -10, 1));
  back->setClipBackplane(true);
  shapes->push_back(back);

  // Left wall.
  Rectangle* left = new Rectangle(glm::vec3(1, 0, 0), glm::vec2(10, 10));
  left->setPosition(glm::vec4(-10, 0, 0, 1));
  left->setClipBackplane(true);
  shapes->push_back(left);

  // Right wall.
  Rectangle* right = new Rectangle(glm::vec3(-1, 0, 0), glm::vec2(10, 10));
  right->setPosition(glm::vec4(10, 0, 0, 1));
  right->setClipBackplane(true);
  shapes->push_back(right);

  // Bottom floor.
  Rectangle* bottom = new Rectangle(glm::vec3(0, 1, 0), glm::vec2(10, 10));
  bottom->setPosition(glm::vec4(0, -10, 0, 1));
  bottom->setClipBackplane(true);
  shapes->push_back(bottom);

  // Top ceiling.
  Rectangle* top = new Rectangle(glm::vec3(0, -1, 0), glm::vec2(10, 10));
  top->setPosition(glm::vec4(0, 10, 0, 1));
  top->setClipBackplane(true);
  shapes->push_back(top);

  // Set material of the walls.
  front->setMaterialPtr(new MonteCarloMaterial(Color(1, 1, 1)));
  back->setMaterialPtr(new MonteCarloMaterial(Color(1, 1, 1)));
  left->setMaterialPtr(new MonteCarloMaterial(Color(1, 0, 0)));
  right->setMaterialPtr(new MonteCarloMaterial(Color(0, 1, 0)));
  bottom->setMaterialPtr(new MonteCarloMaterial(Color(1, 1, 1)));
  top->setMaterialPtr(new MonteCarloMaterial(Color(1, 1, 1)));

  // Top area light.
  Rectangle* rectangleLight =
                    new AreaShape<Rectangle>(glm::vec3(0, -1, 0),
                                             glm::vec2(2, 2));
  rectangleLight->transform(glm::translate(glm::mat4(1),
                            glm::vec3(0, 9.5, 0))
                            * rectangleLight->getTransformMatrix());
  rectangleLight->setMaterialPtr(new ColorMaterial(Color(1, 1, 1)));
  rectangleLight->setClipBackplane(true);
  scenePtr->addShape(rectangleLight);

  // Create a sphere in the middle.
  Ellipsoid* ball = new Ellipsoid(2, 2, 2);
  ball->setPosition(glm::vec4(-4, -8, -2, 1));
  ball->setMaterialPtr(new GlassMaterial(RefractiveIndex::glass));
  scenePtr->addShape(ball);

  /*
  Ellipsoid* ball2 = new Ellipsoid(2, 2, 2);
  shapes->push_back(ball2);
  ball2->setPosition(glm::vec4(4, -3, -4, 1));
  ball2->setMaterialPtr(new GlassMaterial(RefractiveIndex::mirror));
  */

  // Create the box in the room.
  Box* box = new Box(5, 5, 5);
  shapes->push_back(box);
  glm::mat4 trans(glm::translate(glm::mat4(1.0), glm::vec3(4, -7.5, -4)));
  trans = glm::rotate(trans, glm::radians(-25.0f), glm::vec3(0, 1, 0));
  box->setMaterialPtr(new MonteCarloMaterial(Color(1, 1, 1)));
  box->transform(trans);

  /*
  // Create the light.
  Light* light = new PointLight(glm::vec4(0, 8, 0, 1));
  light->setLightColor(Color(3, 3, 3));
  lights->push_back(light);
  */

  // Add the camera.
  size_t imgCount = 1;
  float angle = glm::pi<float>() * 2.0f;
  for (size_t i = 0; i < imgCount; ++i) {
    glm::mat4 trans = glm::rotate(glm::mat4(1.0),
        (angle / imgCount) * i, glm::vec3(0, 1, 0));
    // trans = glm::rotate(trans, glm::radians(20.0f), glm::vec3(-1, 0, 0));
    // trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0, 1, 0));
    PerspectiveCamera* cam = new PerspectiveCamera(1280, 720,
                                                   glm::radians(85.0f));
    cam->transform(glm::translate(trans, glm::vec3(0, 0, 23)));
    cam->setUsePostProcessing(false);
    cameras->push_back(cam);
  }
}

// _____________________________________________________________________________
void openHemisphereScene(vector<Camera*>* cameras,
                         Scene* scenePtr) {
  // A simple Scene that shows full Hemisphere rendering.
  // Bottom floor.
  Rectangle* bottom = new Rectangle(glm::vec3(0, 1, 0), glm::vec2(20, 10));
  bottom->setPosition(glm::vec4(0, -10, 0, 1));
  bottom->setClipBackplane(true);
  bottom->setMaterialPtr(new MonteCarloMaterial(Color(1, 1, 1)));
  scenePtr->addShape(bottom);

  // Left box.
  Box* box = new Box(5, 5, 5);
  glm::mat4 trans(glm::translate(glm::mat4(1.0), glm::vec3(-15, -7.5, -4)));
  trans = glm::rotate(trans, glm::radians(-25.0f), glm::vec3(0, 1, 0));
  box->setMaterialPtr(new MonteCarloMaterial(Color(1, 0.0, 0.0)));
  box->transform(trans);
  // scenePtr->addShape(box);
  // Right ball.
  Ellipsoid* ball = new Ellipsoid(2.5, 2.5, 2.5);
  trans = glm::mat4(glm::translate(glm::mat4(1.0), glm::vec3(15, -7.5, -4)));
  trans = glm::rotate(trans, glm::radians(-25.0f), glm::vec3(0, 1, 0));
  ball->setMaterialPtr(new MonteCarloMaterial(Color(0.0, 0.0, 0.9)));
  ball->transform(trans);
  // scenePtr->addShape(ball);

  // Add the camera.
  PerspectiveCamera* cam = new PerspectiveCamera(1280, 720,
                                                 glm::radians(85.0f));
  trans = glm::mat4(1.0);
  cam->transform(glm::translate(trans, glm::vec3(0, 0, 27)));
  cam->setUsePostProcessing(false);
  cameras->push_back(cam);
}

// _____________________________________________________________________________
Scene::Scene() {
  /*
  PROPERTYCLASS(CompoundShape);
  printf("map value: %s\n", typeid(*(PropertyManager::classProperties["CompoundShape"])).name());
  // testMap.emplace("asd", 1);
  */
  // monteCarloScene(&_shapes, &_lights, &m_Cameras, this);
  pathTraceGlobal(&_shapes, &_lights, &m_Cameras, this);
  // openHemisphereScene(&m_Cameras, this);
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
  for (auto it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
    delete *it;
}

// _____________________________________________________________________________
void Scene::addShape(Shape* shapePtr) {
  _shapes.push_back(shapePtr);
  /// Check if this is an important shape.
  if (ImportantShape* importantPtr = dynamic_cast<ImportantShape*>(shapePtr)) {
    m_ImportantShapes.push_back(importantPtr);
  }
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

// _____________________________________________________________________________
void Scene::render() const {
  render(1, 0);
}

void Scene::render(size_t chunks, size_t chunkNr) const {
  // Should never happen.
  if (chunks == 0)
    return;
  // Loop over all cameras and render.
  for (size_t i = 0; i < m_Cameras.size(); ++i) {
    Camera* const tmpCamera = m_Cameras.at(i);
    // calculate pixels.
    size_t amountPixel =
          tmpCamera->getImage().getWidth()
          * tmpCamera->getImage().getHeight();
    size_t startPixel = (amountPixel / chunks) * chunkNr;
    size_t endPixel =
          (chunks == (chunkNr + 1))
          ? amountPixel
          : startPixel + (amountPixel / chunks);
    double startTime = omp_get_wtime();
    tmpCamera->render(*this, startPixel, endPixel);
    double endTime = omp_get_wtime();
#ifdef WINDOWS
    printf("Image %03lu took %.2f sec to render.\n", i, endTime - startTime);
#else
    printf("Image %03zu took %.2f sec to render.\n", i, endTime - startTime);
#endif  // WINDOWS
  }
}
