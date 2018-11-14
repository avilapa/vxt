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
#include "../utils/perlin.h"

/**
* \file texture.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief .
*
*/
namespace vxt
{

  class Texture
  {
  public:
    virtual vec3 value(float u, float v, const vec3& p) const = 0;
  };

  class ConstantTexture : public Texture
  {
  public:
    ConstantTexture() {}
    ConstantTexture(vec3 c) : color(c) {}

    virtual vec3 value(float u, float v, const vec3& p) const { return color; };

  private:
    vec3 color;
  };

  class CheckerTexture : public Texture
  {
  public:
    CheckerTexture() {}
    CheckerTexture(Texture* t0, Texture* t1) : even(t0), odd(t1) {}

    virtual vec3 value(float u, float v, const vec3& p) const;

  private:
    Texture* odd;
    Texture* even;
  };

  class NoiseTexture : public Texture
  {
  public:
    NoiseTexture() {}
    NoiseTexture(float sc) : scale(sc) {}
    
    virtual vec3 value(float u, float v, const vec3& p) const;

  private:
    PerlinNoise noise;
    float scale = 1.0f;
  };

  class ImageTexture : public Texture
  {
  public:
    ImageTexture() {}
    ImageTexture(unsigned char* pixels, int A, int B) : data(pixels), nx(A), ny(B) {}
    ImageTexture(const char* file);
    ~ImageTexture();

    virtual vec3 value(float u, float v, const vec3& p) const;

  private:
    unsigned char* data;
    int nx, ny;
  };

} /* end of vxt namespace */