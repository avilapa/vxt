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
#include "aabb.h"

#include <vector>

/**
* \file hitable.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief .
*
*/
namespace vxt
{

  class Ray;
  class Material;

  struct Hit
  {
    float t;
    vec3 p;
    vec3 n;
    vec2 uv;
    Material *mat;
  };

  class Hitable
  {
  public:
    virtual bool hit(const Ray& r, float t_min, float t_max, Hit& h) const = 0;
    virtual bool boundingBox(float t0, float t1, AABB& box) const = 0;
  };

  class HitableList : public Hitable
  {
  public:
    HitableList() {}
    HitableList(std::vector<Hitable*> hitables) : hitables_(hitables) {}

    void add(Hitable* hitable);

    virtual bool hit(const Ray& r, float t_min, float t_max, Hit& h) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const;

  private:
    std::vector<Hitable*> hitables_;
  };

  class FlipNormals : public Hitable
  {
  public:
    FlipNormals(Hitable* p) : hitable_(p) {}

    virtual bool hit(const Ray& r, float t_min, float t_max, Hit& h) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const;

  private:
    Hitable* hitable_;
  };

  class Translate : public Hitable
  {
  public:
    Translate(Hitable* p, const vec3& displacement) : hitable_(p), offset_(displacement) {}

    virtual bool hit(const Ray& r, float t_min, float t_max, Hit& h) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const;

  private:
    Hitable* hitable_;
    vec3 offset_;
  };

  class RotateY : public Hitable
  {
  public:
    RotateY(Hitable* p, float angle);

    virtual bool hit(const Ray& r, float t_min, float t_max, Hit& h) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const;

  private:
    Hitable* hitable_;
    float sin_theta_;
    float cos_theta_;
    bool has_box_;
    AABB bbox_;
  };

} /* end of vxt namespace */