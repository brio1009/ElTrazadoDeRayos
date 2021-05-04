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


#ifndef RAYTRACERLIB_PARSER_OBJPARSER_H_
#define RAYTRACERLIB_PARSER_OBJPARSER_H_

#include <glm/glm.hpp>

#include <vector>
#include <string>


namespace parser {

/// This function parses a file given by filename.
/// the return value indicates if the file parsed correctly.
/// The three pointers will be set with the data contained in the file.
bool parseObjFile(
    const std::string& filename,
    std::vector<glm::vec3>* vertices,
    std::vector<glm::vec3>* normals,
    std::vector<glm::vec2>* uvCoords);

}  // namespace parser
#endif  // RAYTRACERLIB_PARSER_OBJPARSER_H_

