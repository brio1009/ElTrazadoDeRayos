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
#include "./Camera.h"
#include "./RegularSampler.h"
#include <vector>
// _____________________________________________________________________________
Camera::Camera(const int width, const int height) : _image(width, height) {
  _sampler = new RegularSampler(2);
  std::vector<Ray> borders;
  borders.push_back(Ray(glm::vec4(0, 0, 0, 0), glm::vec4(0, 1, 0, 0)));
  borders.push_back(Ray(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0)));
  borders.push_back(Ray(glm::vec4(0, 1, 0, 0), glm::vec4(0, 1, 0, 0)));
  borders.push_back(Ray(glm::vec4(1, 1, 0, 0), glm::vec4(0, 1, 0, 0)));
  std::vector<Ray*> sampleRays;
  sampleRays.push_back(_sampler->getSample(0, borders));
  printf("(%.2f|%.2f|%.2f)\n", sampleRays.back()->origin().x,
    sampleRays.back()->origin().y,
    sampleRays.back()->origin().z);
  sampleRays.push_back(_sampler->getSample(1, borders));
  printf("(%.2f|%.2f|%.2f)\n", sampleRays.back()->origin().x,
    sampleRays.back()->origin().y,
    sampleRays.back()->origin().z);
  sampleRays.push_back(_sampler->getSample(2, borders));
  printf("(%.2f|%.2f|%.2f)\n", sampleRays.back()->origin().x,
    sampleRays.back()->origin().y,
    sampleRays.back()->origin().z);
  sampleRays.push_back(_sampler->getSample(3, borders));
  printf("(%.2f|%.2f|%.2f)\n", sampleRays.back()->origin().x,
    sampleRays.back()->origin().y,
    sampleRays.back()->origin().z);
  std::vector<Color> cols;
  cols.push_back(Color(1, 0, 0));
  cols.push_back(Color(1, 1, 0));
  cols.push_back(Color(1, 1, 0));
  cols.push_back(Color(0, 0, 1));
  Color col = _sampler->reconstructColor(&sampleRays, cols);
  printf("(%.2f|%.2f|%.2f)\n", col.r(), col.g(), col.b());
}
// _____________________________________________________________________________
const Image& Camera::getImage() const {
  return _image;
}
// _____________________________________________________________________________
void Camera::setImageSize(const int width, const int height) {
  _image(width, height);
}
