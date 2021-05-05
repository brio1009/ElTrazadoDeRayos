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

#include <genericfactory/GenericFactory_decl.h>

#include <memory>

#include "./Image.h"
#include "./Spatial.h"
#include "factories/PropertyInterface.h"

// Foward declaration.
class Ray;
class Scene;
class Sampler;
class PostProcessor;

/// Camera class.
class Camera : public Spatial, public PropertyInterface<Camera> {
 public:
  /// Constructor.
  Camera(const int width, const int height);

  /// Copy constructor.
  explicit Camera(const Camera& camera);

  /// Virtual destructor.
  virtual ~Camera() {}

  /// Renders the scene to the image.
  virtual void render(const Scene& scene);
  /// Renders the scene with the camera from startPixel to endPixel - 1
  void render(const Scene& scene,
              const size_t& startPixel,
              const size_t& endPixel);

  /// This method creates a Ray for given px and py. (TopLeft most)
  virtual Ray createPixelCornerRay(const size_t& px,
                                   const size_t& py) const = 0;
  /// Returns the image.
  const Image& getImage() const;

  /// Sets the image size. Resets the image.
  virtual void setImageSize(const int width, const int height);

  /// Setter for m_Active.
  void setActive(const bool active) { m_Active = active; }

  /// Getter for m_Active.
  bool active() const { return m_Active; }

  /// Setter for m_UsePostProcessing.
  void setUsePostProcessing(const bool usePostProcessing) {
    m_UsePostProcessing = usePostProcessing;
  }

  /// Getter for m_UsePostProcessing.
  bool usePostProcessing() const { return m_UsePostProcessing; }

  void setWidth(size_t width) { _image = Image(width, _image.getHeight()); }

  void setHeight(size_t height) { _image = Image(_image.getWidth(), height); }

  void setRegularSampleSize(size_t samplesPerDim);

  static const char* name;

  static void registerProperties() {
    static bool m_lock(true);
    if (!m_lock)
      return;
    m_lock = false;
    genericfactory::GenericFactory<Camera>::registerProperty<Camera, REAL>(
        "X", &Camera::setX, &Camera::noGet);
    genericfactory::GenericFactory<Camera>::registerProperty<Camera, REAL>(
        "Y", &Camera::setY, &Camera::noGet);
    genericfactory::GenericFactory<Camera>::registerProperty<Camera, REAL>(
        "Z", &Camera::setZ, &Camera::noGet);
    genericfactory::GenericFactory<Camera>::registerProperty<Camera, glm::vec4>(
        "Rotate", &Camera::rotate, &Camera::noGet);
    genericfactory::GenericFactory<Camera>::registerProperty<Camera, size_t>(
        "W", &Camera::setWidth, &Camera::noGet);
    genericfactory::GenericFactory<Camera>::registerProperty<Camera, size_t>(
        "H", &Camera::setHeight, &Camera::noGet);
    // TODO(bauschp, Di 2. Sep 20:21:00 CEST 2014):
    // be able to change the sampler.
    genericfactory::GenericFactory<Camera>::registerProperty<Camera, size_t>(
        "SamplesPerDim", &Camera::setRegularSampleSize, &Camera::noGet);
  }

 protected:
  /// Sampler used by this camera.
  std::shared_ptr<Sampler> m_Sampler;
  /// Post processor used by this camera.
  std::shared_ptr<PostProcessor> m_PostProcessor;

  /// Boolean to specify if post processor should be used.
  bool m_UsePostProcessing;

  /// Image where the scene is rendered to.
  Image _image;

  /// Boolean to determine if this camera is currently active
  /// (meaning it should render an image when Scene::render()
  /// is called).
  bool m_Active;
};

#endif  // RAYTRACERLIB_CAMERAS_CAMERA_H_
