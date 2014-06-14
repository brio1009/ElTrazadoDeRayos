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

#include <vector>

#include "./Color.h"
#include "./Ray.h"

using std::vector;

// _____________________________________________________________________________
std::vector<float> RegularSampler::getLambdasForSample(
      const size_t& index) const throw() { // NOLINT we are not google
  if (index >= _samplesPerDimension * _samplesPerDimension)
    throw 0;
  size_t x = index % _samplesPerDimension;
  size_t y = (index - x) / _samplesPerDimension;
  vector<float> lambdas;
  lambdas.reserve(2);
  lambdas.push_back(
      static_cast<float>(x) / _samplesPerDimension + _offset);
  lambdas.push_back(
      static_cast<float>(y) / _samplesPerDimension + _offset);
  return lambdas;
}
// _____________________________________________________________________________
Color RegularSampler::reconstructColor(
      const std::vector<Color>& colors,
      const std::vector<float>& lambdas) const {
  assert(colors.size() == _samplesPerDimension * _samplesPerDimension);
  // calculate how much a colors weight is. (RegularSampler = same for all)
  float scale = 1.0f / _samplesPerDimension;
  scale /= _samplesPerDimension;
  // printf("Scale: %.2f\r", scale);
  // printf("SizeColors: %zu\r", colors.size());
  Color out(0, 0, 0);
  auto end = colors.end();
  for (auto it = colors.begin(); it != end; ++it)
    out += scale * *it;
  return out;
}
