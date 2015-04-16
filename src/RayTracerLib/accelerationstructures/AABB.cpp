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

#include <functional>

#include "../shapes/Shape.h"
#include "../shapes/Box.h"
#include "../shapes/Ellipsoid.h"
#include "../FunctionTraits.h"

using std::function;

namespace {
  using accelerationstructures::AABB;
  template <typename Func>
  using arg0 = typename function_traits<Func>::template arg<0>;

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

  AABB boxAABB(const Box& box) {
    const glm::mat4& trans = box.getTransformMatrix();
    glm::vec4 positions[8];
    positions[0] = box.getMaxPosition();
    for (int i = 1; i < 8; ++i) {
      positions[i].x = positions[0].x * (i % 2 == 0 ? 1.0f : -1.0f);
      positions[i].y = positions[0].y * (i % 3 == 0 ? 1.0f : -1.0f);
      positions[i].z = positions[0].z * (i % 4 == 0 ? 1.0f : -1.0f);
    }
    perror("Called Box\n");
    return AABB();
  }
  AABB ellipsoidAABB(const Ellipsoid& box) {
    const glm::mat4& trans = box.getTransformMatrix();
    perror("Called Ellipsoid\n");
    return AABB();
  }
}  // namespace

namespace accelerationstructures {
// _____________________________________________________________________________
AABB aaabbFromShape(const Shape& shape) {
  return dispatcher(shape, boxAABB, ellipsoidAABB);
}
}  // accelerationstructures
