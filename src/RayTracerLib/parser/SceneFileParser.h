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
#ifndef RAYTRACERLIB_PARSER_SCENEFILEPARSER_H_
#define RAYTRACERLIB_PARSER_SCENEFILEPARSER_H_

#include <rapidxml/rapidxml.hpp>

#include <string>

#include "parser/Reflection.h"

class Scene;
// class Material;

// delete
#include "shapes/Shape.h"
#include "shapes/Rectangle.h"
#include "lights/AreaShape.h"
#include "materials/Material.h"
#include "materials/MonteCarloMaterial.h"

class SceneFileParser {
 public:
  /// Default Constructor
  SceneFileParser() { }
  /// parses the file and fills the scene.
  void parse(const std::string& filename, Scene* scene) const;

 private:
  /// reads the content of the file into a fitting sized memory.
  /// IMPORTANT!!! free the memory!!!!!!!
  static char* getFileContents(const std::string& filename);

  template<class T>
  rapidxml::xml_node<>* parseGroup(rapidxml::xml_document<>* doc) const;

  template<class T>
  void parseGroupSpecial(rapidxml::xml_node<>* node, ...) const;
};


template<class T>
rapidxml::xml_node<>* SceneFileParser::parseGroup(
      rapidxml::xml_document<>* doc) const {
  std::string groupName(T::name);
  groupName.append("s");
  rapidxml::xml_node<>* node = doc->first_node(groupName.c_str(), 0, false);
  if (node)
    return node->first_node();
  return nullptr;
}

#endif  // RAYTRACERLIB_PARSER_SCENEFILEPARSER_H_
