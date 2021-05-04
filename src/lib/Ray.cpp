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

#include "./Ray.h"

#include "./Solver.h"

// _____________________________________________________________________________
Ray::Ray(const glm::vec4& origin, const glm::vec4& direction) {
  setOrigin(origin);
  setDirection(direction);
}

// _____________________________________________________________________________
Ray::Ray(const Ray& ray) {
  setOrigin(ray.origin());
  setDirection(ray.direction());
  this->_rayInfo = ray.rayInfo();
}

// _____________________________________________________________________________
void Ray::setOrigin(const glm::vec4& origin) {
  _origin = origin;
  if (solve::isZero(origin[3])) {
    _origin[3] = 1.0;
    return;
  }
  _origin /= _origin[3];
}

// _____________________________________________________________________________
void Ray::setDirection(const glm::vec4& direction) {
  _direction = direction;
  _direction[3] = 0.0;
  _direction = glm::normalize(_direction);
}

