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

#include "../../include/objects/box.h"
#include "../../include/objects/rect.h"
#include "../../include/objects/aabb.h"
#include "../../include/renderer/ray.h"

namespace vxt
{

  Box::Box(const vec3& p0, const vec3& p1, Material* m)
  {
    p_min_ = p0;
    p_max_ = p1;
    HitableList* list = new HitableList();
    list->add(new XYRect(p0.x, p1.x, p0.y, p1.y, p1.z, m));
    list->add(new FlipNormals(new XYRect(p0.x, p1.x, p0.y, p1.y, p0.z, m)));
    list->add(new XZRect(p0.x, p1.x, p0.z, p1.z, p1.y, m));
    list->add(new FlipNormals(new XZRect(p0.x, p1.x, p0.z, p1.z, p0.y, m)));
    list->add(new YZRect(p0.y, p1.y, p0.z, p1.z, p1.x, m));
    list->add(new FlipNormals(new YZRect(p0.y, p1.y, p0.z, p1.z, p0.x, m)));
    list_ptr_ = list;
  }

  bool Box::hit(const Ray& r, float t_min, float t_max, Hit& h) const
  {
    return list_ptr_->hit(r, t_min, t_max, h);
  }

  bool Box::boundingBox(float t0, float t1, AABB& box) const
  {
    box = AABB(p_min_, p_max_);
    return true;
  }

} /* end of vxt namespace */