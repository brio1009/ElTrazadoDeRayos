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

#include <cstring>
#include <map>
#include <stack>
#include <string>
#include <unordered_set>
#include <utility>
#include "factories/Property.h"
#include "lights/AreaShape.h"
// Forward
class Shape;
/// WOW THIS IS UGLY!!!
template<class Base, class Special>
class CreationHelper {
 protected:
  /// Create this object.
  Base* create() { return new Special(); }
  Base* createSpecial() { return nullptr; }
};
template<class Base>
class CreationHelper<Base, Base> {
 protected:
  Base* create() { return nullptr; }
  Base* createSpecial() { return nullptr; }
};
template<>
class CreationHelper<Shape, Shape> {
 protected:
  Shape* create() { return nullptr; }
  Shape* createSpecial() { return nullptr; }
};
template<class Special>
class CreationHelper<Shape, Special> {
 protected:
  Shape* create() { return new Special(); }
  Shape* createSpecial() { return new AreaShape<Special>(); }
};

/// This class is used to create specific Shapes (@see RayTracerLib/shapes)
template <class BaseClass>
class Factory {
 public:
  /// This construct defines an abstract interface to register Shapes
  /// in the factory. It provides a pure method to create a Shape of
  /// some sort.
  struct register_base {
   public:
    virtual const char* name() const = 0;
    virtual void registerProperties() = 0;
    virtual BaseClass* create() = 0;
    virtual BaseClass* createImportant() = 0;
    virtual void setPropertyFromString(BaseClass* const objPtr,
                                   const std::string& propertyName,
                                   const std::string& propertyValue) const = 0;
    virtual std::string getPropertyAsString(const BaseClass* const objPtr,
                                    const std::string& propertyName) const = 0;
    virtual std::string parent() = 0;
  };

  /// This construct specializes the registering
  template<class C>
  struct register_specialized : register_base, CreationHelper<BaseClass, C> {
   public:
    /// Needed to evaluate the class name at compile time.
    static const char* NAME;
    static const char* PARENT;

    /// Constructor.
    register_specialized() : _name(NAME) { }  // force name initialization

    /// Returns the name of this class.
    const char* name() const { return C::name; }
    /// Parent name.
    virtual std::string parent() { return C::parent; }

    ///
    BaseClass* create() { return CreationHelper<BaseClass, C>::create(); }

    BaseClass* createImportant() { return CreationHelper<BaseClass, C>::createSpecial(); }
    ///
    virtual void registerProperties() {
      C::registerAllProperties();
      C::createSpecialProperties();
    }

    /// Get the proeprty with the given name. nullptr if not exist.
    const Property<C>* const getProperty(
          const std::string& propertyName) const {
      auto it = propertyMap().find(propertyName);
      if (it == propertyMap().end())
        return nullptr;
      return it->second;
    }

    /// Set the given property with the given value.
    void setPropertyFromString(BaseClass* const objPtr,
                               const std::string& propertyName,
                               const std::string& propertyValue) const {
      const Property<C>* const prop = getProperty(propertyName);
      if (prop) {
        prop->fromString(reinterpret_cast<C*>(objPtr), propertyValue);
        return;
      }
      // check parent properties.
      if (strcmp(PARENT, "none") == 0)
        return;
      Factory<BaseClass>::setPropertyFromString(
            PARENT,
            objPtr,
            propertyName,
            propertyValue);
    }

    /// Get the the given property as string.
    std::string getPropertyAsString(const BaseClass* const objPtr,
                                    const std::string& propertyName) const {
      const Property<C>* const prop = getProperty(propertyName);
      if (prop)
        return prop->toString(reinterpret_cast<const C* const>(objPtr));
      // check if parent propexists.
      if (strcmp(PARENT, "none") == 0)
        return "";
      return Factory<BaseClass>::getPropertyAsString(
            PARENT,
            objPtr,
            propertyName);
    }

    /// Register property.
    template<class ValueType>
    static void RegisterProperty(const std::string& propertyName,
               void (C::*pSetter)(ValueType value),
               ValueType (C::*pGetter)() const) {
      propertyMap()[propertyName] = new TypeProperty<C, ValueType>(propertyName,
                                                                   pSetter,
                                                                   pGetter);
      printf("\tRegistered property \"%s\"\n", propertyName.c_str());
    }

    /// Map with properties of this class.
    static std::map<std::string, Property<C>*>& propertyMap() {
      /// This map stores each registered Shape. (don't modify.)
      static std::map<std::string, Property<C>*> m_PropertyMap;
      return m_PropertyMap;
    }

   private:
    /// Name of this class.
    const char* _name;
  };

  /// Sets the property from string.
  static void setPropertyFromString(const std::string& className,
                                  BaseClass* const objPtr,
                                  const std::string& propertyName,
                                  const std::string& propertyValue) {
    // First find the register_base.
    auto classIt = myMap().find(className);
    if (classIt == myMap().end()) {
      return;
    }
    register_base* base = classIt->second;
    base->setPropertyFromString(objPtr, propertyName, propertyValue);
  }

  /// Get the property as string.
  static std::string getPropertyAsString(const std::string& className,
                                         const BaseClass* const objPtr,
                                         const std::string& propertyName) {
    // First find the register_base.
    auto classIt = myMap().find(className);
    if (classIt == myMap().end())
      return "";
    register_base* base = classIt->second;
    return base->getPropertyAsString(objPtr, propertyName);
  }

