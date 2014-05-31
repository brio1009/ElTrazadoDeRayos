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

#include "./CompoundShape.h"

#include <glm/glm.hpp>

#include <vector>

#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Ray.h"

using std::vector;

// _____________________________________________________________________________
CompoundShape::CompoundShape()
  : _passTransformation(true),
    _useChildMaterials(true),
    _operator(CompoundShape::Operator::minusOp) {
}

// _____________________________________________________________________________
IntersectionInfo CompoundShape::getIntersectionInfo(const Ray& ray,
    const REAL minimumT,
    const REAL maximumT) const {
  switch (getOperator()) {
    default:
    case CompoundShape::Operator::unionOp:
      return intersectUnion(ray, minimumT, maximumT);
      break;
    case CompoundShape::Operator::intersectionOp:
      return intersectIntersect(ray, minimumT, maximumT);
      break;
    case CompoundShape::Operator::minusOp:
      return intersectMinus(ray, minimumT, maximumT);
      break;
  }
}

// _____________________________________________________________________________
IntersectionInfo CompoundShape::intersectUnion(const Ray& ray,
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

  IntersectionInfo leftInfo = _leftShapePtr->getIntersectionInfo(newRay,
                                                                 minimumT,
                                                                 maximumT);
  IntersectionInfo rightInfo = _rightShapePtr->getIntersectionInfo(newRay,
                                                                  minimumT,
                                                                  maximumT);
  // Automatically returns a no-hit if both were not hit.
  if (leftInfo.t < rightInfo.t) {
    adaptInstersectionInfo(&leftInfo);
    return leftInfo;
  }
  // Else.
  adaptInstersectionInfo(&rightInfo);
  return rightInfo;
}

// _____________________________________________________________________________
IntersectionInfo CompoundShape::intersectIntersect(const Ray& ray,
                                               const REAL minimumT,
                                               const REAL maximumT) const {
  return IntersectionInfo();
}

// _____________________________________________________________________________
IntersectionInfo CompoundShape::intersectMinus(const Ray& ray,
                                               const REAL minimumT,
                                               const REAL maximumT) const {
  // Transform the ray if needed.
  Ray newRay = ray;
  if (passTransformation()) {
    newRay = _inverseTransform * ray;
  }

  // Consider the intersection of the right shape only inside the left
  // shape.
  vector<REAL> intersectionsLeft = _leftShapePtr->intersect(newRay);
  vector<REAL> intersectionsRight = _rightShapePtr->intersect(newRay);

  // Just get all the hits with the right object that fir the range.
  size_t startIndex(0);
  size_t endIndex(intersectionsRight.size());
  for (size_t i = 0; i < intersectionsRight.size(); ++i) {
    REAL t = intersectionsRight.at(i);
    if (t < minimumT) {
      startIndex = i;
    }
    if (t > maximumT) {
      endIndex = i;
      break;
    }
  }
  ++startIndex;

  REAL rightT(0), leftT(0);
  bool hitLeft(false), hitRight(false);
  size_t rightIndex(startIndex);
  for (size_t i = 0; i < intersectionsLeft.size(); ++i) {
    REAL myT = intersectionsLeft.at(i);
    while (rightIndex < endIndex && intersectionsRight.at(rightIndex) < myT) {
      ++rightIndex;
      if (rightIndex >= endIndex) {
        hitLeft = true;
        leftT = intersectionsLeft.at(i);
        break;
      }
    }
    if (rightIndex % 2 == 0) {
      // i is inside of right
      if (rightIndex + 1 >= intersectionsRight.size()
          || i + 1 >= intersectionsLeft.size()) {
        break;
      }
      if (intersectionsRight.at(rightIndex + 1) < intersectionsLeft.at(i + 1)) {
        // Draw the Right shape
        hitRight = true;
        rightT = intersectionsRight.at(rightIndex + 1);
        break;
      }
    } else {
      hitLeft = true;
      leftT = intersectionsLeft.at(i);
      break;
    }    
  }

  // Check if we hit something.
  if (hitRight) {
    IntersectionInfo info = _rightShapePtr->getIntersectionInfo(newRay,
                                            rightT - 2.0 * constants::TEPSILON,
                                            rightT + 2.0 * constants::TEPSILON);
    adaptInstersectionInfo(&info);
    return info;
  }
  if (hitLeft) {
    IntersectionInfo info = _leftShapePtr->getIntersectionInfo(newRay,
                                             leftT - 2.0 * constants::TEPSILON,
                                             leftT + 2.0 * constants::TEPSILON);
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
  }
}

// _____________________________________________________________________________
vector<REAL> CompoundShape::intersect(const Ray& ray) const {
  return vector<REAL>();
}

// _____________________________________________________________________________
glm::vec4 CompoundShape::getNormalAt(const glm::vec4& p) const {
  return glm::vec4(0);
}
