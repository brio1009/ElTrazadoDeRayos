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

#ifndef RAYTRACERLIB_SPATIAL_H_
#define RAYTRACERLIB_SPATIAL_H_

// GlMath include.
#include <glm/glm.hpp>

#include "./Constants.h"

class Spatial {
 public:
  /// Destructor.
  virtual ~Spatial() {}
  // Multiplies the matrix with the current Transformation.
  virtual void transform(const glm::mat4& matrix);  //NOLINT mistaken for std

  /// Getter for the transformation matrix.
  const glm::mat4& getTransformMatrix() const { return _transformation; }

  /// Get the position of the object.
  const glm::vec4& getPosition() const { return _transformation[3]; }

  /// Set the position of the object.
  void setPosition(const glm::vec4& position) {
    _transformation[3] = position;
    _inverseTransform = glm::inverse(_transformation);
  }

 protected:
  // the transformation of this Shape
  glm::mat4 _transformation;
  glm::mat4 _inverseTransform;
};

#endif  // RAYTRACERLIB_SPATIAL_H_
