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

#include "./AreaShape.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include "../samplers/AdaptiveSampler.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Ellipsoid.h"
#include "../IntersectionInfo.h"


// Implementation for Rectangle.
// _____________________________________________________________________________
template<>
size_t AreaShape<Rectangle>::numSamples() const {
  return 1;
}

// _____________________________________________________________________________
template<>
glm::vec4 AreaShape<Rectangle>::getSample(const size_t sampleNr) const {
  glm::vec3 rectPoint = (AdaptiveSampler::generateHalton(sampleNr, 2) - 0.5f)
                          * glm::vec3(2.0f * extent().x, 0, 0)
                        + (AdaptiveSampler::generateHalton(sampleNr, 3) - 0.5f)
                          * glm::vec3(0, 0, 2.0f * extent().y);
  // return getTransformMatrix() * glm::vec4(rectPoint, 1);
  return getPosition();
}

// _____________________________________________________________________________
template<>
float AreaShape<Rectangle>::area() const {
  // Times 4 because extent is radius.
  return this->extent().x * this->extent().y * 4;
}

// Implementation for Ellipsoid.
// _____________________________________________________________________________
template<>
size_t AreaShape<Ellipsoid>::numSamples() const {
  return 20;
}

// _____________________________________________________________________________
template<>
glm::vec4 AreaShape<Ellipsoid>::getSample(const size_t sampleNr) const {
  // TODO(bauschp, Mon Jul 28 11:33:37 CEST 2014): fix so ellipsoids are sampled
  // the right way.
  return getTransformMatrix() *
        glm::vec4(glm::sphericalRand(this->radii().x), 1.0f);
}

// _____________________________________________________________________________
template<>
float AreaShape<Ellipsoid>::area() const {
  // Approximated according to
  // http://en.wikipedia.org/wiki/Ellipsoid#Surface_area
  float p = 1.6075f;
  float a = this->radii().x;
  float b = this->radii().y;
  float c = this->radii().z;
  return 4.0f * constants::PI * pow((((pow(a, p) * pow(b, p))
                                       + (pow(a, p) * pow(c, p))
                                       + (pow(b, p) * pow(c, p))) / 3.0f),
                                     1.0f / p);
}

