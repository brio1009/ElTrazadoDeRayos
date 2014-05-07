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
#include <glm/glm.hpp>
#include "./OrthogonalCamera.h"
#include "./IntersectionInfo.h"
#include "./Constants.h"
#include "./Scene.h"
#include "./Ray.h"
#include "./Material.h"
// _____________________________________________________________________________
void OrthogonalCamera::render(const Scene& scene) {
  // Get the size of the image.
  size_t width = 80;
  size_t height = 80;
  glm::vec4 position(- width/2.0f, -height/2.0f, 0, 1);
  glm::vec4 direction(0, 0, 1, 0);
  direction = glm::inverse(_transformation)* direction;

  glm::vec4 planeX(1, 0, 0, 0);
  glm::vec4 planeY(0, 1, 0, 0);
  planeX = glm::inverse(_transformation) * planeX;
  planeY = glm::inverse(_transformation) * planeY;
  // Send rays.
  for (size_t x = 0; x < width; ++x) {
    for (size_t y = 0; y < height; ++y) {
      Ray r(position
        + ((float)x * planeX) + ((float)y* planeY), direction);
      IntersectionInfo info = scene.traceRay(r);
      if (info.materialPtr) {
        // HIT
        Color tmpColor = info.materialPtr->getColor(info.hitPoint, info.normal, scene);
        _image.setPixel(x, y, tmpColor);
      }
    }
  }
  // DONE!
  _image.saveAsBMP("Ortho.bmp");
}
