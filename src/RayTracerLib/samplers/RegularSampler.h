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
#ifndef RAYTRACERLIB_SAMPLERS_REGULARSAMPLER_H_
#define RAYTRACERLIB_SAMPLERS_REGULARSAMPLER_H_

#include <vector>

#include "./Color.h"
#include "./Ray.h"
#include "./Sampler.h"
class Scene;
/// This class is used to define a sampling based on a regular grid.
/// Corresponding Samples are in the rectanglular defined area.
class RegularSampler : public Sampler {
 public:
  /// Creates a new RegularSampler with given sample ammount per dimension
  explicit RegularSampler(const size_t& samplesPerDimension)
      : _samplesPerDimension(samplesPerDimension),
      _offset(0.5f / samplesPerDimension) { }
  /// Virtual destructor. (Override).
  virtual ~RegularSampler() { }
 protected:
  /// (Override)
  virtual std::vector<float> getLambdasForSample(
      const size_t& size) const;
  /// (Override)
  virtual Color reconstructColor(
      const std::vector<Color>& colors,
      const std::vector<float>& lambdas) const;
 private:
  const size_t _samplesPerDimension;
  const float _offset;
};
#endif  // RAYTRACERLIB_SAMPLERS_REGULARSAMPLER_H_

