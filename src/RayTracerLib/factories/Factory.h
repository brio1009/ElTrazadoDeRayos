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
#ifndef RAYTRACERLIB_FACTORIES_FACTORY_H_
#define RAYTRACERLIB_FACTORIES_FACTORY_H_

#include <map>
#include <utility>
#include <string>
#include "../shapes/Shape.h"
#include "./Property.h"

/// This class is used to create specific Shapes (@see RayTracerLib/shapes)
template <class BaseClass>
class Factory {
 public:
  /// This construct defines an abstract interface to register Shapes
  /// in the factory. It provides a pure method to create a Shape of
  /// some sort.
  struct register_base {
    virtual const char* name() const = 0;
    virtual BaseClass* create() = 0;
  };

  /// This construct specializes the registering
  template<class C>
  struct register_specialized : register_base {
   public:
    /// Needed to evaluate the class name at compile time.
    static const char* NAME;

    /// Constructor.
    register_specialized() : _name(NAME) { }  // force name initialization

    /// Returns the name of this class.
    const char* name() const { return C::name; }

    /// Create this object.
    BaseClass* create() { return new C(); }

    /// Register property.
    template<class ValueType>
    void registerProperty(const std::string& propertyName,
      void (C::*pSetter)(ValueType value),
      ValueType (C::*pGetter)()) {
      propertyMap()[propertyName] = new TypeProperty<C, ValueType>(pSetter, pGetter);
    }

   private:
    /// Name of this class.
    const char* _name;

    /// Map with properties of this class.
    static std::map<std::string, Property<C>*>& propertyMap() {
      /// This map stores each registered Shape. (don't modify.)
      static std::map<std::string, Property<C>*> m_PropertyMap;
      return m_PropertyMap;
    }
  };

  /// This method creates a Shape of given name when called.
  static BaseClass* Create(const char* name) {
    printf("Creating %s one out of %zu many choices.\n", name, myMap().size());
    return myMap()[name]->create();
  }

 private:
  /// This method is called automaticly for every class that inherits from
  /// "register_specialized". Don't call it if you don't understand it.
  static const char* Register(register_base* entry) {
    printf("Registering %s\n", entry->name());
    myMap()[entry->name()] = entry;
    return entry->name();
  }

  // This has to be done or the code will terminate with a segmentation fault
  // 80% of the time.
  static std::map<std::string, register_base*>& myMap() {
    /// This map stores each registered Shape. (don't modify.)
    static std::map<std::string, register_base*> m_Map;
    return m_Map;
  }
};

// This initialization is used to register the specific Shapes.
template <class T>
template <class C>
const char* Factory<T>::register_specialized<C>::NAME =
      Factory<T>::Register(new Factory<T>::register_specialized<C>());

#endif  // RAYTRACERLIB_FACTORIES_FACTORY_H_
