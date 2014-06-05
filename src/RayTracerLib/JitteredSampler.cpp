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
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include "./JitteredSampler.h"
#include "./Sampler.h"
#include "./Ray.h"
/*
// _____________________________________________________________________________
Ray JitteredSampler::getSample(const size_t& index) {
  // Compute random values in the area.
  glm::vec2 lambda = glm::linearRand(
      glm::vec2(0.5f - _area),
      glm::vec2(0.5f + _area));
  // TODO(bauschp, Mon Jun  2 16:28:30 CEST 2014): Outsource interpolation.
  // Maybe create a header named helper or something. template would be nice.
  // Interpolate between origin and the other
  glm::vec4 nextSampleDir =
      _bottomLeft.direction() * (1.0f - lambda.x) * (1.0f - lambda.y)
      + _bottomRight.direction() * lambda.x * (1.0f - lambda.y)
      + _topLeft.direction() * (1.0f - lambda.x) * lambda.y
      + _topRight.direction() * lambda.x * lambda.y;
  glm::vec4 nextSamplePos =
      _bottomLeft.origin() * (1.0f - lambda.x) * (1.0f - lambda.y)
      + _bottomRight.origin() * lambda.x * (1.0f - lambda.y)
      + _topLeft.origin() * (1.0f - lambda.x) * lambda.y
      + _topRight.origin() * lambda.x * lambda.y;
  return Ray(nextSamplePos, nextSampleDir);
}
// _____________________________________________________________________________
void JitteredSampler::nextSampleConfiguration() {
  Sampler::nextSampleConfiguration();
}
*/
// _____________________________________________________________________________
/* Color JitteredSampler::reconstructColor(const std::vector<Color>& colors) {
  if (colors.size() != _sampleRays.size()) {
    // Returnes Red cause something went wrong while sampling.
    perror("SampleSize doesn`t correspond to given colors.");
    return Color(1, 0, 0, 0);
  }
}
*/
