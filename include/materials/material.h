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
* \file material.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief .
*
*/
namespace vxt
{

  class Ray;
  class Texture;

  struct Hit;

  class Material
  {
  public:
    virtual bool scatter(const Ray& r_in, const Hit hit, vec3& attenuation, Ray& scattered) const = 0;
    virtual vec3 emitted(float u, float v, const vec3& p) const { return vec3(0.0f); }
  };

  class Lambertian : public Material
  {
  public:
    Lambertian(Texture* a) : albedo(a) {}

    virtual bool scatter(const Ray& r_in, const Hit hit, vec3& attenuation, Ray& scattered) const;

    Texture* albedo;
  };

  class Metal : public Material
  {
  public:
    Metal(Texture* a, float f) : albedo(a) { if (f < 1.0f) fuzz = f; else fuzz = 1.0f; }

    virtual bool scatter(const Ray& r_in, const Hit hit, vec3& attenuation, Ray& scattered) const;

    Texture* albedo;
    float fuzz;
  };

  class Dielectric : public Material
  {
  public:
    Dielectric(float ri) : index_of_refraction(ri) {}

    virtual bool scatter(const Ray& r_in, const Hit hit, vec3& attenuation, Ray& scattered) const;

    float index_of_refraction;
  };

  class DiffuseLight : public Material
  {
  public:
    DiffuseLight(Texture* a) : emit(a) {}

    virtual bool scatter(const Ray& r_in, const Hit hit, vec3& attenuation, Ray& scattered) const;
    virtual vec3 emitted(float u, float v, const vec3& p) const;

  private:
    Texture* emit;
  };

  class Isotropic : public Material
  {
  public:
    Isotropic(Texture* a) : albedo(a) {}

    virtual bool scatter(const Ray& r_in, const Hit hit, vec3& attenuation, Ray& scattered) const;

  private:
    Texture* albedo;
  };

} /* end of vxt namespace */