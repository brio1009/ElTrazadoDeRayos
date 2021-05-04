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
#ifndef RAYTRACERLIB_MATERIALS_COLORMATERIAL_H_
#define RAYTRACERLIB_MATERIALS_COLORMATERIAL_H_


#include <glm/glm.hpp>
#include "./Material.h"
#include "./Color.h"
#include "./Constants.h"

// Forward declaration.
struct IntersectionInfo;

///
class ColorMaterial : public Material {
 public:
  /// Constructor.
  explicit ColorMaterial(const Color& color) : _color(color) { }

  /// Returns the color for the given position, normal and ray direction.
  virtual Color getColor(const IntersectionInfo& intersectionInfo,
                         const Ray& incomingRay,
                         const Scene& scene) const;

 private:
  /// Color member.
  Color _color;
};

#endif  // RAYTRACERLIB_MATERIALS_COLORMATERIAL_H_

