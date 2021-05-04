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
#ifndef RAYTRACERLIB_SAMPLERS_JITTEREDSAMPLER_H_
#define RAYTRACERLIB_SAMPLERS_JITTEREDSAMPLER_H_

#include <vector>
#include "./Sampler.h"
/// Samples a area between three Rays into the scene.
// TODO(bauschp, Tue May 27 10:05:09 CEST 2014): better doc
class JitteredSampler : public Sampler {
 public:
  /// Creates a new JitteredSampler with given SampleSize
  explicit JitteredSampler(const size_t& sampleDimension)
      : _sampleDimension(sampleDimension) { }
  /// Virtual destructor. (Override).
  virtual ~JitteredSampler() { }
 protected:
  /// (Override)
  virtual std::vector<float> getLambdasForSample(
      const size_t& size) const;
  /// (Override)
  virtual Color reconstructColor(
      const std::vector<Color>& colors,
      const std::vector<float>& lambdas) const;
 private:
  const size_t _sampleDimension;
};
#endif  // RAYTRACERLIB_SAMPLERS_JITTEREDSAMPLER_H_

