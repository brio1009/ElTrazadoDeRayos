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

#pragma once
#ifndef RAYTRACERLIB_SHAPES_BOX_H_
#define RAYTRACERLIB_SHAPES_BOX_H_

#include <glm/glm.hpp>
#include <vector>

#include "./Constants.h"
#include "./Shape.h"

// A Primitive is a Shape that is defined in its own.
class Box : public Shape {
  GETSET(REAL, _rX, DimX)
  GETSET(REAL, _rY, DimY)
  GETSET(REAL, _rZ, DimZ)

 public:
  // Default constructor. Dimensions are 1, 1, 1.
  Box() : Box(1.0, 1.0, 1.0) { }

  /// Constructor with given dimensions. Rotation must be done with
  /// a transformation.
  Box(REAL x, REAL y, REAL z);
  /// Destructor.
  virtual ~Box() { }

  /// Intersection test.
  virtual std::vector<REAL> intersect(const Ray& ray) const;

  /// Returns the min. position.
  glm::vec4 getMinPosition() const;

  /// Returns the max. position.
  glm::vec4 getMaxPosition() const;

  /// The class name. Needed for the Factory creating the object.
  static const char* name;

  Box* create() const {
    return new Box();
  }

  static void registerProperties() {
    static bool m_lock(true);
    if (!m_lock)
      return;
    m_lock = false;
    genericfactory::GenericFactory<Shape>::registerProperty<Box, REAL>(
        "dX",
        &Box::setDimX,
        &Box::noGet);
    genericfactory::GenericFactory<Shape>::registerProperty<Box, REAL>(
        "dY",
        &Box::setDimY,
        &Box::noGet);
    genericfactory::GenericFactory<Shape>::registerProperty<Box, REAL>(
        "dZ",
        &Box::setDimZ,
        &Box::noGet);
  }

 protected:
  /// Return normal.
  virtual glm::vec4 getNormalAt(const glm::vec4& p) const;

 private:
  REAL _rX;
  REAL _rY;
  REAL _rZ;
};

#endif  // RAYTRACERLIB_SHAPES_BOX_H_
