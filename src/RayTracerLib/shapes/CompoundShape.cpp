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

#include "./CompoundShape.h"

#include <glm/glm.hpp>

#include <algorithm>
#include <limits>
#include <vector>

#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Ray.h"
#include "./Solver.h"

using std::vector;

// _____________________________________________________________________________
CompoundShape::CompoundShape()
  : _passTransformation(true),
    _useChildMaterials(true),
    _operator(CompoundShape::Operator::minusOp) {
}

// _____________________________________________________________________________
CompoundShape::CompoundShape(Shape* leftShapePtr, Shape* rightShapePtr)
  : _leftShapePtr(leftShapePtr),
    _rightShapePtr(rightShapePtr),
    _passTransformation(true),
    _useChildMaterials(true),
    _operator(CompoundShape::Operator::minusOp) {
}

// _____________________________________________________________________________
IntersectionInfo CompoundShape::getIntersectionInfo(const Ray& ray,
    const REAL minimumT,
    const REAL maximumT) const {
  // Just get the closest intersection.
  if (!_leftShapePtr || !_rightShapePtr) {
    return IntersectionInfo();
  }
  Ray newRay = ray;
  if (passTransformation()) {
    newRay = _inverseTransform * ray;
  }

  // Create two vectors that save the intersections.
  vector<REAL> rightHits;
  vector<REAL> leftHits;
  getIntersects(newRay, &leftHits, &rightHits);

  // Combine the hits.
  bool hitRight(false);
  bool hitLeft(false);

  // Find the smallest T in both vectors.
  REAL smallestT = std::numeric_limits<REAL>::max();
  for (size_t i = 0; i < rightHits.size(); ++i) {
    if (rightHits.at(i) <= smallestT
        && rightHits.at(i) >= minimumT
        && rightHits.at(i) <= maximumT) {
      smallestT = rightHits.at(i);
      hitRight = true;
    }
  }
  for (size_t i = 0; i < leftHits.size(); ++i) {
    if (leftHits.at(i) <= smallestT
        && leftHits.at(i) >= minimumT
        && leftHits.at(i) <= maximumT) {
      smallestT = leftHits.at(i);
      hitRight = false;
      hitLeft = true;
    }
  }
  // Check if we hit something.
  if (hitRight) {
    IntersectionInfo info = _rightShapePtr->getIntersectionInfo(newRay,
                              smallestT - 2.0 * constants::TEPSILON,
                              smallestT + 2.0 * constants::TEPSILON);
    adaptInstersectionInfo(&info);
    if (getOperator() == CompoundShape::Operator::minusOp) {
      info.normal = -info.normal;
    }
    return info;
  }
  if (hitLeft) {
    IntersectionInfo info = _leftShapePtr->getIntersectionInfo(newRay,
                               smallestT - 2.0 * constants::TEPSILON,
                               smallestT + 2.0 * constants::TEPSILON);
    adaptInstersectionInfo(&info);
    return info;
  }
  // Else.
  return IntersectionInfo();
}

// _____________________________________________________________________________
void CompoundShape::adaptInstersectionInfo(
    IntersectionInfo* infoPtr) const {
  // Adapt material ptr.
  if (infoPtr->materialPtr && !useChildMaterials()) {
    infoPtr->materialPtr = this->getMaterialPtr();
  }
  // Adapt normal.
  if (passTransformation()) {
    infoPtr->normal = glm::normalize(_transformation * infoPtr->normal);
    infoPtr->hitPoint = _transformation * infoPtr->hitPoint;
    if (solve::isZero(infoPtr->hitPoint[3])) {
      infoPtr->hitPoint[3] = 0.0;
      return;
    }
    infoPtr->hitPoint /= infoPtr->hitPoint[3];
  }
}

