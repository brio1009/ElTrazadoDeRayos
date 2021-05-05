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

#include "./KDTree.h"

#include <algorithm>
#include <cstdio>
#include <functional>
#include <stack>
#include <utility>
#include <vector>

#include "../Constants.h"
#include "../IntersectionInfo.h"
#include "../shapes/Shape.h"
#include "./AABB.h"

// #define ISLEAF(n) (n.flag & (unsigned int) (1<<31))
#define ISLEAF(n) (n.flag & 0x80000000)
#define OFFSET(n) (n.flag & 0x7FFFFFFC)
#define DIM(n) (n.flag & 0x00000003)

using std::vector;

namespace {}  // namespace

namespace accelerationstructures {
// _____________________________________________________________________________
REAL KDTree::centroidOfItem(
    unsigned int axis,
    const accelerationstructures::KDTree::KDTreeItem& item) {
  // TODO(bauschp): fix this for CompoundShapes and Meshs.
  return item.shape.shape->getPosition()[axis];
}

// _____________________________________________________________________________
REAL KDTree::computeMedianOfAxis(
    unsigned int axis,
    const vector<accelerationstructures::KDTree::KDTreeItem>& items) {
  size_t numElements = items.size();
  vector<REAL> centroids(numElements);
  std::transform(items.begin(), items.end(), centroids.begin(),
                 std::bind(centroidOfItem, axis, std::placeholders::_1));
  std::nth_element(centroids.begin(), centroids.begin() + numElements / 2,
                   centroids.end());
  return centroids[numElements / 2];
}

// _____________________________________________________________________________
IntersectionInfo KDTree::traceRay(const Ray& ray) const {
  IntersectionInfo info;
  if (!m_Initialized) {
    perror("Initialize after adding Shapes!\n");
    return info;
  }

  return info;
}
// _____________________________________________________________________________
void KDTree::initialize() {
  // Already initialized.
  if (m_Initialized)
    return;
  // clear old stuff if exists.
  m_Nodes.clear();
  m_ItemList.clear();

  // create items from the Shapes.
  vector<KDTreeItem> items = shapesToKDItems();

  // build the tree.
  // TODO(bauschp) Is the split fine enough?
  // TODO(bauschp) push all right nodes onto this stack.
  std::stack<std::pair<KDTreeNode, vector<KDTreeItem> > > rightLeafs;
  unsigned int axis = 0;
  REAL median = computeMedianOfAxis(axis, items);
  KDTreeNode root;
  root.inner.splitCoordinate = median;
  //          axis   is a inner   the next node is right after this.
  root.inner.flag = axis ^ 0x80000000 ^ sizeof(KDTreeNode);
  m_Nodes.push_back(root);

  m_Initialized = true;
}

// _____________________________________________________________________________
vector<KDTree::KDTreeItem> KDTree::shapesToKDItems() const {
  auto end = m_Shapes.end();
  vector<KDTree::KDTreeItem> out;
  // TODO(bauschp): Handle Meshes.
  for (auto it = m_Shapes.begin(); it != end; ++it) {
    KDTreeItem item;
    item.shape.type = 's';
    item.shape.shape = *it;
    out.push_back(item);
  }
  return out;
}
// _____________________________________________________________________________
void KDTree::addShape(Shape* shape) {
  m_Shapes.push_back(shape);
  // Has to be reinitialized after adding a shape.
  // Moving shapes after initializing yields undefined behavior.
  m_Initialized = false;
}
// _____________________________________________________________________________
size_t KDTree::size() const {
  return m_Shapes.size();
}
}  // namespace accelerationstructures
