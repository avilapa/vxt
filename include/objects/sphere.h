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
* \file sphere.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief .
*
*/
namespace vxt
{

  class Sphere : public Hitable
  {
  public:
    Sphere() {}
    Sphere(vec3 c, float r, Material* m) : center(c), radius(r), mat(m) {}

    virtual bool hit(const Ray& r, float t_min, float t_max, Hit& h) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const;

    vec3 center;
    float radius;
    Material *mat;
  };

  class MovingSphere : public Hitable
  {
  public:
    MovingSphere() {}
    MovingSphere(vec3 c0, vec3 c1, float t0, float t1, float r, Material* m) : center0(c0), center1(c1), time0(t0), time1(t1), radius(r), mat(m) {}

    virtual bool hit(const Ray& r, float t_min, float t_max, Hit& h) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const;

    vec3 center(float time) const;

    vec3 center0, center1;
    float time0, time1;
    float radius;
    Material *mat;
  };

} /* end of vxt namespace */