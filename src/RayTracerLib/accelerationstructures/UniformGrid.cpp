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
#include "../shapes/Triangle.h"
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
vec3 posOfIndex(const ivec3& index, const float& cellSize) {
  return vec3(cellSize * index.x, cellSize * index.y, cellSize * index.z);
}

AABB aabbOfTriangle(const std::vector<vec3>& vertices, const size_t& pos) {
  const vec3& v1 = vertices.at(pos);
  const vec3& v2 = vertices.at(pos + 1);
  const vec3& v3 = vertices.at(pos + 2);
  vec3 minVec(
      std::min(std::min(v1.x, v2.x), v3.x),
      std::min(std::min(v1.y, v2.y), v3.y),
      std::min(std::min(v1.z, v2.z), v3.z));
  vec3 maxVec(
      std::max(std::max(v1.x, v2.x), v3.x),
      std::max(std::max(v1.y, v2.y), v3.y),
      std::max(std::max(v1.z, v2.z), v3.z));
  return AABB {minVec, maxVec};
}
void getIndecesForAABBInGrid(
    const AABB& aabb,
    ivec3* min,
    ivec3* max,
    const float& cellSize) {
  min->operator=(cellIndexOf(aabb.min, cellSize));
  max->operator=(cellIndexOf(aabb.max, cellSize));
}
// Signum function.
template <typename T>
int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}
}  // namespace

