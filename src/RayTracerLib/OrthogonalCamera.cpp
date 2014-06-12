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
#include "./OrthogonalCamera.h"
#include <glm/glm.hpp>
#include <cstdio>
#include "./IntersectionInfo.h"
#include "./Constants.h"
#include "./Scene.h"
#include "./Ray.h"
#include "./Material.h"

// _____________________________________________________________________________
Ray OrthogonalCamera::createPixelCornerRay(
    const size_t& px,
    const size_t& py) const {
 return Ray(_imagePlaneTopLeftCorner
      + static_cast<float>(px) * _imagePlaneX
      + static_cast<float>(py) * _imagePlaneY,
      _viewingDirection);
}
// _____________________________________________________________________________
void OrthogonalCamera::transform(const glm::mat4& matrix) {
  // transform as Object does.
  Object::transform(matrix);
  // recalculate the Image plane directions.
  _imagePlaneX = glm::vec4(_unitsPerPixel, 0, 0, 0);
  _imagePlaneY = glm::vec4(0, -_unitsPerPixel, 0, 0);
  _imagePlaneX = _transformation * _imagePlaneX;
  _imagePlaneY = _transformation * _imagePlaneY;
  // recalculate the viewing direction and Image position.
  // TODO(bauschp, Thu Jun 12 15:59:26 CEST 2014): check if centered.
  REAL startX = _image.getWidth() * 0.5 * _unitsPerPixel;
  REAL startY = _image.getHeight() * 0.5 * _unitsPerPixel;
  _imagePlaneTopLeftCorner = glm::vec4(-startX + _unitsPerPixel * 0.5,
                     startY + _unitsPerPixel * 0.5,
                     0,
                     1);
  _imagePlaneTopLeftCorner = _transformation * _imagePlaneTopLeftCorner;
  _viewingDirection = _transformation * glm::vec4(0, 0, -1, 0);
}
