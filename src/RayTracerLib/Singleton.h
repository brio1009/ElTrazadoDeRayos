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

#ifndef RAYTRACERLIB_SINGLETON_H_
#define RAYTRACERLIB_SINGLETON_H_
/// Simple singleton template class, that provides a singleton interface
/// to every class that inherits from it.
/// Be aware: The inheriting class should friend class this Singleton and only
/// have private constructors.
template <class T>
class Singleton {
 public:
  /// Get the instance of this singleton.
  static T* Instance() {
    if (!_TPtr)
      _TPtr = new T();
    return _TPtr;
  }

 protected:
  /// Default constructor.
  Singleton() {
    _TPtr = nullptr;
  }

  /// Default destructor.
  virtual ~Singleton() {
    delete _TPtr;
  }

 private:
  /// The static pointer to the singleton object.
  static T* _TPtr;

  /// Private copy constructor to prevent copy construction.
  Singleton(const Singleton&);

  /// Private assignment operator to prevent assignment.
  Singleton& operator=(const Singleton&);
};
#endif  // RAYTRACERLIB_SINGLETON_H_

