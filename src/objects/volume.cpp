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

#include "../../include/objects/volume.h"
#include "../../include/objects/aabb.h"
#include "../../include/renderer/ray.h"

namespace vxt
{

  bool ConstantMedium::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    Hit h1, h2;
    
    if (boundary_->hit(r, -FLT_MAX, FLT_MAX, h1))
    {
      if (boundary_->hit(r, h1.t + 0.0001f, FLT_MAX, h2))
      {
        if (h1.t < t_min)
        {
          h1.t = t_min;
        }
        if (h2.t > t_max)
        {
          h2.t = t_max;
        }
        if (h1.t >= h2.t)
        {
          return false;
        }
        if (h1.t < 0)
        {
          h1.t = 0;
        }
        float distance_inside_boundary = (h2.t - h1.t) * r.direction().length();
        float hit_distance = - (1 / density_) * log(randomFloat01());
        if (hit_distance < distance_inside_boundary) 
        {
          h.t = h1.t + hit_distance / r.direction().length();
          h.p = r.point_param(h.t);
          h.n = vec3(1, 0, 0);  // Arbitrary
          h.mat = phase_function_;
          return true;
        }
      }
    }
    return false;
  }

  bool ConstantMedium::boundingBox(float t0, float t1, AABB& box) const
  {
    return boundary_->boundingBox(t0, t1, box);
  }

} /* end of vxt namespace */