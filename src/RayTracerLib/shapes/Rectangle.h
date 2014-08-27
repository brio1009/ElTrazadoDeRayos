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
#ifndef RAYTRACERLIB_SHAPES_RECTANGLE_H_
#define RAYTRACERLIB_SHAPES_RECTANGLE_H_

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "./Constants.h"
#include "./Shape.h"

///
class Rectangle : public Shape {
  GETSET(float, m_Extent.x, ExtentX)
  GETSET(float, m_Extent.y, ExtentY)

 public:
  /// Default constructor. Normal is y-up.
  Rectangle() : Rectangle(glm::vec3(0, 1, 0), glm::vec2(1, 1)) { }

  /// Constructor with given normal axis.
  Rectangle(const glm::vec3& normal, const glm::vec2& extent);

  /// Destructor.
  virtual ~Rectangle() { }
  /// Test for intersections.
  virtual std::vector<REAL> intersect(const Ray& ray) const;
  /// Get the normal at a world position p.
  virtual glm::vec4 getNormalAt(const glm::vec4& p) const;

  /// Setter for m_ClipBackplane.
  void setClipBackplane(const bool value) { m_ClipBackplane = value; }

  /// Getter for m_ClipBackplane.
  bool clipBackplane() const { return m_ClipBackplane; }

  /// Getter for extent.
  const glm::vec2& extent() const { return m_Extent; }

  // ################### SPECIAL PROPERTIES #######################
  /// sets the X compnent of the normal.
  void setNormalX(float x) {
    setNormal(glm::vec3(x, m_Normal.y, m_Normal.z));
  }

  /// sets the Y compnent of the normal.
  void setNormalY(float y) {
    setNormal(glm::vec3(m_Normal.x, y, m_Normal.z));
  }

  /// sets the Z compnent of the normal.
  void setNormalZ(float z) {
    setNormal(glm::vec3(m_Normal.x, m_Normal.y, z));
  }
  // ################### SPECIAL PROPERTIES END ###################
  /// The class name. Needed for the Factory creating the object.
  static const char* name;

  void setNormal(const glm::vec3& normal);

  Rectangle* create() const {
    return new Rectangle();
  }

  static void registerProperties() {
    static bool m_lock(true);
    if (!m_lock)
      return;
    m_lock = false;
    genericfactory::GenericFactory<Shape>::registerProperty<Rectangle, float>(
        "nX",
        &Rectangle::setNormalX,
        &Rectangle::noGet);
    genericfactory::GenericFactory<Shape>::registerProperty<Rectangle, float>(
        "nY",
        &Rectangle::setNormalY,
        &Rectangle::noGet);
    genericfactory::GenericFactory<Shape>::registerProperty<Rectangle, float>(
        "nZ",
        &Rectangle::setNormalZ,
        &Rectangle::noGet);
    genericfactory::GenericFactory<Shape>::registerProperty<Rectangle, float>(
        "ExtentX",
        &Rectangle::setExtentX,
        &Rectangle::noGet);
    genericfactory::GenericFactory<Shape>::registerProperty<Rectangle, float>(
        "ExtentY",
        &Rectangle::setExtentY,
        &Rectangle::noGet);
    genericfactory::GenericFactory<Shape>::registerProperty<Rectangle, bool>(
        "NoBack",
        &Rectangle::setClipBackplane,
        &Rectangle::noGet);
  }

 private:
  /// Extent int x- and z-direction.
  glm::vec2 m_Extent;

  glm::vec3 m_Normal;

  /// Signals if the the rectangle should be rendered from the back.
  bool m_ClipBackplane;
};


#endif  // RAYTRACERLIB_SHAPES_RECTANGLE_H_

