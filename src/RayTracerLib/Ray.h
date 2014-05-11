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
#pragma once
#ifndef RAYTRACERLIB_RAY_H_
#define RAYTRACERLIB_RAY_H_

#include <glm/glm.hpp>
#include <glm/gtx/fast_square_root.hpp>


// TODO(bauschp): implement a real ray class
class Ray {
 public:
  glm::vec4 pos;
  glm::vec4 dir;
  // Creates a Ray with no direction!!!
  Ray() : pos(0, 0, 0, 1), dir(0, 0, 0, 0) { }
  // Creates a Ray with given origin and direction.
  Ray(const glm::vec4& pos, const glm::vec4& dir) {
    this->pos = pos;
    this->dir = dir;
    this->pos[3] = 1;
    this->dir[3] = 0;
    this->dir = glm::normalize(this->dir);
  }
};

#endif  // RAYTRACERLIB_RAY_H_


