/*
The MIT License (MIT)

Copyright (c) 2015 CantTouchDis <bauschp@informatik.uni-freiburg.de>
Copyright (c) 2015 brio1009 <christoph1009@gmail.com>

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

#include "./AABB.h"

#include <limits>

#include "../shapes/Shape.h"
#include "../shapes/Box.h"
#include "../shapes/Ellipsoid.h"
#include "../FunctionTraits.h"

using std::function;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using accelerationstructures::AABB;

namespace {
// _____________________________________________________________________________
template <typename Func>
using arg0 = typename function_traits<Func>::template arg<0>;

// _____________________________________________________________________________
template<typename Func>
AABB dispatcher(const Shape& shape, Func&& f) {
  try {
    typename arg0<Func>::type t =
      dynamic_cast<typename arg0<Func>::type>(shape);
    return f(t);
  } catch (std::bad_cast error) {
    return AABB();
  }
}

// _____________________________________________________________________________
template<typename Func, typename... Args>
AABB dispatcher(const Shape& shape, Func&& f, Args&&... args) {
  try {
    typename arg0<Func>::type t =
      dynamic_cast<typename arg0<Func>::type>(shape);
    return f(t);
  } catch (std::bad_cast error) {
    return dispatcher<Args...>(shape, std::forward<Args>(args)...);
  }
}
// Returns the surrounding min and max pos of the boxes
// extent centered around (0/0/0) transformed by trans.
inline void minMaxOfTransformedSymetricBox(
    const vec4& pos,
    vec4* maxPos,
    vec4* minPos,
    const mat4& trans) {
  for (int i = 0; i < 8; ++i) {
    vec4 ithPos;
    ithPos.w = 1.0f;
    // utilize symetry to create all vertices.
    ithPos.x = pos.x * ((i & 4) == 4 ? -1.0f : 1.0f);
    ithPos.y = pos.y * ((i & 2) == 2 ? -1.0f : 1.0f);
    ithPos.z = pos.z * ((i & 1) == 1 ? -1.0f : 1.0f);
    // Transform to global reference frame.
    vec4 transformedITHPos = trans * ithPos;
    // Update min and max pos.
    // 4th coordinate is always 1.0. (see Box.cpp)
    for (int i = 0; i < 3; ++i) {
      if (transformedITHPos[i] < minPos->operator[](i))
        minPos->operator[](i) = transformedITHPos[i];
      if (transformedITHPos[i] > maxPos->operator[](i))
        maxPos->operator[](i) = transformedITHPos[i];
    }
  }
}

// _____________________________________________________________________________
AABB boxAABB(const Box& box) {
  const glm::mat4& trans = box.getTransformMatrix();
  vec4 pos = box.getMaxPosition();
  vec4 maxPos(std::numeric_limits<float>::min());
  vec4 minPos(std::numeric_limits<float>::max());
  minMaxOfTransformedSymetricBox(pos, &maxPos, &minPos, trans);
  // Return a AABB with the minimal and maximal positions.
  return {vec3(minPos), vec3(maxPos)};
}
// _____________________________________________________________________________
AABB ellipsoidAABB(const Ellipsoid& ellipsoid) {
  const glm::mat4& trans = ellipsoid.getTransformMatrix();
  vec4 pos = vec4(ellipsoid.radii(), 1.0f);
  vec4 maxPos(std::numeric_limits<float>::min());
  vec4 minPos(std::numeric_limits<float>::max());
  // we treat the ellipsoid as box. (overestimate its bounding box)
  minMaxOfTransformedSymetricBox(pos, &maxPos, &minPos, trans);
  // Return a AABB with the minimal and maximal positions.
  return {vec3(minPos), vec3(maxPos)};
}

AABB fallbackAABB(const Shape& shape) {
  return AABB();
}
}  // namespace

namespace accelerationstructures {
// _____________________________________________________________________________
AABB aabbFromShape(const Shape& shape) {
  return dispatcher(shape, boxAABB, ellipsoidAABB, fallbackAABB);
}
}  // accelerationstructures
