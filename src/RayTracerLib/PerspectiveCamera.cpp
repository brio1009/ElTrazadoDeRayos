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
#include "./PerspectiveCamera.h"
#include <cmath>
#include <ctime>
#include <vector>
#include "./Constants.h"
#include "./Ray.h"
#include "./IntersectionInfo.h"
#include "./Scene.h"
#include "./Material.h"

using std::vector;
// _____________________________________________________________________________
PerspectiveCamera::PerspectiveCamera(const int width,
      const int height,
      const REAL& fovAngleInRad) : Camera(width, height) {
  // TODO(bauschp): Insert right formula.
  _focalLength = width / fovAngleInRad;
}

// _____________________________________________________________________________
PerspectiveCamera::~PerspectiveCamera() { }

// _____________________________________________________________________________
void PerspectiveCamera::render(const Scene& scene) {
  // Get the size of the image.
  REAL startX = _image.getWidth() / 2.0;
  REAL startY = _image.getHeight() / 2.0;

  glm::vec4 position(0, 0, 0, 1);
  position = _transformation * position;
  // Send rays.
  size_t overallRayCreation = 0;
  size_t overallSceneTrace = 0;
  size_t overallGetColor = 0;
#pragma omp parallel for
  for (int x = 0; x < _image.getWidth(); ++x) {
    for (int y = 0; y < _image.getHeight(); ++y) {
      // Create new direction.
      glm::vec4 direction(-startX + x, startY - y, -_focalLength, 0);
      direction = glm::normalize(direction);
      direction = _transformation * direction;

      Ray r(position, direction);
      // printf("SENDING RAY:(%.2f,%.2f,%.2f|%.2f,%.2f,%.2f)\n", position.x,
      //       position.y, position.z, direction.x, direction.y, direction.z);
      // IntersectionInfo info = scene.traceRay(r);
      vector<Ray> corners;
      corners.resize(4);
      corners[0] = Ray(position, glm::vec4(-startX + x - 0.5f, startY -y + 0.5f, -_focalLength, 0));
      corners[1] = Ray(position, glm::vec4(-startX + x + 0.5f, startY -y + 0.5f, -_focalLength, 0));
      corners[2] = Ray(position, glm::vec4(-startX + x - 0.5f, startY -y - 0.5f, -_focalLength, 0));
      corners[3] = Ray(position, glm::vec4(-startX + x + 0.5f, startY -y - 0.5f, -_focalLength, 0));
      for (size_t i = 0 ; i < 4; ++i)
        corners[i].setDirection(_transformation * corners[i].direction());
      vector<Ray*> samples;
      vector<Color> colors;
      for (size_t i=0; i < 4; ++i) {
        samples.push_back(_sampler->getSample(i, corners));
        IntersectionInfo info = scene.traceRay(*samples.back());
        if (info.materialPtr) {
          // HIT
          colors.push_back(info.materialPtr->getColor(info,
                                                      r,
                                                      scene));
        } else {
          colors.push_back(scene.backgroundColor(r));
        }
      }
      _image.setPixel(x, y, _sampler->reconstructColor(&samples, colors));
    }
  }
}

