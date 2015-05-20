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
#ifndef RAYTRACERLIB_SCENE_H_
#define RAYTRACERLIB_SCENE_H_

#include <vector>
#include <unordered_set>
#include "./Color.h"

// Forward declaration.
class Camera;
class ImportantShape;
class Light;
class Ray;
class Shape;
struct IntersectionInfo;
namespace accelerationstructures {
class AbstractDataStructure;
}

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
  Color backgroundColor(const Ray& ray) const { return m_Background; }
  /// Returnes a reference to all the lights in the scene.
  std::vector<Light*>& lights() { return _lights; }
  /// Returnes a const reference to all the lights in the scene.
  const std::vector<Light*>& lights() const { return _lights; }

  /// Returnes a reference to all the cameras in the scene.
  std::vector<Camera*>& cameras() { return m_Cameras; }

  /// Renders the scene with all cameras in the camera vector.
  void render() const;

  /// Renders the scene with all cameras in the camera vector.
  void render(size_t chunks, size_t chunkNr) const;

  /// Inserts a shape into the shape vector (and maybe also into the
  /// important area shape list (for lighing).
  void addShape(Shape* shapePtr);

  /// Returns a reference to the important shape set.
  const std::vector<ImportantShape*>& importantShapes() const {
    return m_ImportantShapes;
  }

  void setBackgroundColor(const Color& c) {
    m_Background = c;
  }

 private:
  /// Holds the renderable objects.
  accelerationstructures::AbstractDataStructure* m_Shapes;

  /// Holds the the important shapes that should be sampled
  /// with the area form of the rendering equation.
  std::vector<ImportantShape*> m_ImportantShapes;

  /// Holds the lights in a scene.
  std::vector<Light*> _lights;

  /// Holds the cameras in this scene.
  std::vector<Camera*> m_Cameras;

  /// The background color.
  Color m_Background;
};

#endif  // RAYTRACERLIB_SCENE_H_
