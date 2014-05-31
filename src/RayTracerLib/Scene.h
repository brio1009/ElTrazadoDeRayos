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
#ifndef RAYTRACERLIB_SCENE_H_
#define RAYTRACERLIB_SCENE_H_

#include <vector>
#include "./Color.h"

// Forward declaration.
class Ray;
class Shape;
class Light;
struct IntersectionInfo;

/// Holds the scene objects.
class Scene {
 public:
  /// Constructor.
  Scene();
  /// Destructor.
  ~Scene();
  /// Traces a ray through the scene.
  IntersectionInfo traceRay(const Ray& ray) const;
  /// Returns a color for the scene background.
  Color backgroundColor(const Ray& ray) const { return Color(0.7f,
                                                             0.8f,
                                                             0.92f); }
  /// Returnes a reference to all the lights in the scene.
  const std::vector<Light*>& lights() const { return _lights; }
 private:
  /// Initializes the default scene.
  void defaultScene();
  /// Holds the renderable objects.
  std::vector<Shape*> _shapes;
  /// Holds the lights in a scene.
  std::vector<Light*> _lights;
};

#endif  // RAYTRACERLIB_SCENE_H_
