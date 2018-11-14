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

#include "camera.h"
#include "../objects/hitable.h"

/**
* \file scene.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Scene class.
*
*/
namespace vxt
{

  class Background
  {
  public:
    virtual vec3 color(const Ray& r) const { return vec3(0.0f); }
  };

  class GradientBackground : public Background
  {
  public:
    virtual vec3 color(const Ray& r) const;
  };

  class Scene
  {
  public:
    Scene();
    virtual ~Scene();

    void add(Hitable* hitable);

    HitableList* world() const { return world_; }

  protected:
    HitableList* world_;
  };

#define SCENE(name) class name : public Scene \
  {\
  public:\
    name();\
    static Camera* camera(uint32 w, uint32 h);\
  };

  SCENE(RandomScene);         // Ray Tracing in One Weekend final scene
  SCENE(FinalScene);          // Ray Tracing the Next Week final scene
  SCENE(PerlinScene);
  SCENE(EarthScene);
  SCENE(ThreeSpheresScene);
  SCENE(SampleLightScene);
  SCENE(CornellBoxScene);
  SCENE(CornellSmokeScene);

#undef SCENE

} /* end of vxt namespace */