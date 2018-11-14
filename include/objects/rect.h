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

#include "hitable.h"

/**
* \file rect.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief .
*
*/
namespace vxt
{

  class XYRect : public Hitable
  {
  public:
    XYRect() {}
    XYRect(float x0, float x1, float y0, float y1, float k, Material* m) : x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k), mat(m) {}

    virtual bool hit(const Ray& r, float t_min, float t_max, Hit& h) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const;

    float x0_, x1_, y0_, y1_, k_;
    Material *mat;
  };

  class XZRect : public Hitable
  {
  public:
    XZRect() {}
    XZRect(float x0, float x1, float z0, float z1, float k, Material* m) : x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k), mat(m) {}

    virtual bool hit(const Ray& r, float t_min, float t_max, Hit& h) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const;

    float x0_, x1_, z0_, z1_, k_;
    Material *mat;
  };

  class YZRect : public Hitable
  {
  public:
    YZRect() {}
    YZRect(float y0, float y1, float z0, float z1, float k, Material* m) : y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k), mat(m) {}

    virtual bool hit(const Ray& r, float t_min, float t_max, Hit& h) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const;

    float y0_, y1_, z0_, z1_, k_;
    Material *mat;
  };

} /* end of vxt namespace */