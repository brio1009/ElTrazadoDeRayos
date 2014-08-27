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

#include "shapes/Shape.h"
#include "materials/Material.h"

// ALL DECLARATIONS NEED TO BE INCLUDED HERE.
// TODO(Mi 27. Aug 16:26:28 CEST 2014, bauschp): move to cpp.
#include "shapes/Rectangle.h"
#include "materials/MonteCarloMaterial.h"
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
  GenericFactory<Shape>::registerClass<Shape>();
  GenericFactory<Shape>::registerClass<Rectangle>();
  GenericFactory<Shape>::registerClass<AreaShape<Rectangle> >();
  return '1';
}
template<>
inline char GenericFactory<Material>::registerAllForBase() {
  GenericFactory<Material>::registerClass<Material>();
  GenericFactory<Material>::registerClass<MonteCarloMaterial>();
  return '1';
}

// Instantiate GenericFactory.
template class GenericFactory<Shape>;
template class GenericFactory<Material>;
}  // namespace genericfactory
#endif  // RAYTRACERLIB_PARSER_REFLECTION_H_
