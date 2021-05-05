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
#ifndef RAYTRACERLIB_PROPERTYMANAGER_H_
#define RAYTRACERLIB_PROPERTYMANAGER_H_

#include <string>
#include <unordered_map>
#include "./Singleton.h"


/// Simple struct that holds all the necessary data for one property.
struct Property {
  Property() {}
  Property(void* getter, void* setter) : getterPtr(getter), setterPtr(setter) {}
  void* getterPtr;
  void* setterPtr;
};

/// Simple struct that holds all the properties of one class.
class BasicClassProperties {
 public:
  /// Default constructor.
  BasicClassProperties() {}
  /// Destructor.
  virtual ~BasicClassProperties() {}

 protected:
  std::unordered_map<std::string, Property> properties;
};

/// Template class holding the properties of the class.
template <class T>
class ClassProperties : public BasicClassProperties {
 public:
  /// Constructor.
  ClassProperties() : BasicClassProperties() { T::addProperties(&properties); }

  /// Destructor.
  virtual ~ClassProperties() {}
};

///
class PropertyManager : public Singleton<PropertyManager> {
  friend class Singleton<PropertyManager>;

 public:
  ///

  ///
  static std::unordered_map<std::string, BasicClassProperties*> classProperties;

  ///
  static void insertTestMap(const std::string& name,
                            BasicClassProperties* ptr) {
    classProperties.emplace(name, ptr);
  }

 private:
  /// Private default constructor.
  PropertyManager() {}
};

/// Macro.
#define PROPERTYCLASS(x) \
  PropertyManager::insertTestMap(#x, new ClassProperties<x>());

#endif  // RAYTRACERLIB_PROPERTYMANAGER_H_
