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

#include "main.h"

#include "../../include/ray.h"
#include "../../include/camera.h"
#include "../../include/material.h"
#include "../../include/hitable_list.h"
#include "../../include/bitmap.h"

#include <limits>

VXR_DEFINE_APP_MAIN(vxt::Main)

namespace vxt
{

  static vec3 color(const Ray& r, Hitable *world, int depth)
  {
    Hit hit;
    if (world->hit(r, 0.001f, FLT_MAX, hit))
    {
      Ray scattered;
      vec3 attenuation;
      if (depth < 50 && hit.mat->scatter(r, hit, attenuation, scattered))
      {
        return attenuation * color(scattered, world, depth + 1);
      }
      else
      {
        return vec3(0.0f);
      }
    }
    else
    {
      vec3 direction_n = glm::normalize(r.direction());
      float t = 0.5f * (direction_n.y + 1.0f);
      return (1.0f - t) * vec3(1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    }
  }

  static Hitable* randomScene() 
  {
    uint32 n = 500;
    Hitable **list = new Hitable*[n + 1];
    list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5, 0.5, 0.5)));
    uint32 i = 1;
    for (int16 a = -11; a < 11; ++a) 
    {
      for (int16 b = -11; b < 11; ++b) 
      {
        float choose_mat = randomFloat01();
        vec3 center(a + 0.9*randomFloat01(), 0.2, b + 0.9*randomFloat01());
        if ((center - vec3(4, 0.2, 0)).length() > 0.9) 
        {
          if (choose_mat < 0.8) 
          {
            // Diffuse
            list[i++] = new Sphere(center, 0.2, new Lambertian(vec3(randomFloat01()*randomFloat01(), randomFloat01()*randomFloat01(), randomFloat01()*randomFloat01())));
          }
          else if (choose_mat < 0.95) 
          {
            // Metal
            list[i++] = new Sphere(center, 0.2, new Metal(vec3(0.5*(1 + randomFloat01()), 0.5*(1 + randomFloat01()), 0.5*(1 + randomFloat01())), 0.5*randomFloat01()));
          }
          else 
          {
            // Glass
            list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
          }
        }
      }
    }

    list[i++] = new Sphere(vec3(0, 1, 0), 1.0, new Dielectric(1.5));
    list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new HitableList(list, i);
  }

  void Main::init()
  {
    constexpr uint32 width = 1200;
    constexpr uint32 height = 800;
    constexpr uint32 ns = 10;
    constexpr uint32 bytes_per_pixel = 4;
    constexpr uint32 total_pixels = width * height;

    Color* pixels = new Color[total_pixels];
    memset(pixels, 0, total_pixels);

    vec3 pos(13, 2, 3);
    vec3 dir(0, 0, 0);
    float distance = 10.0f; // (pos - dir).length();
    float aperture = 0.1f;
    
    Camera cam(pos, dir, vec3(0, 1, 0), 20, float(width) / float(height), aperture, distance);

    constexpr uint32 NUM_OBJECTS = 5;
    Hitable *list[NUM_OBJECTS];
    list[0] = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new Sphere(vec3(0, -100.5, -1), 100, new Lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new Sphere(vec3(1, -0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2), 0.3f));
    list[3] = new Sphere(vec3(-1, -0, -1), 0.5, new Dielectric(1.5f));
    list[4] = new Sphere(vec3(-1, -0, -1), -0.45, new Dielectric(1.5f));

    Hitable *world = new HitableList(list, NUM_OBJECTS);

    world = randomScene(); // Override world with randomScene()

    for (uint32 j = 0; j < height; ++j)
    {
      for (uint32 i = 0; i < width; ++i)
      {
        vec3 col = vec3(0.0f);
        for (uint32 s = 0; s < ns; s++)
        {
          float u = float(i + randomFloat01()) / float(width);
          float v = float(j + randomFloat01()) / float(height);

          Ray ray = cam.ray(u, v);
          vec3 p = ray.point_param(2.0f); 
          col += color(ray, world, 0);
        }
        col /= float(ns);
        col = vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));
        uint32 idx = j * width + i;
        pixels[idx].r = (uint8)(col.x * 255.99f);
        pixels[idx].g = (uint8)(col.y * 255.99f);
        pixels[idx].b = (uint8)(col.z * 255.99f);
        pixels[idx].a = 0; // not used.
      }
      system("cls");
      printf("\nProgress: [%d / %d]", j, height);
    }

    WriteBitmap(width, height, bytes_per_pixel, reinterpret_cast<const uint8*>(pixels));
    delete pixels;

    exit_ = true;
    Application::init();
  }

} /* end of vxt namespace */