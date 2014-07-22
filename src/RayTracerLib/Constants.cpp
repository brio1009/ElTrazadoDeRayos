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

#include "./Constants.h"

#include <cstdint>
#include <limits>

namespace RefractiveIndex {
  const float glass = 1.52f;
  const float water = 1.333f;
  const float air = 1.00293f;
  const float diamond = 2.42f;
  const float mirror = std::numeric_limits<float>::max();
};

namespace constants {
  // ___________________________________________________________________________
  const double EPSILON = 1e-8;

  // ___________________________________________________________________________
  const double TEPSILON = 1e-2;

  // ___________________________________________________________________________
  const unsigned char maxDepth = 4;

  // ___________________________________________________________________________
  const REAL minColorContribution = 0.001;

  // ___________________________________________________________________________
  const REAL PI = 3.1415926535897932;
  // ___________________________________________________________________________
  const uint64_t DefaultSamplesPerDim = 10;
}  // namespace constants
