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
#ifndef RAYTRACERLIB_SAMPLERS_ADAPTIVESAMPLER_H_
#define RAYTRACERLIB_SAMPLERS_ADAPTIVESAMPLER_H_
#include <vector>
#include "./Sampler.h"
class Color;
class Scene;
/// The adaptive Sampler returns a new sample whenever more samples are needed.
/// The maximum can be defined when constructing an object of this class.
class AdaptiveSampler : public Sampler {
 public:
  /// Creates a new AdaptiveSampler with fiven maximum Samples.
  explicit AdaptiveSampler(const size_t& maxSamples)
      : _maxSamples(maxSamples) {}
  /// Virtual destructor. (Override).
  virtual ~AdaptiveSampler() {}
  static float generateHalton(const size_t& index, const size_t& base);
  static float calculateVariance(const std::vector<Color>& colors);

 protected:
  /// (Override)
  virtual std::vector<float> getLambdasForSample(const size_t& size) const;
  /// (Override)
  virtual bool addNextLambdasToList(const std::vector<Color>& color,
                                    std::vector<float>* lambdas) const;
  /// (Override)
  virtual Color reconstructColor(const std::vector<Color>& colors,
                                 const std::vector<float>& lambdas) const;

 private:
  const size_t _maxSamples;
};
#endif  // RAYTRACERLIB_SAMPLERS_ADAPTIVESAMPLER_H_
