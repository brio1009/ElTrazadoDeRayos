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
#include "./AdaptiveSampler.h"
#include <glm/glm.hpp>
#include <vector>
#include "../Color.h"

using std::vector;
// _____________________________________________________________________________
bool AdaptiveSampler::addNextLambdasToList(const std::vector<Color>& color,
                                           std::vector<float>* lambdas) const {
  // Check if we need to send more Samples.
  if (color.size() > 4 && calculateVariance(color) < 1e-2)
    return false;
  return Sampler::addNextLambdasToList(color, lambdas);
}
// _____________________________________________________________________________
float AdaptiveSampler::calculateVariance(const vector<Color>& colors) {
  size_t n = colors.size();
  glm::vec3 sum = glm::vec3(0, 0, 0);
  glm::vec3 sumSquared = glm::vec3(0, 0, 0);
  for (size_t i = 0; i < n; ++i) {
    Color c = colors.at(i);
    glm::vec3 cv = glm::vec3(c.r(), c.g(), c.b());
    sum += cv;
    sumSquared += cv * cv;
  }
  glm::vec3 variance =
      (sumSquared - (sum * sum) * (1.0f / n)) * (1.0f / (n - 1));
  return variance[0] + variance[1] + variance[2];
}
// _____________________________________________________________________________
vector<float> AdaptiveSampler::getLambdasForSample(const size_t& index) const {
  // Don't produce more then requested.
  if (index >= _maxSamples)
    return vector<float>();
  vector<float> lambdas;
  lambdas.reserve(2);
  lambdas.push_back(generateHalton(index, 2));
  lambdas.push_back(generateHalton(index, 3));
  return lambdas;
}
// _____________________________________________________________________________
float AdaptiveSampler::generateHalton(const size_t& index, const size_t& base) {
  float ret = 0;
  float f = 1.0f / base;
  int i = index;
  while (i > 0.0f) {
    ret += f * (i % base);
    i = static_cast<int>(floor(i / base));
    f /= base;
  }
  return ret;
}
// #define ShowSampleCount
// _____________________________________________________________________________
Color AdaptiveSampler::reconstructColor(
    const std::vector<Color>& colors,
    const std::vector<float>& lambdas) const {
  Color out(0, 0, 0);
  for (size_t i = 0; i < colors.size(); ++i)
    out += colors.at(i);
  out *= 1.0f / colors.size();
#ifdef ShowSampleCount
  return Color(1, 1, 1) * ((1.0f * colors.size()) / _maxSamples);
#else
  return out;
#endif  // ShowSampleCount
}
