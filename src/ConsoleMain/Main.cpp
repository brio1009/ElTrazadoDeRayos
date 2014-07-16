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
#include <Ellipsoid.h>
#include <Image.h>
#include <Material.h>
#include <OrthogonalCamera.h>
#include <PerspectiveCamera.h>
#include <Plane.h>
#include <Ray.h>
#include <Scene.h>
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
void renderTestScene(const char* fileName) {
  // Create the scene. This also adds all the objects.
  Scene scene;

  // Render all the images.
  scene.render();

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
  if (argc > 3) {
    printf("Usage: %s <optional: output> <optional: random seed>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  // Initialize the rand function.
  unsigned int seed = static_cast<unsigned int>(time(NULL));
  if (argc > 1) {
    fileName = argv[1];
  }
  if (argc == 3) {
    seed = static_cast<unsigned int>(atoi(argv[2]));
  }
  printf("Random seed used: %u\n\n", seed);
  srand(seed);

  // Render our test scene.
  renderTestScene(fileName);
}

