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

#ifndef RAYTRACERLIB_PARSER_REFLECTION_H_
#define RAYTRACERLIB_PARSER_REFLECTION_H_

#include <genericfactory/GenericFactory_decl.h>
#include <genericfactory/Property.h>

#include <string>

#include "cameras/Camera.h"
#include "shapes/Shape.h"
#include "materials/Material.h"

// ALL DECLARATIONS NEED TO BE INCLUDED HERE.
// TODO(Mi 27. Aug 16:26:28 CEST 2014, bauschp): move to cpp.
#include "shapes/Rectangle.h"
#include "shapes/Box.h"
#include "shapes/Ellipsoid.h"
#include "materials/MonteCarloMaterial.h"
#include "cameras/PerspectiveCamera.h"
#include "lights/AreaShape.h"

namespace genericfactory {
template <>
inline std::string StringCastHelper<bool>::toString(const bool& value) {
  return std::to_string(value);
}

template <>
inline bool StringCastHelper<bool>::fromString(const std::string& value) {
  return std::stoi(value) == 1;
}
template <>
inline std::string StringCastHelper<float>::toString(const float& value) {
  return std::to_string(value);
}

template <>
inline float StringCastHelper<float>::fromString(const std::string& value) {
  return std::stof(value);
}

template <>
inline std::string StringCastHelper<double>::toString(const double& value) {
  return std::to_string(value);
}

template <>
inline double StringCastHelper<double>::fromString(const std::string& value) {
  return std::stod(value);
}

template <>
inline std::string StringCastHelper<size_t>::toString(const size_t& value) {
  return std::to_string(value);
}

template <>
inline size_t StringCastHelper<size_t>::fromString(const std::string& value) {
  return std::stoul(value);
}

template <>
inline std::string StringCastHelper<glm::vec4>::toString(
      const glm::vec4& value) {
  char buffer[255];
#ifdef WINDOWS
  size_t length = _snprintf_s(buffer, sizeof(buffer), "%.4f,%.4f,%.4f,%.4f",
#else
  size_t length = snprintf(buffer, sizeof(buffer), "%.4f,%.4f,%.4f,%.4f",
#endif
      value[0],
      value[1],
      value[2],
      value[3]);
  return std::string(buffer, 0, length);
}

template <>
inline glm::vec4 StringCastHelper<glm::vec4>::fromString(
      const std::string& value) {
  size_t comma = value.find(",");
  size_t start = 0;
  glm::vec4 out;
  out.x = std::stof(value.substr(start, comma - start));
  start = comma + 1;
  comma = value.find(",", start);
  out.y = std::stof(value.substr(start, comma - start));
  start = comma + 1;
  comma = value.find(",", start);
  out.z = std::stof(value.substr(start, comma - start));
  start = comma + 1;
  out[3] = std::stof(value.substr(start));
  return out;
}

template <>
inline std::string StringCastHelper<Material const*>::toString(
      const Material* const & value) {
  std::string result = std::to_string(reinterpret_cast<size_t>(value));
  return result;
}

template <>
inline Material const* StringCastHelper<Material const*>::fromString(
      const std::string& value) {
  Material const* result =
        reinterpret_cast<Material* const>(std::stoull(value));
  return result;
}
template<>
inline char GenericFactory<Shape>::registerAllForBase() {
  printf("Adding Shapes\n");
  GenericFactory<Shape>::registerClass<Shape>();
  // rectangles.
  GenericFactory<Shape>::registerClass<Rectangle>();
  GenericFactory<Shape>::registerClass<AreaShape<Rectangle> >();
  // Ellipsoids.
  GenericFactory<Shape>::registerClass<Ellipsoid>();
  GenericFactory<Shape>::registerClass<AreaShape<Ellipsoid> >();
  // Boxes.
  GenericFactory<Shape>::registerClass<Box>();
  GenericFactory<Shape>::registerClass<AreaShape<Box> >();
  printf("Done adding Shapes\n");
  return '1';
}
template<>
inline char GenericFactory<Material>::registerAllForBase() {
  printf("Adding Materials\n");
  GenericFactory<Material>::registerClass<Material>();
  GenericFactory<Material>::registerClass<MonteCarloMaterial>();
  printf("Done adding Materials\n");
  return '1';
}
template<>
inline char GenericFactory<Camera>::registerAllForBase() {
  printf("Adding Cameras\n");
  GenericFactory<Camera>::registerClass<Camera>();
  GenericFactory<Camera>::registerClass<PerspectiveCamera>();
  printf("Done adding Cameras\n");
  return '1';
}

// Instantiate GenericFactory.
template class GenericFactory<Shape>;
template class GenericFactory<Material>;
template class GenericFactory<Camera>;
}  // namespace genericfactory
#endif  // RAYTRACERLIB_PARSER_REFLECTION_H_
