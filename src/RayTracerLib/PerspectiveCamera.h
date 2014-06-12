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

#ifndef RAYTRACERLIB_PERSPECTIVECAMERA_H_
#define RAYTRACERLIB_PERSPECTIVECAMERA_H_

#include "./Constants.h"
#include "./Camera.h"

class PerspectiveCamera : public Camera {
 private:
  /// Variables to store the field of view angle.
  REAL _focalLength;

 public:
  // Constructor.
  PerspectiveCamera(const int width,
      const int height,
      const REAL& fovAngleInRad);

  /// This overrides Object::transform(...)
  virtual void transform(const glm::mat4& matrix);  //NOLINT mistaken for std
  /// This defines Camera::create...
  virtual Ray createPixelCornerRay(
      const size_t& px,
      const size_t& py) const;

  // Destructor.
  virtual ~PerspectiveCamera();

};

#endif  // RAYTRACERLIB_PERSPECTIVECAMERA_H_
