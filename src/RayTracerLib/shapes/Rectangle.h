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

#include <vector>

#include "./Constants.h"
#include "./Shape.h"
#include "../factories/Factory.h"

///
class Rectangle : public Shape,
      private Factory<Shape>::register_specialized<Rectangle>  {
  // Create properties (also generates getter and setter).
  PROPERTIES(Rectangle,
        REAL, m_Extent.x, ExtentX,
        REAL, m_Extent.y, ExtentY)

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

  static void createSpecialProperties() {
    if (!onceSpecial)
      return;
    onceSpecial = false;
    printf("ADDING SPECIAL PROPS TO RECTANGLE\n");
    RegisterProperty<float>("nX", &Rectangle::setNormalX, nullptr);
    RegisterProperty<float>("nY", &Rectangle::setNormalY, nullptr);
    RegisterProperty<float>("nZ", &Rectangle::setNormalZ, nullptr);
    RegisterProperty<bool>("NoBack", &Rectangle::setClipBackplane, nullptr);
  }
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
  static const char* parent;

  void setNormal(const glm::vec3& normal);


 private:
  /// Extent int x- and z-direction.
  glm::vec2 m_Extent;

  glm::vec3 m_Normal;

  /// Signals if the the rectangle should be rendered from the back.
  bool m_ClipBackplane;
  static bool onceSpecial;
};


#endif  // RAYTRACERLIB_SHAPES_RECTANGLE_H_

