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

#include "../../include/materials/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../deps/stb/stb_image.h"

namespace vxt
{
  vec3 CheckerTexture::value(float u, float v, const vec3& p) const
  {
    float sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
    if (sines < 0)
    {
      return odd->value(u, v, p);
    }
    else
    {
      return even->value(u, v, p);
    }
  }

  vec3 NoiseTexture::value(float u, float v, const vec3& p) const
  {
    // Noise
    //return vec3(1) * 0.5f * (1 + noise.noise(p * scale));
    // Turbulence
    //return vec3(1) * (noise.turb(p * scale)); 
    // Marble
    return vec3(1) * 0.5f * (1 + sin(scale * p.z + 10 * noise.turb(p)));
  }

  ImageTexture::~ImageTexture()
  {
    if (data != nullptr)
    {
      free(data);
      data = 0;
    }
  }

  vec3 ImageTexture::value(float u, float v, const vec3& p) const
  {
    int i = u * nx;
    int j = (1 - v) * ny - 0.001;

    if (i < 0) i = 0;
    if (j < 0) j = 0;
    if (i > nx - 1) i = nx - 1;
    if (j > ny - 1) j = ny - 1;
    return vec3(int(data[3 * i + 3 * nx * j + 0]) / 255.0f,
                int(data[3 * i + 3 * nx * j + 1]) / 255.0f,
                int(data[3 * i + 3 * nx * j + 2]) / 255.0f);
  }

  ImageTexture::ImageTexture(const char* file)
  {
    int nn;
    data = stbi_load(file, &nx, &ny, &nn, 0);
    
    if (data == nullptr)
    {
      printf("Texture could not be loaded");
    }
  }

} /* end of vxt namespace */