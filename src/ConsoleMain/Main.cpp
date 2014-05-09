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

#include <algorithm>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// RayTracerLib
#include <Ray.h>
#include <Ellipsoid.h>
#include <Plane.h>
#include <Image.h>
#include <Material.h>
#include <Scene.h>
#include <OrthogonalCamera.h>
#include <PerspectiveCamera.h>
//
void renderTestScene() {
  Scene scene;
  // REMEMBER VIM: use :r !date to insert current time.
  // TODO(allofus, Wed May  7 21:13:19 CEST 2014): to scale a image transform
  // the thrid param to match smaller change.
  // e.g. when converting from 80x80 (first trace) to  512x512 divide 80 by 512
  // OrthogonalCamera cam(512, 512, 0.1);
  PerspectiveCamera cam(512, 512, 70.0 / 180.0 * 3.14159);

  // TODO(bauschp, Wed May  7 21:57:12 CEST 2014): Remove this.
  for (size_t i = 0; i < 20; ++i) {
    glm::mat4 trans = glm::rotate(glm::mat4(1.0), 18.0f * i, glm::vec3(0, 1, 0));
    trans = glm::rotate(trans, 35.0f, glm::vec3(1, 0, 0));
    cam.transform(glm::translate(trans, glm::vec3(20, 0, 80)));
    cam.render(scene);
    // Save the image under different names.
    char buff[100];
#ifdef WINDOWS
    _snprintf(buff, 100, "Ortho%03lu.bmp", i);
#else
    snprintf(buff, 100, "Ortho%03zu.bmp", i);
#endif  // WINDOWS
    cam.getImage().saveAsBMP(buff);
  }
}

// The main method.
int main(int argc, char** argv) {
  // Initialize the rand function.
  srand(static_cast<unsigned int>(time(NULL)));

  // Render our test scene.
  renderTestScene();
  return 8;

  // Print usage info.
  if (argc != 10) {
    printf("9 arguments needed.\n3 values to define Ellipsoid.\n"
           "3 to define a point to shoot a ray from.\n"
           "3 to define a direction the ray will shoot to.\n");
    exit(EXIT_FAILURE);
  }
}
