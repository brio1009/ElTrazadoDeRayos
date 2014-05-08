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

size_t img = 0;
// _____________________________________________________________________________
void OrthogonalCamera::render(const Scene& scene) {
  // Get the size of the image.
  REAL startX = _image.getWidth() * 0.5 * _unitsPerPixel;
  REAL startY = _image.getHeight() * 0.5 * _unitsPerPixel;

  glm::vec4 position(-startX + _unitsPerPixel * 0.5, -startY + _unitsPerPixel * 0.5, 0, 1);
  position = _transformation * position;
  glm::vec4 direction(0, 0, -1, 0);
  direction = _transformation * direction;

  glm::vec4 planeX(_unitsPerPixel, 0, 0, 0);
  glm::vec4 planeY(0, _unitsPerPixel, 0, 0);
  planeX = _transformation * planeX;
  planeY = _transformation * planeY;
  // Send rays.
  for (int x = 0; x < _image.getWidth(); ++x) {
    for (int y = 0; y < _image.getHeight(); ++y) {
      Ray r(position
        + ((float)x * planeX) + ((float)y* planeY), direction);
      IntersectionInfo info = scene.traceRay(r);
      if (info.materialPtr) {
        // HIT
        Color tmpColor = info.materialPtr->getColor(info.hitPoint, info.normal, -direction, scene);
        _image.setPixel(x, y, tmpColor);
      } else {
        _image.setPixel(x, y, Color(0, 0, 0, 0));
      }
    }
  }
  // DONE!
  char buff[100];
  snprintf(buff, 100, "Ortho%zu.bmp", img);
  ++img;
  _image.saveAsBMP(buff);
}
