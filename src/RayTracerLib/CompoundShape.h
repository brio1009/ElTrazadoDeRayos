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
  /// Intersection test.
  virtual std::vector<REAL> intersect(const Ray& ray) const;
  /// Return normal.
  virtual glm::vec4 getNormalAt(const glm::vec4& p) const;

  /// Setter for operator.
  void setOperator(const CompoundShape::Operator& op) {
    _operator = op;
  }
  /// Getter for operator.
  const CompoundShape::Operator& getOperator() const { return _operator; }

  /// Multiplies the matrix with the current Transformation.
  // TODO(all, 05/17/2014): Maybe we want to override this here so we can
  // apply the transofrmation to the children as well.
  // virtual void transform(const glm::mat4& matrix);  //NOLINT mistaken for std

 protected:
    

 private:
  /// Pointer to the left(in the sense of the operator) shape.
  Shape*  _leftShapePtr;
  /// Pointer to the right(in the sense of the operator) shape.
  Shape* _rightShapePtr;
  /// Boolean to define if the child shapes use their own material or if always
  /// the compound shapes material is used.
  bool _useChildMaterials;
  /// Defines how the child shapes are combined.
  Operator _operator;
};

#endif  // RAYTRACERLIB_COMPOUNDSHAPE_H_
