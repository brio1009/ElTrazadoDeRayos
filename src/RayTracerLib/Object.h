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
#ifndef RAYTRACERLIB_OBJECT_H_
#define RAYTRACERLIB_OBJECT_H_

// GlMath include.
#include <glm/glm.hpp>

#include "./Constants.h"

class Object {
 public:
  /// Destructor.
  virtual ~Object() {}
  // Multiplies the matrix with the current Transformation.
  virtual void transform(const glm::mat4& matrix);  //NOLINT mistaken for std

  /// Get the position of the object.
  const glm::vec4& getPosition() const { return _transformation[3]; }

  /// Set the position of the object.
  void setPosition(const glm::vec4& position) { _transformation[3] = position; }

 protected:
  // the transformation of this Shape
  glm::mat4 _transformation;
};

#endif  // RAYTRACERLIB_OBJECT_H_
