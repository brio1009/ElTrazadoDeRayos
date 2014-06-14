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
#include <glm/gtc/constants.hpp>
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
void renderTestScene() {
  Scene scene;
  // REMEMBER VIM: use :r !date to insert current time.
  // TODO(allofus, Wed May  7 21:13:19 CEST 2014): to scale a image transform
  // the thrid param to match smaller change.
  // e.g. when converting from 80x80 (first trace) to  512x512 divide 80 by 512
  // OrthogonalCamera cam(512, 512, 0.1);
  PerspectiveCamera cam(1024, 1024, glm::radians(70.0f));
  size_t imgCount = 30;
  float angle = glm::pi<float>() * 2.0f;
  for (size_t i = 0; i < imgCount; ++i) {
    glm::mat4 trans = glm::rotate(glm::mat4(1.0),
        (angle / imgCount) * i, glm::vec3(0, 1, 0));
    trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(-1, 0, 0));
    cam.transform(glm::translate(trans, glm::vec3(0, 0, 100)));
    double startTime = omp_get_wtime();
    cam.render(scene);
    double endTime = omp_get_wtime();
    // Save the image under different names.
    char buff[100];
#ifdef WINDOWS
    printf("Image %03lu took %.2f sec to render.\n", i,
        endTime - startTime);
    _snprintf_s(buff, sizeof(buff), "Img%03lu.bmp", i);
#else
    printf("Image %03zu took %.2f sec to render.\n", i,
        endTime - startTime);
    snprintf(buff, sizeof(buff), "Img%03zu.bmp", i);
#endif  // WINDOWS
    cam.getImage().saveAsBMP(buff);
  }
}

// The main method.
int main(int argc, char** argv) {
  // Print usage info.
  if (argc > 2) {
    printf("Usage: %s <optional: random seed>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  // Initialize the rand function.
  unsigned int seed = static_cast<unsigned int>(time(NULL));
  if (argc == 2) {
    seed = static_cast<unsigned int>(atoi(argv[1]));
  }
  printf("Random seed used: %u\n\n", seed);
  srand(seed);

  // Render our test scene.
  renderTestScene();
}

