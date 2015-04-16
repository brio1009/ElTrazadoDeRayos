/*
The MIT License (MIT)

Copyright (c) 2015 CantTouchDis <bauschp@informatik.uni-freiburg.de>
Copyright (c) 2015 brio1009 <christoph1009@gmail.com>

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

#include "./ObjParser.h"

#include <glm/glm.hpp>

#include <cstdio>
#include <cstring>
#include <vector>
#include <string>


using std::string;
using std::vector;

using glm::vec3;
using glm::vec2;

namespace {
// reads a line of the file to the buffer vector. Returns the last character.
int readLine(FILE* file, vector<char>* buffer) {
  // ensure buffer is clear.
  buffer->clear();
  int character = EOF;
  // read as long as we are allowed to.
  while ((character = fgetc(file)) != EOF
      && character != '\n') {
    buffer->push_back(static_cast<char>(character));
  }
  // append 0 to ensure buffer.data works like a cstring.
  buffer->push_back('\0');
  return character;
}

void interpretLine(
    const vector<char>& buffer,
    vector<vec3>* vertices,
    vector<vec3>* normals,
    vector<vec2>* uvCoords,
    vector<unsigned int>* vIndices,
    vector<unsigned int>* nIndices,
    vector<unsigned int>* uvIndices) {
  if (strncmp(buffer.data(), "v ", 2) == 0) {
    // This line contains vertex data.
    vec3 vertex;
    sscanf(buffer.data(), "%*s %f %f %f", &vertex.x, &vertex.y, &vertex.z);
    vertices->push_back(vertex);
  }

  if (strncmp(buffer.data(), "vn ", 3) == 0) {
    // This line contains a normal.
    vec3 normal;
    sscanf(buffer.data(), "%*s %f %f %f", &normal.x, &normal.y, &normal.z);
    normals->push_back(normal);
  }

  if (strncmp(buffer.data(), "vt ", 3) == 0) {
    // This line contains uv coordinates.
    vec2 uv;
    sscanf(buffer.data(), "%*s %f %f", &uv.x, &uv.y);
    uvCoords->push_back(uv);
  }
  if (strncmp(buffer.data(), "f  ", 2) == 0) {
    // This line contains a face.
    unsigned int vInd[3], nInd[3], uvInd[3];
    int numMatches = sscanf(buffer.data(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
        &vInd[0], &nInd[0], &uvInd[0],
        &vInd[1], &nInd[1], &uvInd[1],
        &vInd[2], &nInd[2], &uvInd[2]);
    if (numMatches != 9) {
      fprintf(stderr, "couldn't parse a face\n");
      return;
    }
    // vertex indices
    vIndices->push_back(vInd[0]);
    vIndices->push_back(vInd[1]);
    vIndices->push_back(vInd[2]);
    // normal indices
    nIndices->push_back(nInd[0]);
    nIndices->push_back(nInd[1]);
    nIndices->push_back(nInd[2]);
    // uv Indices
    uvIndices->push_back(uvInd[0]);
    uvIndices->push_back(uvInd[1]);
    uvIndices->push_back(uvInd[2]);
  }
}
}  // namespace

namespace parser {

// _____________________________________________________________________________
bool parseObjFile(
    const string& filename,
    vector<vec3>* vertices,
    vector<vec3>* normals,
    vector<vec2>* uvCoords) {
  // opengl-tutorial.org like obj parser.

  // Open the file.
#ifdef WINDOWS
  FILE* file = nullptr;
  fopen_s(&file, filename.c_str(), "rb");
#else
  FILE* file = fopen(filename.c_str(), "r");
#endif  // WINDOWS

  // Is the file ready to be read?
  if (!file) {
    fprintf(stderr, "ERROR: could not open file %s\n", filename.c_str());
    return false;
  }

  // Tmp variables to store read informations.
  vector<unsigned int> vIndices, nIndices, uvIndices;
  vector<vec3> tmpVertices;
  vector<vec3> tmpNormals;
  vector<vec2> tmpUVCoords;
  // stores the line.
  vector<char> buffer;
  // Read the file line by line into buffer.
  while (readLine(file, &buffer) != EOF) {
    // The line is now stored in buffer.
    interpretLine(buffer, &tmpVertices, &tmpNormals, &tmpUVCoords,
        &vIndices, &nIndices, &uvIndices);
  }
  // Last line of the file is parsed here.
  interpretLine(buffer, &tmpVertices, &tmpNormals, &tmpUVCoords,
      &vIndices, &nIndices, &uvIndices);
  // We are done reading the file.
  fclose(file);
  // Now we remove the indeces and store every face in the output vectors.
  size_t numIndices = vIndices.size();
  // Resize the output vectors.
  vertices->clear();
  vertices->reserve(numIndices);
  normals->clear();
  normals->reserve(numIndices);
  uvCoords->clear();
  uvCoords->reserve(numIndices);
  for (size_t i = 0; i < numIndices; ++i) {
    vertices->push_back(tmpVertices[vIndices[i] - 1]);
    normals->push_back(tmpNormals[nIndices[i] - 1]);
    uvCoords->push_back(tmpUVCoords[uvIndices[i] - 1]);
  }
  printf("parsed a obj file with %lu faces\n", numIndices / 3);
  return true;
}

}  // namespace parser
