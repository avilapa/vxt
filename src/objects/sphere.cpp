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

#include "../../include/objects/sphere.h"
#include "../../include/objects/aabb.h"
#include "../../include/renderer/ray.h"

namespace vxt
{

  static void uv(const vec3& p, vec2& uv)
  {
    float phi = atan2(p.z, p.x);
    float theta = asin(p.y);
    uv = vec2(1 - (phi + PI) / (2 * PI), (theta + PI / 2) / PI);
  }

  bool Sphere::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    vec3 dir = r.origin() - center;
    float A = glm::dot(r.direction(), r.direction());
    float B = glm::dot(dir, r.direction());
    float C = glm::dot(dir, dir) - radius * radius;

    float discriminant = B * B - A * C;

    if (discriminant > 0.0f)
    {
      float temp;
      temp = (-B - sqrt(B * B - A * C)) / A;
      if (temp < t_max && temp > t_min)
      {
        h.t = temp;
        h.p = r.point_param(h.t);
        h.n = (h.p - center) / radius;
        uv(h.n, h.uv);
        h.mat = mat;
        return true;
      }
      temp = (-B + sqrt(B * B - A * C)) / A;
      if (temp < t_max && temp > t_min)
      {
        h.t = temp;
        h.p = r.point_param(h.t);
        h.n = (h.p - center) / radius;
        uv(h.n, h.uv);
        h.mat = mat;
        return true;
      }
    }
    return false;
  }

  bool Sphere::boundingBox(float t0, float t1, AABB& box) const
  {
    box = AABB(center - vec3(radius), center + vec3(radius));
    return true;
  }

  vec3 MovingSphere::center(float time) const
  {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
  }

  bool MovingSphere::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    vec3 dir = r.origin() - center(r.time());
    float A = glm::dot(r.direction(), r.direction());
    float B = glm::dot(dir, r.direction());
    float C = glm::dot(dir, dir) - radius * radius;

    float discriminant = B * B - A * C;

    if (discriminant > 0.0f)
    {
      float temp;
      temp = (-B - sqrt(B * B - A * C)) / A;
      if (temp < t_max && temp > t_min)
      {
        h.t = temp;
        h.p = r.point_param(h.t);
        h.n = (h.p - center(r.time())) / radius;
        h.mat = mat;
        return true;
      }
      temp = (-B + sqrt(B * B - A * C)) / A;
      if (temp < t_max && temp > t_min)
      {
        h.t = temp;
        h.p = r.point_param(h.t);
        h.n = (h.p - center(r.time())) / radius;
        h.mat = mat;
        return true;
      }
    }
    return false;
  }

  bool MovingSphere::boundingBox(float t0, float t1, AABB& box) const
  {
    box = AABB(AABB(center(t0) - vec3(radius), center(t0) + vec3(radius)),
               AABB(center(t1) - vec3(radius), center(t1) + vec3(radius)));
    return true;
  }


} /* end of vxt namespace */