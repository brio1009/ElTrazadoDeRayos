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


#pragma once
#ifndef RAYTRACERLIB_ACCELERATIONSTRUCTURES_UNIFORMGRID_H_
#define RAYTRACERLIB_ACCELERATIONSTRUCTURES_UNIFORMGRID_H_

#include <glm/glm.hpp>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <set>
#include <cstdint>
#include <utility>

#include "./AbstractDataStructure.h"


#define PRIME1 73856093
#define PRIME2 19349663
#define PRIME3 83492791
#define HASHTABLESIZE 786433

class Shape;

struct SmartHash {
  size_t operator()(const glm::ivec3& vec) const {
    return ((vec.x * PRIME1) ^ (vec.y * PRIME2) ^ (vec.z * PRIME3)) % HASHTABLESIZE;
  }
};

namespace accelerationstructures {

struct AABB;

class UniformGrid : public AbstractDataStructure {
 public:
  UniformGrid(const float& cellSize);
  virtual ~UniformGrid() override;
  /// see AbstractDataStructure.
  virtual IntersectionInfo traceRay(const Ray& ray) const override;
  /// see AbstractDataStructure.
  virtual void addShape(Shape* shape) override;
  /// Returns the number of shapes contained in this structure.
  virtual size_t size() const override;
 private:
  void insertShapeIntoMatchingCells(const AABB& aabb, Shape* shape);
  void intersectCellShapes(const glm::ivec3& index,
      const Ray& ray,
      std::vector<IntersectionInfo>* intersections) const;
  /// contains a map cellHash -> primitives. If the tuples shape is a Mesh
  /// the second element provides the offset to the first triangle in the list
  /// of vertices.
  typedef std::vector<std::pair<Shape*, uint64_t> > cellList;
  std::unordered_map<glm::ivec3,
    cellList, SmartHash> m_Shapes;
  /// A list of all added Shapes.
  std::set<Shape*> m_ShapeList;
  /// A list of all infinite shapes.
  std::set<Shape*> m_InfShapeList;

  /// The dimensionality of a cell. (All cells are cubical)
  float m_CellSize;
};
}  // namespace accelerationstructures

#endif  // RAYTRACERLIB_ACCELERATIONSTRUCTURES_UNIFORMGRID_H_

