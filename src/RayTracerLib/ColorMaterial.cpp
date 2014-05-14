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

#include <glm/glm.hpp>
#include "./Color.h"
#include "./ColorMaterial.h"
#include "./IntersectionInfo.h"
#include "./Ray.h"
#include "./Scene.h"

// _____________________________________________________________________________
Color ColorMaterial::getColor(const glm::vec4& position,
                              const glm::vec4& normal,
                              const Ray& incomingRay,
                              const Scene& scene) const {
  // Build the returncolor.
  Color returnColor(_color);

  // Check if we need to send another ray.
  if (_color.a() < 1.0f) {
    // Build a new ray.
    Ray newRay(position,
               incomingRay.dir);
    // Trace the ray.
    IntersectionInfo info = scene.traceRay(newRay);
    Color addColor;
    if (info.materialPtr) {
      addColor = info.materialPtr->getColor(info.hitPoint,
                                            info.normal,
                                            newRay,
                                            scene);
    }
    returnColor = (static_cast<double>(_color.a())) * _color
                  + (1.0 - static_cast<double>(_color.a())) * addColor;
  }

  return returnColor;
}
