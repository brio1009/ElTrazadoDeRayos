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
#ifndef RAYTRACERLIB_CAMERA_H_
#define RAYTRACERLIB_CAMERA_H_

#include "./Image.h"
#include "./Object.h"

// Foward declaration.
class Scene;

class Camera : public Object {
 public:
  /// Constructor.
  Camera(const int width, const int height);
  /// Renders the scene to the image.
  virtual void render(const Scene& scene) = 0;

  /// Returns the image.
  const Image& getImage() const;

  /// Sets the image size. Resets the image.
  virtual void setImageSize(const int width, const int height);

 protected:
  /// Image where the scene is rendered to.
  Image _image;
};

#endif  // RAYTRACERLIB_CAMERA_H_