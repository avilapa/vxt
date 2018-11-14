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

/**
* \file perlin.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Bitmap utility functions.
*
*/
namespace vxt
{

  class PerlinNoise
  {
  public:
    float noise(const vec3& p) const;
    float turb(const vec3& p, int depth = 7) const;

    static vec3 *randomVec_;
    static int *perm_x_;
    static int *perm_y_;
    static int *perm_z_;
  };
 
  inline float perlinInterp(vec3 c[2][2][2], float u, float v, float w)
  {
    float uu = u * u * (3 - 2 * u);
    float vv = v * v * (3 - 2 * v);
    float ww = w * w * (3 - 2 * w);
    float accum = 0.0f;
    for (uint32 i = 0; i < 2; ++i)
    {
      for (uint32 j = 0; j < 2; ++j)
      {
        for (uint32 k = 0; k < 2; ++k)
        {
          vec3 weight_vec(u - i, v - j, w - k);
          accum += (i * uu + (1 - i) * (1 - uu)) * 
                   (j * vv + (1 - j) * (1 - vv)) * 
                   (k * ww + (1 - k) * (1 - ww)) *
                   glm::dot(c[i][j][k], weight_vec);
        }
      }
    }
    return accum;
  }

} /* end of vxt namespace */