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

#include "./Camera.h"

#include <vector>

#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Material.h"
#include "./Ray.h"
#include "./Scene.h"

using std::vector;

// _____________________________________________________________________________
Camera::Camera(const int width, const int height) : _image(width, height) {
  _sampler = new RegularSampler(1);
}
// _____________________________________________________________________________
const Image& Camera::getImage() const {
  return _image;
}
// _____________________________________________________________________________
void Camera::setImageSize(const int width, const int height) {
  _image(width, height);
}
// _____________________________________________________________________________
void Camera::render(const Scene& scene) {
  // Send rays.
  size_t progress(0);
  size_t amount = _image.getWidth() * _image.getHeight();
  // #pragma omp parallel for
  for (int x = 0; x < _image.getWidth(); ++x) {
    for (int y = 0; y < _image.getHeight(); ++y) {
      // TODO(bauschp, Thu Jun 12 16:33:05 CEST 2014): remove overhead.
      vector<Ray> borders;
      borders.reserve(4);
      for (unsigned char piy = 0; piy <= 1; ++piy)
        for (unsigned char pix = 0; pix <= 1; ++pix)
          borders.push_back(createPixelCornerRay(x + pix, y + piy));
      _image.setPixel(x, y,
            _sampler->getSampledColor(borders, scene));
      // #pragma omp critical
      {
        printf("Progress: %.2f%%\r", (100.0f * progress++) / amount);
      }
    }
  }
}
