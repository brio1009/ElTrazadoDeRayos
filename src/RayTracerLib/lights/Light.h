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
#ifndef RAYTRACERLIB_LIGHTS_LIGHT_H_
#define RAYTRACERLIB_LIGHTS_LIGHT_H_

#include <ctime>
#include <glm/glm.hpp>
#include "../Object.h"
#include "../Color.h"

// Forward declaration.
class Ray;

/// An abstract interface to interact with Lights.
class Light : public Object {
 public:
  /// Destructor.
  virtual ~Light() { }

  /// Returns a ray from this light to the given position pos.
  virtual Ray getRay(const glm::vec4& pos) const = 0;

  /// Returns the color of this light.
  virtual const Color& getColor() const {
    return _color;
  }

  /// Sets the color of this light.
  virtual void setLightColor(const Color& color) {
    _color = color;
  }

  /// Returns the number of samples a material should use to determine if
  /// it's lit by this light.
  virtual size_t numberOfSamples() const { return 1; }

 private:
  /// The color of the light.
  Color _color;
};

#endif  // RAYTRACERLIB_LIGHTS_LIGHT_H_
