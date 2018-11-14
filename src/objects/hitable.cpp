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

#include "../../include/objects/hitable.h"
#include "../../include/objects/aabb.h"
#include "../../include/renderer/ray.h"

namespace vxt
{

  void HitableList::add(Hitable* hitable)
  {
    hitables_.push_back(hitable);
  }

  bool HitableList::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    Hit temp_hit;
    bool hit_anything = false;
    double closest = t_max;
    for (uint32 i = 0; i < hitables_.size(); ++i)
    {
      if (hitables_[i]->hit(r, t_min, closest, temp_hit))
      {
        hit_anything = true;
        closest = temp_hit.t;
        h = temp_hit;
      }
    }
    return hit_anything;
  }

  bool HitableList::boundingBox(float t0, float t1, AABB& box) const
  {
    if (hitables_.size() < 1)
    {
      return false;
    }

    AABB temp_box;
    if (hitables_[0]->boundingBox(t0, t1, temp_box))
    {
      return false;
    }
    else
    {
      box = temp_box;
    }

    for (uint32 i = 0; i < hitables_.size(); ++i)
    {
      if (hitables_[0]->boundingBox(t0, t1, temp_box))
      {
        box = AABB(box, temp_box);
      }
      else
      {
        return false;
      }
    }
    return true;
  }

  bool FlipNormals::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    if (hitable_->hit(r, t_min, t_max, h))
    {
      h.n = -h.n;
      return true;
    }
    else
    {
      return false;
    }
  }

  bool FlipNormals::boundingBox(float t0, float t1, AABB& box) const
  {
    return hitable_->boundingBox(t0, t1, box);
  }

  bool Translate::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    Ray moved_r(r.origin() - offset_, r.direction(), r.time());
    if (hitable_->hit(moved_r, t_min, t_max, h))
    {
      h.p += offset_;
      return true;
    }
    else
    {
      return false;
    }
  }

  bool Translate::boundingBox(float t0, float t1, AABB& box) const
  {
    if (hitable_->boundingBox(t0, t1, box))
    {
      box = AABB(box.min_vec() + offset_, box.max_vec() + offset_);
      return true;
    }
    else
    {
      return false;
    }
  }

  RotateY::RotateY(Hitable* p, float angle) : hitable_(p)
  {
    float radians = (PI / 180.0f) * angle;
    sin_theta_ = sin(radians);
    cos_theta_ = cos(radians);
    has_box_ = hitable_->boundingBox(0, 1, bbox_);
    vec3 min_f(FLT_MAX);
    vec3 max_f(-FLT_MAX);

    for (uint32 i = 0; i < 2; ++i)
    {
      for (uint32 j = 0; j < 2; ++j)
      {
        for (uint32 k = 0; k < 2; ++k)
        {
          float x = i * bbox_.max_vec().x + (1 - i) * bbox_.min_vec().x;
          float y = j * bbox_.max_vec().y + (1 - j) * bbox_.min_vec().y;
          float z = k * bbox_.max_vec().z + (1 - k) * bbox_.min_vec().z;
          float newx = cos_theta_ * x + sin_theta_ * z;
          float newz = -sin_theta_ * x + cos_theta_ * z;
          vec3 tester(newx, y, newz);

          for (uint32 c = 0; c < 3; ++c)
          {
            if (tester[c] > max_f[c])
            {
              max_f[c] = tester[c];
            }
            if (tester[c] < min_f[c])
            {
              min_f[c] = tester[c];
            }
          }
        }
      }
    }
    bbox_ = AABB(min_f, max_f);
  }

  bool RotateY::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    vec3 origin = r.origin();
    vec3 direction = r.direction();
    origin.x = cos_theta_ * r.origin().x - sin_theta_ * r.origin().z;
    origin.z = sin_theta_ * r.origin().x + cos_theta_ * r.origin().z;
    direction.x = cos_theta_ * r.direction().x - sin_theta_ * r.direction().z;
    direction.z = sin_theta_ * r.direction().x + cos_theta_ * r.direction().z;
    Ray rotated_r(origin, direction, r.time());
    if (hitable_->hit(rotated_r, t_min, t_max, h))
    {
      vec3 p = h.p;
      vec3 n = h.n;
      p.x = cos_theta_ * h.p.x + sin_theta_ * h.p.z;
      p.z = -sin_theta_ * h.p.x + cos_theta_ * h.p.z;
      n.x = cos_theta_ * h.n.x + sin_theta_ * h.n.z;
      n.z = -sin_theta_ * h.n.x + cos_theta_ * h.n.z;
      h.p = p;
      h.n = n;
      return true;
    }
    else
    {
      return false;
    }
  }

  bool RotateY::boundingBox(float t0, float t1, AABB& box) const
  {
    box = bbox_;
    return has_box_;
  }

} /* end of vxt namespace */