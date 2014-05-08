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
#ifndef RAYTRACERLIB_LIGHT_H_
#define RAYTRACERLIB_LIGHT_H_

#include <glm/glm.hpp>
#include "./Object.h"
#include "./Color.h"

class Ray;

/// An abstract interface to interact with Lights.
class Light : public Object {
 public:
  /// A light holds a array of its diffuse and specular light color.
  Color _colors[2];
 public:
  enum Component {
    DIFFUSE = 0,
    SPECULAR 
  };
  /// Destructor.
  virtual ~Light() { };
  /// Creates a Ray from the given Point to this Light.
  virtual Ray getShadowRay(const glm::vec4& pos) const = 0;

  virtual const Color& getColorComponent(Component c) const {
    return _colors[c];
  }
  // Returns the color of this light
  /// Sets the Lights Component color to given color. a is not used yet.
  virtual void setLightColor(unsigned char r,
      unsigned char g,
      unsigned char b,
      unsigned char a,
      Component c) {
    _colors[c].r = r;
    _colors[c].g = g;
    _colors[c].b = b;
    _colors[c].a = a;
  }
  virtual void setLightColor(const Color& color, Component c) {
      setLightColor(color.r, color.g, color.b, color.a, c);
  }
};

#endif  // RAYTRACERLIB_LIGHT_H_
