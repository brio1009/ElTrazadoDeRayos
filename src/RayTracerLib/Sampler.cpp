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

#include "./Sampler.h"

#include <glm/gtc/constants.hpp>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <utility>

// Wow a lot of stl usings.
using std::vector;
using std::unordered_map;
using std::mutex;
using std::lock_guard;
using std::pair;

// _____________________________________________________________________________
Ray* const Sampler::getSample(
    const size_t& index,
    const vector<Ray>& rays) const {
  pair<float, float> lambda = getNextLambda(index);
  lock_guard<mutex> lock(_rayLambdasMutex);
  auto iterator = _rayLambdas.emplace(
        createSample(rays, lambda), lambda);
  // if this assertion fails an Ray didn`t get inserted.
  // This should never happen.
  assert(iterator.second);
  return iterator.first->first;
  // The lock will be released automaticly if we leave scope.
}
// _____________________________________________________________________________
Color Sampler::reconstructColor(
    vector<Ray*>* rays,
    const vector<Color>& colors) const {
  // Assert if the ammount of rays given is equal to the size of colors.
  assert(rays->size() == colors.size());
  // TODO(bauschp, Thu Jun  5 14:51:35 CEST 2014): calculate it.
  size_t rayCount = rays->size();
  Color out(0, 0, 0);
  float twoStandardDeviationSquared = 1.0f;
  lock_guard<mutex> lock(_rayLambdasMutex);
  for (size_t i = 0; i < rayCount; ++i) {
    pair<float, float> lambdas = _rayLambdas.at(rays->at(i));
    float x = lambdas.first - 0.5f;
    float y = lambdas.second - 0.5f;
    float scale = 1.0f / (glm::pi<float>() * twoStandardDeviationSquared);
    scale *= exp((x * x + y * y) / twoStandardDeviationSquared);
    out += scale * colors.at(i);
  }
  for (size_t i = 0; i < rayCount; ++i) {
    _rayLambdas.erase(rays->at(i));
    delete rays->at(i);
  }
  // The references are now invalid so clear the vector to prevent mistakes.
  rays->clear();
  return out;
  // The lock will be released automaticly if we leave scope.
}
