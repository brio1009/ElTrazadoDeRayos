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
#ifndef RAYTRACERLIB_FACTORIES_PROPERTY_H_
#define RAYTRACERLIB_FACTORIES_PROPERTY_H_

/// Class that holds properties (setter, getter).
template <class OwnerClass>
class Property {
};

/// Proeprty specialized for one ValueType.
template <class OwnerClass, class ValueType>
class TypeProperty : public Property<OwnerClass> {
 public:
  /// Constructor setting the getter and setter ptr.
  TypeProperty(void (OwnerClass::*pSetter)(ValueType value),
               ValueType (OwnerClass::*pGetter)())
    : setterPtr(pSetter),
      getterPtr(pGetter) {
  }

  /// Getter for the property.
  const ValueType& getValue(const OwnerClass* const objPtr) const {
    return (objPtr->*getterPtr)();
  }

  /// Setter for the property.
  void setValue(OwnerClass* const objPtr, ValueType value) {
    (objPtr->*setterPtr)(value);
  }

 private:
   /// Saves pointer to the setter (offset to class pointer).
   void (OwnerClass::*setterPtr)(ValueType value);
   /// Saves pointer to the getter (offset to class pointer).
   ValueType (OwnerClass::*getterPtr)();
};

#endif  // RAYTRACERLIB_FACTORIES_PROPERTY_H_
