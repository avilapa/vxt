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

#include "scene.h"
#include <mutex>

/**
* \file renderer.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Renderer class.
*
*/
namespace vxt
{

  class Ray;

  class Renderer
  {
  public:
    Renderer(uint32 width, uint32 height, uint32 samples_per_pixel);
    ~Renderer();

    void render(Scene* scene, Background* bg, Camera* camera, const char* output_file);
    void render(Scene* scene, Background* bg, Camera* camera, const char* output_file, uint32 num_threads);

  private:
    void renderTile(Scene* scene, Background* bg, Camera* camera, const uvec4& tile);
    vec3 renderPixel(Scene* scene, Background* bg, Camera* camera, uint32 i, uint32 j);
    vec3 compute(const Ray& r, Hitable* world, Background* bg, int depth);

  private:
    uint32 width_, height_, spp_;
    Color* pixels_;
  };

} /* end of vxt namespace */