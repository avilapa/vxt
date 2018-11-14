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

#include "../../include/objects/rect.h"
#include "../../include/objects/aabb.h"
#include "../../include/renderer/ray.h"

namespace vxt
{

  bool XYRect::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    float t = (k_ - r.origin().z) / r.direction().z;

    if (t < t_min || t > t_max)
    {
      return false;
    }

    float x = r.origin().x + t * r.direction().x;
    float y = r.origin().y + t * r.direction().y;

    if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
    {
      return false;
    }

    h.p = r.point_param(t);
    h.n = vec3(0, 0, 1);
    h.uv = vec2((x - x0_) / (x1_ - x0_), (y - y0_) / (y1_ - y0_));
    h.t = t;
    h.mat = mat;
    return true;
  }

  bool XYRect::boundingBox(float t0, float t1, AABB& box) const
  {
    box = AABB(vec3(x0_, y0_, k_ - 0.0001f), vec3(x1_, y1_, k_ + 0.0001f));
    return true;
  }

  bool XZRect::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    float t = (k_ - r.origin().y) / r.direction().y;

    if (t < t_min || t > t_max)
    {
      return false;
    }

    float x = r.origin().x + t * r.direction().x;
    float z = r.origin().z + t * r.direction().z;

    if (x < x0_ || x > x1_ || z < z0_ || z > z1_)
    {
      return false;
    }

    h.p = r.point_param(t);
    h.n = vec3(0, 1, 0);
    h.uv = vec2((x - x0_) / (x1_ - x0_), (z - z0_) / (z1_ - z0_));
    h.t = t;
    h.mat = mat;
    return true;
  }

  bool XZRect::boundingBox(float t0, float t1, AABB& box) const
  {
    box = AABB(vec3(x0_, k_ - 0.0001f, z0_), vec3(x1_, k_ + 0.0001f, z1_));
    return true;
  }

  bool YZRect::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    float t = (k_ - r.origin().x) / r.direction().x;

    if (t < t_min || t > t_max)
    {
      return false;
    }

    float y = r.origin().y + t * r.direction().y;
    float z = r.origin().z + t * r.direction().z;

    if (y < y0_ || y > y1_ || z < z0_ || z > z1_)
    {
      return false;
    }

    h.p = r.point_param(t);
    h.n = vec3(1, 0, 0);
    h.uv = vec2((y - y0_) / (y1_ - y0_), (z - z0_) / (z1_ - z0_));
    h.t = t;
    h.mat = mat;
    return true;
  }

  bool YZRect::boundingBox(float t0, float t1, AABB& box) const
  {
    box = AABB(vec3(k_ - 0.0001f, y0_, z0_), vec3(k_ + 0.0001f, y1_, z1_));
    return true;
  }

} /* end of vxt namespace */