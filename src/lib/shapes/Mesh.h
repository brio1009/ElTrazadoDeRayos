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
#ifndef RAYTRACERLIB_SHAPES_MESH_H_
#define RAYTRACERLIB_SHAPES_MESH_H_

// C
#include <glm/glm.hpp>
// C++
#include <vector>
#include <string>

// Project
#include "./Shape.h"
#include "./Constants.h"

class Mesh : public Shape {
 public:
  Mesh();

  void loadObjFromFile(const std::string& filename);
  IntersectionInfo getIntersectionInfo(
      const Ray& ray,
      const REAL minimumT,
      const REAL maximumT) const override;
  /// Intersection test.
  virtual std::vector<REAL> intersect(const Ray& ray) const override;
  const std::vector<glm::vec3>& getVertices() const;
  const std::vector<glm::vec3>& getNormals() const;

 protected:
  virtual glm::vec4 getNormalAt(const glm::vec4& p) const;
 private:
  std::vector<glm::vec3> m_Vertices;
  std::vector<glm::vec3> m_Normals;
  std::vector<glm::vec2> m_UVCoords;
};

#endif  // RAYTRACERLIB_SHAPES_MESH_H_


