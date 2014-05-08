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
#ifndef RAYTRACERLIB_DIRECTIONALLIGHT_H_
#define RAYTRACERLIB_DIRECTIONALLIGHT_H_ 

#include <glm/glm.hpp>
#include "./Light.h"

class DirectionalLight : public Light {
 private:
  // The direction the Light is shining to.
  glm::vec4 _direction;
 public:
  /// Constructor of a Directional Light.
  DirectionalLight(const glm::vec4& dir) : _direction(dir) { }
  /// Destructor
  virtual ~DirectionalLight() { }
  /// Overrides
  virtual Ray getDirection(const glm::vec4& pos) const;
};

#endif  // RAYTRACERLIB_DIRECTIONALLIGHT_H_