  /// This method creates a Shape of given name when called.
  static BaseClass* Create(const char* name) {
    // inheritProps();
    return myMap()[name]->create();
  }
  /// This method creates a Shape of given name when called.
  static BaseClass* CreateImportant(const char* name) {
    // inheritProps();
    return myMap()[name]->createImportant();
  }

 private:
  /// This method is called automaticly for every class that inherits from
  /// "register_specialized". Don't call it if you don't understand it.
  static const char* Register(register_base* entry) {
    printf("Registering %s\n", entry->name());
    myMap()[entry->name()] = entry;
    entry->registerProperties();
    printf("\n");
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
template <class T>
template <class C>
const char* Factory<T>::register_specialized<C>::PARENT = C::parent;

#define GETSET(type, varname, propname)\
public:\
  void set##propname(type value) {\
    varname = value; \
  }\
  type get##propname() const {\
    return varname;\
  }\
// This counts the number of args.
#define NARGS_SEQ(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
                  _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, \
                  _22, _23, _24, _25, _26, _27, _28, _29, _30, N, ...) N
#define NARGS(...) NARGS_SEQ(__VA_ARGS__, 30, 1, 1, 27, 1, 1, 24, 1, 1, 21, \
                   1, 1, 18, 1, 1, 15, 1, 1, 12, 1, 1, \
                   9, 1, 1, 6, 1, 1, 3, 1, 0, 0)

// This will let macros expand before concating them.
#define PRIMITIVE_CAT(x, y) x ## y
#define CAT(x, y) PRIMITIVE_CAT(x, y)
// Create just the start so no compile error occurs when no properties are set.
#define RegisterP_1(classname, ...)\
  static_assert(0, "Wrong argument count");\
  public: static void registerAllProperties() {
#define RegisterP_0(classname, ...)\
  public: static void registerAllProperties() {
// This macro ends a recursion.
#define RegisterP_3(classname, type, varname, propname)\
  GETSET(type, varname, propname)\
  using register_specialized<classname>::RegisterProperty;\
  static void registerAllProperties() {\
    RegisterProperty<type>(#propname, \
          &classname::set##propname, \
          &classname::get##propname);
// These macros are used to enable a recursion up to 10.
#define RegisterP_6(classname, type, varname, propname, ...)\
  GETSET(type, varname, propname)\
  CAT(RegisterP_, NARGS(__VA_ARGS__))(classname, __VA_ARGS__)\
    RegisterProperty<type>(#propname, \
          &classname::set##propname, \
          &classname::get##propname);
#define RegisterP_9(classname, type, varname, propname, ...)\
  GETSET(type, varname, propname)\
  CAT(RegisterP_, NARGS(__VA_ARGS__))(classname, __VA_ARGS__)\
    RegisterProperty<type>(#propname, \
          &classname::set##propname, \
          &classname::get##propname);
#define RegisterP_12(classname, type, varname, propname, ...)\
  GETSET(type, varname, propname)\
  CAT(RegisterP_, NARGS(__VA_ARGS__))(classname, __VA_ARGS__)\
    RegisterProperty<type>(#propname, \
          &classname::set##propname, \
          &classname::get##propname);
#define RegisterP_15(classname, type, varname, propname, ...)\
  GETSET(type, varname, propname)\
  CAT(RegisterP_, NARGS(__VA_ARGS__))(classname, __VA_ARGS__)\
    RegisterProperty<type>(#propname, \
          &classname::set##propname, \
          &classname::get##propname);
#define RegisterP_18(classname, type, varname, propname, ...)\
  GETSET(type, varname, propname)\
  CAT(RegisterP_, NARGS(__VA_ARGS__))(classname, __VA_ARGS__)\
    RegisterProperty<type>(#propname, \
          &classname::set##propname, \
          &classname::get##propname);
#define RegisterP_21(classname, type, varname, propname, ...)\
  GETSET(type, varname, propname)\
  CAT(RegisterP_, NARGS(__VA_ARGS__))(classname, __VA_ARGS__)\
    RegisterProperty<type>(#propname, \
          &classname::set##propname, \
          &classname::get##propname);
#define RegisterP_24(classname, type, varname, propname, ...)\
  GETSET(type, varname, propname)\
  CAT(RegisterP_, NARGS(__VA_ARGS__))(classname, __VA_ARGS__)\
    RegisterProperty<type>(#propname, \
          &classname::set##propname, \
          &classname::get##propname);
#define RegisterP_27(classname, type, varname, propname, ...)\
  GETSET(type, varname, propname)\
  CAT(RegisterP_, NARGS(__VA_ARGS__))(classname, __VA_ARGS__)\
    RegisterProperty<type>(#propname, \
          &classname::set##propname, \
          &classname::get##propname);
#define RegisterP_30(classname, type, varname, propname, ...)\
  GETSET(type, varname, propname)\
  CAT(RegisterP_, NARGS(__VA_ARGS__))(classname, __VA_ARGS__)\
    RegisterProperty<type>(#propname, \
          &classname::set##propname, \
          &classname::get##propname);
// The macro that is called from outside to create and register properties.
// Also defines the name method so we can use setFromString etc.
#define PROPERTIES(classname, \
      ...)\
  CAT(RegisterP_, NARGS(__VA_ARGS__))(classname, __VA_ARGS__)\
  }\
  virtual const char* className() const { return classname::name; }

#endif  // RAYTRACERLIB_FACTORIES_FACTORY_H_
