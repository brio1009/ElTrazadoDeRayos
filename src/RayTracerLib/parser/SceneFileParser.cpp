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

#include <cstdio>
#include <string>
#include <map>
#include <unordered_map>

#include "./Scene.h"
#include "factories/Factory.h"
#include "shapes/Shape.h"
#include "cameras/PerspectiveCamera.h"


using std::string;

// _____________________________________________________________________________
void SceneFileParser::parse(const std::string& filename, Scene* scene) const {
  rapidxml::xml_document<> doc;    // character type defaults to char
  char* text = getFileContents(filename);
  if (!text)
    return;
  try {
    doc.parse<0>(text);    // 0 means default parse flags
  } catch (rapidxml::parse_error e) {
    printf("%s\n", e.what());
    printf("Problem: %s\n", (e.where<char>() - 5));
  }
  // now create the scene.
  // first of all create the materials in the material group.
  // TODO(bauschp, Fr 8. Aug 23:38:28 CEST 2014): change to material.
  rapidxml::xml_node<>* node = doc.first_node("shapes");
  std::unordered_map<char*, Shape*> materialMap;
  rapidxml::xml_node<>* child = node->first_node();
  // TODO(bauschp, Fr 8. Aug 23:21:24 CEST 2014): automaticly read the class.
  while (child) {
    // add this child.
    // TODO(bauschp, Fr 8. Aug 23:28:39 CEST 2014): check if pointer alreadt ex.
    Shape* shape = Factory<Shape>::Create(child->name());
    // call all the needed atributes.
    for (rapidxml::xml_attribute<>* attr = child->first_attribute();
         attr; attr = attr->next_attribute()) {
      printf("Calling %s with value %s on %s\n", string("set").append(attr->name()).c_str(), attr->value(), child->name());
      shape->setFromString(attr->name(), attr->value());
    }
    scene->addShape(shape);
    child = child->next_sibling();
  }
  // TODO(bauschp, Sa 9. Aug 09:08:20 CEST 2014): Read the cams from the file.
  float angle = glm::pi<float>() * 2.0f;
  glm::mat4 trans = glm::rotate(glm::mat4(1.0),
      (angle / 20) * (-1), glm::vec3(0, 1, 0));
  trans = glm::translate(trans, glm::vec3(0, 0, 23));
  PerspectiveCamera* cam = new PerspectiveCamera(1280, 720,
                                                 glm::radians(85.0f));
  cam->transform(trans);
  cam->setUsePostProcessing(false);
  scene->cameras().push_back(cam);
  // Don`t forget to free the memory!
  free(text);
}
// _____________________________________________________________________________
char* SceneFileParser::getFileContents(const std::string& filename) {
  FILE* file = fopen(filename.c_str(), "r");
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
    perror("unexpected file length\n");
    fclose(file);
    free(buffer);
    return nullptr;
  }
  buffer[sizeInBytes] = 0;
  // close the file.
  fclose(file);
  return buffer;
}
