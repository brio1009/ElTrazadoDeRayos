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

#include <cstdio>
#include <cstdlib>

#include <vector>


#include <glm/glm.hpp>
#include <Ray.h>
#include <Plane.h>

void binaryTraceOrth() {
  Plane test(0, -0.25, -0.25);
  Ray r;
  r.pos[3] = -2;
  r.dir = glm::vec4(0, 0, 1, 0);
  for (float y = -2.0f; y < 2.0f; y += 0.05f) {
    r.pos[1] = y;
    for (float x = -2.0f; x < 2.0f; x += 0.05f) {
      r.pos[0] = x;
      auto erg = test.intersect(r);
      if (erg.size() !=  0) {
        // HIT
        if (erg[0] < 0)
          printf("-");
        else
          printf("o");
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
  if (argc != 10) {
    printf("9 arguments needed.\n3 values to define Ellipsoid.\n"
           "3 to define a point to shoot a ray from.\n"
           "3 to define a direction the ray will shoot to.\n");
    exit(EXIT_FAILURE);
  }
/*
  Ellipsoid testEllipsoid(atof(argv[1]), atof(argv[2]), atof(argv[3]));

  Ray r;
  r.pos = glm::vec4(atof(argv[4]), atof(argv[5]), atof(argv[6]), 1);
  r.dir = glm::vec4(atof(argv[7]), atof(argv[8]), atof(argv[9]), 0);
  r.dir = glm::normalize(r.dir);
  std::vector<double> erg = testEllipsoid.intersect(r);
  for (int i = 0; i < erg.size(); ++i)
    printf("t_%d=%.2f\n", i, erg[i]);
    */
  exit(EXIT_FAILURE);
}
