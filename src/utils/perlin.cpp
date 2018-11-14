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

#include "../../include/utils/perlin.h"

namespace vxt
{

  float PerlinNoise::noise(const vec3& p) const 
  {
    float u = p.x - floor(p.x);
    float v = p.y - floor(p.y);
    float w = p.z - floor(p.z);
    int i = floor(p.x);
    int j = floor(p.y);
    int k = floor(p.z);
    vec3 c[2][2][2];
    for (uint32 di = 0; di < 2; ++di)
    {
      for (uint32 dj = 0; dj < 2; ++dj)
      {
        for (uint32 dk = 0; dk < 2; ++dk)
        {
          c[di][dj][dk] = randomVec_[perm_x_[(i + di) & 255] ^ perm_y_[(j + dj) & 255] ^ perm_z_[(k + dk) & 255]];
        }
      }
    }
    return perlinInterp(c, u, v, w);
  }

  float PerlinNoise::turb(const vec3& p, int depth /* = 7 */) const
  {
    float accum = 0;
    vec3 temp_p = p;
    float weight = 1.0f;
    for (uint32 i = 0; i < depth; ++i)
    {
      accum += weight * noise(temp_p);
      weight *= 0.5f;
      temp_p *= 2.0f;
    }
    return fabs(accum);
  }

  static vec3* generatePerlinNoise()
  {
    vec3* p = new vec3[256];
    for (uint32 i = 0; i < 256; ++i)
    {
      p[i] = glm::normalize(vec3(-1 + 2 * randomFloat01(), -1 + 2 * randomFloat01(), -1 + 2 * randomFloat01()));
    }
    return p;
  }

  static void permute(int* p, int n)
  {
    for (uint32 i = n - 1; i > 0; --i)
    {
      uint32 target = uint32(randomFloat01() * (i + 1));
      uint32 tmp = p[i];
      p[i] = p[target];
      p[target] = tmp;
    }
  }

  static int* generatePermPerlinNoise()
  {
    int* p = new int[256];
    for (uint32 i = 0; i < 256; ++i)
    {
      p[i] = i;
    }
    permute(p, 256);
    return p;
  }

  vec3* PerlinNoise::randomVec_ = generatePerlinNoise();
  int* PerlinNoise::perm_x_ = generatePermPerlinNoise();
  int* PerlinNoise::perm_y_ = generatePermPerlinNoise();
  int* PerlinNoise::perm_z_ = generatePermPerlinNoise();

} /* end of vxt namespace */