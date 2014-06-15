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
#ifndef RAYTRACERLIB_LIGHTS_AREALIGHT_H_
#define RAYTRACERLIB_LIGHTS_AREALIGHT_H_

#include <glm/glm.hpp>
#include <ctime>
#include "./Light.h"

/// A simple area light that varies its position in a sphere.
class AreaLight : public Light {
 public:
  /// Constructor of a Area Light. The sphere radius of the light must be given.
  AreaLight(const glm::vec4& position, REAL sphereRadius)
    : m_SphereRadius(sphereRadius),
      m_NumSamples(10) {
    setPosition(position);
  }
  /// Destructor.
  virtual ~AreaLight() { }

  /// Returns the ray from the light source to the given point. The area light
  /// always varies its position in a sphere.
  virtual Ray getRay(const glm::vec4& pos) const;

  /// Returns the number of samples a material should use to determine if
  /// it's lit by this light.
  size_t numberOfSamples() const { return m_NumSamples; }

  /// Sets the number of samples used for this light. This also rescales the
  /// color.
  void setNumberOfSamples(size_t numSamples) {
    // First rescale the light.
    Light::setLightColor((static_cast<float>(numberOfSamples()) * getColor())
                         * (1.0 / numberOfSamples()));
    // Set the number of samples.
    m_NumSamples = numSamples;
  }

  /// Getter for sphere radius.
  REAL sphereRadius() const { return m_SphereRadius; }

  /// Sets the color of this light. This is overridden here, so we can adapt the
  /// light color to the number of samples (scale it down).
  virtual void setLightColor(const Color& color) {
    Light::setLightColor((1.0 / static_cast<float>(numberOfSamples())) * color);
  }

 private:
  /// The number of samples that should be used by a material to sample
  /// this area light.
  size_t m_NumSamples;

  /// The radius of the sphere in which this area light varies its position.
  REAL m_SphereRadius;
};

#endif  // RAYTRACERLIB_LIGHTS_AREALIGHT_H_
