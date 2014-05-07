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

#include <cstdio>
#include <cstdlib>

#include <vector>


#include <glm/glm.hpp>
// RayTracerLib
#include <Ray.h>
#include <Ellipsoid.h>
#include <Plane.h>
#include <Image.h>

void binaryTraceOrth() {
  Ellipsoid test(2, 1, 1);
  Plane testPlane(-1, 0, -1);
  Ray r;
  testPlane.transform(glm::mat4(0,1,0,0,-1,0,0,0,0,0,1,0,0,0,0,1));
  testPlane.transform(glm::mat4(1,0,0,0,0,1,0,-1,0,0,1,0,0,0,0,1));
  Image img(80,80);
  r.pos[2] = 0.9;
  r.dir[3] = 0;
  r.pos[3] = 1;
  r.dir = glm::vec4(0, 0, -1, 0);
  for (size_t i = 0; i < 80; ++i)
    for (size_t j = 0; j < 80; ++j)
      img(i, j)->r = 255;
  img.saveAsBMP("test.bmp");
  return;
  for (float y = 2.0f; y > -2.0f; y -= 0.05f) {
    r.pos[1] = y;
    for (float x = -2.0f; x < 2.0f; x += 0.05f) {
      r.pos[0] = x;
      auto erg = test.intersect(r);
      auto ergPlane = testPlane.intersect(r);
      if (erg.size() !=  0 || ergPlane.size() != 0) {
        // HIT
        if (erg.size() == 0) {
          printf("\033[0;36mP\033[0m");
          continue;
        }
        if (ergPlane.size() == 0) {
          printf("O");
          continue;
        }
        if (erg[0] < ergPlane[0])
          printf("\033[0;31mO\033[0m");
        else
          printf("P");
      } else {
        // MISS
        printf("#");
      }
    }
    printf("\n");
  }
}


// The main method.
int main(int argc, char** argv) {
  // Print usage.
  binaryTraceOrth();
  return 9;
  if (argc != 10) {
    printf("9 arguments needed.\n3 values to define Ellipsoid.\n"
           "3 to define a point to shoot a ray from.\n"
           "3 to define a direction the ray will shoot to.\n");
    exit(EXIT_FAILURE);
  }

  Ellipsoid testEllipsoid(atof(argv[1]), atof(argv[2]), atof(argv[3]));

  Ray r;
  r.pos = glm::vec4(atof(argv[4]), atof(argv[5]), atof(argv[6]), 1);
  r.dir = glm::vec4(atof(argv[7]), atof(argv[8]), atof(argv[9]), 0);
  r.dir = glm::normalize(r.dir);
  std::vector<double> erg = testEllipsoid.intersect(r);
  for (int i = 0; i < static_cast<int>(erg.size()); ++i)
    printf("t_%d=%.2f\n", i, erg[i]);
    
  exit(EXIT_FAILURE);
}
