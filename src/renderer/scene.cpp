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

#include "../../include/renderer/scene.h"
#include "../../include/objects/objects.h"
#include "../../include/materials/texture.h"

namespace vxt
{

  vec3 GradientBackground::color(const Ray& r) const
  {
    vec3 direction_n = glm::normalize(r.direction());
    float t = 0.5f * (direction_n.y + 1.0f);
    return (1.0f - t) * vec3(1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
  }

  Scene::Scene()
  {
    world_ = new HitableList();
  }

  Scene::~Scene()
  {
    delete world_;
  }

  void Scene::add(Hitable* hitable)
  {
    world_->add(hitable);
  }

  RandomScene::RandomScene()
  {
    Texture *checker = new CheckerTexture(new ConstantTexture(vec3(0.2, 0.3, 0.1)), new ConstantTexture(vec3(0.9)));
    world_->add(new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(checker)));
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
            world_->add(new MovingSphere(center, center + vec3(0, 0.5 * randomFloat01(), 0), 0.0, 1.0, 0.2, new Lambertian(new ConstantTexture(vec3(randomFloat01()*randomFloat01(), randomFloat01()*randomFloat01(), randomFloat01()*randomFloat01())))));
          }
          else if (choose_mat < 0.95)
          {
            // Metal
            world_->add(new Sphere(center, 0.2, new Metal(new ConstantTexture(vec3(0.5*(1 + randomFloat01()), 0.5*(1 + randomFloat01()), 0.5*(1 + randomFloat01()))), 0.5*randomFloat01())));
          }
          else
          {
            // Glass
            world_->add(new Sphere(center, 0.2, new Dielectric(1.5)));
          }
        }
      }
    }

    world_->add(new Sphere(vec3(0, 1, 0), 1.0, new Dielectric(1.5)));
    world_->add(new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(new ConstantTexture(vec3(0.4, 0.2, 0.1)))));
    world_->add(new Sphere(vec3(4, 1, 0), 1.0, new Metal(new ConstantTexture(vec3(0.7, 0.6, 0.5)), 0.0)));
  }

  Camera* RandomScene::camera(uint32 w, uint32 h)
  {
    vec3 pos(13, 2, 3);
    vec3 dir(0, 0, 0);
    return new Camera(pos, dir, vec3(0, 1, 0), 20, float(w) / float(h), 0.1f, 10.0f, 0.0f, 1.0f);
  }

  PerlinScene::PerlinScene()
  {
    Texture *perlin = new NoiseTexture(2.0f);
    world_->add(new Sphere(vec3(0, -1001.75, 0), 1000, new Lambertian(new ConstantTexture(vec3(0.8f)))));
    world_->add(new Sphere(vec3(0, 0.25, 0), 2, new Lambertian(perlin)));
  }
  
  Camera* PerlinScene::camera(uint32 w, uint32 h)
  {
    vec3 pos(13, 2, 3);
    vec3 dir(0, 0, 0);
    return new Camera(pos, dir, vec3(0, 1, 0), 20, float(w) / float(h), 0.1f, 10.0f, 0.0f, 1.0f);
  }

  EarthScene::EarthScene()
  {
    world_->add(new Sphere(vec3(0, -1001.75, 0), 1000, new Lambertian(new ConstantTexture(vec3(0.8f)))));
    world_->add(new Sphere(vec3(0, 0.25, 0), 2, new Lambertian(new ImageTexture("../../assets/textures/earth_map.jpg"))));
  }

  Camera* EarthScene::camera(uint32 w, uint32 h)
  {
    vec3 pos(13, 2, 3);
    vec3 dir(0, 0, 0);
    return new Camera(pos, dir, vec3(0, 1, 0), 20, float(w) / float(h), 0.1f, 10.0f, 0.0f, 1.0f);
  }

  ThreeSpheresScene::ThreeSpheresScene()
  {
    world_->add(new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(new ConstantTexture(vec3(0.1, 0.2, 0.5)))));
    world_->add(new Sphere(vec3(0, -100.5, -1), 100, new Lambertian(new ConstantTexture(vec3(0.8, 0.8, 0.0)))));
    world_->add(new Sphere(vec3(1, -0, -1), 0.5, new Metal(new ConstantTexture(vec3(0.8, 0.6, 0.2)), 0.3f)));
    world_->add(new Sphere(vec3(-1, -0, -1), 0.5, new Dielectric(1.5f)));
    world_->add(new Sphere(vec3(-1, -0, -1), -0.45, new Dielectric(1.5f)));
  }

  Camera* ThreeSpheresScene::camera(uint32 w, uint32 h)
  {
    vec3 pos(3, 3, 2);
    vec3 dir(0, 0, -1);
    float aperture = 0.01f;
    float dist = (pos - dir).length();
    return new Camera(pos, dir, vec3(0, 1, 0), 50, float(w) / float(h), aperture, dist, 0.0f, 1.0f);
  }

  SampleLightScene::SampleLightScene()
  {
    Texture *checker = new CheckerTexture(new ConstantTexture(vec3(0.2, 0.3, 0.1)), new ConstantTexture(vec3(0.9)));
    Texture *perlin = new NoiseTexture(4.0f);
    world_->add(new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(checker)));
    world_->add(new Sphere(vec3(0, 2, 0), 2, new Lambertian(perlin)));
    world_->add(new Sphere(vec3(0, 7, 0), 2, new DiffuseLight(new ConstantTexture(vec3(4.0f)))));
    world_->add(new XYRect(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(vec3(4.0f)))));
  }

  Camera* SampleLightScene::camera(uint32 w, uint32 h)
  {
    vec3 pos(13, 2, 3);
    vec3 dir(0, 0, 0);
    return new Camera(pos, dir, vec3(0, 1, 0), 50, float(w) / float(h), 0.1f, 10.0f, 0.0f, 1.0f);
  }

  CornellBoxScene::CornellBoxScene()
  {
    Material* red = new Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
    Material* white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
    Material* green = new Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(vec3(15)));
    world_->add(new FlipNormals(new YZRect(0, 555, 0, 555, 555, green)));
    world_->add(new YZRect(0, 555, 0, 555, 0, red));
    world_->add(new XZRect(213, 343, 227, 332, 554, light));
    world_->add(new FlipNormals(new XZRect(0, 555, 0, 555, 555, white)));
    world_->add(new XZRect(0, 555, 0, 555, 0, white));
    world_->add(new FlipNormals(new XYRect(0, 555, 0, 555, 555, white)));

    world_->add(new Translate(new RotateY(new Box(vec3(0), vec3(165, 165, 165), white), -18.0f), vec3(130, 0, 65)));
    world_->add(new Translate(new RotateY(new Box(vec3(0), vec3(165, 330, 165), white), 15.0f), vec3(265, 0, 295)));
  }

  Camera* CornellBoxScene::camera(uint32 w, uint32 h)
  {
    vec3 pos(278, 278, -800);
    vec3 dir(278, 278, 0);
    float distance = 10.0f;
    float aperture = 0.0f;
    float vfov = 40.0f;
    return new Camera(pos, dir, vec3(0, 1, 0), vfov, float(w) / float(h), aperture, distance, 0.0f, 1.0f);
  }

  CornellSmokeScene::CornellSmokeScene()
  {
    Material* red = new Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
    Material* white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
    Material* green = new Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(vec3(15)));
    world_->add(new FlipNormals(new YZRect(0, 555, 0, 555, 555, green)));
    world_->add(new YZRect(0, 555, 0, 555, 0, red));
    world_->add(new XZRect(113, 443, 127, 432, 554, light));
    world_->add(new FlipNormals(new XZRect(0, 555, 0, 555, 555, white)));
    world_->add(new XZRect(0, 555, 0, 555, 0, white));
    world_->add(new FlipNormals(new XYRect(0, 555, 0, 555, 555, white)));

    Hitable* b1 = new Translate(new RotateY(new Box(vec3(0), vec3(165, 165, 165), white), -18.0f), vec3(130, 0, 65));
    Hitable* b2 = new Translate(new RotateY(new Box(vec3(0), vec3(165, 330, 165), white), 15.0f), vec3(265, 0, 295));

    world_->add(new ConstantMedium(b1, 0.01, new ConstantTexture(vec3(1.0f))));
    world_->add(new ConstantMedium(b2, 0.01, new ConstantTexture(vec3(0.0f))));
  }

  Camera* CornellSmokeScene::camera(uint32 w, uint32 h)
  {
    vec3 pos(278, 278, -800);
    vec3 dir(278, 278, 0);
    float distance = 10.0f;
    float aperture = 0.0f;
    float vfov = 40.0f;
    return new Camera(pos, dir, vec3(0, 1, 0), vfov, float(w) / float(h), aperture, distance, 0.0f, 1.0f);
  }

  FinalScene::FinalScene()
  {
    uint32 nb = 20;
    Hitable** boxlist = new Hitable*[10000];
    Hitable** boxlist2 = new Hitable*[10000];
    Material* white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
    Material* ground = new Lambertian(new ConstantTexture(vec3(0.48, 0.83, 0.53)));
    uint32 b = 0;
    for (int i = 0; i < nb; ++i)
    {
      for (int j = 0; j < nb; ++j)
      {
        float w = 100.0f;
        float x0 = -1000.0f + i * w;
        float z0 = -1000.0f + j * w;
        float y0 = 0.0f;
        float x1 = x0 + w;
        float y1 = 100.0f * (randomFloat01() + 0.01f);
        float z1 = z0 + w;
        boxlist[b++] = new Box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
      }
    }
    int l = 0;
    world_->add(new BVHNode(boxlist, b, 0, 1));
    Material* light = new DiffuseLight(new ConstantTexture(vec3(7.0f)));
    world_->add(new XZRect(123, 423, 147, 412, 554, light));
    vec3 center(400, 400, 200);
    world_->add(new MovingSphere(center, center + vec3(30, 0, 0), 0, 1, 50, new Lambertian(new ConstantTexture(vec3(0.7, 0.3, 0.1)))));
    world_->add(new Sphere(vec3(260, 150, 45), 50, new Dielectric(1.5)));
    world_->add(new Sphere(vec3(0, 150, 145), 50, new Metal(new ConstantTexture(vec3(0.8, 0.8, 0.9)), 10.0)));
    Hitable* boundary = new Sphere(vec3(360, 150, 145), 70, new Dielectric(1.5));
    world_->add(boundary);
    world_->add(new ConstantMedium(boundary, 0.2, new ConstantTexture(vec3(0.2, 0.4, 0.9))));
    boundary = new Sphere(vec3(0, 0, 0), 5000, new Dielectric(1.5));
    world_->add(new ConstantMedium(boundary, 0.0001, new ConstantTexture(vec3(1.0, 1.0, 1.0))));
    int nx, ny, nn;
    Material *emat = new Lambertian(new ImageTexture("../../assets/textures/earth_map.jpg"));
    world_->add(new Sphere(vec3(400, 200, 400), 100, emat));
    Texture *pertext = new NoiseTexture(0.1);
    world_->add(new Sphere(vec3(220, 280, 300), 80, new Lambertian(pertext)));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
      boxlist2[j] = new Sphere(vec3(165 * randomFloat01(), 165 * randomFloat01(), 165 * randomFloat01()), 10, white);
    }
    world_->add(new Translate(new RotateY(new BVHNode(boxlist2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395)));
  }

  Camera* FinalScene::camera(uint32 w, uint32 h)
  {
    vec3 pos(278, 278, -800);
    vec3 dir(278, 278, 0);
    float distance = 10.0f;
    float aperture = 0.0f;
    float vfov = 40.0f;
    return new Camera(pos, dir, vec3(0, 1, 0), vfov, float(w) / float(h), aperture, distance, 0.0f, 1.0f);
  }

} /* end of vxt namespace */