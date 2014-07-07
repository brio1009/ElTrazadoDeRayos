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

#include <string>

/// Class that holds properties (setter, getter).
template <class OwnerClass>
class Property {
 public:
  /// Constructor with given name.
  explicit Property(const std::string& name) : m_Name(name) { }

  /// Returns the value of the property as string.
  virtual std::string toString(const OwnerClass* const objPtr) const = 0;

  /// Sets the value from string.
  virtual void fromString(OwnerClass* const objPtr,
                          const std::string& value) const = 0;

  /// Getter for the name.
  const std::string& name() const { return m_Name; }

 protected:
  /// The name of this property.
  const std::string m_Name;
};

/// Helper struct to partially specialize.
template <class ValueType>
struct StringCastHelper {
  /// Returns the value of the property as string.
  static std::string toString(const ValueType& value);

  /// Returns the value from string.
  static ValueType fromString(const std::string& value);
};

/// Proeprty specialized for one ValueType.
template <class OwnerClass, class ValueType>
class TypeProperty : public Property<OwnerClass> {
 public:
  /// Constructor setting the getter and setter ptr.
  TypeProperty(const std::string& name,
               void (OwnerClass::*pSetter)(ValueType value),
               ValueType (OwnerClass::*pGetter)() const)
    : setterPtr(pSetter),
      getterPtr(pGetter),
      Property<OwnerClass>(name) {
  }

  /// Getter for the property.
  ValueType getValue(const OwnerClass* const objPtr) const {
    return (objPtr->*getterPtr)();
  }

  /// Setter for the property.
  void setValue(OwnerClass* const objPtr, ValueType value) const {
    (objPtr->*setterPtr)(value);
  }

  /// Returns the value of the property as string.
  std::string toString(const OwnerClass* const objPtr) const {
    return StringCastHelper<ValueType>::toString(getValue(objPtr));
  }

  /// Sets the value from string.
  void fromString(OwnerClass* const objPtr, const std::string& value) const {
    setValue(objPtr, StringCastHelper<ValueType>::fromString(value));
  }

 private:
  /// Saves pointer to the setter (offset to class pointer).
  void (OwnerClass::*setterPtr)(ValueType value);
  /// Saves pointer to the getter (offset to class pointer).
  ValueType (OwnerClass::*getterPtr)() const;
};

#endif  // RAYTRACERLIB_FACTORIES_PROPERTY_H_