namespace accelerationstructures {
// _____________________________________________________________________________
UniformGrid::UniformGrid(const float& cellSize) : m_CellSize(cellSize) {}
// _____________________________________________________________________________
UniformGrid::~UniformGrid() {
}
// _____________________________________________________________________________
void UniformGrid::intersectCellShapes(const glm::ivec3& index,
    const Ray& ray,
    std::vector<IntersectionInfo>* intersections) const {
  auto it = m_Shapes.find(index);
  if (it != m_Shapes.end()) {
    const size_t numShapes = it->second.size();
    for (size_t i = 0; i < numShapes; ++i) {
      const std::pair<Shape*, uint64_t>& current = it->second.at(i);
      Mesh* mesh;
      if ((mesh = dynamic_cast<Mesh*>(current.first))) {
        std::vector<size_t> hits;
        std::vector<REAL> t = intersectTriangles(ray,
            mesh->getVertices(),
            current.second, current.second + 3, &hits);
        if (t.size() == 0) {
          continue;
        }
        float tVal = t[0];
        glm::vec4 position = ray.origin()
                         + static_cast<float>(tVal) * ray.direction();
        const std::vector<vec3>& normals = mesh->getNormals();
        intersections->push_back(IntersectionInfo(tVal,
            position,
            glm::vec4(
              (normals[current.second]
              + normals[current.second + 1]
              + normals[current.second + 2]) / 3.0f, 0.0f),
            mesh->getMaterialPtr(),
            glm::vec2(0.0f)));
      } else {
        intersections->push_back(current.first->getIntersectionInfo(ray));
      }
    }
  }
}
// _____________________________________________________________________________
IntersectionInfo UniformGrid::traceRay(const Ray& ray) const {
  // initialize the indices.
  const glm::vec4& dir = ray.direction();
  const glm::vec4& origin = ray.origin();
  // current cell index.
  ivec3 index = cellIndexOf(ray.originVec3(), m_CellSize);
  // The end of the cell.
  vec3 maxTVal = (
      posOfIndex(ivec3(
        dir.x < 0 ? index.x : index.x + 1,
        dir.y < 0 ? index.y : index.y + 1,
        dir.z < 0 ? index.z : index.z + 1), m_CellSize)
      - vec3(origin)) / vec3(dir);
  // The distance from cell to cell in each direction in t.
  // The directions the indices are moving.
  ivec3 step(
      dir.x < 0 ? -1 : 1,
      dir.y < 0 ? -1 : 1,
      dir.z < 0 ? -1 : 1);
  vec3 deltaTValue = m_CellSize / vec3(fabs(dir.x), fabs(dir.y), fabs(dir.z));
  // printf("(%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f)\n",
  //     deltaTValue.x, deltaTValue.y, deltaTValue.z,
  //     dir.x, dir.y, dir.z);
  

  // maximal number of cells allowed.
  for (size_t i = 0; i < 2 * 1e2; ++i) {
    float minT = std::min(std::min(maxTVal.x, maxTVal.y), maxTVal.z);
    // Check for intersection.
    std::vector<IntersectionInfo> intersections;
    intersectCellShapes(index, ray, &intersections);
    auto endIt = intersections.end();
    IntersectionInfo* out = NULL;
    for (auto it = intersections.begin(); it != endIt; ++it) {
      // printf("Found intersections in (%d, %d, %d) t: %.4f/%.4f\n", index.x, index.y, index.z, it->t, minT);
      if (it->t < minT) {
        // printf("Found real intersections in (%d, %d, %d)\n", index.x, index.y, index.z);
        out = &(*it);
        minT = it->t;
      }
    }
    if (out) {
      return *out;
    }
    // Traverse.
    if (maxTVal.x < maxTVal.y) {
      if (maxTVal.x < maxTVal.z) {
        // X is minimal.
        index.x += step.x;
        maxTVal.x += deltaTValue.x;
      } else {
        // Z is minimal.
        index.z += step.z;
        maxTVal.z += deltaTValue.z;
      }
    } else {
      if (maxTVal.y < maxTVal.z) {
        // Y is minimal.
        index.y += step.y;
        maxTVal.y += deltaTValue.y;
      } else {
        // Z is minimal.
        index.z += step.z;
        maxTVal.z += deltaTValue.z;
      }
    }
  }
  return IntersectionInfo();
}
// _____________________________________________________________________________
void UniformGrid::insertShapeIntoMatchingCells(const AABB& aabb, Shape* shape) {
  ivec3 minCellIndex;
  ivec3 maxCellIndex;
  Mesh* mesh;
  std::vector<std::pair<ivec3, ivec3> > minMaxIndices;
  if ((mesh = dynamic_cast<Mesh*>(shape))) {
    // For meshs we loop multiple times.
    const std::vector<vec3>& vertices = mesh->getVertices();
    const size_t vertCount = vertices.size();
    minMaxIndices.reserve(vertCount / 3);
    // loop over all the faces.
    for (size_t i = 0; i < vertCount; i += 3) {
      // Get the indices of this face.
      getIndecesForAABBInGrid(
          aabbOfTriangle(vertices, i),
          &minCellIndex,
          &maxCellIndex,
          m_CellSize);
      
      // add the indices.
      minMaxIndices.push_back(std::make_pair(minCellIndex, maxCellIndex));
      // printf("Adding minMax (%d,%d,%d)(%d,%d,%d)\n", minCellIndex.x, minCellIndex.y, minCellIndex.z
    }
  } else {
    // Calculate the indeces.
    getIndecesForAABBInGrid(aabb, &minCellIndex, &maxCellIndex, m_CellSize);
    minMaxIndices.push_back(std::make_pair(minCellIndex, maxCellIndex));
  }
  // Insert the shape into matching cells.
  // Order irrelevant cause we hash anyways.
  for (size_t i = 0; i < minMaxIndices.size(); ++i) {
    minCellIndex = minMaxIndices.at(i).first;
    maxCellIndex = minMaxIndices.at(i).second;
    for (int x = minCellIndex.x; x <= maxCellIndex.x; ++x) {
      for (int y = minCellIndex.y; y <= maxCellIndex.y; ++y) {
        for (int z = minCellIndex.z; z <= maxCellIndex.z; ++z) {
          // printf("Shape in (%d,%d,%d)\n", x,y,z);
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
          it->second.push_back(std::make_pair(shape, 3 * i));
        }
      }
    }
  }
  for (auto it = m_Shapes.begin(); it != m_Shapes.end(); ++it) {
    printf("Key: (%d %d %d), Num elem: %lu)\n", it->first.x, it->first.y, it->first.z, it->second.size());
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
