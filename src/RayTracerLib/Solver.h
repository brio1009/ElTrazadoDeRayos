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
#ifndef RAYTRACERLIB_SOLVER_H_
#define RAYTRACERLIB_SOLVER_H_
#include <math.h>
#include <stdio.h>
#include <vector>
#include "./Constants.h"

// Helper functions to solve equations of polynoms of a certain degree.
namespace solve {
  // Checks if the given value is zero.
  inline bool isZero(REAL val) {
    return (val <= constants::EPSILON) && (val >= -constants::EPSILON);
  }

  // Returnes the roots of a linear equation:
  // val1 * x + val0 = 0
  inline void solveLinearEquation(
      std::vector<REAL>* result,
      REAL val1,
      REAL val0) {
    result->resize(0);
    if (!isZero(val1)) {
      result->push_back(isZero(val0) ? 0.0 : -val0 / val1);
    }
  }
  // Returnes the real roots of a quadratic equation:
  // val2 * x * x + val 1 * x + val0 = 0
  inline void solveQuadraticEquation(
      std::vector<REAL>* result,
      REAL val2,
      REAL val1,
      REAL val0) {
    if (isZero(val2)) {
      solveLinearEquation(result, val1, val0);
    } else {
      result->resize(0);
      // Mitternacht
      REAL determinante = val1 * val1 - 4 * val2 * val0;
      if (isZero(determinante)) {
        result->push_back(-val1 / (2 * val2));
      } else {
        if (determinante > 0) {
          result->push_back((-val1 - sqrt(determinante)) / (2 * val2));
          result->push_back((-val1 + sqrt(determinante)) / (2 * val2));
        }
      }
    }
  }
  // Returnes the real roots of a qubic equation:
  // val3 * x * x * x + val2 * x * x + val 1 * x + val0 = 0
  inline void solveQubicEquation(
      std::vector<REAL>* result,
      REAL val3,
      REAL val2,
      REAL val1,
      REAL val0) {
    if (isZero(val2)) {
      solveQuadraticEquation(result, val2, val1, val0);
    }
    perror("void solveQubicEquation(): Not implemented!");
  }

}  // namespace solve
#endif  // RAYTRACERLIB_SOLVER_H_

