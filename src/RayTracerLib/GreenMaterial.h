/*
The MIT License (MIT)

Copyright (c) 2014 CantTouchDis

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
#ifndef RAYTRACERLIB_GREENMATERIAL_H_
#define RAYTRACERLIB_GREENMATERIAL_H_


#include <glm/glm.hpp>
#include "./Material.h"
#include "./Color.h"
#include "./Constants.h"

class GreenMaterial : public Material {
 public:
  virtual Color getColor(const glm::vec4& position,
                         const glm::vec4& normal,
                         const Scene& scene) const {
    REAL scale = glm::dot(glm::normalize(normal), glm::normalize(glm::vec4(1, 1, -1, 0)));
    scale = (scale >= 0 ? scale : 0);
    // REAL scaleTwo = glm::dot(glm::normalize(normal), glm::vec4(-1, 0, 0, 0));
    // scaleTwo = (scaleTwo >= 0 ? scaleTwo : 0);
    // REAL scaleThree = glm::dot(glm::normalize(normal), glm::normalize(glm::vec4(1, -1, -1, 0)));
    // scaleThree = (scaleThree >= 0 ? scaleThree : 0);
    return Color(0, 255 * scale, 0, 0);
  }
};

#endif  // RAYTRACERLIB_GREENMATERIAL_H_

