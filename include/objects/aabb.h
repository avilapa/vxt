#pragma once

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

#include "../core/types.h"
#include "../renderer/ray.h"

/**
* \file aabb.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Axis Aligned Bounding Box.
*
*/
namespace vxt
{

  inline float ffmin(float a, float b) { return a < b ? a : b; }
  inline float ffmax(float a, float b) { return a > b ? a : b; }

  class AABB
  {
  public:
    AABB() {}
    AABB(const vec3& A, const vec3& B) : min_(A), max_(B) {}
    AABB(AABB box0, AABB box1);


    vec3 min_vec() const { return min_; }
    vec3 max_vec() const { return max_; }

    inline bool hit(const Ray& r, float tmin, float tmax) const
    {
      for (uint32 i = 0; i < 3; ++i)
      {
        /*float t0 = ffmin((min_[i] - r.origin()[i]) / r.direction()[i],
          (max_[i] - r.origin()[i]) / r.direction()[i]);
        float t1 = ffmax((min_[i] - r.origin()[i]) / r.direction()[i],
          (max_[i] - r.origin()[i]) / r.direction()[i]);

        tmin = ffmax(t0, tmin);
        tmax = ffmin(t1, tmax);
        */

        float invD = 1.0f / r.direction()[i];
        float t0 = (min_[i] - r.origin()[i]) * invD;
        float t1 = (max_[i] - r.origin()[i]) * invD;

        if (invD < 0.0f)
        {
          std::swap(t0, t1);
        }

        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 > tmax ? t1 : tmax;

        if (tmax <= tmin)
        {
          return false;
        }
      }
      return true;
    }

  private:
    vec3 min_;
    vec3 max_;
  };

} /* end of vxt namespace */