// _____________________________________________________________________________
void CompoundShape::getIntersects(const Ray& ray,
                                  vector<REAL>* leftHits,
                                  vector<REAL>* rightHits) const {
  // Consider the intersection of the right shape only inside the left
  // shape.
  vector<REAL> intersectionsLeft = _leftShapePtr->intersect(ray);
  vector<REAL> intersectionsRight = _rightShapePtr->intersect(ray);

  leftHits->resize(0);
  rightHits->resize(0);

  switch (getOperator()) {
    default:
    case CompoundShape::Operator::unionOp:
      // We first filter the right hits. This is only hit inside the left one.
      for (size_t i = 0; i < intersectionsRight.size(); ++i) {
        REAL currentT = intersectionsRight.at(i);
        // Get the t from the left intersections that is smaller and check if it
        // has an uneven index.
        size_t leftIndex(0);
        for (size_t j = 0; j < intersectionsLeft.size(); ++j) {
          if (intersectionsLeft.at(j) > currentT) {
            break;
          }
          ++leftIndex;
        }
        if (leftIndex % 2 == 0) {
          rightHits->push_back(currentT);
        }
      }

      // Do the same thing for the left hits.
      for (size_t i = 0; i < intersectionsLeft.size(); ++i) {
        // We do not hit inside the right object, so check that.
        REAL currentT = intersectionsLeft.at(i);
        size_t rightIndex(0);
        for (size_t j = 0; j < intersectionsRight.size(); ++j) {
          if (intersectionsRight.at(j) > currentT) {
            break;
          }
          ++rightIndex;
        }
        if (rightIndex % 2 == 0) {
          leftHits->push_back(currentT);
        }
      }
      break;
    case CompoundShape::Operator::minusOp:
      // We first filter the right hits. This is only hit inside the left one.
      for (size_t i = 0; i < intersectionsRight.size(); ++i) {
        REAL currentT = intersectionsRight.at(i);
        // Get the t from the left intersections that is smaller and check if it
        // has an uneven index.
        size_t leftIndex(0);
        for (size_t j = 0; j < intersectionsLeft.size(); ++j) {
          if (intersectionsLeft.at(j) > currentT) {
            break;
          }
          ++leftIndex;
        }
        if (leftIndex % 2 != 0) {
          rightHits->push_back(currentT);
        }
      }

      // Do the same thing for the left hits.
      for (size_t i = 0; i < intersectionsLeft.size(); ++i) {
        // We do not hit inside the right object, so check that.
        REAL currentT = intersectionsLeft.at(i);
        size_t rightIndex(0);
        for (size_t j = 0; j < intersectionsRight.size(); ++j) {
          if (intersectionsRight.at(j) > currentT) {
            break;
          }
          ++rightIndex;
        }
        if (rightIndex % 2 == 0) {
          leftHits->push_back(currentT);
        }
      }
      break;
    case CompoundShape::Operator::intersectionOp:
      // We first filter the right hits. This is only hit inside the left one.
      for (size_t i = 0; i < intersectionsRight.size(); ++i) {
        REAL currentT = intersectionsRight.at(i);
        // Get the t from the left intersections that is smaller and check if it
        // has an uneven index.
        size_t leftIndex(0);
        for (size_t j = 0; j < intersectionsLeft.size(); ++j) {
          if (intersectionsLeft.at(j) > currentT) {
            break;
          }
          ++leftIndex;
        }
        if (leftIndex % 2 != 0) {
          rightHits->push_back(currentT);
        }
      }

      // Do the same thing for the left hits.
      for (size_t i = 0; i < intersectionsLeft.size(); ++i) {
        // We do not hit inside the right object, so check that.
        REAL currentT = intersectionsLeft.at(i);
        size_t rightIndex(0);
        for (size_t j = 0; j < intersectionsRight.size(); ++j) {
          if (intersectionsRight.at(j) > currentT) {
            break;
          }
          ++rightIndex;
        }
        if (rightIndex % 2 != 0) {
          leftHits->push_back(currentT);
        }
      }
      break;
  }
}

// _____________________________________________________________________________
vector<REAL> CompoundShape::intersect(const Ray& ray) const {
  // Just get the closest intersection.
  if (!_leftShapePtr || !_rightShapePtr) {
    return vector<REAL>();
  }
  Ray newRay = ray;
  if (passTransformation()) {
    newRay = _inverseTransform * ray;
  }

  // Create two vectors that save the intersections.
  vector<REAL> rightHits;
  vector<REAL> leftHits;
  getIntersects(newRay, &leftHits, &rightHits);

  leftHits.insert(leftHits.end(), rightHits.begin(), rightHits.end());
  std::sort(leftHits.begin(), leftHits.end());
  return leftHits;
}

// _____________________________________________________________________________
glm::vec4 CompoundShape::getNormalAt(const glm::vec4& p) const {
  return glm::vec4(0);
}
