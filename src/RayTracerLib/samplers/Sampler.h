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
#ifndef RAYTRACERLIB_SAMPLERS_SAMPLER_H_
#define RAYTRACERLIB_SAMPLERS_SAMPLER_H_

#include <vector>

#include "./Ray.h"
#include "./Color.h"
// forward declaration.
class Scene;
/// This class provides access to get a sampled color. (e.g. RegularSampler
/// would use a regular grid to sample.
class Sampler {
 public:
  /// Returnes the sampled color of the given area. The area is / defined by
  /// the Polygon connecting the origins of the Rays and spans over the
  /// directions. If they cross always the bigger volume is considered.
  virtual Color getSampledColor(
      const std::vector<Ray>& borders,
      const Scene& scene) const;
  /// Virtual destructor.
  virtual ~Sampler() { }

 protected:
// TODO(bauschp, Tue Jun 10 20:19:16 CEST 2014):
// For adaptive sampling we might need to give the colors.
  /// This methos calculates the next index of the sample and proceeds, to add
  /// its corresponding lambda values to the back of the given lambdas list.
  virtual bool addNextLambdasToList(
      std::vector<float>* lambdas) const;
// TODO(bauschp, Tue Jun 10 20:10:38 CEST 2014):
// All these protected methodes could be put in their own classes and should be
// member of this class to reduce the responsability of this class.
  /// This should Return a Ray that is created by given lambdas and borders.
  /// In most cases this should be a linear/multilinear interpolation.
  /// Therefor the default is a bilinear interpolation for two lambda values and
  /// between four Rays. Start and end are used to identify the borders of the
  /// used lambdas. With start being the first index and end being the last
  /// index (points to the last lambda) used to create the Ray.
  virtual Ray createRayByLambdas(
      const std::vector<float>& lambdas,
      const size_t& start,
      const size_t& end,
      const std::vector<Ray>& borders) const;
  /// This returnes the lambdas to create a Ray from. e.g. Regular sampler would
  /// return two lambdas per sample which then can be used to construct
  /// a Ray via bilinear Interpolation. Returns a empty list if no other sample
  // can be created
  virtual std::vector<float> getLambdasForSample(
      const size_t& size) const = 0;
  /// Reconstructes the returned color values with given lambda values.
  virtual Color reconstructColor(
      const std::vector<Color>& colors,
      const std::vector<float>& lambdas) const = 0;
};
#endif  // RAYTRACERLIB_SAMPLERS_SAMPLER_H_
