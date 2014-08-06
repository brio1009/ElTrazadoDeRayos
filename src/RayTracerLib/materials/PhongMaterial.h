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
#ifndef RAYTRACERLIB_MATERIALS_PHONGMATERIAL_H_
#define RAYTRACERLIB_MATERIALS_PHONGMATERIAL_H_

#include <glm/glm.hpp>
#include "./Material.h"
#include "./Color.h"

/// Phong material that shades the object with the phong reflection model.
class PhongMaterial : public Material {
 public:
  /// Constructor. Randoms a color.
  PhongMaterial();

  /// Constructor.
  explicit PhongMaterial(const Color& color)
                  : PhongMaterial(color, 0.6f, 0.4f, 0.0f, 20.0f) { }

  /// Constructor
  PhongMaterial(const Color& color,
                const float kd,
                const float ks,
                const float ka,
                const float shiny)
                  : m_color(color),
                  m_shiny(shiny),
                  m_kd(kd),
                  m_ks(ks),
                  m_ka(ka) { }

  /// Returns the color for the given position, normal and ray direction.
  virtual Color getColor(const IntersectionInfo& intersectionInfo,
                         const Ray& incomingRay,
                         const Scene& scene) const;

 protected:
  /// Getter for the color.
  virtual const Color& color(const glm::vec2& uv = glm::vec2()) const {
    return m_color;
  }

  /// Helper to compute the ambient color.
  Color ambientTerm(const Color& color, const float skalar) const;
  /// Helper to compute the diffuse color.
  Color diffuseTerm(const Color& color,
      const glm::vec4& lightDir,
      const glm::vec4& normal,
      const float skalar) const;
  /// Helper to compute the specular color.
  Color specularTerm(const Color& color,
      const glm::vec4& lightDir,
      const glm::vec4& normal,
      const glm::vec4& viewer,
      const float skalar,
      const float shininess = 5.0f) const;

 private:
  /// Color member.
  Color m_color;
  float m_shiny;
  float m_kd;
  float m_ks;
  float m_ka;
};

#endif  // RAYTRACERLIB_MATERIALS_PHONGMATERIAL_H_


