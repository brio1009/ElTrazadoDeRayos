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
#ifndef RAYTRACERLIB_SAMPLER_H_
#define RAYTRACERLIB_SAMPLER_H_

#include <vector>
#include <utility>
#include <unordered_map>
#include <map>
#include <mutex>
#include "./Ray.h"
#include "./Color.h"
/// This pure class is used to define all sampling approaches.
/// A Sampler is used to return Sample Rays for a given Ray/Position/everything.
/// For example a hemisphere sampler should take a ray and sample the hemisphere
/// in direction of the Ray.
class Sampler {
 public:
  /// Returnes a const reference to a sample constructed by the inherited class
  /// (e.g. Hemisphere/Bilinear Interpolation) with given vector of rays.
  /// This can throw an exception depending on inherited class when index is too
  /// large.
  virtual Ray* const getSample(
      const size_t& index,
      const std::vector<Ray>& rays) const;
  /// Reconstructs the overall color for given Rays and colors.
  /// TODO(bauschp, Thu Jun  5 14:15:16 CEST 2014): this should be easy to exch.
  /// This invalidates the given vector of Rays (clears it). And removes them
  /// from the internal rayLambda storage.
  virtual Color reconstructColor(
      std::vector<Ray*>* rays,
      const std::vector<Color>& colors) const;
  /// virtual Destructor.
  virtual ~Sampler() {
    auto last = _rayLambdas.end();
    for (auto it = _rayLambdas.begin(); it != last; ++it) {
      delete it->first;
    }
  }
 protected:
  /// This method creates the Sample with given lambda values.
  /// This could be a a bilinear interpolation.
  virtual Ray* createSample(
      const std::vector<Ray>& rays,
      const std::pair<float, float>& lambda) const = 0;
  /// Returnes the lambda values needed to create a sample.
  virtual std::pair<float, float> getNextLambda(
      const size_t& index) const = 0;
 private:
  /// This map holds the lambdas that produced the sample
  mutable std::unordered_map<Ray*, std::pair<float, float>> _rayLambdas;
  /// The mutex to enable threadsafe writing to the map.
  mutable std::mutex _rayLambdasMutex;
};
/*
/// Samples a area between four Rays into the scene.
// TODO(bauschp, Tue May 27 10:05:09 CEST 2014): better doc
class Sampler {
 public:
  /// Sets the pixel area that is then used to sample the area.
  void setPixelArea(const Ray& bottomLeft,
      const Ray& topLeft,
      const Ray& bottomRight,
      const Ray& topRight) {
    _bottomLeft = bottomLeft;
    _topLeft = topLeft;
    _bottomRight = bottomRight;
    _topRight = topRight;
    nextSampleConfiguration();
  }
  /// Moves the configuration. This is used if different sample configurations
  /// are used for different pixel areas. e.g. rotate samples in circle area.
  virtual void nextSampleConfiguration() {
    _sampleRays.clear();
  }
  /// Reconstructs a Color from given colores.
  /// the color at index i should be the color that was created with sample i.
  virtual Color reconstructColor(const std::vector<Color>& colors) {
    if (colors.size() != _sampleRays.size()) {
      // Returnes Red cause something went wrong while sampling.
      perror("SampleSize doesn`t correspond to given colors.");
      return Color(1, 0, 0, 0);
    }
    Color out(0, 0, 0);
    // Just average them by defualt.
    for (auto it = colors.begin(); it != colors.end(); ++it) {
      // Order of operations is of importence!!!!
      // TODO(gisslerc, Tue May 27 15:20:02 CEST 2014): Change/Add operator
      // override to be order invariant. (const correctness).
      out = out + ((1.0f /  colors.size()) * (*it));
    }
    return out;
  }
  /// Returnes the next SampleRay.
  virtual const Ray& nextSample() {
    // Creates the sample with index of the next sampleRay.
    _sampleRays.push_back(getSample(_sampleRays.size()));
    return _sampleRays.back();
  }

 protected:
  /// Returnes the Sample Ray for given Index. Different indices should
  /// provide different sample Rays. This throws an exception if index is too
  /// large for a sampler.
  virtual Ray getSample(const size_t& index) = 0;
  /// a vector that stores all Samples until nextSampleConfiguration is called.
  std::vector<Ray> _sampleRays;
  /// The Bottom Left corner of the sample area.
  Ray _bottomLeft;
  /// The Bottom Right corner of the sample area.
  Ray _bottomRight;
  /// The Top Left corner of the sample area.
  Ray _topLeft;
  /// The Top Right corner of the sample area.
  Ray _topRight;
};

*/
#endif  // RAYTRACERLIB_SAMPLER_H_
