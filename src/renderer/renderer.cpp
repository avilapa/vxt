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

#include "../../include/renderer/renderer.h"
#include "../../include/objects/objects.h"
#include "../../include/utils/bitmap.h"

#include <thread>
#include <iostream>

namespace vxt
{

  Renderer::Renderer(uint32 width, uint32 height, uint32 samples_per_pixel) : width_(width), height_(height), spp_(samples_per_pixel)
  {
    pixels_ = new Color[width * height];
    memset(pixels_, 0, width * height);
  }

  Renderer::~Renderer()
  {
    delete pixels_;
  }

  void Renderer::render(Scene* scene, Background* bg, Camera* camera, const char* output_file)
  {
    printf("\nRendering %d x %d image at %d samples per pixel.", width_, height_, spp_);
    printf("\nLaunched 0 rendering threads. Press any key to generate a preview (may take a while for high spp renders).");
    for (uint32 j = 0; j < height_; ++j)
    {
      for (uint32 i = 0; i < width_; ++i)
      {
        // Compute pixel
        vec3 col = renderPixel(scene, bg, camera, i, j);

        // Write pixel
        uint32 idx = j * width_ + i;
        pixels_[idx].r = (uint8)(col.x * 255.99f);
        pixels_[idx].g = (uint8)(col.y * 255.99f);
        pixels_[idx].b = (uint8)(col.z * 255.99f);
        pixels_[idx].a = 0; // not used.
        
        // Generate Preview
        if (_kbhit())
        {
          _getch();
          constexpr uint32 bytes_per_pixel = 4;
          WriteBitmap("preview.bmp", width_, height_, bytes_per_pixel, reinterpret_cast<const uint8*>(pixels_));
        }
      }
    }

    // Save bitmap image
    constexpr uint32 bytes_per_pixel = 4;
    WriteBitmap(output_file, width_, height_, bytes_per_pixel, reinterpret_cast<const uint8*>(pixels_));
  }

  void Renderer::render(Scene* scene, Background* bg, Camera* camera, const char* output_file, uint32 num_threads)
  {
    // Divide image in tiles based on number of threads
    uint32 tile_height = height_ / num_threads;
    for (uint32 i = 0, sum = 0; i < num_threads; ++i)
    {
      sum += tile_height;
      uint32 tile_height_pos = tile_height * i;
      if (i == (num_threads - 1) && sum != height_)
      {
        tile_height = height_ - tile_height_pos;
      }

      tiles_.push_back(Tile{ 0, tile_height_pos, width_, tile_height });
    }

    // Launch threads to render tiles
    std::vector<std::thread> threads;
    for (uint32 i = 0; i < num_threads; i++)
    {
      threads.push_back(std::thread(&Renderer::renderTile, this, scene, bg, camera));
    }

    printf("\nRendering %d x %d image at %d samples per pixel.", width_, height_, spp_);
    printf("\nLaunched %d rendering threads. Press any key to generate a preview (may take a while for high spp renders).", num_threads);

    // Join threads
    for (auto &t : threads)
    {
      if (t.joinable())
      {
        t.join();
      }
    }

    // Save bitmap image
    constexpr uint32 bytes_per_pixel = 4;
    WriteBitmap(output_file, width_, height_, bytes_per_pixel, reinterpret_cast<const uint8*>(pixels_));
  }

  void Renderer::renderTile(Scene* scene, Background* bg, Camera* camera)
  {
    Tile tile;
    uint32 tile_id;
    {
      std::lock_guard<std::mutex> lock(tile_mutex_);

      // Get tile
      tile = tiles_.back();
      tile_id = tiles_.size();
      tiles_.pop_back();
    }

    // Render Tile
    for (uint32 j = tile.y; j < tile.y + tile.h; ++j)
    {
      for (uint32 i = tile.x; i < tile.x + tile.w; ++i)
      {
        vec3 col = renderPixel(scene, bg, camera, i, j);

        // Write pixel
        uint32 idx = j * width_ + i;
        pixels_[idx].r = (uint8)(col.x * 255.99f);
        pixels_[idx].g = (uint8)(col.y * 255.99f);
        pixels_[idx].b = (uint8)(col.z * 255.99f);
        pixels_[idx].a = 0; // not used.

        // Generate Preview
        if (_kbhit())
        {
          _getch();
          constexpr uint32 bytes_per_pixel = 4;
          WriteBitmap("preview.bmp", width_, height_, bytes_per_pixel, reinterpret_cast<const uint8*>(pixels_));
        }
      }
    }
  }

  vec3 Renderer::renderPixel(Scene* scene, Background* bg, Camera* camera, uint32 i, uint32 j)
  {
    // Compute pixel
    vec3 col = vec3(0.0f);
    for (uint32 s = 0; s < spp_; s++)
    {
      float u = float(i + randomFloat01()) / float(width_);
      float v = float(j + randomFloat01()) / float(height_);

      Ray ray = camera->ray(u, v);
      vec3 p = ray.point_param(2.0f);
      col += compute(ray, scene->world(), bg, 0);
    }

    // Average color
    col /= float(spp_);
    col = vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));
    col = glm::clamp(col, vec3(0), vec3(1));

    return col;
  }

  vec3 Renderer::compute(const Ray& r, Hitable *world, Background* bg, int depth)
  {
    Hit hit;
    if (world->hit(r, 0.001f, FLT_MAX, hit))
    {
      Ray scattered;
      vec3 attenuation;
      vec3 emitted = hit.mat->emitted(hit.uv.x, hit.uv.y, hit.p);
      if (depth < 50 && hit.mat->scatter(r, hit, attenuation, scattered))
      {
        return emitted + attenuation * compute(scattered, world, bg, depth + 1);
      }
      else
      {
        return emitted;
      }
    }
    else
    {
      return bg->color(r);
    }
  }

} /* end of vxt namespace */