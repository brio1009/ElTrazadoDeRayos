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

// C Header.
#include <omp.h>

// C++ Header.
#include <vector>
#include <memory>

#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Material.h"
#include "./Ray.h"
#include "./Scene.h"
#include "../samplers/RegularSampler.h"
#include "../samplers/AdaptiveSampler.h"

using std::vector;

// _____________________________________________________________________________
Camera::Camera(const int width, const int height) : _image(width, height) {
  m_Sampler = std::make_shared<RegularSampler>(1);
}

// _____________________________________________________________________________
Camera::Camera(const Camera& camera)
  : _image(camera._image),
    m_Sampler(camera.m_Sampler) {
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
  int amountPixels = _image.getWidth() * _image.getHeight();
  render(scene, 0, amountPixels);
}

// _____________________________________________________________________________
void Camera::render(const Scene& scene,
      const size_t& startPixel,
      const size_t& endPixel) {
  // Send rays.
  size_t progress(0);
  int amountPixels = endPixel - startPixel;
  #pragma omp parallel for schedule(dynamic, 100)
  for (int i = startPixel; i < static_cast<int>(endPixel); ++i) {
    // Get the pixel coordinates from i.
    int x = i % _image.getWidth();
    int y = i / _image.getWidth();
    // TODO(bauschp, Thu Jun 12 16:33:05 CEST 2014): remove overhead.
    vector<Ray> borders;
    borders.reserve(4);
    for (unsigned char piy = 0; piy <= 1; ++piy)
      for (unsigned char pix = 0; pix <= 1; ++pix)
        borders.push_back(createPixelCornerRay(x + pix, y + piy));
    _image.setPixel(x, y,
          m_Sampler->getSampledColor(borders, scene));
    // Print progress (only if we are in first thread).
    if (omp_get_thread_num() == 0) {
      printf("Progress: %.2f%%\r", (100.0f * (i - startPixel)) / amountPixels);
    }
  }
}
