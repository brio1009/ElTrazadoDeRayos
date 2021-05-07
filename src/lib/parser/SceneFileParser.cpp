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

#define GLM_FORCE_RADIANS

#include "parser/SceneFileParser.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <genericfactory/GenericFactory_impl.h>

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>

#include "./Reflection.h"
#include "./Scene.h"
#include "accelerationstructures/AbstractDataStructure.h"
#include "shapes/Shape.h"

// REMOVE ME!!!!
#include "cameras/PerspectiveCamera.h"
#include "materials/ColorMaterial.h"
// UP TO HERE!!!

using std::string;

struct cstringcomp {
  bool operator()(const char* s1, const char* s2) const {
    return strcmp(s1, s2) < 0;
  }
};
// _____________________________________________________________________________
template <>
void SceneFileParser::parseGroupSpecial<Material>(rapidxml::xml_node<>* node,
                                                  ...) const {
  // Get the material map from the variadic
  va_list ap;
  // Read needed stuff.
  va_start(ap, node);
  std::map<char*, Material*, cstringcomp>* materialMap =
      reinterpret_cast<std::map<char*, Material*, cstringcomp>*>(
          va_arg(ap, void*));
  va_end(ap);
  rapidxml::xml_node<>* child = node;
  while (child) {
    // add this child.
    // TODO(bauschp, Fr 8. Aug 23:28:39 CEST 2014): check if pointer alreadt ex.
    Material* mat =
        genericfactory::GenericFactory<Material>::create(child->name());
    if (!mat) {
      child = child->next_sibling();
      continue;
    }
    // call all the needed atributes.
    for (rapidxml::xml_attribute<>* attr = child->first_attribute(); attr;
         attr = attr->next_attribute()) {
      if (strcmp(attr->name(), "id") == 0) {
        materialMap->emplace(attr->value(), mat);
        continue;
      }
      mat->setFromString(attr->name(), attr->value());
    }
    child = child->next_sibling();
  }
  printf("%d materials loaded.\n", static_cast<int>(materialMap->size()));
}

// _____________________________________________________________________________
template <>
void SceneFileParser::parseGroupSpecial<Shape>(rapidxml::xml_node<>* node,
                                               ...) const {
  // Get the material map from the variadic
  va_list ap;
  // Read needed stuff.
  va_start(ap, node);
  std::map<char*, Material*, cstringcomp>* materialMap =
      reinterpret_cast<std::map<char*, Material*, cstringcomp>*>(
          va_arg(ap, void*));
  Scene* scene = reinterpret_cast<Scene*>(va_arg(ap, void*));
  va_end(ap);

  // start working.
  rapidxml::xml_node<>* child = node;
  while (child) {
    // add this child.
    rapidxml::xml_attribute<>* isImportantAttr =
        child->first_attribute("important");
    Shape* shape;
    if (isImportantAttr && strcmp(isImportantAttr->value(), "1") == 0) {
      shape = genericfactory::GenericFactory<Shape>::create(
          std::string(child->name()) + "Important");
    } else {
      shape = genericfactory::GenericFactory<Shape>::create(child->name());
    }
    if (!shape) {
      child = child->next_sibling();
      continue;
    }
    // call all the needed atributes.
    for (rapidxml::xml_attribute<>* attr = child->first_attribute(); attr;
         attr = attr->next_attribute()) {
      // First try to set the property if it is not a special attribute
      // (important and material).
      if (strcmp(attr->name(), Material::name) != 0)
      //|| strcmp(attr->name(), "important") != 0)
      {
        shape->setFromString(attr->name(), attr->value());
        continue;
      }
      // Ok, it seems to be a material. ->
      auto map_entry = materialMap->find(attr->value());
      if (map_entry == materialMap->end()) {
        perror("Material not defined above\n");
        continue;
      }
      shape->setFromString(
          Material::name,
          genericfactory::StringCastHelper<const Material*>::toString(
              map_entry->second));
    }
    scene->addShape(shape);
    child = child->next_sibling();
  }
  printf("%d shapes loaded.\n", static_cast<int>(scene->shapes()->size()));
}

// _____________________________________________________________________________
template <>
void SceneFileParser::parseGroupSpecial<Camera>(rapidxml::xml_node<>* node,
                                                ...) const {
  // Get the material map from the variadic
  va_list ap;
  // Read needed stuff.
  va_start(ap, node);
  Scene* scene = reinterpret_cast<Scene*>(va_arg(ap, void*));
  va_end(ap);

  // Read the cam and add it to the map.
  rapidxml::xml_node<>* child = node;
  while (child) {
    // add this child.
    Camera* cam = genericfactory::GenericFactory<Camera>::create(child->name());
    if (!cam) {
      child = child->next_sibling();
      continue;
    }
    // call all the needed atributes.
    for (rapidxml::xml_attribute<>* attr = child->first_attribute(); attr;
         attr = attr->next_attribute()) {
      if (strcmp(attr->name(), Material::name) != 0) {
        cam->setFromString(attr->name(), attr->value());
        continue;
      }
    }
    cam->setUsePostProcessing(false);
    scene->cameras().push_back(cam);
    child = child->next_sibling();
  }
}
// _____________________________________________________________________________
void SceneFileParser::parse(const std::string& filename, Scene* scene) const {
  rapidxml::xml_document<> doc;  // character type defaults to char
  char* text = getFileContents(filename);
  if (!text)
    return;
  try {
    doc.parse<0>(text);  // 0 means default parse flags
  } catch (rapidxml::parse_error e) {
    printf("%s\n", e.what());
    printf("Problem: %s\n", (e.where<char>() - 5));
  }
  // now create the scene.
  // first of all create the materials in the material group.
  std::map<char*, Material*, cstringcomp> materialMap;
  rapidxml::xml_node<>* groupStart = parseGroup<Material>(&doc);
  parseGroupSpecial<Material>(groupStart, &materialMap);

  // now add the shapes.
  groupStart = parseGroup<Shape>(&doc);
  parseGroupSpecial<Shape>(groupStart, &materialMap, scene);

  // add all cameras.
  groupStart = parseGroup<Camera>(&doc);
  parseGroupSpecial<Camera>(groupStart, scene);
  // Don`t forget to free the memory!
  free(text);
}
// _____________________________________________________________________________
char* SceneFileParser::getFileContents(const std::string& filename) {
  // Open file.
#ifdef WINDOWS
  FILE* file = nullptr;
  fopen_s(&file, filename.c_str(), "rb");
#else
  FILE* file = fopen(filename.c_str(), "r");
#endif  // WINDOWS

  if (!file)
    return nullptr;
  // file exists.
  // get the number of bytes.
  fseek(file, 0, SEEK_END);
  // TODO(bauschp, Fr 8. Aug 22:39:05 CEST 2014): DONT TRY TO LOAD BINARY FILES!
  size_t sizeInBytes = ftell(file);
  fseek(file, 0, SEEK_SET);
  char* buffer = static_cast<char*>(malloc(sizeInBytes + 1));  // + 1 needed?
  if (fread(buffer, 1LU, sizeInBytes, file) != sizeInBytes) {
    perror("SceneFileParser::getFileContents(): Unexpected file length.\n");
    fclose(file);
    free(buffer);
    return nullptr;
  }
  buffer[sizeInBytes] = 0;
  // close the file.
  fclose(file);
  return buffer;
}
