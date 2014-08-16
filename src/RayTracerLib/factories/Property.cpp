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

#include "./Property.h"

#include <string>

class Material;

// Specializations for StringCastHelper.
template <>
std::string StringCastHelper<double>::toString(const double& value) {
  return std::to_string(value);
}

template <>
double StringCastHelper<double>::fromString(const std::string& value) {
  return std::stod(value);
}

template <>
std::string StringCastHelper<float>::toString(const float& value) {
  return std::to_string(value);
}

template <>
float StringCastHelper<float>::fromString(const std::string& value) {
  return std::stod(value);
}

template <>
std::string StringCastHelper<bool>::toString(const bool& value) {
  return std::to_string(value);
}

template <>
bool StringCastHelper<bool>::fromString(const std::string& value) {
  return std::stoi(value) == 1;
}

template <>
std::string StringCastHelper<Material const*>::toString(
      const Material* const & value) {
  std::string result = std::to_string(reinterpret_cast<size_t>(value));
  return result;
}

template <>
Material const* StringCastHelper<Material const*>::fromString(
      const std::string& value) {
  Material const* result =
        reinterpret_cast<Material* const>(std::stoull(value));
  return result;
}
