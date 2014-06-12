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

#include "./Sampler.h"

#include <glm/glm.hpp>
#include <vector>

#include "./Ray.h"
#include "./Scene.h"
#include "./IntersectionInfo.h"
#include "./Material.h"

using std::vector;

// _____________________________________________________________________________
Color Sampler::getSampledColor(
      const std::vector<Ray>& borders,
      const Scene& scene) const {
 vector<float> lambdas; 
 vector<Color> colors;
 size_t startLambda = lambdas.size();
 while(addNextLambdasToList(&lambdas)) {
   size_t endLambda = lambdas.size() - 1;
   // Calculate the Ray for given lambdas
   Ray sample = createRayByLambdas(lambdas, startLambda, endLambda, borders);
   // Get the color for given sample Ray.
   IntersectionInfo info = scene.traceRay(sample);
   if (info.materialPtr) {
     colors.push_back(info.materialPtr->getColor(info,
       sample,
       scene));
   } else {
     colors.push_back(scene.backgroundColor(sample));
   }
   startLambda = lambdas.size();
 }
 return reconstructColor(colors, lambdas);
}
// _____________________________________________________________________________
bool Sampler::addNextLambdasToList(
      std::vector<float>* lambdas) const {
  // This creation only works for two lambdas.
  assert(lambdas->size() % 2 == 0);
  size_t nextSampleIndex = lambdas->size() / 2;
  // Get the next lambdas.
  try {
    vector<float> nextLambdas = getLambdasForSample(nextSampleIndex);
    // Append the lambdas to the list
    lambdas->insert(lambdas->end(), nextLambdas.begin(), nextLambdas.end());
    return true;
  } catch (int) {
    // Whenever an exception occures we couldnt produce a new sample.
    return false;
  } catch ( ... ) {
    throw;
  }
}
// _____________________________________________________________________________
Ray Sampler::createRayByLambdas(
      const std::vector<float>& lambdas,
      const size_t& start,
      const size_t& end,
      const std::vector<Ray>& borders) const {
  assert(borders.size() == 4);
  assert(end - start == 1);
  assert(lambdas.size() > end);
  // Interpolate between origin and the other
  // TODO(bauschp, Tue Jun 10 20:46:33 CEST 2014): This is fugly.
  glm::vec4 nextSampleDir =
      borders.at(0).direction()
        * (1.0f - lambdas.at(start)) * (1.0f - lambdas.at(end))
      + borders.at(1).direction()
        * lambdas.at(start) * (1.0f - lambdas.at(end))
      + borders.at(2).direction()
        * (1.0f - lambdas.at(start)) * lambdas.at(end)
      + borders.at(3).direction()
        * lambdas.at(start) * lambdas.at(end);
  glm::vec4 nextSamplePos =
      borders.at(0).origin()
        * (1.0f - lambdas.at(start)) * (1.0f - lambdas.at(end))
      + borders.at(1).origin()
        * lambdas.at(start) * (1.0f - lambdas.at(end))
      + borders.at(2).origin()
        * (1.0f - lambdas.at(start)) * lambdas.at(end)
      + borders.at(3).origin()
        * lambdas.at(start) * lambdas.at(end);
  return Ray(nextSamplePos, nextSampleDir);
}
