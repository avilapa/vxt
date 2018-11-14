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

#include "../../include/materials/material.h"
#include "../../include/materials/texture.h"
#include "../../include/objects/hitable.h"
#include "../../include/renderer/ray.h"

namespace vxt
{

  static vec3 reflect(const vec3& v, const vec3& n)
  {
    return v - 2 * glm::dot(v, n) * n;
  }

  static bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
  {
    vec3 norm = glm::normalize(v);
    float dot = glm::dot(norm, n);
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dot * dot);
    if (discriminant > 0.0f)
    {
      refracted = ni_over_nt * (norm - n * dot) - n * sqrt(discriminant);
      return true;
    }
    else
    {
      return false;
    }
  }

  static float schlick(float cosine, float index_of_refraction)
  {
    float r0 = (1 - index_of_refraction) / (1 + index_of_refraction);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }

  bool Lambertian::scatter(const Ray& r_in, const Hit hit, vec3& attenuation, Ray& scattered) const
  {
    vec3 target = hit.p + hit.n + randomUnitSphere();
    scattered = Ray(hit.p, target - hit.p, r_in.time());
    attenuation = albedo->value(hit.uv.x, hit.uv.y, hit.p);
    return true;
  }

  bool Metal::scatter(const Ray& r_in, const Hit hit, vec3& attenuation, Ray& scattered) const
  {
    vec3 reflected = reflect(glm::normalize(r_in.direction()), hit.n);
    scattered = Ray(hit.p, reflected + fuzz * randomUnitSphere(), r_in.time());
    attenuation = albedo->value(hit.uv.x, hit.uv.y, hit.p);
    return glm::dot(scattered.direction(), hit.n) > 0.0f;
  }

  bool Dielectric::scatter(const Ray& r_in, const Hit hit, vec3& attenuation, Ray& scattered) const
  {
    vec3 outward_normal;
    vec3 reflected = reflect(r_in.direction(), hit.n);
    vec3 refracted;
    float ni_over_nt;
    float reflect_prob;
    float cosine;
    attenuation = vec3(1.0f);
    if (glm::dot(r_in.direction(), hit.n) > 0.0f)
    {
      outward_normal = -hit.n;
      ni_over_nt = index_of_refraction;
      cosine = index_of_refraction * glm::dot(r_in.direction(), hit.n) / r_in.direction().length();
    }
    else
    {
      outward_normal = hit.n;
      ni_over_nt = 1.0f / index_of_refraction;
      cosine = -glm::dot(r_in.direction(), hit.n) / r_in.direction().length();
    }
    if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
    {
      reflect_prob = schlick(cosine, index_of_refraction);
    }
    else
    {
      reflect_prob = 1.0f;
    }
    if (randomFloat01() < reflect_prob)
    {
      scattered = Ray(hit.p, reflected, r_in.time());
    }
    else
    {
      scattered = Ray(hit.p, refracted, r_in.time());
    }
    return true;
  }

  bool DiffuseLight::scatter(const Ray& r_in, const Hit hit, vec3& attenuation, Ray& scattered) const
  {
    return false;
  }

  vec3 DiffuseLight::emitted(float u, float v, const vec3& p) const
  {
    vec3 aaa = emit->value(u, v, p);
    
    return aaa;
  }

  bool Isotropic::scatter(const Ray& r_in, const Hit hit, vec3& attenuation, Ray& scattered) const
  {
    scattered = Ray(hit.p, randomUnitSphere());
    attenuation = albedo->value(hit.uv.x, hit.uv.y, hit.p);
    return true;
  }


} /* end of vxt namespace */