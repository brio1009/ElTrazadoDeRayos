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
#include "./PerspectiveCamera.h"
#include "./Ray.h"

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
Ray PerspectiveCamera::createPixelCornerRay(
    const size_t& px,
    const size_t& py) const {
  REAL startX = _image.getWidth() / 2.0;
  REAL startY = _image.getHeight() / 2.0;
  glm::vec4 direction(-startX + px, startY - py, -_focalLength, 0);
  direction = glm::normalize(direction);
  direction = _transformation * direction;

  return Ray(getPosition(), direction);
}
// _____________________________________________________________________________
void PerspectiveCamera::transform(const glm::mat4& matrix) {
  // transform as Object does.
  Object::transform(matrix);
  // recalculate the Image plane directions.
  // _position = _transformation * glm::vec4(0, 0, 0, 1);
}

