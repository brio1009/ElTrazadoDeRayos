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
#ifndef _ELTRAZADODERAYOS_SRC_RAYTRACERLIB_SOLVER_H_
#define _ELTRAZADODERAYOS_SRC_RAYTRACERLIB_SOLVER_H_
#include <math.h>

const double EPSILON = 1e-5;

// Helper functions to solve equations of polynoms of a certain degree.
namespace solve {
  // Stores up to 10 roots.
  struct Result_t {
    int numResults;
    double roots[10];
  };
  // Checks if the given value is zero.
  bool isZero(double val) {
    return (val <= EPSILON) && (val >= -EPSILON);
  }

  // Returnes the roots of a linear equation:
  // val1 * x + val0 = 0
  void solveLinearEquation(
      Result_t* result,
      double val1,
      double val0) {
    if (isZero(val1))
      result->numResults = 0;
    else {
      result->numResults = 1;
      result->roots[0] = isZero(val0) ? 0.0 : -val0 / val1;
    }
  }
  // Returnes the real roots of a quadratic equation:
  // val2 * x * x + val 1 * x + val0 = 0
  void solveQuadraticEquation(
      Result_t* result,
      double val2,
      double val1,
      double val0) {
    if (isZero(val2))
      solveLinearEquation(result, val1, val0);
    else {
      // Mitternacht
      double determinante = val1 * val1 + 4 * val2 * val0;
      if (isZero(determinante)) {
        result->numResults = 1;
        result->roots[0] = -val1 / (2 * val2);
      } else {
        if (determinante > 0) {
          result->numResults = 2;
          result->roots[0] = (-val1 - sqrt(determinante)) / (2 * val2);
          result->roots[1] = (-val1 + sqrt(determinante)) / (2 * val2);
        } else
          result->numResults = 0;
      }
    }
  }
}
#endif  // _ELTRAZADODERAYOS_SRC_RAYTRACERLIB_SOLVER_H_

