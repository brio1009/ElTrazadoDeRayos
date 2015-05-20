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

// GLM-define, because degree-methods are deprected.
#define GLM_FORCE_RADIANS

// RayTracerLib.
#include <Image.h>
#include <Ray.h>
#include <Scene.h>
#include <cameras/PerspectiveCamera.h>
#include <materials/Material.h>
#include <materials/ColorMaterial.h>
#include <materials/PhongMaterial.h>
#include <materials/DoubleMaterial.h>
#include <materials/GlassMaterial.h>
#include <materials/PhongBRDF.h>
#include <lights/PointLight.h>
#include <parser/SceneFileParser.h>
#include <shapes/Plane.h>
#include <shapes/Mesh.h>
#include <accelerationstructures/AABB.h>
// C Header.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <omp.h>
// C++ Header.
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

//
void renderTestScene(const char* fileName, size_t chunks, size_t chunkNr) {
  // Create the scene. This also adds all the objects.
  Scene scene;
  // Load the desired Scene.
  // SceneFileParser sceneParser;
  // sceneParser.parse("../../scenes/testScene.xml", &scene);
  std::vector<Material*> mats;
  mats.push_back(new ColorMaterial(Color(0.0f, 0.0f, 0.0f)));
  mats.push_back(new ColorMaterial(Color(0.0f, 0.0f, 1.0f)));
  mats.push_back(new DoubleMaterial(mats[0], mats[1], 1, 1));
  mats.push_back(new MonteCarloMaterial(Color(1.0f, 0.0f, 0.0f), std::make_shared<PhongBRDF>(0.0, 0.1, 10.0f)));
  mats.push_back(new GlassMaterial(RefractiveIndex::glass));
  mats.push_back(new ColorMaterial(Color(1.0f, 1.0f, 1.0f)));
  Plane* p = new Plane(0, 0, 1);
  p->transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10.5)));
  p->setMaterialPtr(mats[2]);
  scene.addShape(p);
  Mesh* m = new Mesh();
  // m->setMaterialPtr(new GlassMaterial(RefractiveIndex::glass));
  m->setMaterialPtr(mats[3]);
  m->loadObjFromFile("../../scenes/FinalBaseMesh.obj");
  glm::mat4 trans =
    glm::rotate(
        glm::mat4(1.0f),
        3.14150f * 0.25f,
        glm::vec3(0, 1.0f, 0));
  m->transform(glm::translate(trans, glm::vec3(0, -5.0f, 0.0f)));

  Rectangle* areaLight = new Rectangle(
      glm::vec3(0, 0, -1), glm::vec2(10, 10));
  areaLight->setMaterialPtr(mats[5]);
  areaLight->transform(glm::translate(glm::mat4(1.0f),
        glm::vec3(-5.0f, 0.0f, 21.0f)));
  scene.addShape(areaLight);
  scene.addShape(m);
  scene.lights().push_back(new PointLight(glm::vec4(0, 10, 20, 1)));
  scene.lights().back()->setLightColor(Color(1.0f, 1.0f, 1.0f));
  // PerspectiveCamera* cam = new PerspectiveCamera(1920, 1080,
  //                                                glm::radians(85.0f));
  PerspectiveCamera* cam = new PerspectiveCamera(400, 400,
                                                 glm::radians(85.0f));
  cam->setRegularSampleSize(25);
  scene.setBackgroundColor(Color(0, 0, 0));
  trans = glm::mat4(1.0);
  trans = glm::rotate(trans, 3.14150f * 0.25f, glm::vec3(0, 1.0f, 0));
  cam->transform(glm::translate(trans, glm::vec3(0.001, 10.00001, 19.99999)));
  cam->setUsePostProcessing(true);
  scene.cameras().push_back(cam);
  // Render all the images.
  scene.render(chunks, chunkNr);
  // Remove This as well.
  for (auto it = mats.begin(); it != mats.end(); ++it)
    delete *it;
  // Get the cameras and save the images.
  for (size_t i = 0; i < scene.cameras().size(); ++i) {
    // Save the image under different names.
    char buff[100];
#ifdef WINDOWS
    _snprintf_s(buff, sizeof(buff), "%s%03lu.bmp", fileName, i);
#else
    snprintf(buff, sizeof(buff), "%s%03zu.bmp", fileName, i);
#endif  // WINDOWS
    scene.cameras().at(i)->getImage().saveAsBMP(buff);
  }
}

// The main method.
int main(int argc, char** argv) {
  const char* fileName = "Img";
  // Print usage info.
  if (argc > 4) {
    printf("Usage: %s <optional: output> <optional: chunks chunknr> "
           "<optional: random seed>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  // Initialize the rand function.
  unsigned int seed = static_cast<unsigned int>(time(NULL));
  if (argc > 1) {
    fileName = argv[1];
  }
  size_t chunks = 1;
  size_t chunkNr = 0;
  if (argc > 3) {
    chunks = atol(argv[2]);
    chunkNr = atol(argv[3]);
  }
  if (argc == 5) {
    seed = static_cast<unsigned int>(atoi(argv[4]));
  }
  printf("Random seed used: %u\n\n", seed);
  srand(seed);

  // Render our test scene.
  renderTestScene(fileName, chunks, chunkNr);
}

