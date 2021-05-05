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

#ifndef RAYTRACERLIB_RAYINFO_H_
#define RAYTRACERLIB_RAYINFO_H_

#include <stack>
#include "./Constants.h"

/// This truct holds additional data for the ray.
class RayInfo {
 public:
  /// Constructor.
  RayInfo() : depth(0), colorContribution(1) {}

  /// Remove the top element from the refractiveIndex stack.
  void popRefractiveIndex() {
    if (!_refractiveIndex.empty())
      _refractiveIndex.pop();
  }

  /// Get the top element from the refractive Index stack.
  float topRefractiveIndex() const {
    if (_refractiveIndex.empty()) {
      return RefractiveIndex::air;
    }
    return _refractiveIndex.top();
  }

  /// Push back a new refractive Index.
  void pushRefractiveIndex(const float value) { _refractiveIndex.push(value); }

  std::stack<float> copyStack() const { return _refractiveIndex; }

  void setRefractionStack(const std::stack<float>& stack) {
    _refractiveIndex = stack;
  }

  /// The number of the ray since the camera. This helps us to prevent
  /// too long recursive ray traversal. The first ray has therefore depth
  /// 0.
  unsigned char depth;

  /// Contribution of this ray to the final pixel color. Use as stopping
  /// criteria for path tracing.
  REAL colorContribution;

 private:
  /// The refractive index at the origin of the ray.
  std::stack<float> _refractiveIndex;
};

#endif  // RAYTRACERLIB_RAYINFO_H_
