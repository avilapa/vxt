// ----------------------------------------------------------------------------------------
// MIT License
// 
// Copyright(c) 2018 Víctor Ávila
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ----------------------------------------------------------------------------------------

#include "../../include/objects/bvh.h"

#include <iostream>

namespace vxt
{

  static int boxCompareX(const void* a, const void* b)
  {
    AABB box_left, box_right;
    Hitable *ah = *(Hitable**)a;
    Hitable *bh = *(Hitable**)b;

    if (!ah->boundingBox(0, 0, box_left) || !bh->boundingBox(0, 0, box_right))
    {
      std::cerr << "No bounding box in BVHNode constructor\n";
    }

    if (box_left.min_vec().x - box_right.min_vec().x < 0.0f)
    {
      return -1;
    }
    else
    {
      return 1;
    }
  }

  static int boxCompareY(const void* a, const void* b)
  {
    AABB box_left, box_right;
    Hitable *ah = *(Hitable**)a;
    Hitable *bh = *(Hitable**)b;

    if (!ah->boundingBox(0, 0, box_left) || !bh->boundingBox(0, 0, box_right))
    {
      std::cerr << "No bounding box in BVHNode constructor\n";
    }

    if (box_left.min_vec().y - box_right.min_vec().y < 0.0f)
    {
      return -1;
    }
    else
    {
      return 1;
    }
  }

  static int boxCompareZ(const void* a, const void* b)
  {
    AABB box_left, box_right;
    Hitable *ah = *(Hitable**)a;
    Hitable *bh = *(Hitable**)b;

    if (!ah->boundingBox(0, 0, box_left) || !bh->boundingBox(0, 0, box_right))
    {
      std::cerr << "No bounding box in BVHNode constructor\n";
    }

    if (box_left.min_vec().z - box_right.min_vec().z < 0.0f)
    {
      return -1;
    }
    else
    {
      return 1;
    }
  }

  BVHNode::BVHNode(Hitable **l, int n, float time0, float time1)
  {
    uint32 axis = uint32(3 * randomFloat01());

    if (axis == 0)
    {
      qsort(l, n, sizeof(Hitable*), boxCompareX);
    }
    else if (axis == 1)
    {
      qsort(l, n, sizeof(Hitable*), boxCompareY);
    }
    else
    {
      qsort(l, n, sizeof(Hitable*), boxCompareZ);
    }

    if (n == 1)
    {
      left = right = l[0];
    }
    else if (n == 2)
    {
      left = l[0];
      right = l[1];
    }
    else
    {
      left = new BVHNode(l, n / 2, time0, time1);
      right = new BVHNode(l + n / 2, n - n / 2, time0, time1);
    }

    AABB box_left, box_right;

    if (!left->boundingBox(time0, time1, box_left) || !right->boundingBox(time0, time1, box_right))
    {
      std::cerr << "No bounding box in BVHNode constructor\n";
    }

    box = AABB(box_left, box_right);
  }

  bool BVHNode::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    if (box.hit(r, t_min, t_max))
    {
      Hit left_hit, right_hit;
      bool hit_left = left->hit(r, t_min, t_max, left_hit);
      bool hit_right = right->hit(r, t_min, t_max, right_hit);

      if (hit_left && hit_right)
      {
        if (left_hit.t < right_hit.t)
        {
          h = left_hit;
        }
        else
        {
          h = right_hit;
        }
        return true;
      }
      else if (hit_left)
      {
        h = left_hit;
        return true;
      }
      else if (hit_right)
      {
        h = right_hit;
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }

  bool BVHNode::boundingBox(float t0, float t1, AABB& b) const
  {
    b = box;
    return true;
  }

} /* end of vxt namespace */