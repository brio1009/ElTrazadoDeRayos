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

#include "./RegularSampler.h"

#include <glm/glm.hpp>
#include "./Sampler.h"
#include "./Ray.h"

// _____________________________________________________________________________
Ray RegularSampler::getSample(const size_t& index) {
  if (index >= _samplesPerImageDimension * _samplesPerImageDimension)
    throw "OutOfBounds";
  size_t x = index % _samplesPerImageDimension;
  // not sure if -x is needed because we have integer division.
  size_t y = (index - x) / _samplesPerImageDimension;
  // Calculate the two lambda values needed for interpolation.
  // UNIT SQUARE!!!!
  float lambda1 = static_cast<float>(x) / _samplesPerImageDimension
      + _offsetLambda;
  float lambda2 = static_cast<float>(y) / _samplesPerImageDimension
      + _offsetLambda;
  // Interpolate between origin and the other
  glm::vec4 nextSampleDir =
      _bottomLeft.direction() * (1.0f - lambda1) * (1.0f - lambda2)
      + _bottomRight.direction() * lambda1 * (1.0f - lambda2)
      + _topLeft.direction() * (1.0f - lambda1) * lambda2
      + _topRight.direction() * lambda1 * lambda2;
  glm::vec4 nextSamplePos =
      _bottomLeft.origin() * (1.0f - lambda1) * (1.0f - lambda2)
      + _bottomRight.origin() * lambda1 * (1.0f - lambda2)
      + _topLeft.origin() * (1.0f - lambda1) * lambda2
      + _topRight.origin() * lambda1 * lambda2;
  return Ray(nextSamplePos, nextSampleDir);
}
// _____________________________________________________________________________
void RegularSampler::nextSampleConfiguration() {
  Sampler::nextSampleConfiguration();
}
