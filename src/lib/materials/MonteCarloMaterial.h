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
#ifndef RAYTRACERLIB_MATERIALS_MONTECARLOMATERIAL_H_
#define RAYTRACERLIB_MATERIALS_MONTECARLOMATERIAL_H_

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include "./Color.h"
#include "./Constants.h"
#include "materials/Material.h"


// Forward declaration.
class BRDF;

/// Phong material that shades the object with the phong reflection model.
class MonteCarloMaterial : public Material {
  GETSET(REAL, m_Color.r(), R)
  GETSET(REAL, m_Color.g(), G)
  GETSET(REAL, m_Color.b(), B)

 public:
  /// Constructor. Randoms a color.
  MonteCarloMaterial();

  /// Constructor.
  explicit MonteCarloMaterial(const Color& color);

  /// Constructor.
  MonteCarloMaterial(const Color& color, std::shared_ptr<BRDF> brdf);

  /// Returns the color for the given position, normal and ray direction.
  virtual Color getColor(const IntersectionInfo& intersectionInfo,
                         const Ray& incomingRay,
                         const Scene& scene) const;
  virtual void setColor(const float r, const float g, const float b) {
    m_Color = Color(r, g, b);
  }

  /// Name of the material used to serialize/deserialize.
  static const char* name;

  MonteCarloMaterial* create() const { return new MonteCarloMaterial(); }
  static void registerProperties() {
    static bool m_lock(true);
    if (!m_lock)
      return;
    m_lock = false;
    genericfactory::GenericFactory<Material>::registerProperty<
        MonteCarloMaterial, REAL>("R", &MonteCarloMaterial::setR,
                                  &MonteCarloMaterial::getR);
    genericfactory::GenericFactory<Material>::registerProperty<
        MonteCarloMaterial, REAL>("G", &MonteCarloMaterial::setG,
                                  &MonteCarloMaterial::getG);
    genericfactory::GenericFactory<Material>::registerProperty<
        MonteCarloMaterial, REAL>("B", &MonteCarloMaterial::setB,
                                  &MonteCarloMaterial::getB);
  }

 private:
  std::shared_ptr<BRDF> m_BRDF;
  Color m_Color;
};

#endif  // RAYTRACERLIB_MATERIALS_MONTECARLOMATERIAL_H_
