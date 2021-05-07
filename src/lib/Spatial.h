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
  virtual void transform(const glm::mat4& matrix);

  /// Getter for the transformation matrix.
  const glm::mat4& getTransformMatrix() const { return _transformation; }
  /// Getter for the inverse transformation matrix.
  const glm::mat4& getInverseTransformMatrix() const {
    return _inverseTransform;
  }

  /// Get the position of the object.
  const glm::vec4& getPosition() const { return _transformation[3]; }

  /// Rotate
  virtual void rotate(const glm::vec3& axis, const float rad);

  /// Rotate by vec4.
  /// rotates the transformation around the
  /// axis defined by the first 3 entries by the 4th entry in radians.
  virtual void rotate(const glm::vec4 value);

  /// Set the position of the object.
  void setPosition(const glm::vec3& position) {
    _transformation[3] = glm::vec4(position, 1);
    _inverseTransform = glm::inverse(_transformation);
  }

  // The following 3 setters cant be created automaticly because inverse has to
  // be updated.
  /// Sets the X value of the position vector.
  void setX(REAL x) {
    _transformation[3].x = x;
    _inverseTransform = glm::inverse(_transformation);
  }
  REAL getX() const { return _transformation[3].x; }

  /// Sets the Y value of the position vector.
  void setY(REAL y) {
    _transformation[3].y = y;
    _inverseTransform = glm::inverse(_transformation);
  }
  REAL getY() const { return _transformation[3].y; }

  /// Sets the Z value of the position vector.
  void setZ(REAL z) {
    _transformation[3].z = z;
    _inverseTransform = glm::inverse(_transformation);
  }
  REAL getZ() const { return _transformation[3].z; }

 protected:
  // the transformation of this Shape
  glm::mat4 _transformation = glm::mat4(1);
  glm::mat4 _inverseTransform = glm::mat4(1);
};

#endif  // RAYTRACERLIB_SPATIAL_H_
