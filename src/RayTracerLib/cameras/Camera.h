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
#ifndef RAYTRACERLIB_CAMERAS_CAMERA_H_
#define RAYTRACERLIB_CAMERAS_CAMERA_H_

#include <memory>

#include "./Image.h"
#include "./Spatial.h"

// Foward declaration.
class Ray;
class Scene;
class Sampler;

class Camera : public Spatial {
 public:
  /// Constructor.
  Camera(const int width, const int height);

  /// Copy constructor.
  explicit Camera(const Camera& camera);

  /// Virtual destructor.
  virtual ~Camera() { }

  /// Renders the scene to the image.
  virtual void render(
      const Scene& scene);
  /// Renders the scene with the camera from startPixel to endPixel - 1
  void render(const Scene& scene,
      const size_t& startPixel,
      const size_t& endPixel);

  /// This method creates a Ray for given px and py. (TopLeft most)
  virtual Ray createPixelCornerRay(
      const size_t& px,
      const size_t& py) const = 0;
  /// Returns the image.
  const Image& getImage() const;

  /// Sets the image size. Resets the image.
  virtual void setImageSize(const int width, const int height);

  /// Setter for m_Active.
  void setActive(const bool active) { m_Active = active; }

  /// Getter for m_Active.
  bool active() const { return m_Active; }

 protected:
  /// Sampler used by this camera.
  std::shared_ptr<Sampler> m_Sampler;
  /// Image where the scene is rendered to.
  Image _image;
  /// Boolean to determine if this camera is currently active
  /// (meaning it should render an image when Scene::render()
  /// is called).
  bool m_Active;
};

#endif  // RAYTRACERLIB_CAMERAS_CAMERA_H_
