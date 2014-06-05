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
Ray* RegularSampler::createSample(
    const std::vector<Ray>& rays,
    const std::pair<float, float>& lambda) const {
  // Interpolate between origin and the other
  glm::vec4 nextSampleDir =
      rays.at(0).direction() * (1.0f - lambda.first) * (1.0f - lambda.second)
      + rays.at(1).direction() * lambda.first * (1.0f - lambda.second)
      + rays.at(2).direction() * (1.0f - lambda.first) * lambda.second
      + rays.at(3).direction() * lambda.first * lambda.second;
  glm::vec4 nextSamplePos =
      rays.at(0).origin() * (1.0f - lambda.first) * (1.0f - lambda.second)
      + rays.at(1).origin() * lambda.first * (1.0f - lambda.second)
      + rays.at(2).origin() * (1.0f - lambda.first) * lambda.second
      + rays.at(3).origin() * lambda.first * lambda.second;
  return new Ray(nextSamplePos, nextSampleDir);
}
// _____________________________________________________________________________
std::pair<float, float> RegularSampler::getNextLambda(
    const size_t& index) const {
  if (index >= _samplesPerImageDimension * _samplesPerImageDimension)
    throw "RegularSampler was called with to large index.\n";
  size_t x = index % _samplesPerImageDimension;
  size_t y = (index - x) / _samplesPerImageDimension;

  return std::pair<float, float>(
      static_cast<float>(x) / _samplesPerImageDimension + _offsetLambda,
      static_cast<float>(y) / _samplesPerImageDimension + _offsetLambda);
}
