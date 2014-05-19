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
#ifndef RAYTRACERLIB_COMPOUNDSHAPE_H_
#define RAYTRACERLIB_COMPOUNDSHAPE_H_

#include <glm/glm.hpp>

#include <vector>

#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Shape.h"

///
class CompoundShape : public Shape {
 public:
  /// Enum to define how the child shapes are combined.
  enum Operator {
    unionOp,
    intersectionOp,
    minusOp
  };

  /// Constructor.
  CompoundShape();
  /// Destructor.
  virtual ~CompoundShape() { }

  /// TODO(all, 05/17/2014): Find the right epsilon.
  virtual IntersectionInfo getIntersectionInfo(const Ray& ray,
    const REAL minimumT = constants::TEPSILON,
    const REAL maximumT = std::numeric_limits<REAL>::max()) const;

  /// Setter for operator.
  void setOperator(const CompoundShape::Operator& op) {
    _operator = op;
  }

  /// Getter for operator.
  const CompoundShape::Operator& getOperator() const { return _operator; }

  /// Getter for _useChildMaterials.
  const bool useChildMaterials() const { return _useChildMaterials; }

  /// Getter for _passTransformation.
  const bool passTransformation() const { return _passTransformation; }

  /// Set the left object.
  void setLeftShapePtr(const Shape* shapePtr) { _leftShapePtr = shapePtr; }

  /// Set the right object.
  void setRightShapePtr(const Shape* shapePtr) { _rightShapePtr = shapePtr; }

 protected:
  IntersectionInfo intersectUnion(const Ray& ray,
    const REAL minimumT = constants::TEPSILON,
    const REAL maximumT = std::numeric_limits<REAL>::max()) const;

  IntersectionInfo intersectIntersect(const Ray& ray,
    const REAL minimumT = constants::TEPSILON,
    const REAL maximumT = std::numeric_limits<REAL>::max()) const;

  IntersectionInfo intersectMinus(const Ray& ray,
    const REAL minimumT = constants::TEPSILON,
    const REAL maximumT = std::numeric_limits<REAL>::max()) const;

  /// Intersection test. Not used here.
  virtual std::vector<REAL> intersect(const Ray& ray) const;
  /// Return normal. Not used here.
  virtual glm::vec4 getNormalAt(const glm::vec4& p) const;

 private:
  /// Adapts the material pointer according to _useChildMaterials;
  void adaptInstersectionInfo(IntersectionInfo* infoPtr) const;

  /// Pointer to the left(in the sense of the operator) shape.
  const Shape*  _leftShapePtr;
  /// Pointer to the right(in the sense of the operator) shape.
  const Shape* _rightShapePtr;
  /// Boolean to define if the child shapes use their own material or if always
  /// the compound shapes material is used.
  bool _useChildMaterials;
  /// Boolean to check if the children should inherit this shapes
  /// transformation.
  bool _passTransformation;
  /// Defines how the child shapes are combined.
  Operator _operator;
};

#endif  // RAYTRACERLIB_COMPOUNDSHAPE_H_
