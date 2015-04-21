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

#include "./UniformGrid.h"

#include <glm/glm.hpp>

#include <unordered_map>

#include "../IntersectionInfo.h"
#include "../shapes/Shape.h"
#include "../shapes/Mesh.h"
#include "../Ray.h"
#include "./AABB.h"

using glm::ivec3;
using glm::vec3;

namespace {
using accelerationstructures::AABB;

// Returns the cell index for a point in 3dspace.
ivec3 cellIndexOf(const vec3& point, const float& cellSize) {
  // Compute the cell index
  vec3 dividedPos = point / cellSize;
  return ivec3(floor(dividedPos.x), floor(dividedPos.y), floor(dividedPos.z));
}
void getIndecesForAABBInGrid(
    const AABB& aabb,
    ivec3* min,
    ivec3* max,
    const float& cellSize) {
  min->operator=(cellIndexOf(aabb.min, cellSize));
  max->operator=(cellIndexOf(aabb.max, cellSize));
}
}  // namespace

namespace accelerationstructures {

// _____________________________________________________________________________
UniformGrid::~UniformGrid() {
}
// _____________________________________________________________________________
IntersectionInfo UniformGrid::traceRay(const Ray& ray) const {
  return IntersectionInfo();
}
// _____________________________________________________________________________
void UniformGrid::insertShapeIntoMatchingCells(const AABB& aabb, Shape* shape) {
  ivec3 minCellIndex;
  ivec3 maxCellIndex;
  // Calculate the indeces.
  getIndecesForAABBInGrid(aabb, &minCellIndex, &maxCellIndex, m_CellSize);
  // Insert the shape into matching cells.
  // Order irrelevant cause we hash anyways.
  for (int x = minCellIndex.x; x < maxCellIndex.x; ++x) {
    for (int y = minCellIndex.y; y < maxCellIndex.y; ++y) {
      for (int z = minCellIndex.z; z < maxCellIndex.z; ++z) {
        ivec3 key = ivec3(x, y, z);
        auto it = m_Shapes.find(key);
        // Key is not in the map. Create a empty vector of shapes.
        if (it == m_Shapes.end()) {
          auto p = m_Shapes.insert({
              key,
              cellList()});
          if (!p.second) {
            perror("Abort insert: could not insert key");
            return;
          }
          it = p.first;
        }
        // If the Shape is a mesh add a pair for each triangle
        // intersecting the cell
        if (dynamic_cast<Mesh*>(shape)) {
          // TODO(bauschp): Implement Mesh handling.
        } else {
          it->second.push_back(std::make_pair(shape, 0));
        }
      }
    }
  }
}
// _____________________________________________________________________________
void UniformGrid::addShape(Shape* shape) {
  // The shape will be added to all cells that intersect its aabb.
  AABB aabb = aabbFromShape(*shape);
  insertShapeIntoMatchingCells(aabb, shape);
}
// _____________________________________________________________________________
size_t UniformGrid::size() const {
  return 0;
}
}  // namespace accelerationstructures
