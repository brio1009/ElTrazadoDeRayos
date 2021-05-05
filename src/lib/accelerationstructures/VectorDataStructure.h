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

#ifndef RAYTRACERLIB_ACCELERATIONSTRUCTURES_VECTORDATASTRUCTURE_H_
#define RAYTRACERLIB_ACCELERATIONSTRUCTURES_VECTORDATASTRUCTURE_H_

#include <cstdlib>
#include <vector>
#include "./AbstractDataStructure.h"

class Ray;
class Shape;
struct IntersectionInfo;

namespace accelerationstructures {
/// This is the most simple way to traverse through the data. It is also the
/// slowest.
class VectorDataStructure : public AbstractDataStructure {
 public:
  explicit VectorDataStructure(std::vector<Shape*>&& shapes);
  VectorDataStructure();
  virtual ~VectorDataStructure() override;
  /// Traverses through the data structure and returns the IntersectionInfo.
  virtual IntersectionInfo traceRay(const Ray& ray) const override;
  /// Adds the Shape to the vector.
  virtual void addShape(Shape* shape) override;
  /// Returns the number of shapes in this structure.
  virtual size_t size() const override;

 private:
  std::vector<Shape*> m_Shapes;
};
}  // namespace accelerationstructures
#endif  // RAYTRACERLIB_ACCELERATIONSTRUCTURES_VECTORDATASTRUCTURE_H_
