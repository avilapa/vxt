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

#include "../include/camera.h"
#include "../include/ray.h"

namespace vxt
{

  static vec3 randomUnitDisk()
  {
    vec3 p;
    do 
    {
      p = 2.0f * vec3(randomFloat01(), randomFloat01(), 0.0f) - vec3(1, 1, 0);
    } while (glm::dot(p, p) >= 1.0f);
    return p;
  }

  Camera::Camera(vec3 look_from, vec3 look_at, vec3 up, float vfov, float aspect, float aperture, float focus_dist)
  {
    lens_radius = aperture / 2.0f;
    float theta = vfov * 3.14159265359 / 180;
    float half_height = tan(theta / 2.0f);
    float half_width = aspect * half_height;
    origin = look_from;
    w = glm::normalize(look_from - look_at);
    u = glm::normalize(glm::cross(up, w));
    v = glm::cross(w, u);
    x0y0 = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
    axis_x = 2 * half_width * focus_dist * u;
    axis_y = 2 * half_height * focus_dist * v;
  }

  Ray Camera::ray(float s, float t)
  {
    vec3 rd = lens_radius * randomUnitDisk();
    vec3 offset = u * rd.x + v * rd.y;
    return Ray(origin + offset, x0y0 + s * axis_x + t * axis_y - origin - offset);
  }

} /* end of vxt namespace */